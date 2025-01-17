#include <EditorPluginAssets/EditorPluginAssetsPCH.h>

#include <EditorFramework/GUI/ExposedParameters.h>
#include <EditorPluginAngelScript/AngelScriptAsset/AngelScriptAsset.h>
#include <GuiFoundation/NodeEditor/NodeScene.moc.h>
#include <ToolsFoundation/NodeObject/NodeCommandAccessor.h>
#include <ToolsFoundation/Serialization/DocumentObjectConverter.h>

// clang-format off
EZ_BEGIN_DYNAMIC_REFLECTED_TYPE(ezAngelScriptParameter, 1, ezRTTIDefaultAllocator<ezAngelScriptParameter>)
{
  EZ_BEGIN_PROPERTIES
  {
    EZ_MEMBER_PROPERTY("Name", m_sName)->AddFlags(ezPropertyFlags::ReadOnly),
    EZ_MEMBER_PROPERTY("Declaration", m_sDeclaration)->AddFlags(ezPropertyFlags::ReadOnly),
    EZ_MEMBER_PROPERTY("DefaultValue", m_DefaultValue)->AddAttributes(new ezHiddenAttribute()),
    EZ_MEMBER_PROPERTY("Expose", m_bExpose),
  }
  EZ_END_PROPERTIES;
}
EZ_END_DYNAMIC_REFLECTED_TYPE;

EZ_BEGIN_DYNAMIC_REFLECTED_TYPE(ezAngelScriptAssetProperties, 1, ezRTTIDefaultAllocator<ezAngelScriptAssetProperties>)
{
  EZ_BEGIN_PROPERTIES
  {
    EZ_MEMBER_PROPERTY("SourceFile", m_sScriptFile)->AddAttributes(new ezFileBrowserAttribute("Select Script", "*.as")),
    EZ_MEMBER_PROPERTY("ClassName", m_sClassName)->AddAttributes(new ezDefaultValueAttribute("ScriptObject")),
    EZ_ARRAY_MEMBER_PROPERTY("Parameters", m_Parameters)->AddAttributes(new ezContainerAttribute(false, false, false)),
  }
  EZ_END_PROPERTIES;
}
EZ_END_DYNAMIC_REFLECTED_TYPE;

EZ_BEGIN_DYNAMIC_REFLECTED_TYPE(ezAngelScriptAssetDocument, 1, ezRTTINoAllocator)
EZ_END_DYNAMIC_REFLECTED_TYPE;
// clang-format on

ezAngelScriptAssetDocument::ezAngelScriptAssetDocument(ezStringView sDocumentPath)
  : ezSimpleAssetDocument<ezAngelScriptAssetProperties>(sDocumentPath, ezAssetDocEngineConnection::Simple)
{
}

ezTransformStatus ezAngelScriptAssetDocument::InternalTransformAsset(ezStreamWriter& stream, ezStringView sOutputTag, const ezPlatformProfile* pAssetProfile, const ezAssetFileHeader& AssetHeader, ezBitflags<ezTransformFlags> transformFlags)
{
  EZ_ASSERT_NOT_IMPLEMENTED;
  return ezTransformStatus("");
}

ezTransformStatus ezAngelScriptAssetDocument::InternalTransformAsset(const char* szTargetFile, ezStringView sOutputTag, const ezPlatformProfile* pAssetProfile, const ezAssetFileHeader& AssetHeader, ezBitflags<ezTransformFlags> transformFlags)
{
  SyncInfos();

  return ezAssetDocument::RemoteExport(AssetHeader, szTargetFile);
}

void ezAngelScriptAssetDocument::SyncInfos()
{
  auto pProps = GetProperties();

  {
    ezDocumentConfigMsgToEngine cfg;
    cfg.m_sWhatToDo = "InputFile";
    cfg.m_sValue = pProps->m_sScriptFile;
    GetEditorEngineConnection()->SendMessage(&cfg);
  }

  {
    ezDocumentConfigMsgToEngine cfg;
    cfg.m_sWhatToDo = "Class";
    cfg.m_sValue = pProps->m_sClassName;
    GetEditorEngineConnection()->SendMessage(&cfg);
  }
}

void ezAngelScriptAssetDocument::UpdateAssetDocumentInfo(ezAssetDocumentInfo* pInfo) const
{
  SUPER::UpdateAssetDocumentInfo(pInfo);

  ezExposedParameters* pExposedParams = EZ_DEFAULT_NEW(ezExposedParameters);

  for (const auto& p : GetProperties()->m_Parameters)
  {
    if (p.m_bExpose == false || p.m_sName.IsEmpty() || !p.m_DefaultValue.IsValid())
      continue;

    ezExposedParameter* param = EZ_DEFAULT_NEW(ezExposedParameter);
    param->m_sName = p.m_sName;
    param->m_DefaultValue = p.m_DefaultValue;

    // TODO AngelScript: support resource handles and pass through the necessary attributes
    //
    // if (p.m_DefaultValue.IsA<ezString>())
    //{
    //   param->m_Attributes.PushBack(new ezAssetBrowserAttribute("CompatibleAsset_Material"));
    // }

    pExposedParams->m_Parameters.PushBack(param);
  }

  // Info takes ownership of meta data.
  pInfo->m_MetaInfo.PushBack(pExposedParams);
}

void ezAngelScriptAssetDocument::OpenExternalEditor()
{
  ezStringBuilder sScriptFile(GetProperties()->m_sScriptFile);

  if (GetProperties()->m_sScriptFile.IsEmpty())
  {
    ShowDocumentStatus("No script file specified.");
    return;
  }

  if (!ezFileSystem::ExistsFile(sScriptFile))
  {
    ShowDocumentStatus("Script file doesn't exist.");
    return;
  }

  ezStringBuilder sScriptFileAbs;
  if (ezFileSystem::ResolvePath(sScriptFile, &sScriptFileAbs, nullptr).Failed())
    return;

  {
    QStringList args;

    args.append(sScriptFileAbs.GetData());

    if (ezQtUiServices::OpenInVsCode(args).Failed())
    {
      // try again with a different program
      ezQtUiServices::OpenFileInDefaultProgram(sScriptFileAbs);
    }
  }
}

void ezAngelScriptAssetDocument::SyncExposedParameters()
{
  SyncInfos();

  ezDocumentConfigMsgToEngine cfg;
  cfg.m_sWhatToDo = "SyncExposedParams";
  GetEditorEngineConnection()->SendMessage(&cfg);
}
