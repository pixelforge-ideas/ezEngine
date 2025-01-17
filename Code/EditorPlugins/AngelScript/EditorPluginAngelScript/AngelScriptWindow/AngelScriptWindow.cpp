#include <EditorPluginAssets/EditorPluginAssetsPCH.h>

#include <AngelScriptPlugin/Utils/AngelScriptUtils.h>
#include <EditorFramework/Assets/AssetCurator.h>
#include <EditorFramework/Assets/AssetStatusIndicator.moc.h>
#include <EditorPluginAngelScript/AngelScriptAsset/AngelScriptAsset.h>
#include <EditorPluginAngelScript/AngelScriptWindow/AngelScriptWindow.moc.h>
#include <GuiFoundation/ActionViews/MenuBarActionMapView.moc.h>
#include <GuiFoundation/ActionViews/ToolBarActionMapView.moc.h>
#include <GuiFoundation/DockPanels/DocumentPanel.moc.h>
#include <GuiFoundation/NodeEditor/NodeView.moc.h>
#include <GuiFoundation/PropertyGrid/PropertyGridWidget.moc.h>
#include <ToolsFoundation/Object/ObjectCommandAccessor.h>

ezQtAngelScriptAssetDocumentWindow::ezQtAngelScriptAssetDocumentWindow(ezAngelScriptAssetDocument* pDocument)
  : ezQtEngineDocumentWindow(pDocument)
{
  m_pAssetDoc = pDocument;

  // Menu Bar
  {
    ezQtMenuBarActionMapView* pMenuBar = static_cast<ezQtMenuBarActionMapView*>(menuBar());
    ezActionContext context;
    context.m_sMapping = "AngelScriptAssetMenuBar";
    context.m_pDocument = pDocument;
    context.m_pWindow = this;
    pMenuBar->SetActionContext(context);
  }

  // Tool Bar
  {
    ezQtToolBarActionMapView* pToolBar = new ezQtToolBarActionMapView("Toolbar", this);
    ezActionContext context;
    context.m_sMapping = "AngelScriptAssetToolBar";
    context.m_pDocument = pDocument;
    context.m_pWindow = this;
    pToolBar->SetActionContext(context);
    pToolBar->setObjectName("AngelScriptAssetWindowToolBar");
    addToolBar(pToolBar);
  }

  // Central Widget
  {
    QFont font;
    font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPointSize(10);

    m_pSourceLabel = new QTextEdit(this);
    m_pSourceLabel->setFont(font);
    m_pSourceLabel->setReadOnly(true);

    m_pHighlighter = new ASHighlighter(m_pSourceLabel->document());

    ezQtDocumentPanel* pCentral = new ezQtDocumentPanel(this, pDocument);
    pCentral->setObjectName("AngelScriptView");
    pCentral->setWindowTitle("Script");
    pCentral->setWidget(m_pSourceLabel);

    m_pDockManager->setCentralWidget(pCentral);

    UpdateFileContentDisplay();
  }

  // Property Grid
  {
    ezQtDocumentPanel* pPropertyPanel = new ezQtDocumentPanel(this, pDocument);
    pPropertyPanel->setObjectName("AngelScriptAssetDockWidget");
    pPropertyPanel->setWindowTitle("Angel Script Properties");
    pPropertyPanel->show();

    ezQtPropertyGridWidget* pPropertyGrid = new ezQtPropertyGridWidget(pPropertyPanel, pDocument);

    QWidget* pWidget = new QWidget();
    pWidget->setObjectName("Group");
    pWidget->setLayout(new QVBoxLayout());
    pWidget->setContentsMargins(0, 0, 0, 0);

    pWidget->layout()->setContentsMargins(0, 0, 0, 0);
    pWidget->layout()->addWidget(new ezQtAssetStatusIndicator((ezAssetDocument*)GetDocument()));
    pWidget->layout()->addWidget(pPropertyGrid);

    pPropertyPanel->setWidget(pWidget, ads::CDockWidget::ForceNoScrollArea);

    m_pDockManager->addDockWidgetTab(ads::RightDockWidgetArea, pPropertyPanel);

    pDocument->GetSelectionManager()->SetSelection(pDocument->GetObjectManager()->GetRootObject()->GetChildren()[0]);
  }

  ezAssetCurator::GetSingleton()->m_Events.AddEventHandler(ezMakeDelegate(&ezQtAngelScriptAssetDocumentWindow::AssetEventHandler, this));

  FinishWindowCreation();
}

