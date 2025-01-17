#include <AngelScriptPlugin/AngelScriptPluginPCH.h>

#include <AngelScript/include/angelscript.h>
#include <AngelScript/source/add_on/scriptstdstring/scriptstdstring.h>
#include <AngelScriptPlugin/Runtime/AngelScriptEngineSingleton.h>
#include <AngelScriptPlugin/Runtime/AngelScriptInstance.h>
#include <AngelScriptPlugin/Utils/AngelScriptUtils.h>
#include <Core/World/Component.h>
#include <Foundation/Configuration/Startup.h>
#include <Foundation/Reflection/ReflectionUtils.h>
#include <Foundation/Types/Variant.h>

EZ_IMPLEMENT_SINGLETON(ezAngelScriptEngineSingleton);

// clang-format off
EZ_BEGIN_SUBSYSTEM_DECLARATION(AngelScriptPlugin, AngelScriptEngineSingleton)

BEGIN_SUBSYSTEM_DEPENDENCIES
  "Foundation"
END_SUBSYSTEM_DEPENDENCIES

ON_HIGHLEVELSYSTEMS_STARTUP
{
  EZ_DEFAULT_NEW(ezAngelScriptEngineSingleton);
}

ON_HIGHLEVELSYSTEMS_SHUTDOWN
{
  ezAngelScriptEngineSingleton* pDummy = ezAngelScriptEngineSingleton::GetSingleton();
  EZ_DEFAULT_DELETE(pDummy);
}

EZ_END_SUBSYSTEM_DECLARATION;
// clang-format on

static void ezLog_Info(ezStringView text)
{
  ezLog::Info(text);
}

static void ezLog_Error(ezStringView text)
{
  ezLog::Error(text);
}

static void ezLog_Warning(ezStringView text)
{
  ezLog::Warning(text);
}

static void ezLog_Success(ezStringView text)
{
  ezLog::Success(text);
}

class ezAsStringFactory : public asIStringFactory
{
public:
  ezAsStringFactory() = default;
  ~ezAsStringFactory() = default;

  const void* GetStringConstant(const char* data, asUINT length) override
  {
    ezHashedString hs;
    hs.Assign(ezStringView(data, length));

    // we need to give out a pointer to a StringView that doesn't vanish
    EZ_LOCK(m_Mutex);
    auto it = m_Strings.Insert(hs.GetView());
    const ezStringView& view = it.Key();

    return &view;
  }


  int ReleaseStringConstant(const void* str) override
  {
    // we don't clean up the strings
    return 0;
  }

  int GetRawStringData(const void* str, char* data, asUINT* length) const override
  {
    const ezStringView* pView = (const ezStringView*)str;

    *length = pView->GetElementCount();

    if (data)
    {
      ezStringUtils::Copy(data, *length + 1, pView->GetStartPointer());
    }

    return 0;
  }

private:
  ezMutex m_Mutex;
  ezSet<ezStringView> m_Strings;
};

ezGameObject* GetAngelScriptOwnerObject(asIScriptObject* pSelf)
{
  if (pSelf)
  {
    ezAngelScriptInstance* pInstance = (ezAngelScriptInstance*)pSelf->GetUserData(ezAsUserData::ScriptInstancePtr);
    pSelf->Release();

    return pInstance->GetOwnerComponent()->GetOwner();
  }

  return nullptr;
}

ezComponent* GetAngelScriptOwnerComponent(asIScriptObject* pSelf)
{
  if (pSelf)
  {
    ezAngelScriptInstance* pInstance = (ezAngelScriptInstance*)pSelf->GetUserData(ezAsUserData::ScriptInstancePtr);
    pSelf->Release();

    return pInstance->GetOwnerComponent();
  }

  return nullptr;
}

