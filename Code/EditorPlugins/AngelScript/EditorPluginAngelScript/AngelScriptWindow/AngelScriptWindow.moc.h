#pragma once

#include <EditorFramework/DocumentWindow/EngineDocumentWindow.moc.h>
#include <Foundation/Basics.h>
#include <GuiFoundation/DocumentWindow/DocumentWindow.moc.h>
#include <ToolsFoundation/Object/DocumentObjectManager.h>

class ezAngelScriptAssetDocument;
struct ezAssetCuratorEvent;
class ezEditorEngineDocumentMsg;
class QTextEdit;

class ezQtAngelScriptAssetDocumentWindow : public ezQtEngineDocumentWindow
{
  Q_OBJECT

public:
  ezQtAngelScriptAssetDocumentWindow(ezAngelScriptAssetDocument* pDocument);
  ~ezQtAngelScriptAssetDocumentWindow();

  virtual const char* GetWindowLayoutGroupName() const override { return "AngelScript"; }

private Q_SLOTS:

private:
  void AssetEventHandler(const ezAssetCuratorEvent& e);
  void UpdateFileContentDisplay();
  virtual void ProcessMessageEventHandler(const ezEditorEngineDocumentMsg* pMsg) override;

  struct ExposedParam
  {
    ezString m_sName;
    ezVariant m_DefaultValue;
    bool m_bExpose = false;
  };

  ezDynamicArray<ExposedParam> m_ExposedParams;

  ezAngelScriptAssetDocument* m_pAssetDoc = nullptr;

  QTextEdit* m_pSourceLabel = nullptr;
  QSyntaxHighlighter* m_pHighlighter = nullptr;
};


//////////////////////////////////////////////////////////////////////////

#include <QSyntaxHighlighter>

struct ASEdit
{
  enum ColorComponent
  {
    Comment,
    Number,
    String,
    Operator,
    KeywordBlue,
    KeywordPink,
    KeywordGreen,
    BuiltIn,

    Count,
  };
};

class ASBlockData : public QTextBlockUserData
{
public:
  QList<int> bracketPositions;
};

class ASHighlighter : public QSyntaxHighlighter
{
public:
  ASHighlighter(QTextDocument* pParent = 0);

protected:
  void highlightBlock(const QString& text) override;

private:
  QSet<QString> m_KeywordsBlue;
  QSet<QString> m_KeywordsPink;
  QSet<QString> m_KeywordsGreen;
  QSet<QString> m_BuiltIn;
  QColor m_Colors[ASEdit::Count];
};