ezQtAngelScriptAssetDocumentWindow::~ezQtAngelScriptAssetDocumentWindow()
{
  ezAssetCurator::GetSingleton()->m_Events.RemoveEventHandler(ezMakeDelegate(&ezQtAngelScriptAssetDocumentWindow::AssetEventHandler, this));
}

void ezQtAngelScriptAssetDocumentWindow::AssetEventHandler(const ezAssetCuratorEvent& e)
{
  if (e.m_AssetGuid == m_pAssetDoc->GetGuid())
  {
    if (e.m_Type == ezAssetCuratorEvent::Type::AssetUpdated)
    {
      UpdateFileContentDisplay();
    }
  }
}

void ezQtAngelScriptAssetDocumentWindow::UpdateFileContentDisplay()
{
  const auto& sFile = m_pAssetDoc->GetProperties()->m_sScriptFile;

  if (sFile.IsEmpty())
  {
    m_pSourceLabel->setText("No script file specified.");
    return;
  }

  ezFileReader file;
  if (file.Open(sFile).Failed())
  {
    m_pSourceLabel->setText(QString("Script file '%1' does not exist. Click 'Edit Script' to create it.").arg(sFile.GetData()));
    return;
  }

  ezStringBuilder content;
  content.ReadAll(file);

  m_pSourceLabel->setText(content.GetData());
}

void ezQtAngelScriptAssetDocumentWindow::ProcessMessageEventHandler(const ezEditorEngineDocumentMsg* pMsg0)
{
  if (auto pMsg = ezDynamicCast<const ezSimpleDocumentConfigMsgToEditor*>(pMsg0))
  {
    if (pMsg->m_sWhatToDo == "SyncExposedParams_Clear")
    {
      m_ExposedParams.Clear();
    }

    if (pMsg->m_sWhatToDo == "SyncExposedParams_Add")
    {
      auto& p = m_ExposedParams.ExpandAndGetRef();
      p.m_sName = pMsg->m_sPayload;
      p.m_DefaultValue = pMsg->m_PayloadValue;
    }

    if (pMsg->m_sWhatToDo == "SyncExposedParams_Finish")
    {
      ezCommandHistory* history = m_pAssetDoc->GetCommandHistory();

      ezObjectCommandAccessor accessor(history);
      accessor.StartTransaction("Sync Parameters");

      auto pProps = m_pAssetDoc->GetPropertyObject();

      const ezInt32 uiNum = accessor.GetCountByName(pProps, "Parameters");

      ezStringBuilder sDecl;

      for (ezInt32 ui = 0; ui < uiNum; ++ui)
      {
        const ezDocumentObject* pArgObj = accessor.GetChildObjectByName(pProps, "Parameters", ui);

        ezVariant name, expose, def;
        accessor.GetValueByName(pArgObj, "Name", name).AssertSuccess();
        accessor.GetValueByName(pArgObj, "Expose", expose).AssertSuccess();
        accessor.GetValueByName(pArgObj, "DefaultValue", def).AssertSuccess();

        const ezString sName = name.ConvertTo<ezString>();
        bool bExpose = expose.ConvertTo<bool>();

        for (auto& ep : m_ExposedParams)
        {
          if (ep.m_sName == sName)
          {
            ep.m_bExpose = bExpose;

            if (ep.m_DefaultValue.GetType() == def.GetType())
            {
              // keep the user provided default value
              ep.m_DefaultValue = def;
            }

            break;
          }
        }
      }

      // clear the entire array
      accessor.ClearByName(pProps, "Parameters").AssertSuccess();

      // and fill it again
      for (ezUInt32 clip = 0; clip < m_ExposedParams.GetCount(); ++clip)
      {
        ezUuid newItemGuid = ezUuid::MakeUuid();
        accessor.AddObjectByName(pProps, "Parameters", -1, ezGetStaticRTTI<ezAngelScriptParameter>(), newItemGuid).AssertSuccess();

        const ezDocumentObject* pNewItem = accessor.GetObject(newItemGuid);

        sDecl.SetFormat("{} {} = {}", ezAngelScriptUtils::VariantTypeToString(m_ExposedParams[clip].m_DefaultValue.GetType()), m_ExposedParams[clip].m_sName, m_ExposedParams[clip].m_DefaultValue);

        accessor.SetValueByName(pNewItem, "Name", m_ExposedParams[clip].m_sName).AssertSuccess();
        accessor.SetValueByName(pNewItem, "Declaration", sDecl.GetData()).AssertSuccess();
        accessor.SetValueByName(pNewItem, "Expose", m_ExposedParams[clip].m_bExpose).AssertSuccess();
        accessor.SetValueByName(pNewItem, "DefaultValue", m_ExposedParams[clip].m_DefaultValue).AssertSuccess();
      }

      accessor.FinishTransaction();
    }
  }
}