ezAngelScriptEngineSingleton::ezAngelScriptEngineSingleton()
  : m_SingletonRegistrar(this)
{
  m_pEngine = asCreateScriptEngine();

  AS_CHECK(m_pEngine->SetMessageCallback(asFUNCTION(MessageCallback), 0, asCALL_CDECL));

  m_pStringFactory = EZ_DEFAULT_NEW(ezAsStringFactory);

  // TODO: AS Remove
  RegisterStdString(m_pEngine);

  // register all standard types
  RegisterStandardTypes();

  m_pEngine->RegisterStringFactory("ezStringView", m_pStringFactory);

  AS_CHECK(m_pEngine->RegisterInterface("ezAsScriptComponent"));

  AS_CHECK(m_pEngine->RegisterGlobalFunction("ezGameObject@ GetScriptOwnerObject(ezAsScriptComponent@ self)", asFUNCTION(GetAngelScriptOwnerObject), asCALL_CDECL));
  AS_CHECK(m_pEngine->RegisterGlobalFunction("ezComponent@ GetScriptOwnerComponent(ezAsScriptComponent@ self)", asFUNCTION(GetAngelScriptOwnerComponent), asCALL_CDECL));

  Register_GlobalReflectedFunctions();
  Register_ReflectedTypes();

  // TODO: AS Remove
  {
    m_pEngine->SetDefaultNamespace("ezLog");
    AS_CHECK(m_pEngine->RegisterGlobalFunction("void Info(ezStringView)", asFUNCTION(ezLog_Info), asCALL_CDECL));
    AS_CHECK(m_pEngine->RegisterGlobalFunction("void Error(ezStringView)", asFUNCTION(ezLog_Error), asCALL_CDECL));
    AS_CHECK(m_pEngine->RegisterGlobalFunction("void Warning(ezStringView)", asFUNCTION(ezLog_Warning), asCALL_CDECL));
    AS_CHECK(m_pEngine->RegisterGlobalFunction("void Success(ezStringView)", asFUNCTION(ezLog_Success), asCALL_CDECL));
    m_pEngine->SetDefaultNamespace("");
  }
}

ezAngelScriptEngineSingleton::~ezAngelScriptEngineSingleton()
{
  m_pEngine->Release();

  if (m_pStringFactory)
  {
    ezAsStringFactory* pFactor = (ezAsStringFactory*)m_pStringFactory;
    EZ_DEFAULT_DELETE(pFactor);
  }
}


asIScriptModule* ezAngelScriptEngineSingleton::SetModuleCode(ezStringView sModuleName, ezStringView sCode)
{
  ezStringBuilder tmp;

  asIScriptModule* pModule = m_pEngine->GetModule(sModuleName.GetData(tmp), asGM_ALWAYS_CREATE);

  pModule->AddScriptSection("MainSection", sCode.GetStartPointer(), sCode.GetElementCount());

  if (int r = pModule->Build(); r < 0)
  {
    // The build failed. The message stream will have received
    // compiler errors that shows what needs to be fixed

    pModule->Discard();
    return nullptr;
  }

  return pModule;
}

asIScriptModule* ezAngelScriptEngineSingleton::CompileModule(ezStringView sModuleName, ezStringView sMainClass, ezStringView sCode)
{
  ezStringBuilder tmp;

  asIScriptModule* pModule = SetModuleCode(sModuleName, sCode);

  if (pModule == nullptr)
    return nullptr;

  const asITypeInfo* pClassType = pModule->GetTypeInfoByName(sMainClass.GetData(tmp));

  if (pClassType == nullptr)
  {
    ezLog::Error("AngelScript code doesn't contain class '{}'", sMainClass);
    return nullptr;
  }

  if (ValidateModule(pModule).Failed())
  {
    return nullptr;
  }

  return pModule;
}

void ezAngelScriptEngineSingleton::AddForbiddenType(const char* szTypeName)
{
  asITypeInfo* pTypeInfo = m_pEngine->GetTypeInfoByName(szTypeName);
  EZ_ASSERT_DEV(pTypeInfo != nullptr, "Type '{}' not found", szTypeName);

  m_ForbiddenTypes.PushBack(pTypeInfo);
}

bool ezAngelScriptEngineSingleton::IsTypeForbidden(const asITypeInfo* pType) const
{
  return m_ForbiddenTypes.Contains(pType);
}

void ezAngelScriptEngineSingleton::MessageCallback(const asSMessageInfo* msg, void* param)
{
  switch (msg->type)
  {
    case asMSGTYPE_ERROR:
      ezLog::Error("AngelScript: {} ({}, {}) : {}", msg->section, msg->row, msg->col, msg->message);
      break;
    case asMSGTYPE_WARNING:
      ezLog::Warning("AngelScript: {} ({}, {}) : {}", msg->section, msg->row, msg->col, msg->message);
      break;
    case asMSGTYPE_INFORMATION:
      ezLog::Info("AngelScript: {} ({}, {}) : {}", msg->section, msg->row, msg->col, msg->message);
      break;
  }
}

