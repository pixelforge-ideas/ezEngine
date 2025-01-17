#pragma once

#include <AngelScriptPlugin/AngelScriptPluginDLL.h>
#include <Foundation/Configuration/Singleton.h>
#include <Foundation/Reflection/Reflection.h>
#include <Foundation/Types/UniquePtr.h>

class asIScriptEngine;
class asIScriptModule;
class asIStringFactory;
struct asSMessageInfo;

class EZ_ANGELSCRIPTPLUGIN_DLL ezAngelScriptEngineSingleton
{
  EZ_DECLARE_SINGLETON(ezAngelScriptEngineSingleton);

public:
  ezAngelScriptEngineSingleton();
  ~ezAngelScriptEngineSingleton();

  asIScriptEngine* GetEngine() const { return m_pEngine; }

  asIScriptModule* SetModuleCode(ezStringView sModuleName, ezStringView sCode);

  asIScriptModule* CompileModule(ezStringView sModuleName, ezStringView sMainClass, ezStringView sCode);

  void AddForbiddenType(const char* szTypeName);

  bool IsTypeForbidden(const asITypeInfo* pType) const;

  ezResult ValidateModule(asIScriptModule* pModule) const;

  static void SaveByteCode(asIScriptModule* pModule, ezDynamicArray<ezUInt8>& out_ByteCode);
  asIScriptModule* LoadFromByteCode(ezStringView sModuleName, ezDynamicArray<ezUInt8>& out_ByteCode) const;

private:
  static void MessageCallback(const asSMessageInfo* msg, void* param);

  void RegisterStandardTypes();
  void Register_RTTI();
  void Register_Vec2();
  void Register_Vec3();
  void Register_Vec4();
  void Register_Angle();
  void Register_Quat();
  void Register_Transform();
  void Register_GameObject();
  void Register_Component();
  void Register_Time();
  void Register_Mat3();
  void Register_Mat4();
  void Register_World();
  void Register_Clock();
  void Register_String();
  void Register_StringView();
  void Register_StringBuilder();
  void Register_TempHashedString();
  void Register_HashedString();
  void Register_Color();
  void Register_ColorGammaUB();
  void Register_Random();

  void Register_GlobalReflectedFunctions();
  void Register_ReflectedType(const ezRTTI* pBaseType, bool bCreatable);
  void Register_ReflectedTypes();
  void RegisterTypeFunctions(const char* szTypeName, const ezRTTI* pRtti);
  void RegisterTypeProperties(const char* szTypeName, const ezRTTI* pRtti);
  void RegisterGenericFunction(const char* szTypeName, const ezAbstractFunctionProperty* const pFunc);
  bool AppendType(ezStringBuilder& decl, const ezRTTI* pRtti);
  bool AppendFuncArgs(ezStringBuilder& decl, const ezAbstractFunctionProperty* pFunc, ezUInt32 uiArg);
  ezStringView Register_EnumType(const ezRTTI* pEnumType);


  template <typename T>
  void RegisterPodValueType()
  {
    const ezRTTI* pRtti = ezGetStaticRTTI<T>();
    int typeId = m_pEngine->RegisterObjectType(pRtti->GetTypeName().GetStartPointer(), sizeof(T), asOBJ_VALUE | asOBJ_POD | asGetTypeTraits<T>());
    AS_CHECK(typeId);

    m_pEngine->GetTypeInfoById(typeId)->SetUserData((void*)pRtti, ezAsUserData::RttiPtr);

    m_WhitelistedTypes.Insert(pRtti->GetTypeName());
  }

  template <typename T>
  void RegisterNonPodValueType()
  {
    const ezRTTI* pRtti = ezGetStaticRTTI<T>();
    int typeId = m_pEngine->RegisterObjectType(pRtti->GetTypeName().GetStartPointer(), sizeof(T), asOBJ_VALUE | asGetTypeTraits<T>());
    AS_CHECK(typeId);

    m_pEngine->GetTypeInfoById(typeId)->SetUserData((void*)pRtti, ezAsUserData::RttiPtr);

    m_WhitelistedTypes.Insert(pRtti->GetTypeName());
  }

  template <typename T>
  void RegisterRefType()
  {
    const ezRTTI* pRtti = ezGetStaticRTTI<T>();
    int typeId = m_pEngine->RegisterObjectType(pRtti->GetTypeName().GetStartPointer(), 0, asOBJ_REF | asOBJ_NOCOUNT);
    AS_CHECK(typeId);

    m_pEngine->GetTypeInfoById(typeId)->SetUserData((void*)pRtti, ezAsUserData::RttiPtr);

    AddForbiddenType(pRtti->GetTypeName().GetStartPointer());

    // m_WhitelistedTypes.Insert(pRtti->GetTypeName());
  }

  asIScriptEngine* m_pEngine = nullptr;
  ezSet<ezString> m_WhitelistedTypes;

  ezSet<ezString> m_ShownTypes;

  ezHybridArray<const asITypeInfo*, 16> m_ForbiddenTypes;

  asIStringFactory* m_pStringFactory = nullptr;
};