//////////////////////////////////////////////////////////////////////////

ASHighlighter::ASHighlighter(QTextDocument* pParent)
  : QSyntaxHighlighter(pParent)
{
  // default color scheme
  m_Colors[ASEdit::Comment] = QColor("#6A8A35");
  m_Colors[ASEdit::Number] = QColor("#B5CEA8");
  m_Colors[ASEdit::String] = QColor("#CE916A");
  m_Colors[ASEdit::Operator] = QColor("#808000");
  m_Colors[ASEdit::KeywordBlue] = QColor("#569CCA");
  m_Colors[ASEdit::KeywordPink] = QColor("#C586C0");
  m_Colors[ASEdit::KeywordGreen] = QColor("#4EC9B0");
  m_Colors[ASEdit::BuiltIn] = QColor("#DCDCAA");

  m_KeywordsPink << "break";
  m_KeywordsPink << "case";
  m_KeywordsPink << "continue";
  m_KeywordsPink << "default";
  m_KeywordsPink << "do";
  m_KeywordsPink << "for";
  m_KeywordsPink << "return";
  m_KeywordsPink << "switch";
  m_KeywordsPink << "while";
  m_KeywordsPink << "if";
  m_KeywordsPink << "else";
  m_KeywordsPink << "in";
  m_KeywordsPink << "out";
  m_KeywordsPink << "inout";
  m_KeywordsPink << "is";
  m_KeywordsPink << "ezAsScriptComponent";

  m_KeywordsBlue << "function";
  m_KeywordsBlue << "funcdef";
  m_KeywordsBlue << "import";
  m_KeywordsBlue << "cast";
  m_KeywordsBlue << "this";
  m_KeywordsBlue << "void";
  m_KeywordsBlue << "true";
  m_KeywordsBlue << "false";
  m_KeywordsBlue << "null";
  m_KeywordsBlue << "class";
  m_KeywordsBlue << "struct";
  m_KeywordsBlue << "const";
  m_KeywordsBlue << "enum";
  m_KeywordsBlue << "private";
  m_KeywordsBlue << "protected";
  m_KeywordsBlue << "auto";
  m_KeywordsBlue << "explicit";
  m_KeywordsBlue << "external";
  m_KeywordsBlue << "final";
  m_KeywordsBlue << "namespace";
  m_KeywordsBlue << "interface";
  m_KeywordsBlue << "mixin";
  m_KeywordsBlue << "abstract";
  m_KeywordsBlue << "not";
  m_KeywordsBlue << "and";
  m_KeywordsBlue << "or";
  m_KeywordsBlue << "xor";
  m_KeywordsBlue << "override";
  m_KeywordsBlue << "property";
  m_KeywordsBlue << "shared";
  m_KeywordsBlue << "super";
  m_KeywordsBlue << "try";
  m_KeywordsBlue << "catch";
  m_KeywordsBlue << "typedef";

  m_KeywordsGreen << "bool";
  m_KeywordsGreen << "float";
  m_KeywordsGreen << "double";
  m_KeywordsGreen << "string";
  m_KeywordsGreen << "int";
  m_KeywordsGreen << "int8";
  m_KeywordsGreen << "int16";
  m_KeywordsGreen << "int32";
  m_KeywordsGreen << "int64";
  m_KeywordsGreen << "uint8";
  m_KeywordsGreen << "uint16";
  m_KeywordsGreen << "uint32";
  m_KeywordsGreen << "uint64";
  m_KeywordsGreen << "ezRTTI";
  m_KeywordsGreen << "ezVec2";
  m_KeywordsGreen << "ezVec3";
  m_KeywordsGreen << "ezVec4";
  m_KeywordsGreen << "ezMat3";
  m_KeywordsGreen << "ezMat4";
  m_KeywordsGreen << "ezQuat";
  m_KeywordsGreen << "ezString";
  m_KeywordsGreen << "ezStringBuilder";
  m_KeywordsGreen << "ezStringView";
  m_KeywordsGreen << "ezTransform";
  m_KeywordsGreen << "ezWorld";
  m_KeywordsGreen << "ezClock";
  m_KeywordsGreen << "ezRandom";
  m_KeywordsGreen << "ezComponent";
  m_KeywordsGreen << "ezComponentHandle";
  m_KeywordsGreen << "ezGameObject";
  m_KeywordsGreen << "ezGameObjectHandle";
  m_KeywordsGreen << "ezColor";
  m_KeywordsGreen << "ezColorGammaUB";
  m_KeywordsGreen << "ezMath";
  m_KeywordsGreen << "ezAngle";
  m_KeywordsGreen << "ezTime";
  m_KeywordsGreen << "ezMessage";

  // built-in and other popular objects + properties
  m_BuiltIn << "abs";
  m_BuiltIn << "acos";
  m_BuiltIn << "asin";
  m_BuiltIn << "atan";
  m_BuiltIn << "atan2";
  m_BuiltIn << "ceil";
  m_BuiltIn << "cos";
  m_BuiltIn << "exp";
  m_BuiltIn << "floor";
  m_BuiltIn << "log";
  m_BuiltIn << "max";
  m_BuiltIn << "min";
  m_BuiltIn << "pow";
  m_BuiltIn << "random";
  m_BuiltIn << "round";
  m_BuiltIn << "sin";
  m_BuiltIn << "sqrt";
  m_BuiltIn << "tan";
  m_BuiltIn << "Initialize";
  m_BuiltIn << "Deinitialize";
  m_BuiltIn << "OnActivated";
  m_BuiltIn << "OnActivated";
  m_BuiltIn << "OnSimulationStarted";
  m_BuiltIn << "Update";
}