void CastToBase(asIScriptGeneric* gen)
{
  int derivedTypeId = gen->GetObjectTypeId();
  auto derivedTypeInfo = gen->GetEngine()->GetTypeInfoById(derivedTypeId);
  const ezRTTI* pDerivedRtti = (const ezRTTI*)derivedTypeInfo->GetUserData(ezAsUserData::RttiPtr);

  const ezRTTI* pBaseRtti = (const ezRTTI*)gen->GetAuxiliary();

  if (pDerivedRtti != nullptr && pBaseRtti != nullptr)
  {
    if (pDerivedRtti->IsDerivedFrom(pBaseRtti))
    {
      gen->SetReturnObject(gen->GetObject());
      return;
    }
  }

  gen->SetReturnObject(nullptr);
}

void CastToDerived(asIScriptGeneric* gen)
{
  int baseTypeId = gen->GetObjectTypeId();
  auto baseTypeInfo = gen->GetEngine()->GetTypeInfoById(baseTypeId);
  const ezRTTI* pBaseRtti = (const ezRTTI*)baseTypeInfo->GetUserData(ezAsUserData::RttiPtr);

  const ezRTTI* pDerivedRtti = (const ezRTTI*)gen->GetAuxiliary();

  if (pBaseRtti != nullptr && pDerivedRtti != nullptr)
  {
    if (pDerivedRtti->IsDerivedFrom(pBaseRtti))
    {
      gen->SetReturnObject(gen->GetObject());
      return;
    }
  }

  gen->SetReturnObject(nullptr);
}

void ezAngelScriptEngineSingleton::Register_ReflectedTypes()
{
  Register_ReflectedType(ezGetStaticRTTI<ezComponent>(), false);
  Register_ReflectedType(ezGetStaticRTTI<ezMessage>(), true);
}

struct RefInstance
{
  ezUInt32 m_uiRefCount = 1;
  const ezRTTI* m_pRtti = nullptr;
};

static ezMutex s_RefCountMutex;
static ezMap<void*, RefInstance> s_RefCounts;

static void* ezRtti_Create(const ezRTTI* pRtti)
{
  auto inst = pRtti->GetAllocator()->Allocate<ezReflectedClass>();

  EZ_LOCK(s_RefCountMutex);
  s_RefCounts[inst.m_pInstance].m_pRtti = pRtti;

  return inst.m_pInstance;
}

static void ezRtti_AddRef(void* instance)
{
  EZ_LOCK(s_RefCountMutex);
  ++s_RefCounts[instance].m_uiRefCount;
}

static void ezRtti_Release(void* instance)
{
  EZ_LOCK(s_RefCountMutex);
  RefInstance& ri = s_RefCounts[instance];
  if (--ri.m_uiRefCount == 0)
  {
    ri.m_pRtti->GetAllocator()->Deallocate(instance);
  }
}

