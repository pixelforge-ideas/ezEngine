#include <EnginePluginAngelScript/EnginePluginAngelScriptPCH.h>

#include <AngelScript/include/angelscript.h>
#include <AngelScriptPlugin/Runtime/AngelScriptEngineSingleton.h>
#include <AngelScriptPlugin/Runtime/AngelScriptInstance.h>
#include <AngelScriptPlugin/Utils/AngelScriptUtils.h>
#include <EnginePluginAngelScript/AngelScriptAsset/AngelScriptContext.h>
#include <Foundation/IO/FileSystem/DeferredFileWriter.h>
#include <Foundation/IO/FileSystem/FileReader.h>
#include <Foundation/Utilities/AssetFileHeader.h>

// clang-format off
EZ_BEGIN_DYNAMIC_REFLECTED_TYPE(ezAngelScriptDocumentContext, 1, ezRTTIDefaultAllocator<ezAngelScriptDocumentContext>)
{
  EZ_BEGIN_PROPERTIES
  {
    EZ_CONSTANT_PROPERTY("DocumentType", (const char*) "AngelScript"),
  }
  EZ_END_PROPERTIES;
}
EZ_END_DYNAMIC_REFLECTED_TYPE;
// clang-format on

static ezAtomicInteger32 s_iCompileCounter;

ezAngelScriptDocumentContext::ezAngelScriptDocumentContext()
  : ezEngineProcessDocumentContext(ezEngineProcessDocumentContextFlags::CreateWorld)
{
}

ezAngelScriptDocumentContext::~ezAngelScriptDocumentContext() = default;

ezStatus ezAngelScriptDocumentContext::ExportDocument(const ezExportDocumentMsgToEngine* pMsg)
{
  ezLogSystemToBuffer logBuffer;
  logBuffer.SetLogLevel(ezLogMsgType::ErrorMsg);

  ezLogSystemScope logScope(&logBuffer);

  asIScriptModule* pModule = CompileModule();

  if (pModule == nullptr)
  {
    return ezStatus(logBuffer.m_sBuffer.GetView());
  }

  ezHybridArray<ezUInt8, 1024 * 8> bytecode;
  ezAngelScriptEngineSingleton::SaveByteCode(pModule, bytecode);
  pModule->Discard();

  ezDeferredFileWriter out;
  out.SetOutput(pMsg->m_sOutputFile);

  {
    // File Header
    ezAssetFileHeader header;
    header.SetFileHashAndVersion(pMsg->m_uiAssetHash, pMsg->m_uiVersion);
    header.Write(out).AssertSuccess();

    ezUInt8 uiVersion = 2;
    out << uiVersion;
  }

  out << m_sClass;
  out.WriteArray(bytecode).AssertSuccess();

  return ezStatus(EZ_SUCCESS);
}

void ezAngelScriptDocumentContext::HandleMessage(const ezEditorEngineDocumentMsg* pMsg)
{
  if (const ezDocumentConfigMsgToEngine* pMsg2 = ezDynamicCast<const ezDocumentConfigMsgToEngine*>(pMsg))
  {
    if (pMsg2->m_sWhatToDo == "InputFile")
    {
      m_sInputFile = pMsg2->m_sValue;
    }
    else if (pMsg2->m_sWhatToDo == "Class")
    {
      m_sClass = pMsg2->m_sValue;
    }
    else if (pMsg2->m_sWhatToDo == "SyncExposedParams")
    {
      SyncExposedParameters();
    }
  }

  ezEngineProcessDocumentContext::HandleMessage(pMsg);
}

ezEngineProcessViewContext* ezAngelScriptDocumentContext::CreateViewContext()
{
  EZ_ASSERT_NOT_IMPLEMENTED;
  return nullptr;
}

void ezAngelScriptDocumentContext::DestroyViewContext(ezEngineProcessViewContext* pContext)
{
  EZ_ASSERT_NOT_IMPLEMENTED;
}

class ezLogSystemNull : public ezLogInterface
{
public:
  void HandleLogMessage(const ezLoggingEventData& le) override
  {
  }
};

void ezAngelScriptDocumentContext::SyncExposedParameters()
{
  ezLogSystemNull logNull;
  ezLogSystemScope scope(&logNull); // disable logging

  asIScriptModule* pModule = CompileModule();
  if (pModule == nullptr)
    return;

  EZ_SCOPE_EXIT(pModule->Discard());

  const asITypeInfo* pClassType = pModule->GetTypeInfoByName(m_sClass);

  asIScriptContext* pContext = pModule->GetEngine()->CreateContext();
  EZ_SCOPE_EXIT(pContext->Release());

  AS_CHECK(pContext->Prepare(pClassType->GetFactoryByIndex(0)));
  AS_CHECK(pContext->Execute());
  asIScriptObject* pInstance = (asIScriptObject*)pContext->GetReturnObject();
  pInstance->AddRef();
  EZ_SCOPE_EXIT(pInstance->Release());

  ezStringBuilder sTypeName;

  {
    ezSimpleDocumentConfigMsgToEditor msg;
    msg.m_DocumentGuid = m_DocumentGuid;
    msg.m_sWhatToDo = "SyncExposedParams_Clear";
    SendProcessMessage(&msg);
  }

  for (ezUInt32 i2 = 0; i2 < pClassType->GetPropertyCount(); ++i2)
  {
    const char* szName;
    int typeId;

    bool isPrivate = false, isProtected = false, isReference = false;
    pClassType->GetProperty(i2, &szName, &typeId, &isPrivate, &isProtected, nullptr, &isReference);
    ezStringBuilder sDecl = pClassType->GetPropertyDeclaration(i2);

    if (isPrivate || isProtected)
      continue;

    ezVariant defVal;
    if (ezAngelScriptUtils::ReadAsProperty(typeId, pInstance->GetAddressOfProperty(i2), pModule->GetEngine(), defVal).Succeeded())
    {
      ezSimpleDocumentConfigMsgToEditor msg;
      msg.m_DocumentGuid = m_DocumentGuid;
      msg.m_sWhatToDo = "SyncExposedParams_Add";
      msg.m_sPayload = szName;
      msg.m_PayloadValue = defVal;
      SendProcessMessage(&msg);
    }
  }

  {
    ezSimpleDocumentConfigMsgToEditor msg;
    msg.m_DocumentGuid = m_DocumentGuid;
    msg.m_sWhatToDo = "SyncExposedParams_Finish";
    SendProcessMessage(&msg);
  }
}

asIScriptModule* ezAngelScriptDocumentContext::CompileModule()
{
  if (m_sInputFile.IsEmpty())
  {
    ezLog::Error("No AngelScript file specified.");
    return nullptr;
  }

  ezFileReader file;
  if (file.Open(m_sInputFile).Failed())
  {
    ezLog::Error("Failed to open script file '{}'.", m_sInputFile);
    return nullptr;
  }

  ezStringBuilder sCode;
  sCode.ReadAll(file);

  ezStringBuilder sTempName;
  sTempName.SetFormat("asTempModule-{}", s_iCompileCounter.Increment());

  auto pAs = ezAngelScriptEngineSingleton::GetSingleton();
  auto pModule = pAs->CompileModule(sTempName, m_sClass, sCode);

  if (pModule == nullptr)
    return nullptr;

  if (pAs->ValidateModule(pModule).Failed())
  {
    pModule->Discard();
    return nullptr;
  }

  return pModule;
}