void ASHighlighter::highlightBlock(const QString& text)
{
  // parsing state
  enum
  {
    Start = 0,
    Number = 1,
    Identifier = 2,
    String = 3,
    Comment = 4,
    Regex = 5
  };

  QList<int> bracketPositions;

  int blockState = previousBlockState();
  int bracketLevel = blockState >> 4;
  int state = blockState & 15;
  if (blockState < 0)
  {
    bracketLevel = 0;
    state = Start;
  }

  int start = 0;
  int i = 0;
  while (i <= text.length())
  {
    QChar ch = (i < text.length()) ? text.at(i) : QChar();
    QChar next = (i < text.length() - 1) ? text.at(i + 1) : QChar();

    switch (state)
    {

      case Start:
        start = i;
        if (ch.isSpace())
        {
          ++i;
        }
        else if (ch.isDigit())
        {
          ++i;
          state = Number;
        }
        else if (ch.isLetter() || ch == '_')
        {
          ++i;
          state = Identifier;
        }
        else if (ch == '\'' || ch == '\"')
        {
          ++i;
          state = String;
        }
        else if (ch == '/' && next == '*')
        {
          ++i;
          ++i;
          state = Comment;
        }
        else if (ch == '/' && next == '/')
        {
          i = text.length();
          setFormat(start, text.length(), m_Colors[ASEdit::Comment]);
        }
        else if (ch == '/' && next != '*')
        {
          ++i;
          state = Regex;
        }
        else
        {
          if (!QString("(){}[]").contains(ch))
            setFormat(start, 1, m_Colors[ASEdit::Operator]);
          if (ch == '{' || ch == '}')
          {
            bracketPositions += i;
            if (ch == '{')
              bracketLevel++;
            else
              bracketLevel--;
          }
          ++i;
          state = Start;
        }
        break;

      case Number:
        if (ch.isSpace() || !ch.isDigit())
        {
          setFormat(start, i - start, m_Colors[ASEdit::Number]);
          state = Start;
        }
        else
        {
          ++i;
        }
        break;

      case Identifier:
        if (ch.isSpace() || !(ch.isDigit() || ch.isLetter() || ch == '_'))
        {
          QString token = text.mid(start, i - start).trimmed();
          if (m_KeywordsBlue.contains(token))
            setFormat(start, i - start, m_Colors[ASEdit::KeywordBlue]);
          if (m_KeywordsPink.contains(token))
            setFormat(start, i - start, m_Colors[ASEdit::KeywordPink]);
          if (m_KeywordsGreen.contains(token))
            setFormat(start, i - start, m_Colors[ASEdit::KeywordGreen]);
          else if (m_BuiltIn.contains(token))
            setFormat(start, i - start, m_Colors[ASEdit::BuiltIn]);
          state = Start;
        }
        else
        {
          ++i;
        }
        break;

      case String:
        if (ch == text.at(start))
        {
          QChar prev = (i > 0) ? text.at(i - 1) : QChar();
          if (prev != '\\')
          {
            ++i;
            setFormat(start, i - start, m_Colors[ASEdit::String]);
            state = Start;
          }
          else
          {
            ++i;
          }
        }
        else
        {
          ++i;
        }
        break;

      case Comment:
        if (ch == '*' && next == '/')
        {
          ++i;
          ++i;
          setFormat(start, i - start, m_Colors[ASEdit::Comment]);
          state = Start;
        }
        else
        {
          ++i;
        }
        break;

      case Regex:
        if (ch == '/')
        {
          QChar prev = (i > 0) ? text.at(i - 1) : QChar();
          if (prev != '\\')
          {
            ++i;
            setFormat(start, i - start, m_Colors[ASEdit::String]);
            state = Start;
          }
          else
          {
            ++i;
          }
        }
        else
        {
          ++i;
        }
        break;

      default:
        state = Start;
        break;
    }
  }

  if (state == Comment)
    setFormat(start, text.length(), m_Colors[ASEdit::Comment]);
  else
    state = Start;

  if (!bracketPositions.isEmpty())
  {
    ASBlockData* blockData = reinterpret_cast<ASBlockData*>(currentBlock().userData());
    if (!blockData)
    {
      blockData = new ASBlockData;
      currentBlock().setUserData(blockData);
    }
    blockData->bracketPositions = bracketPositions;
  }

  blockState = (state & 15) | (bracketLevel << 4);
  setCurrentBlockState(blockState);
}