void ezAngelScriptEngineSingleton::Register_ReflectedType(const ezRTTI* pBaseType, bool bCreatable)
{
  ezStringBuilder typeName, parentName, op;

  // first register the type
  ezRTTI::ForEachDerivedType(pBaseType, [&](const ezRTTI* pRtti)
    {
      if (pRtti == pBaseType)
        return;

      typeName = pRtti->GetTypeName();

      if (bCreatable)
      {
        const int typeId = m_pEngine->RegisterObjectType(typeName, 0, asOBJ_REF);
        AS_CHECK(typeId);
        m_pEngine->GetTypeInfoById(typeId)->SetUserData((void*)pRtti, ezAsUserData::RttiPtr);

        if (pRtti->GetAllocator() != nullptr && pRtti->GetAllocator()->CanAllocate())
        {
          op.Set(typeName, "@ f()");
          m_pEngine->RegisterObjectBehaviour(typeName, asBEHAVE_FACTORY, op, asFUNCTION(ezRtti_Create), asCALL_CDECL_OBJLAST, (void*)pRtti);
          m_pEngine->RegisterObjectBehaviour(typeName, asBEHAVE_ADDREF, "void f()", asFUNCTION(ezRtti_AddRef), asCALL_CDECL_OBJLAST, (void*)pRtti);
          m_pEngine->RegisterObjectBehaviour(typeName, asBEHAVE_RELEASE, "void f()", asFUNCTION(ezRtti_Release), asCALL_CDECL_OBJLAST, (void*)pRtti);
        }
      }
      else
      {
        const int typeId = m_pEngine->RegisterObjectType(typeName, 0, asOBJ_REF | asOBJ_NOCOUNT);
        AS_CHECK(typeId);
        m_pEngine->GetTypeInfoById(typeId)->SetUserData((void*)pRtti, ezAsUserData::RttiPtr);
      }

      AddForbiddenType(typeName);

      RegisterTypeFunctions(typeName, pRtti);
      RegisterTypeProperties(typeName, pRtti);

      //
    },
    ezRTTI::ForEachOptions::None);

  // then register the type hierarchy
  ezRTTI::ForEachDerivedType(pBaseType, [&](const ezRTTI* pRtti)
    {
      if (pRtti == pBaseType)
        return;

      typeName = pRtti->GetTypeName();

      const ezRTTI* pParentRtti = pRtti->GetParentType();

      while (pParentRtti)
      {
        parentName = pParentRtti->GetTypeName();
        op.Set(parentName, "@ opImplCast()");

        AS_CHECK(m_pEngine->RegisterObjectMethod(typeName, op, asFUNCTION(CastToBase), asCALL_GENERIC, (void*)pParentRtti));

        op.Set(typeName, "@ opCast()");
        AS_CHECK(m_pEngine->RegisterObjectMethod(parentName, op, asFUNCTION(CastToDerived), asCALL_GENERIC, (void*)pRtti));

        if (pParentRtti == pBaseType)
          break;

        pParentRtti = pParentRtti->GetParentType();
      }
      //
    },
    ezRTTI::ForEachOptions::None);
}

static void RetrieveArg(asIScriptGeneric* gen, ezUInt32 uiArg, const ezAbstractFunctionProperty* pAbstractFuncProp, ezVariant& out_arg)
{
  const ezRTTI* pArgRtti = pAbstractFuncProp->GetArgumentType(uiArg);

  switch (pArgRtti->GetVariantType())
  {
    case ezVariantType::Bool:
      out_arg = gen->GetArgByte(uiArg) != 0;
      return;
    case ezVariantType::Double:
      out_arg = gen->GetArgDouble(uiArg);
      return;
    case ezVariantType::Float:
      out_arg = gen->GetArgFloat(uiArg);
      return;
    case ezVariantType::Int8:
      out_arg = (ezInt8)gen->GetArgByte(uiArg);
      return;
    case ezVariantType::Int16:
      out_arg = (ezInt16)gen->GetArgWord(uiArg);
      return;
    case ezVariantType::Int32:
      out_arg = (ezInt32)gen->GetArgDWord(uiArg);
      return;
    case ezVariantType::Int64:
      out_arg = (ezInt64)gen->GetArgQWord(uiArg);
      return;
    case ezVariantType::UInt8:
      out_arg = (ezUInt8)gen->GetArgByte(uiArg);
      return;
    case ezVariantType::UInt16:
      out_arg = (ezUInt16)gen->GetArgWord(uiArg);
      return;
    case ezVariantType::UInt32:
      out_arg = (ezUInt32)gen->GetArgDWord(uiArg);
      return;
    case ezVariantType::UInt64:
      out_arg = (ezUInt64)gen->GetArgQWord(uiArg);
      return;

    case ezVariantType::Vector2:
      out_arg = *((const ezVec2*)gen->GetArgAddress(uiArg));
      return;
    case ezVariantType::Vector3:
      out_arg = *((const ezVec3*)gen->GetArgAddress(uiArg));
      return;
    case ezVariantType::Vector4:
      out_arg = *((const ezVec4*)gen->GetArgAddress(uiArg));
      return;
    case ezVariantType::Quaternion:
      out_arg = *((const ezQuat*)gen->GetArgAddress(uiArg));
      return;
    case ezVariantType::Matrix3:
      out_arg = *((const ezMat3*)gen->GetArgAddress(uiArg));
      return;
    case ezVariantType::Matrix4:
      out_arg = *((const ezMat4*)gen->GetArgAddress(uiArg));
      return;
    case ezVariantType::Transform:
      out_arg = *((const ezTransform*)gen->GetArgAddress(uiArg));
      return;
    case ezVariantType::Time:
      out_arg = *((const ezTime*)gen->GetArgAddress(uiArg));
      return;
    case ezVariantType::Angle:
      out_arg = *((const ezAngle*)gen->GetArgAddress(uiArg));
      return;

    case ezVariantType::String:
    case ezVariantType::HashedString:
    case ezVariantType::StringView:
    case ezVariantType::TempHashedString:
    {
      // TODO AS: own string type
      const std::string* pString = (const std::string*)gen->GetAddressOfArg(uiArg);
      out_arg = ezString(pString->c_str());
      return;
    }

    case ezVariantType::TypedPointer:
    case ezVariantType::TypedObject:
    {
      if (pArgRtti->IsDerivedFrom(ezGetStaticRTTI<ezComponent>()))
      {
        ezComponent* pComp = (ezComponent*)gen->GetArgObject(uiArg);
        return;
      }

      break;
    }

    default:
      break;
  }

  EZ_ASSERT_NOT_IMPLEMENTED;
}

static void MakeGenericFunctionCall(asIScriptGeneric* gen)
{
  const ezAbstractFunctionProperty* pAbstractFuncProp = (const ezAbstractFunctionProperty*)gen->GetAuxiliary();
  void* pObject = gen->GetObject();

  ezVariant args[8];

  for (ezUInt32 uiArg = 0; uiArg < pAbstractFuncProp->GetArgumentCount(); ++uiArg)
  {
    RetrieveArg(gen, uiArg, pAbstractFuncProp, args[uiArg]);
  }

  ezVariant ret;
  pAbstractFuncProp->Execute(pObject, args, ret);

  if (pAbstractFuncProp->GetReturnType() != nullptr)
  {
    switch (pAbstractFuncProp->GetReturnType()->GetVariantType())
    {
      case ezVariantType::Bool:
        gen->SetReturnByte(ret.Get<bool>() ? 1 : 0);
        return;
      case ezVariantType::Double:
        gen->SetReturnDouble(ret.Get<double>());
        return;
      case ezVariantType::Float:
        gen->SetReturnFloat(ret.Get<float>());
        return;
      case ezVariantType::Int8:
        gen->SetReturnByte(ret.Get<ezInt8>());
        return;
      case ezVariantType::Int16:
        gen->SetReturnWord(ret.Get<ezInt16>());
        return;
      case ezVariantType::Int32:
        gen->SetReturnDWord(ret.Get<ezInt32>());
        return;
      case ezVariantType::Int64:
        gen->SetReturnQWord(ret.Get<ezInt64>());
        return;
      case ezVariantType::UInt8:
        gen->SetReturnByte(ret.Get<ezUInt8>());
        return;
      case ezVariantType::UInt16:
        gen->SetReturnWord(ret.Get<ezUInt16>());
        return;
      case ezVariantType::UInt32:
        gen->SetReturnDWord(ret.Get<ezUInt32>());
        return;
      case ezVariantType::UInt64:
        gen->SetReturnQWord(ret.Get<ezUInt64>());
        return;

      case ezVariantType::Vector2:
        *((ezVec2*)gen->GetAddressOfReturnLocation()) = ret.Get<ezVec2>();
        return;
      case ezVariantType::Vector3:
        *((ezVec3*)gen->GetAddressOfReturnLocation()) = ret.Get<ezVec3>();
        return;
      case ezVariantType::Vector4:
        *((ezVec4*)gen->GetAddressOfReturnLocation()) = ret.Get<ezVec4>();
        return;
      case ezVariantType::Quaternion:
        *((ezQuat*)gen->GetAddressOfReturnLocation()) = ret.Get<ezQuat>();
        return;
      case ezVariantType::Matrix3:
        *((ezMat3*)gen->GetAddressOfReturnLocation()) = ret.Get<ezMat3>();
        return;
      case ezVariantType::Matrix4:
        *((ezMat4*)gen->GetAddressOfReturnLocation()) = ret.Get<ezMat4>();
        return;
      case ezVariantType::Transform:
        *((ezTransform*)gen->GetAddressOfReturnLocation()) = ret.Get<ezTransform>();
        return;
      case ezVariantType::Time:
        *((ezTime*)gen->GetAddressOfReturnLocation()) = ret.Get<ezTime>();
        return;
      case ezVariantType::Angle:
        *((ezAngle*)gen->GetAddressOfReturnLocation()) = ret.Get<ezAngle>();
        return;

      case ezVariantType::String:
      case ezVariantType::HashedString:
      case ezVariantType::StringView:
      case ezVariantType::TempHashedString:
      {
        // TODO AS: own string type
        void* dst = gen->GetAddressOfReturnLocation();
        new (dst) std::string(ret.ConvertTo<ezString>());
        return;
      }

      default:
        EZ_ASSERT_NOT_IMPLEMENTED;
        break;
    }
  }
}

bool ezAngelScriptEngineSingleton::AppendType(ezStringBuilder& decl, const ezRTTI* pRtti)
{
  if (pRtti == nullptr)
  {
    decl.Append("void");
    return true;
  }

  if (const char* szTypeName = ezAngelScriptUtils::VariantTypeToString(pRtti->GetVariantType()); szTypeName != nullptr)
  {
    decl.Append(szTypeName);
    return true;
  }

  // TODO: other types:
  // ezGameObjectHandle
  // ezComponentHandle

  if (m_WhitelistedTypes.Contains(pRtti->GetTypeName()))
  {
    decl.Append(pRtti->GetTypeName());
    return true;
  }

  return false;
}

bool ezAngelScriptEngineSingleton::AppendFuncArgs(ezStringBuilder& decl, const ezAbstractFunctionProperty* pFunc, ezUInt32 uiArg)
{
  if (uiArg > 8)
    return false;

  if (uiArg > 0)
  {
    decl.Append(", ");
  }

  return AppendType(decl, pFunc->GetArgumentType(uiArg));
}

void ezAngelScriptEngineSingleton::RegisterGenericFunction(const char* szTypeName, const ezAbstractFunctionProperty* const pFunc)
{
  ezStringBuilder decl;

  if (!AppendType(decl, pFunc->GetReturnType()))
  {
    return;
  }

  ezStringBuilder sFuncName = pFunc->GetPropertyName();
  decl.Append(" ");
  sFuncName.TrimWordStart("Reflection_");

  if (pFunc->GetFunctionType() == ezFunctionType::StaticMember)
  {
    ezStringBuilder className;

    // turn things like 'ezScriptExtensionClass_CVar' into 'ezCVar'
    if (const char* szUnderScore = ezStringUtils::FindLastSubString(szTypeName, "_"))
    {
      if (ezStringUtils::StartsWith(szTypeName, "ez"))
      {
        decl.Append("ez");
      }

      decl.Append(szUnderScore + 1, "_");
    }
    else
    {
      decl.Append(szTypeName, "_");
    }
  }

  decl.Append(sFuncName, "(");

  for (ezUInt32 uiArg = 0; uiArg < pFunc->GetArgumentCount(); ++uiArg)
  {
    if (!AppendFuncArgs(decl, pFunc, uiArg))
      return;
  }

  decl.Append(")");

  if (pFunc->GetFunctionType() == ezFunctionType::Member)
  {
    if (pFunc->GetFlags().IsSet(ezPropertyFlags::Const))
      decl.Append(" const");

    // only register methods that have not been registered before
    // this allows us to register more optimized versions first
    if (m_pEngine->GetTypeInfoByName(szTypeName)->GetMethodByDecl(decl) == nullptr)
    {
      AS_CHECK(m_pEngine->RegisterObjectMethod(szTypeName, decl, asFUNCTION(MakeGenericFunctionCall), asCALL_GENERIC, (void*)pFunc));
    }
  }
  else if (pFunc->GetFunctionType() == ezFunctionType::StaticMember)
  {
    // only register functions that have not been registered before
    // this allows us to register more optimized versions first
    if (m_pEngine->GetGlobalFunctionByDecl(decl) == nullptr)
    {
      AS_CHECK(m_pEngine->RegisterGlobalFunction(decl, asFUNCTION(MakeGenericFunctionCall), asCALL_GENERIC, (void*)pFunc));
    }
  }

  if (((pFunc->GetReturnType() != nullptr) || (pFunc->GetArgumentCount() > 0)) && !m_ShownTypes.Contains(decl))
  {
    ezLog::Info(decl);

    m_ShownTypes.Insert(decl);
  }
}

void ezAngelScriptEngineSingleton::RegisterTypeFunctions(const char* szTypeName, const ezRTTI* pRtti)
{
  for (auto pFunc : pRtti->GetFunctions())
  {
    if (!pFunc->GetAttributeByType<ezScriptableFunctionAttribute>())
      continue;

    RegisterGenericFunction(szTypeName, pFunc);
  }

  if (pRtti == nullptr || pRtti == ezGetStaticRTTI<ezReflectedClass>())
    return;

  RegisterTypeFunctions(szTypeName, pRtti->GetParentType());
}

void ezAngelScriptEngineSingleton::Register_GlobalReflectedFunctions()
{
  ezRTTI::ForEachType([&](const ezRTTI* pRtti)
    {
      if (pRtti->GetParentType() != nullptr && pRtti->GetParentType() != ezGetStaticRTTI<ezNoBase>())
        return;

      for (auto pFunc : pRtti->GetFunctions())
      {
        if (!pFunc->GetAttributeByType<ezScriptableFunctionAttribute>())
          continue;

        if (pFunc->GetFunctionType() != ezFunctionType::StaticMember)
          continue;

        RegisterGenericFunction(pRtti->GetTypeName().GetStartPointer(), pFunc);
      }

      //
    });
}

static void SetPropertyGeneric(asIScriptGeneric* gen)
{
  const ezAbstractMemberProperty* pMember = static_cast<const ezAbstractMemberProperty*>(gen->GetAuxiliary());

  pMember->SetValuePtr(gen->GetObject(), gen->GetAddressOfArg(0));
}

static void GetPropertyGeneric(asIScriptGeneric* gen)
{
  const ezAbstractMemberProperty* pMember = static_cast<const ezAbstractMemberProperty*>(gen->GetAuxiliary());

  pMember->GetValuePtr(gen->GetObject(), gen->GetAddressOfReturnLocation());
}

void ezAngelScriptEngineSingleton::RegisterTypeProperties(const char* szTypeName, const ezRTTI* pRtti)
{
  ezHybridArray<const ezAbstractProperty*, 32> properties;
  pRtti->GetAllProperties(properties);

  ezStringBuilder funcName, sVarTypeName;

  for (auto pProp : properties)
  {
    if (pProp->GetCategory() == ezPropertyCategory::Member)
    {
      const ezAbstractMemberProperty* pMember = static_cast<const ezAbstractMemberProperty*>(pProp);

      sVarTypeName.Clear();

      if (pMember->GetFlags().IsSet(ezPropertyFlags::IsEnum))
      {
        const ezAbstractEnumerationProperty* pEnumProp = static_cast<const ezAbstractEnumerationProperty*>(pMember);

        sVarTypeName = Register_EnumType(pEnumProp->GetSpecificType());
      }
      else
      {
        const ezRTTI* pPropRtti = pMember->GetSpecificType();

        sVarTypeName = ezAngelScriptUtils::VariantTypeToString(pPropRtti->GetVariantType());
      }

      if (!sVarTypeName.IsEmpty())
      {
        if (!pMember->GetFlags().IsAnySet(ezPropertyFlags::Const | ezPropertyFlags::ReadOnly))
        {
          funcName.Set("void set_", pMember->GetPropertyName(), "(", sVarTypeName, ") property ");
          AS_CHECK(m_pEngine->RegisterObjectMethod(szTypeName, funcName, asFUNCTION(SetPropertyGeneric), asCALL_GENERIC, (void*)pMember));
        }

        funcName.Set(sVarTypeName, " get_", pMember->GetPropertyName(), "() const property");
        AS_CHECK(m_pEngine->RegisterObjectMethod(szTypeName, funcName, asFUNCTION(GetPropertyGeneric), asCALL_GENERIC, (void*)pMember));
      }
    }
  }
}

ezStringView ezAngelScriptEngineSingleton::Register_EnumType(const ezRTTI* pEnumType)
{
  ezStringBuilder enumName = pEnumType->GetTypeName();
  enumName.ReplaceAll("::", "_");

  asITypeInfo* pEnumTypeInfo = m_pEngine->GetTypeInfoByName(enumName);
  if (pEnumTypeInfo != nullptr)
    return enumName;

  m_pEngine->RegisterEnum(enumName);

  ezHybridArray<ezReflectionUtils::EnumKeyValuePair, 16> enumValues;
  ezReflectionUtils::GetEnumKeysAndValues(pEnumType, enumValues, ezReflectionUtils::EnumConversionMode::ValueNameOnly);
  for (auto& enumValue : enumValues)
  {
    m_pEngine->RegisterEnumValue(enumName, enumValue.m_sKey, enumValue.m_iValue);
  }

  return enumName;
}

ezResult ezAngelScriptEngineSingleton::ValidateModule(asIScriptModule* pModule) const
{
  ezResult res = EZ_SUCCESS;

  for (ezUInt32 i = 0; i < pModule->GetGlobalVarCount(); ++i)
  {
    const char* szName;
    int typeId;

    if (pModule->GetGlobalVar(i, &szName, nullptr, &typeId) == asSUCCESS)
    {
      if (const asITypeInfo* pInfo = pModule->GetEngine()->GetTypeInfoById(typeId))
      {
        if (IsTypeForbidden(pInfo))
        {
          ezLog::Error("Global variable '{}' uses forbidden type '{}'", szName, pInfo->GetName());
          res = EZ_FAILURE;
        }
      }
    }
  }

  for (ezUInt32 i = 0; i < pModule->GetObjectTypeCount(); ++i)
  {
    const asITypeInfo* pType = pModule->GetObjectTypeByIndex(i);

    for (ezUInt32 i2 = 0; i2 < pType->GetPropertyCount(); ++i2)
    {
      const char* szName;
      int typeId;

      pType->GetProperty(i2, &szName, &typeId);

      if (const asITypeInfo* pInfo = pModule->GetEngine()->GetTypeInfoById(typeId))
      {
        if (IsTypeForbidden(pInfo))
        {
          ezLog::Error("Property '{}::{}' uses forbidden type '{}'", pType->GetName(), szName, pInfo->GetName());
          res = EZ_FAILURE;
        }
      }
    }
  }

  return res;
}

class BytecodeStream : public asIBinaryStream
{
public:
  int Write(const void* ptr, asUINT uiSize)
  {
    m_pBuffer->PushBackRange(ezConstByteArrayPtr((const ezUInt8*)ptr, uiSize));
    return uiSize;
  }

  int Read(void* pPtr, asUINT uiSize)
  {
    const ezUInt32 uiReadSize = ezMath::Min(uiSize, m_pBuffer->GetCount() - m_uiReadPos);

    ezMemoryUtils::RawByteCopy(pPtr, m_pBuffer->GetData() + m_uiReadPos, uiReadSize);

    m_uiReadPos += uiReadSize;
    return uiReadSize;
  }

  ezUInt32 m_uiReadPos = 0;
  ezDynamicArray<ezUInt8>* m_pBuffer = nullptr;
};

void ezAngelScriptEngineSingleton::SaveByteCode(asIScriptModule* pModule, ezDynamicArray<ezUInt8>& out_ByteCode)
{
  BytecodeStream stream;
  stream.m_pBuffer = &out_ByteCode;

#if EZ_ENABLED(EZ_COMPILE_FOR_DEVELOPMENT)
  pModule->SaveByteCode(&stream, false); // TODO AS: strip debug info ? (flag?)
#else
  pModule->SaveByteCode(&stream, true);
#endif
}

asIScriptModule* ezAngelScriptEngineSingleton::LoadFromByteCode(ezStringView sModuleName, ezDynamicArray<ezUInt8>& out_ByteCode) const
{
  BytecodeStream stream;
  stream.m_pBuffer = &out_ByteCode;

  ezStringBuilder tmp;
  asIScriptModule* pModule = m_pEngine->GetModule(sModuleName.GetData(tmp), asGM_ALWAYS_CREATE);

  if (pModule->LoadByteCode(&stream) < 0)
  {
    return nullptr;
  }

  return pModule;
}
