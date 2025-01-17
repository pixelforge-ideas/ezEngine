#include <AngelScriptPlugin/AngelScriptPluginPCH.h>

#include <AngelScript/include/angelscript.h>
#include <AngelScriptPlugin/Runtime/AngelScriptEngineSingleton.h>
#include <Foundation/Strings/HashedString.h>

void ezAngelScriptEngineSingleton::RegisterStandardTypes()
{
  AS_CHECK(m_pEngine->RegisterObjectType("ezRTTI", 0, asOBJ_REF | asOBJ_NOCOUNT));

  RegisterPodValueType<ezVec2>();
  RegisterPodValueType<ezVec3>();
  RegisterPodValueType<ezVec4>();
  RegisterPodValueType<ezAngle>();
  RegisterPodValueType<ezQuat>();
  RegisterPodValueType<ezMat3>();
  RegisterPodValueType<ezMat4>();
  RegisterPodValueType<ezTransform>();
  RegisterPodValueType<ezTime>();
  RegisterPodValueType<ezColor>();
  RegisterPodValueType<ezColorGammaUB>();
  RegisterPodValueType<ezStringView>();
  RegisterPodValueType<ezGameObjectHandle>();
  RegisterPodValueType<ezComponentHandle>();
  RegisterPodValueType<ezTempHashedString>();
  RegisterPodValueType<ezHashedString>();

  RegisterNonPodValueType<ezString>();
  RegisterNonPodValueType<ezStringBuilder>();

  RegisterRefType<ezGameObject>();
  RegisterRefType<ezComponent>();
  RegisterRefType<ezWorld>();
  RegisterRefType<ezMessage>();
  RegisterRefType<ezClock>();

  AS_CHECK(m_pEngine->RegisterObjectType("ezRandom", 0, asOBJ_REF | asOBJ_NOCOUNT));

  Register_RTTI();
  Register_Vec2();
  Register_Vec3();
  Register_Vec4();
  Register_Angle();
  Register_Quat();
  Register_Transform();
  Register_GameObject();
  Register_Component();
  Register_Time();
  Register_Mat3();
  Register_Mat4();
  Register_World();
  Register_Clock();
  Register_StringView();
  Register_String();
  Register_StringBuilder();
  Register_HashedString();
  Register_TempHashedString();
  Register_Color();
  Register_ColorGammaUB();
  Register_Random();
  // ezBoundingBox
  // ezBoundingSphere
  // ezMath
  // ezPlane
}

//////////////////////////////////////////////////////////////////////////
// ezRTTI
//////////////////////////////////////////////////////////////////////////

const ezRTTI* ezRTTI_GetType(ezStringView name)
{
  return ezRTTI::FindTypeByName(name);
}

void ezAngelScriptEngineSingleton::Register_RTTI()
{
  // static functions
  {
    m_pEngine->SetDefaultNamespace("ezRTTI");

    AS_CHECK(m_pEngine->RegisterGlobalFunction("const ezRTTI@ GetType(ezStringView)", asFUNCTION(ezRTTI_GetType), asCALL_CDECL));

    m_pEngine->SetDefaultNamespace("");
  }
}

//////////////////////////////////////////////////////////////////////////
// ezVec2
//////////////////////////////////////////////////////////////////////////

static int ezVec2_opCmp(const ezVec2& lhs, const ezVec2& rhs)
{
  if (lhs < rhs)
    return -1;
  if (rhs < lhs)
    return +1;

  return 0;
}

static void ezVec2_Construct1(void* memory, float xyz)
{
  new (memory) ezVec2(xyz);
}

static void ezVec2_Construct2(void* memory, float x, float y)
{
  new (memory) ezVec2(x, y);
}

void ezAngelScriptEngineSingleton::Register_Vec2()
{
  AS_CHECK(m_pEngine->RegisterObjectProperty("ezVec2", "float x", asOFFSET(ezVec2, x)));
  AS_CHECK(m_pEngine->RegisterObjectProperty("ezVec2", "float y", asOFFSET(ezVec2, y)));

  // static functions
  {
    m_pEngine->SetDefaultNamespace("ezVec2");
    AS_CHECK(m_pEngine->RegisterGlobalFunction("ezVec2 MakeNaN()", asFUNCTION(ezVec2::MakeNaN<float>), asCALL_CDECL));
    AS_CHECK(m_pEngine->RegisterGlobalFunction("ezVec2 MakeZero()", asFUNCTION(ezVec2::MakeZero), asCALL_CDECL));
    m_pEngine->SetDefaultNamespace("");
  }

  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec2", "ezVec3 GetAsVec3(float) const", asMETHOD(ezVec2, GetAsVec3), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec2", "ezVec4 GetAsVec4(float, float) const", asMETHOD(ezVec2, GetAsVec4), asCALL_THISCALL));

  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec2", "void Set(float)", asMETHODPR(ezVec2, Set, (float), void), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec2", "void Set(float, float)", asMETHODPR(ezVec2, Set, (float, float), void), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec2", "void SetZero()", asMETHOD(ezVec2, SetZero), asCALL_THISCALL));

  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec2", "float GetLength() const", asMETHOD(ezVec2, GetLength<float>), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec2", "float GetDistanceTo() const", asMETHOD(ezVec2, GetDistanceTo<float>), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec2", "float GetSquaredDistanceTo() const", asMETHOD(ezVec2, GetSquaredDistanceTo<float>), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec2", "float GetLengthSquared() const", asMETHOD(ezVec2, GetLengthSquared), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec2", "float GetLengthAndNormalize()", asMETHOD(ezVec2, GetLengthAndNormalize<float>), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec2", "ezVec2 GetNormalized() const", asMETHOD(ezVec2, GetNormalized<float>), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec2", "void Normalize()", asMETHOD(ezVec2, Normalize<float>), asCALL_THISCALL));

  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec2", "bool IsZero() const", asMETHODPR(ezVec2, IsZero, () const, bool), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec2", "bool IsZero(float) const", asMETHODPR(ezVec2, IsZero, (float) const, bool), asCALL_THISCALL));

  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec2", "bool IsNormalized(float) const", asMETHODPR(ezVec2, IsNormalized, (float) const, bool), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec2", "bool IsNaN() const", asMETHOD(ezVec2, IsNaN), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec2", "bool IsValid() const", asMETHOD(ezVec2, IsValid), asCALL_THISCALL));

  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec2", "ezVec2 opAddAssign(ezVec2)", asMETHOD(ezVec2, operator+=), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec2", "ezVec2 opSubAssign(ezVec2)", asMETHOD(ezVec2, operator-=), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec2", "ezVec2 opMulAssign(float)", asMETHODPR(ezVec2, operator*=, (float), void), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec2", "ezVec2 opDivAssign(float)", asMETHODPR(ezVec2, operator/=, (float), void), asCALL_THISCALL));

  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec2", "bool IsIdentical(ezVec2) const", asMETHOD(ezVec2, IsIdentical), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec2", "bool IsEqual(ezVec2, float) const", asMETHOD(ezVec2, IsEqual), asCALL_THISCALL));

  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec2", "float Dot(ezVec2) const", asMETHOD(ezVec2, Dot), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec2", "ezVec2 CompMin(ezVec2) const", asMETHOD(ezVec2, CompMin), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec2", "ezVec2 CompMax(ezVec2) const", asMETHOD(ezVec2, CompMax), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec2", "ezVec2 CompClamp(ezVec2) const", asMETHOD(ezVec2, CompClamp), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec2", "ezVec2 CompMul(ezVec2) const", asMETHOD(ezVec2, CompMul), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec2", "ezVec2 CompDiv(ezVec2) const", asMETHOD(ezVec2, CompDiv), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec2", "ezVec2 Abs() const", asMETHOD(ezVec2, Abs), asCALL_THISCALL));

  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec2", "void MakeOrthogonalTo(ezVec2)", asMETHOD(ezVec2, MakeOrthogonalTo<float>), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec2", "ezVec2 GetOrthogonalVector()", asMETHOD(ezVec2, GetOrthogonalVector), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec2", "ezVec2 GetReflectedVector(ezVec2)", asMETHOD(ezVec2, GetReflectedVector<float>), asCALL_THISCALL));

  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec2", "ezVec2 opAdd(ezVec2) const", asFUNCTIONPR(operator+, (const ezVec2&, const ezVec2&), const ezVec2), asCALL_CDECL_OBJFIRST));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec2", "ezVec2 opSub(ezVec2) const", asFUNCTIONPR(operator-, (const ezVec2&, const ezVec2&), const ezVec2), asCALL_CDECL_OBJFIRST));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec2", "ezVec2 opMul(float) const", asFUNCTIONPR(operator*, (const ezVec2&, float), const ezVec2), asCALL_CDECL_OBJFIRST));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec2", "ezVec2 opMul_r(float) const", asFUNCTIONPR(operator*, (float, const ezVec2&), const ezVec2), asCALL_CDECL_OBJLAST));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec2", "ezVec2 opDiv(float) const", asFUNCTIONPR(operator/, (const ezVec2&, float), const ezVec2), asCALL_CDECL_OBJFIRST));

  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec2", "bool opEquals(ezVec2) const", asFUNCTIONPR(operator==, (const ezVec2&, const ezVec2&), bool), asCALL_CDECL_OBJFIRST));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec2", "int opCmp(ezVec2) const", asFUNCTIONPR(ezVec2_opCmp, (const ezVec2&, const ezVec2&), int), asCALL_CDECL_OBJFIRST));

  AS_CHECK(m_pEngine->RegisterObjectBehaviour("ezVec2", asBEHAVE_CONSTRUCT, "void f(float, float)", asFUNCTION(ezVec2_Construct2), asCALL_CDECL_OBJFIRST));
  AS_CHECK(m_pEngine->RegisterObjectBehaviour("ezVec2", asBEHAVE_CONSTRUCT, "void f(float)", asFUNCTION(ezVec2_Construct1), asCALL_CDECL_OBJFIRST));
}


//////////////////////////////////////////////////////////////////////////
// ezVec3
//////////////////////////////////////////////////////////////////////////

static int ezVec3_opCmp(const ezVec3& lhs, const ezVec3& rhs)
{
  if (lhs < rhs)
    return -1;
  if (rhs < lhs)
    return +1;

  return 0;
}

static void ezVec3_Construct1(void* memory, float xyz)
{
  new (memory) ezVec3(xyz);
}
static void ezVec3_Construct3(void* memory, float x, float y, float z)
{
  new (memory) ezVec3(x, y, z);
}

void ezAngelScriptEngineSingleton::Register_Vec3()
{
  AS_CHECK(m_pEngine->RegisterObjectProperty("ezVec3", "float x", asOFFSET(ezVec3, x)));
  AS_CHECK(m_pEngine->RegisterObjectProperty("ezVec3", "float y", asOFFSET(ezVec3, y)));
  AS_CHECK(m_pEngine->RegisterObjectProperty("ezVec3", "float z", asOFFSET(ezVec3, z)));

  // static functions
  {
    m_pEngine->SetDefaultNamespace("ezVec3");
    AS_CHECK(m_pEngine->RegisterGlobalFunction("ezVec3 MakeNaN()", asFUNCTION(ezVec3::MakeNaN<float>), asCALL_CDECL));
    AS_CHECK(m_pEngine->RegisterGlobalFunction("ezVec3 MakeZero()", asFUNCTION(ezVec3::MakeZero), asCALL_CDECL));
    AS_CHECK(m_pEngine->RegisterGlobalFunction("ezVec3 MakeAxisX()", asFUNCTION(ezVec3::MakeAxisX), asCALL_CDECL));
    AS_CHECK(m_pEngine->RegisterGlobalFunction("ezVec3 MakeAxisY()", asFUNCTION(ezVec3::MakeAxisY), asCALL_CDECL));
    AS_CHECK(m_pEngine->RegisterGlobalFunction("ezVec3 MakeAxisZ()", asFUNCTION(ezVec3::MakeAxisZ), asCALL_CDECL));
    AS_CHECK(m_pEngine->RegisterGlobalFunction("ezVec3 Make(float, float, float)", asFUNCTION(ezVec3::Make), asCALL_CDECL));
    m_pEngine->SetDefaultNamespace("");
  }

  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec3", "ezVec2 GetAsVec2() const", asMETHOD(ezVec3, GetAsVec2), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec3", "ezVec4 GetAsVec4(float) const", asMETHOD(ezVec3, GetAsVec4), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec3", "ezVec4 GetAsPositionVec4() const", asMETHOD(ezVec3, GetAsPositionVec4), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec3", "ezVec4 GetAsDirectionVec4() const", asMETHOD(ezVec3, GetAsDirectionVec4), asCALL_THISCALL));

  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec3", "void Set(float)", asMETHODPR(ezVec3, Set, (float), void), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec3", "void Set(float, float, float)", asMETHODPR(ezVec3, Set, (float, float, float), void), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec3", "void SetZero()", asMETHOD(ezVec3, SetZero), asCALL_THISCALL));

  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec3", "float GetLength() const", asMETHOD(ezVec3, GetLength<float>), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec3", "float GetDistanceTo() const", asMETHOD(ezVec3, GetDistanceTo<float>), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec3", "float GetSquaredDistanceTo() const", asMETHOD(ezVec3, GetSquaredDistanceTo<float>), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec3", "float GetLengthSquared() const", asMETHOD(ezVec3, GetLengthSquared), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec3", "float GetLengthAndNormalize()", asMETHOD(ezVec3, GetLengthAndNormalize<float>), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec3", "ezVec3 GetNormalized() const", asMETHOD(ezVec3, GetNormalized<float>), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec3", "void Normalize()", asMETHOD(ezVec3, Normalize<float>), asCALL_THISCALL));

  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec3", "bool IsZero() const", asMETHODPR(ezVec3, IsZero, () const, bool), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec3", "bool IsZero(float) const", asMETHODPR(ezVec3, IsZero, (float) const, bool), asCALL_THISCALL));

  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec3", "bool IsNormalized(float) const", asMETHODPR(ezVec3, IsNormalized, (float) const, bool), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec3", "bool IsNaN() const", asMETHOD(ezVec3, IsNaN), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec3", "bool IsValid() const", asMETHOD(ezVec3, IsValid), asCALL_THISCALL));

  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec3", "ezVec3 opAddAssign(ezVec3)", asMETHOD(ezVec3, operator+=), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec3", "ezVec3 opSubAssign(ezVec3)", asMETHOD(ezVec3, operator-=), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec3", "ezVec3 opMulAssign(ezVec3)", asMETHODPR(ezVec3, operator*=, (const ezVec3&), void), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec3", "ezVec3 opDivAssign(ezVec3)", asMETHODPR(ezVec3, operator/=, (const ezVec3&), void), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec3", "ezVec3 opMulAssign(float)", asMETHODPR(ezVec3, operator*=, (float), void), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec3", "ezVec3 opDivAssign(float)", asMETHODPR(ezVec3, operator/=, (float), void), asCALL_THISCALL));

  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec3", "bool IsIdentical(ezVec3) const", asMETHOD(ezVec3, IsIdentical), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec3", "bool IsEqual(ezVec3, float) const", asMETHOD(ezVec3, IsEqual), asCALL_THISCALL));

  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec3", "float Dot(ezVec3) const", asMETHOD(ezVec3, Dot), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec3", "ezVec3 CrossRH(ezVec3) const", asMETHOD(ezVec3, CrossRH), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec3", "ezVec3 CompMin(ezVec3) const", asMETHOD(ezVec3, CompMin), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec3", "ezVec3 CompMax(ezVec3) const", asMETHOD(ezVec3, CompMax), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec3", "ezVec3 CompClamp(ezVec3) const", asMETHOD(ezVec3, CompClamp), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec3", "ezVec3 CompMul(ezVec3) const", asMETHOD(ezVec3, CompMul), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec3", "ezVec3 CompDiv(ezVec3) const", asMETHOD(ezVec3, CompDiv), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec3", "ezVec3 Abs() const", asMETHOD(ezVec3, Abs), asCALL_THISCALL));

  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec3", "void MakeOrthogonalTo(ezVec3)", asMETHOD(ezVec3, MakeOrthogonalTo<float>), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec3", "ezVec3 GetOrthogonalVector()", asMETHOD(ezVec3, GetOrthogonalVector<float>), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec3", "ezVec3 GetReflectedVector(ezVec3)", asMETHOD(ezVec3, GetReflectedVector<float>), asCALL_THISCALL));

  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec3", "ezVec3 opAdd(ezVec3) const", asFUNCTIONPR(operator+, (const ezVec3&, const ezVec3&), const ezVec3), asCALL_CDECL_OBJFIRST));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec3", "ezVec3 opSub(ezVec3) const", asFUNCTIONPR(operator-, (const ezVec3&, const ezVec3&), const ezVec3), asCALL_CDECL_OBJFIRST));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec3", "ezVec3 opMul(float) const", asFUNCTIONPR(operator*, (const ezVec3&, float), const ezVec3), asCALL_CDECL_OBJFIRST));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec3", "ezVec3 opMul_r(float) const", asFUNCTIONPR(operator*, (float, const ezVec3&), const ezVec3), asCALL_CDECL_OBJLAST));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec3", "ezVec3 opDiv(float) const", asFUNCTIONPR(operator/, (const ezVec3&, float), const ezVec3), asCALL_CDECL_OBJFIRST));

  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec3", "bool opEquals(ezVec3) const", asFUNCTIONPR(operator==, (const ezVec3&, const ezVec3&), bool), asCALL_CDECL_OBJFIRST));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec3", "int opCmp(ezVec3) const", asFUNCTIONPR(ezVec3_opCmp, (const ezVec3&, const ezVec3&), int), asCALL_CDECL_OBJFIRST));

  AS_CHECK(m_pEngine->RegisterObjectBehaviour("ezVec3", asBEHAVE_CONSTRUCT, "void f(float, float, float)", asFUNCTION(ezVec3_Construct3), asCALL_CDECL_OBJFIRST));
  AS_CHECK(m_pEngine->RegisterObjectBehaviour("ezVec3", asBEHAVE_CONSTRUCT, "void f(float)", asFUNCTION(ezVec3_Construct1), asCALL_CDECL_OBJFIRST));
}


//////////////////////////////////////////////////////////////////////////
// ezVec4
//////////////////////////////////////////////////////////////////////////

static int ezVec4_opCmp(const ezVec4& lhs, const ezVec4& rhs)
{
  if (lhs < rhs)
    return -1;
  if (rhs < lhs)
    return +1;

  return 0;
}

static void ezVec4_Construct1(void* memory, float xyzw)
{
  new (memory) ezVec4(xyzw);
}
static void ezVec4_Construct4(void* memory, float x, float y, float z, float w)
{
  new (memory) ezVec4(x, y, z, w);
}

void ezAngelScriptEngineSingleton::Register_Vec4()
{
  AS_CHECK(m_pEngine->RegisterObjectProperty("ezVec4", "float x", asOFFSET(ezVec4, x)));
  AS_CHECK(m_pEngine->RegisterObjectProperty("ezVec4", "float y", asOFFSET(ezVec4, y)));
  AS_CHECK(m_pEngine->RegisterObjectProperty("ezVec4", "float z", asOFFSET(ezVec4, z)));
  AS_CHECK(m_pEngine->RegisterObjectProperty("ezVec4", "float w", asOFFSET(ezVec4, w)));

  // static functions
  {
    m_pEngine->SetDefaultNamespace("ezVec4");
    AS_CHECK(m_pEngine->RegisterGlobalFunction("ezVec4 MakeNaN()", asFUNCTION(ezVec4::MakeNaN<float>), asCALL_CDECL));
    AS_CHECK(m_pEngine->RegisterGlobalFunction("ezVec4 MakeZero()", asFUNCTION(ezVec4::MakeZero), asCALL_CDECL));
    m_pEngine->SetDefaultNamespace("");
  }

  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec4", "ezVec2 GetAsVec2() const", asMETHOD(ezVec4, GetAsVec2), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec4", "ezVec3 GetAsVec3() const", asMETHOD(ezVec4, GetAsVec3), asCALL_THISCALL));

  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec4", "void Set(float)", asMETHODPR(ezVec4, Set, (float), void), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec4", "void Set(float, float, float, float)", asMETHODPR(ezVec4, Set, (float, float, float, float), void), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec4", "void SetZero()", asMETHOD(ezVec4, SetZero), asCALL_THISCALL));

  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec4", "float GetLength() const", asMETHOD(ezVec4, GetLength<float>), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec4", "float GetLengthSquared() const", asMETHOD(ezVec4, GetLengthSquared), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec4", "float GetLengthAndNormalize()", asMETHOD(ezVec4, GetLengthAndNormalize<float>), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec4", "ezVec4 GetNormalized() const", asMETHOD(ezVec4, GetNormalized<float>), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec4", "void Normalize()", asMETHOD(ezVec4, Normalize<float>), asCALL_THISCALL));

  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec4", "bool IsZero() const", asMETHODPR(ezVec4, IsZero, () const, bool), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec4", "bool IsZero(float) const", asMETHODPR(ezVec4, IsZero, (float) const, bool), asCALL_THISCALL));

  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec4", "bool IsNormalized(float) const", asMETHODPR(ezVec4, IsNormalized, (float) const, bool), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec4", "bool IsNaN() const", asMETHOD(ezVec4, IsNaN), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec4", "bool IsValid() const", asMETHOD(ezVec4, IsValid), asCALL_THISCALL));

  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec4", "ezVec4 opAddAssign(ezVec4)", asMETHOD(ezVec4, operator+=), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec4", "ezVec4 opSubAssign(ezVec4)", asMETHOD(ezVec4, operator-=), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec4", "ezVec4 opMulAssign(float)", asMETHODPR(ezVec4, operator*=, (float), void), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec4", "ezVec4 opDivAssign(float)", asMETHODPR(ezVec4, operator/=, (float), void), asCALL_THISCALL));

  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec4", "bool IsIdentical(ezVec4) const", asMETHOD(ezVec4, IsIdentical), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec4", "bool IsEqual(ezVec4, float) const", asMETHOD(ezVec4, IsEqual), asCALL_THISCALL));

  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec4", "float Dot(ezVec4) const", asMETHOD(ezVec4, Dot), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec4", "ezVec4 CompMin(ezVec4) const", asMETHOD(ezVec4, CompMin), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec4", "ezVec4 CompMax(ezVec4) const", asMETHOD(ezVec4, CompMax), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec4", "ezVec4 CompClamp(ezVec4) const", asMETHOD(ezVec4, CompClamp), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec4", "ezVec4 CompMul(ezVec4) const", asMETHOD(ezVec4, CompMul), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec4", "ezVec4 CompDiv(ezVec4) const", asMETHOD(ezVec4, CompDiv), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec4", "ezVec4 Abs() const", asMETHOD(ezVec4, Abs), asCALL_THISCALL));

  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec4", "ezVec4 opAdd(ezVec4) const", asFUNCTIONPR(operator+, (const ezVec4&, const ezVec4&), const ezVec4), asCALL_CDECL_OBJFIRST));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec4", "ezVec4 opSub(ezVec4) const", asFUNCTIONPR(operator-, (const ezVec4&, const ezVec4&), const ezVec4), asCALL_CDECL_OBJFIRST));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec4", "ezVec4 opMul(float) const", asFUNCTIONPR(operator*, (const ezVec4&, float), const ezVec4), asCALL_CDECL_OBJFIRST));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec4", "ezVec4 opMul_r(float) const", asFUNCTIONPR(operator*, (float, const ezVec4&), const ezVec4), asCALL_CDECL_OBJLAST));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec4", "ezVec4 opDiv(float) const", asFUNCTIONPR(operator/, (const ezVec4&, float), const ezVec4), asCALL_CDECL_OBJFIRST));

  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec4", "bool opEquals(ezVec4) const", asFUNCTIONPR(operator==, (const ezVec4&, const ezVec4&), bool), asCALL_CDECL_OBJFIRST));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezVec4", "int opCmp(ezVec4) const", asFUNCTIONPR(ezVec4_opCmp, (const ezVec4&, const ezVec4&), int), asCALL_CDECL_OBJFIRST));

  AS_CHECK(m_pEngine->RegisterObjectBehaviour("ezVec4", asBEHAVE_CONSTRUCT, "void f(float, float, float, float)", asFUNCTION(ezVec4_Construct4), asCALL_CDECL_OBJFIRST));
  AS_CHECK(m_pEngine->RegisterObjectBehaviour("ezVec4", asBEHAVE_CONSTRUCT, "void f(float)", asFUNCTION(ezVec4_Construct1), asCALL_CDECL_OBJFIRST));
}

//////////////////////////////////////////////////////////////////////////
// ezGameObject
//////////////////////////////////////////////////////////////////////////

void ezGameObject_TryGetComponentOfBaseType(asIScriptGeneric* gen)
{
  ezGameObject* pObj = (ezGameObject*)gen->GetObject();
  int typeId = gen->GetArgTypeId(0);

  if (auto info = gen->GetEngine()->GetTypeInfoById(typeId))
  {
    if (const ezRTTI* pRtti = static_cast<const ezRTTI*>(info->GetUserData(ezAsUserData::RttiPtr)))
    {
      ezComponent* pComponent;
      if (pObj->TryGetComponentOfBaseType(pRtti, pComponent))
      {
        ezComponent** ref = (ezComponent**)gen->GetArgAddress(0);
        *ref = pComponent;

        gen->SetReturnByte(1);
        return;
      }
    }
  }

  gen->SetReturnByte(0);
}

void ezAngelScriptEngineSingleton::Register_GameObject()
{
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezGameObjectHandle", "void Invalidate()", asMETHOD(ezGameObjectHandle, Invalidate), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezGameObjectHandle", "bool IsInvalidated() const", asMETHOD(ezGameObjectHandle, IsInvalidated), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezGameObjectHandle", "bool opEquals(ezGameObjectHandle) const", asMETHODPR(ezGameObjectHandle, operator==, (ezGameObjectHandle) const, bool), asCALL_THISCALL));

  AS_CHECK(m_pEngine->RegisterObjectMethod("ezComponentHandle", "void Invalidate()", asMETHOD(ezComponentHandle, Invalidate), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezComponentHandle", "bool IsInvalidated() const", asMETHOD(ezComponentHandle, IsInvalidated), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezComponentHandle", "bool opEquals(ezComponentHandle) const", asMETHODPR(ezComponentHandle, operator==, (ezComponentHandle) const, bool), asCALL_THISCALL));

  AS_CHECK(m_pEngine->RegisterObjectMethod("ezGameObject", "ezGameObjectHandle GetHandle() const", asMETHOD(ezGameObject, GetHandle), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezGameObject", "void MakeDynamic()", asMETHOD(ezGameObject, MakeDynamic), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezGameObject", "void MakeStatic()", asMETHOD(ezGameObject, MakeStatic), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezGameObject", "bool IsDynamic() const", asMETHOD(ezGameObject, IsDynamic), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezGameObject", "bool IsStatic() const", asMETHOD(ezGameObject, IsStatic), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezGameObject", "void SetActiveFlag(bool)", asMETHOD(ezGameObject, SetActiveFlag), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezGameObject", "bool GetActiveFlag() const", asMETHOD(ezGameObject, GetActiveFlag), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezGameObject", "bool IsActive() const", asMETHOD(ezGameObject, IsActive), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezGameObject", "void SetName(ezStringView)", asMETHODPR(ezGameObject, SetName, (ezStringView), void), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezGameObject", "void SetName(const ezHashedString& in)", asMETHODPR(ezGameObject, SetName, (const ezHashedString&), void), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezGameObject", "ezStringView GetName() const", asMETHOD(ezGameObject, GetName), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezGameObject", "bool HasName(const ezTempHashedString& in) const", asMETHOD(ezGameObject, HasName), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezGameObject", "void SetGlobalKey(ezStringView)", asMETHODPR(ezGameObject, SetGlobalKey, (ezStringView), void), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezGameObject", "void SetGlobalKey(const ezHashedString& in)", asMETHODPR(ezGameObject, SetGlobalKey, (const ezHashedString&), void), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezGameObject", "ezStringView GetGlobalKey() const", asMETHOD(ezGameObject, GetGlobalKey), asCALL_THISCALL));
  // AS_CHECK(m_pEngine->RegisterObjectMethod("ezGameObject", "void SetParent()", asMETHOD(ezGameObject, SetParent), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezGameObject", "ezGameObject@ GetParent() const", asMETHODPR(ezGameObject, GetParent, () const, const ezGameObject*), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezGameObject", "ezGameObject@ GetParent()", asMETHODPR(ezGameObject, GetParent, (), ezGameObject*), asCALL_THISCALL));
  // AS_CHECK(m_pEngine->RegisterObjectMethod("ezGameObject", "void AddChild()", asMETHOD(ezGameObject, AddChild), asCALL_THISCALL));
  // AS_CHECK(m_pEngine->RegisterObjectMethod("ezGameObject", "void AddChildren()", asMETHOD(ezGameObject, AddChildren), asCALL_THISCALL));
  // AS_CHECK(m_pEngine->RegisterObjectMethod("ezGameObject", "void DetachChild()", asMETHOD(ezGameObject, DetachChild), asCALL_THISCALL));
  // AS_CHECK(m_pEngine->RegisterObjectMethod("ezGameObject", "void DetachChildren()", asMETHOD(ezGameObject, DetachChildren), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezGameObject", "uint32 GetChildCount()", asMETHOD(ezGameObject, GetChildCount), asCALL_THISCALL));
  // AS_CHECK(m_pEngine->RegisterObjectMethod("ezGameObject", "void GetChildren()", asMETHOD(ezGameObject, GetChildren), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezGameObject", "ezGameObject@ FindChildByName(const ezTempHashedString& in, bool recursive = true)", asMETHOD(ezGameObject, FindChildByName), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezGameObject", "ezGameObject@ FindChildByPath(ezStringView)", asMETHOD(ezGameObject, FindChildByPath), asCALL_THISCALL));
  // AS_CHECK(m_pEngine->RegisterObjectMethod("ezGameObject", "ezGameObject@ SearchForChildByNameSequence(string)", asMETHOD(ezGameObject, SearchForChildByNameSequence), asCALL_THISCALL));
  // AS_CHECK(m_pEngine->RegisterObjectMethod("ezGameObject", "ezGameObject@ SearchForChildrenByNameSequence(string)", asMETHOD(ezGameObject, SearchForChildrenByNameSequence), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezGameObject", "ezWorld@ GetWorld()", asMETHODPR(ezGameObject, GetWorld, (), ezWorld*), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezGameObject", "const ezWorld@ GetWorld() const", asMETHODPR(ezGameObject, GetWorld, () const, const ezWorld*), asCALL_THISCALL));

  AS_CHECK(m_pEngine->RegisterObjectMethod("ezGameObject", "void SetLocalPosition(ezVec3)", asMETHODPR(ezGameObject, SetLocalPosition, (ezVec3), void), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezGameObject", "ezVec3 GetLocalPosition() const", asMETHOD(ezGameObject, GetLocalPosition), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezGameObject", "void SetLocalRotation(ezQuat)", asMETHODPR(ezGameObject, SetLocalRotation, (ezQuat), void), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezGameObject", "ezQuat GetLocalRotation() const", asMETHOD(ezGameObject, GetLocalRotation), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezGameObject", "void SetLocalScaling(ezVec3)", asMETHODPR(ezGameObject, SetLocalScaling, (ezVec3), void), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezGameObject", "ezVec3 GetLocalScaling() const", asMETHOD(ezGameObject, GetLocalScaling), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezGameObject", "void SetLocalUniformScaling(float)", asMETHODPR(ezGameObject, SetLocalUniformScaling, (float), void), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezGameObject", "float GetLocalUniformScaling() const", asMETHOD(ezGameObject, GetLocalUniformScaling), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezGameObject", "ezTransform GetLocalTransform() const", asMETHOD(ezGameObject, GetLocalTransform), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezGameObject", "void SetGlobalPosition(ezVec3)", asMETHODPR(ezGameObject, SetGlobalPosition, (const ezVec3&), void), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezGameObject", "ezVec3 GetGlobalPosition() const", asMETHOD(ezGameObject, GetGlobalPosition), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezGameObject", "void SetGlobalRotation(ezQuat)", asMETHODPR(ezGameObject, SetGlobalRotation, (const ezQuat&), void), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezGameObject", "void SetGlobalScaling(ezVec3)", asMETHODPR(ezGameObject, SetGlobalScaling, (const ezVec3&), void), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezGameObject", "ezVec3 GetGlobalScaling() const", asMETHOD(ezGameObject, GetGlobalScaling), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezGameObject", "void SetGlobalTransform(ezTransform)", asMETHODPR(ezGameObject, SetGlobalTransform, (const ezTransform&), void), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezGameObject", "ezTransform GetGlobalTransform() const", asMETHOD(ezGameObject, GetGlobalTransform), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezGameObject", "ezTransform GetLastGlobalTransform() const", asMETHOD(ezGameObject, GetLastGlobalTransform), asCALL_THISCALL));

  AS_CHECK(m_pEngine->RegisterObjectMethod("ezGameObject", "ezVec3 GetGlobalDirForwards() const", asMETHOD(ezGameObject, GetGlobalDirForwards), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezGameObject", "ezVec3 GetGlobalDirRight() const", asMETHOD(ezGameObject, GetGlobalDirRight), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezGameObject", "ezVec3 GetGlobalDirUp() const", asMETHOD(ezGameObject, GetGlobalDirUp), asCALL_THISCALL));

#if EZ_ENABLED(EZ_GAMEOBJECT_VELOCITY)
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezGameObject", "ezVec3 GetLinearVelocity() const", asMETHOD(ezGameObject, GetLinearVelocity), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezGameObject", "ezVec3 GetAngularVelocity() const", asMETHOD(ezGameObject, GetAngularVelocity), asCALL_THISCALL));
#endif

  AS_CHECK(m_pEngine->RegisterObjectMethod("ezGameObject", "void UpdateGlobalTransform()", asMETHOD(ezGameObject, UpdateGlobalTransform), asCALL_THISCALL));

  // AS_CHECK(m_pEngine->RegisterObjectMethod("ezGameObject", "ezBoundingBoxSphere GetLocalBounds() const", asMETHOD(ezGameObject, GetLocalBounds), asCALL_THISCALL));
  // AS_CHECK(m_pEngine->RegisterObjectMethod("ezGameObject", "ezBoundingBoxSphere GetGlobalBounds() const", asMETHOD(ezGameObject, GetGlobalBounds), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezGameObject", "void UpdateLocalBounds()", asMETHOD(ezGameObject, UpdateLocalBounds), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezGameObject", "void UpdateGlobalBounds()", asMETHOD(ezGameObject, UpdateGlobalBounds), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezGameObject", "void UpdateGlobalTransformAndBounds()", asMETHOD(ezGameObject, UpdateGlobalTransformAndBounds), asCALL_THISCALL));

  AS_CHECK(m_pEngine->RegisterObjectMethod("ezGameObject", "bool TryGetComponentOfBaseType(const ezRTTI@, ezComponent@& out)", asMETHODPR(ezGameObject, TryGetComponentOfBaseType, (const ezRTTI*, ezComponent*&), bool), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezGameObject", "bool TryGetComponentOfBaseType(?& out)", asFUNCTION(ezGameObject_TryGetComponentOfBaseType), asCALL_GENERIC));
  // TryGetComponentOfBaseType


  // AS_CHECK(m_pEngine->RegisterObjectMethod("ezGameObject", "void GetComponents()", asMETHOD(ezGameObject, GetComponents), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezGameObject", "uint16 GetComponentVersion()", asMETHOD(ezGameObject, GetComponentVersion), asCALL_THISCALL));

  AS_CHECK(m_pEngine->RegisterObjectMethod("ezGameObject", "bool SendMessage(ezMessage& inout)", asMETHODPR(ezGameObject, SendMessage, (ezMessage&), bool), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezGameObject", "bool SendMessage(ezMessage& inout) const", asMETHODPR(ezGameObject, SendMessage, (ezMessage&) const, bool), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezGameObject", "bool SendMessageRecursive(ezMessage& inout)", asMETHODPR(ezGameObject, SendMessageRecursive, (ezMessage&), bool), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezGameObject", "bool SendMessageRecursive(ezMessage& inout) const", asMETHODPR(ezGameObject, SendMessageRecursive, (ezMessage&) const, bool), asCALL_THISCALL));

  AS_CHECK(m_pEngine->RegisterObjectMethod("ezGameObject", "void PostMessage(const ezMessage& in, ezTime, uint8 type = 2) const", asMETHOD(ezGameObject, PostMessage), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezGameObject", "void PostMessageRecursive(const ezMessage& in, ezTime, uint8 type = 2) const", asMETHOD(ezGameObject, PostMessageRecursive), asCALL_THISCALL));

  AS_CHECK(m_pEngine->RegisterObjectMethod("ezGameObject", "bool SendEventMessage(ezMessage& inout, const ezComponent@ sender)", asMETHODPR(ezGameObject, SendEventMessage, (ezMessage&, const ezComponent*), bool), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezGameObject", "bool SendEventMessage(ezMessage& inout, const ezComponent@ sender) const", asMETHODPR(ezGameObject, SendEventMessage, (ezMessage&, const ezComponent*) const, bool), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezGameObject", "bool PostEventMessage(ezMessage& inout, const ezComponent@ sender, ezTime delay, uint8 type = 2) const", asMETHOD(ezGameObject, PostEventMessage), asCALL_THISCALL));

  // GetTags
  // SetTags
  // SetTag
  // RemoveTag
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezGameObject", "bool HasTag(const ezTempHashedString& in) const", asMETHOD(ezGameObject, HasTag), asCALL_THISCALL));

  AS_CHECK(m_pEngine->RegisterObjectMethod("ezGameObject", "uint16 GetTeamID()", asMETHOD(ezGameObject, GetTeamID), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezGameObject", "void SetTeamID(uint16)", asMETHOD(ezGameObject, SetTeamID), asCALL_THISCALL));

  AS_CHECK(m_pEngine->RegisterObjectMethod("ezGameObject", "uint32 GetStableRandomSeed()", asMETHOD(ezGameObject, GetStableRandomSeed), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezGameObject", "void SetStableRandomSeed(uint32)", asMETHOD(ezGameObject, SetStableRandomSeed), asCALL_THISCALL));

  // GetVisibilityState

  // TODO AngelScript: Register ezGameObject
}


void ezAngelScriptEngineSingleton::Register_Component()
{
  // TODO AngelScript: Register ezComponent ?
  // TODO AS: all these should just be registered through reflection

  // AS_CHECK(m_pEngine->RegisterObjectMethod("ezComponent", "void SetActiveFlag(bool)", asMETHOD(ezComponent, SetActiveFlag), asCALL_THISCALL));
  // AS_CHECK(m_pEngine->RegisterObjectMethod("ezComponent", "bool GetActiveFlag() const", asMETHOD(ezComponent, GetActiveFlag), asCALL_THISCALL));
  // AS_CHECK(m_pEngine->RegisterObjectMethod("ezComponent", "bool IsActive() const", asMETHOD(ezComponent, IsActive), asCALL_THISCALL));
  // AS_CHECK(m_pEngine->RegisterObjectMethod("ezComponent", "bool IsActiveAndInitialized() const", asMETHOD(ezComponent, IsActiveAndInitialized), asCALL_THISCALL));
  // AS_CHECK(m_pEngine->RegisterObjectMethod("ezComponent", "bool IsActiveAndSimulating() const", asMETHOD(ezComponent, IsActiveAndSimulating), asCALL_THISCALL));
  // AS_CHECK(m_pEngine->RegisterObjectMethod("ezComponent", "const ezGameObject@ GetOwner() const", asMETHODPR(ezComponent, GetOwner, () const, const ezGameObject*), asCALL_THISCALL));
  // AS_CHECK(m_pEngine->RegisterObjectMethod("ezComponent", "ezGameObject@ GetOwner()", asMETHODPR(ezComponent, GetOwner, (), ezGameObject*), asCALL_THISCALL));

  // AS_CHECK(m_pEngine->RegisterObjectMethod("ezComponent", "const ezWorld@ GetWorld() const", asMETHODPR(ezComponent, GetWorld, () const, const ezWorld*), asCALL_THISCALL));
  // AS_CHECK(m_pEngine->RegisterObjectMethod("ezComponent", "ezWorld@ GetWorld()", asMETHODPR(ezComponent, GetWorld, (), ezWorld*), asCALL_THISCALL));

  // AS_CHECK(m_pEngine->RegisterObjectMethod("ezComponent", "uint32 GetUniqueID() const", asMETHOD(ezComponent, GetUniqueID), asCALL_THISCALL));
  // AS_CHECK(m_pEngine->RegisterObjectMethod("ezComponent", "void SetUniqueID(uint32)", asMETHOD(ezComponent, SetUniqueID), asCALL_THISCALL));
}

//////////////////////////////////////////////////////////////////////////
// ezAngle
//////////////////////////////////////////////////////////////////////////

static int ezAngle_opCmp(const ezAngle& lhs, const ezAngle& rhs)
{
  if (lhs < rhs)
    return -1;
  if (rhs < lhs)
    return +1;

  return 0;
}

void ezAngelScriptEngineSingleton::Register_Angle()
{
  // static functions
  {
    m_pEngine->SetDefaultNamespace("ezAngle");

    AS_CHECK(m_pEngine->RegisterGlobalFunction("float DegToRad(float)", asFUNCTION(ezAngle::DegToRad<float>), asCALL_CDECL));
    AS_CHECK(m_pEngine->RegisterGlobalFunction("float RadToDeg(float)", asFUNCTION(ezAngle::RadToDeg<float>), asCALL_CDECL));
    AS_CHECK(m_pEngine->RegisterGlobalFunction("ezAngle MakeZero()", asFUNCTION(ezAngle::MakeZero), asCALL_CDECL));
    AS_CHECK(m_pEngine->RegisterGlobalFunction("ezAngle MakeFromDegree(float)", asFUNCTION(ezAngle::MakeFromDegree), asCALL_CDECL));
    AS_CHECK(m_pEngine->RegisterGlobalFunction("ezAngle MakeFromRadian(float)", asFUNCTION(ezAngle::MakeFromRadian), asCALL_CDECL));
    AS_CHECK(m_pEngine->RegisterGlobalFunction("ezAngle AngleBetween(ezAngle, ezAngle)", asFUNCTION(ezAngle::AngleBetween), asCALL_CDECL));

    m_pEngine->SetDefaultNamespace("");
  }

  AS_CHECK(m_pEngine->RegisterObjectMethod("ezAngle", "float GetDegree() const", asMETHOD(ezAngle, GetDegree), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezAngle", "float GetRadian() const", asMETHOD(ezAngle, GetRadian), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezAngle", "void SetRadian(float)", asMETHOD(ezAngle, SetRadian), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezAngle", "void NormalizeRange()", asMETHOD(ezAngle, NormalizeRange), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezAngle", "ezAngle GetNormalizedRange() const", asMETHOD(ezAngle, GetNormalizedRange), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezAngle", "bool IsEqualSimple(ezAngle rhs, ezAngle epsilon) const", asMETHOD(ezAngle, IsEqualSimple), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezAngle", "bool IsEqualNormalized(ezAngle rhs, ezAngle epsilon) const", asMETHOD(ezAngle, IsEqualNormalized), asCALL_THISCALL));

  AS_CHECK(m_pEngine->RegisterObjectMethod("ezAngle", "ezAngle opNeg() const", asMETHODPR(ezAngle, operator-, () const, ezAngle), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezAngle", "ezAngle opAdd(ezAngle) const", asMETHODPR(ezAngle, operator+, (ezAngle) const, ezAngle), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezAngle", "ezAngle opSub(ezAngle) const", asMETHODPR(ezAngle, operator-, (ezAngle) const, ezAngle), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezAngle", "ezAngle opAddAssign(ezAngle)", asMETHOD(ezAngle, operator+=), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezAngle", "ezAngle opSubAssign(ezAngle)", asMETHOD(ezAngle, operator-=), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezAngle", "bool opEquals(ezAngle) const", asMETHODPR(ezAngle, operator==, (const ezAngle&) const, bool), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezAngle", "int opCmp(ezAngle) const", asFUNCTIONPR(ezAngle_opCmp, (const ezAngle&, const ezAngle&), int), asCALL_CDECL_OBJFIRST));

  AS_CHECK(m_pEngine->RegisterObjectMethod("ezAngle", "ezAngle opMul(float) const", asFUNCTIONPR(operator*, (ezAngle, float), ezAngle), asCALL_CDECL_OBJFIRST));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezAngle", "ezAngle opDiv(float) const", asFUNCTIONPR(operator/, (ezAngle, float), ezAngle), asCALL_CDECL_OBJFIRST));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezAngle", "float opDiv(ezAngle) const", asFUNCTIONPR(operator/, (ezAngle, ezAngle), float), asCALL_CDECL_OBJFIRST));
}

//////////////////////////////////////////////////////////////////////////
// ezQuat
//////////////////////////////////////////////////////////////////////////

void ezAngelScriptEngineSingleton::Register_Quat()
{
  AS_CHECK(m_pEngine->RegisterObjectProperty("ezQuat", "float x", asOFFSET(ezQuat, x)));
  AS_CHECK(m_pEngine->RegisterObjectProperty("ezQuat", "float y", asOFFSET(ezQuat, y)));
  AS_CHECK(m_pEngine->RegisterObjectProperty("ezQuat", "float z", asOFFSET(ezQuat, z)));
  AS_CHECK(m_pEngine->RegisterObjectProperty("ezQuat", "float w", asOFFSET(ezQuat, w)));

  // static functions
  {
    m_pEngine->SetDefaultNamespace("ezQuat");

    AS_CHECK(m_pEngine->RegisterGlobalFunction("ezQuat MakeIdentity()", asFUNCTION(ezQuat::MakeIdentity), asCALL_CDECL));
    AS_CHECK(m_pEngine->RegisterGlobalFunction("ezQuat MakeFromElements(float, float, float, float)", asFUNCTION(ezQuat::MakeFromElements), asCALL_CDECL));
    AS_CHECK(m_pEngine->RegisterGlobalFunction("ezQuat MakeFromAxisAndAngle(ezVec3, ezAngle)", asFUNCTION(ezQuat::MakeFromAxisAndAngle), asCALL_CDECL));
    AS_CHECK(m_pEngine->RegisterGlobalFunction("ezQuat MakeShortestRotation(ezVec3, ezVec3)", asFUNCTION(ezQuat::MakeShortestRotation), asCALL_CDECL));
    AS_CHECK(m_pEngine->RegisterGlobalFunction("ezQuat MakeFromMat3(ezMat3)", asFUNCTION(ezQuat::MakeFromMat3), asCALL_CDECL));
    AS_CHECK(m_pEngine->RegisterGlobalFunction("ezQuat MakeSlerp(ezQuat, ezQuat, float)", asFUNCTION(ezQuat::MakeSlerp), asCALL_CDECL));
    AS_CHECK(m_pEngine->RegisterGlobalFunction("ezQuat MakeFromEulerAngles(ezAngle, ezAngle, ezAngle)", asFUNCTION(ezQuat::MakeFromEulerAngles), asCALL_CDECL));

    m_pEngine->SetDefaultNamespace("");
  }

  AS_CHECK(m_pEngine->RegisterObjectMethod("ezQuat", "void SetIdentity()", asMETHOD(ezQuat, SetIdentity), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezQuat", "void ReconstructFromMat3(ezMat3)", asMETHOD(ezQuat, ReconstructFromMat3), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezQuat", "void ReconstructFromMat4(ezMat3)", asMETHOD(ezQuat, ReconstructFromMat4), asCALL_THISCALL));

  AS_CHECK(m_pEngine->RegisterObjectMethod("ezQuat", "void Normalize()", asMETHOD(ezQuat, Normalize), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezQuat", "void GetRotationAxisAndAngle(ezVec3& out, ezAngle& out, float eps = 0.00001) const", asMETHOD(ezQuat, GetRotationAxisAndAngle), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezQuat", "ezVec3 GetVectorPart() const", asMETHOD(ezQuat, GetVectorPart), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezQuat", "ezMat3 GetAsMat3() const", asMETHOD(ezQuat, GetAsMat3), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezQuat", "ezMat3 GetAsMat4() const", asMETHOD(ezQuat, GetAsMat4), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezQuat", "bool IsValid(float eps = 0.00001) const", asMETHOD(ezQuat, IsValid), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezQuat", "bool IsNaN() const", asMETHOD(ezQuat, IsNaN), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezQuat", "bool IsEqualRotation(ezQuat, float eps = 0.00001) const", asMETHOD(ezQuat, IsEqualRotation), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezQuat", "void Invert()", asMETHOD(ezQuat, Invert), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezQuat", "ezQuat GetInverse() const", asMETHOD(ezQuat, GetInverse), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezQuat", "ezQuat GetNegated() const", asMETHOD(ezQuat, GetNegated), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezQuat", "float Dot(ezQuat) const", asMETHOD(ezQuat, Dot), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezQuat", "ezVec3 Rotate(ezVec3) const", asMETHOD(ezQuat, Rotate), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezQuat", "void GetAsEulerAngles(float& out, float& out, float& out) const", asMETHOD(ezQuat, GetAsEulerAngles), asCALL_THISCALL));


  AS_CHECK(m_pEngine->RegisterObjectMethod("ezQuat", "ezQuat opMul(ezQuat) const", asFUNCTIONPR(operator*, (const ezQuat&, const ezQuat&), const ezQuat), asCALL_CDECL_OBJFIRST));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezQuat", "ezVec3 opMul(ezVec3) const", asFUNCTIONPR(operator*, (const ezQuat&, const ezVec3&), const ezVec3), asCALL_CDECL_OBJFIRST));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezQuat", "bool opEquals(ezQuat) const", asFUNCTIONPR(operator==, (const ezQuat&, const ezQuat&), bool), asCALL_CDECL_OBJFIRST));
}

//////////////////////////////////////////////////////////////////////////
// ezTime
//////////////////////////////////////////////////////////////////////////

static int ezTime_opCmp(const ezTime& lhs, const ezTime& rhs)
{
  if (lhs < rhs)
    return -1;
  if (rhs < lhs)
    return +1;

  return 0;
}

void ezAngelScriptEngineSingleton::Register_Time()
{
  // static functions
  {
    m_pEngine->SetDefaultNamespace("ezTime");

    AS_CHECK(m_pEngine->RegisterGlobalFunction("ezTime Now()", asFUNCTION(ezTime::Now), asCALL_CDECL));

    AS_CHECK(m_pEngine->RegisterGlobalFunction("ezTime MakeFromNanoseconds(double)", asFUNCTION(ezTime::MakeFromNanoseconds), asCALL_CDECL));
    AS_CHECK(m_pEngine->RegisterGlobalFunction("ezTime Nanoseconds(double)", asFUNCTION(ezTime::Nanoseconds), asCALL_CDECL));

    AS_CHECK(m_pEngine->RegisterGlobalFunction("ezTime MakeFromMicroseconds(double)", asFUNCTION(ezTime::MakeFromMicroseconds), asCALL_CDECL));
    AS_CHECK(m_pEngine->RegisterGlobalFunction("ezTime Microseconds(double)", asFUNCTION(ezTime::Microseconds), asCALL_CDECL));

    AS_CHECK(m_pEngine->RegisterGlobalFunction("ezTime MakeFromMilliseconds(double)", asFUNCTION(ezTime::MakeFromMilliseconds), asCALL_CDECL));
    AS_CHECK(m_pEngine->RegisterGlobalFunction("ezTime Milliseconds(double)", asFUNCTION(ezTime::Milliseconds), asCALL_CDECL));

    AS_CHECK(m_pEngine->RegisterGlobalFunction("ezTime MakeFromSeconds(double)", asFUNCTION(ezTime::MakeFromSeconds), asCALL_CDECL));
    AS_CHECK(m_pEngine->RegisterGlobalFunction("ezTime Seconds(double)", asFUNCTION(ezTime::Seconds), asCALL_CDECL));

    AS_CHECK(m_pEngine->RegisterGlobalFunction("ezTime MakeFromMinutes(double)", asFUNCTION(ezTime::MakeFromMinutes), asCALL_CDECL));
    AS_CHECK(m_pEngine->RegisterGlobalFunction("ezTime Minutes(double)", asFUNCTION(ezTime::Minutes), asCALL_CDECL));

    AS_CHECK(m_pEngine->RegisterGlobalFunction("ezTime MakeFromHours(double)", asFUNCTION(ezTime::MakeFromHours), asCALL_CDECL));
    AS_CHECK(m_pEngine->RegisterGlobalFunction("ezTime Hours(double)", asFUNCTION(ezTime::Hours), asCALL_CDECL));

    AS_CHECK(m_pEngine->RegisterGlobalFunction("ezTime MakeZero()", asFUNCTION(ezTime::MakeZero), asCALL_CDECL));

    m_pEngine->SetDefaultNamespace("");
  }

  AS_CHECK(m_pEngine->RegisterObjectMethod("ezTime", "bool IsZero() const", asMETHOD(ezTime, IsZero), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezTime", "bool IsNegative() const", asMETHOD(ezTime, IsNegative), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezTime", "bool IsPositive() const", asMETHOD(ezTime, IsPositive), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezTime", "bool IsZeroOrNegative() const", asMETHOD(ezTime, IsZeroOrNegative), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezTime", "bool IsZeroOrPositive() const", asMETHOD(ezTime, IsZeroOrPositive), asCALL_THISCALL));

  AS_CHECK(m_pEngine->RegisterObjectMethod("ezTime", "float AsFloatInSeconds() const", asMETHOD(ezTime, AsFloatInSeconds), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezTime", "double GetNanoseconds() const", asMETHOD(ezTime, GetNanoseconds), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezTime", "double GetMicroseconds() const", asMETHOD(ezTime, GetMicroseconds), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezTime", "double GetMilliseconds() const", asMETHOD(ezTime, GetMilliseconds), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezTime", "double GetSeconds() const", asMETHOD(ezTime, GetSeconds), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezTime", "double GetMinutes() const", asMETHOD(ezTime, GetMinutes), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezTime", "double GetHours() const", asMETHOD(ezTime, GetHours), asCALL_THISCALL));

  AS_CHECK(m_pEngine->RegisterObjectMethod("ezTime", "void opSubAssign(ezTime)", asMETHOD(ezTime, operator-=), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezTime", "void opAddAssign(ezTime)", asMETHOD(ezTime, operator+=), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezTime", "void opMulAssign(double)", asMETHOD(ezTime, operator*=), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezTime", "void opDivAssign(double)", asMETHOD(ezTime, operator/=), asCALL_THISCALL));

  AS_CHECK(m_pEngine->RegisterObjectMethod("ezTime", "ezTime opSub(ezTime) const", asMETHODPR(ezTime, operator-, (const ezTime&) const, ezTime), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezTime", "ezTime opAdd(ezTime) const", asMETHODPR(ezTime, operator+, (const ezTime&) const, ezTime), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezTime", "ezTime opNeg() const", asMETHODPR(ezTime, operator-, () const, ezTime), asCALL_THISCALL));

  AS_CHECK(m_pEngine->RegisterObjectMethod("ezTime", "int opCmp(ezTime) const", asFUNCTIONPR(ezTime_opCmp, (const ezTime&, const ezTime&), int), asCALL_CDECL_OBJFIRST));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezTime", "bool opEquals(ezTime) const", asMETHODPR(ezTime, operator==, (const ezTime&) const, bool), asCALL_THISCALL));

  AS_CHECK(m_pEngine->RegisterObjectMethod("ezTime", "ezTime opMul(double) const", asFUNCTIONPR(operator*, (ezTime, double), ezTime), asCALL_CDECL_OBJFIRST));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezTime", "ezTime opMul_r(double) const", asFUNCTIONPR(operator*, (double, ezTime), ezTime), asCALL_CDECL_OBJLAST));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezTime", "ezTime opMul(ezTime) const", asFUNCTIONPR(operator*, (ezTime, ezTime), ezTime), asCALL_CDECL_OBJFIRST));

  AS_CHECK(m_pEngine->RegisterObjectMethod("ezTime", "ezTime opDiv(double) const", asFUNCTIONPR(operator/, (ezTime, double), ezTime), asCALL_CDECL_OBJFIRST));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezTime", "ezTime opDiv_r(double) const", asFUNCTIONPR(operator/, (double, ezTime), ezTime), asCALL_CDECL_OBJLAST));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezTime", "ezTime opDiv(ezTime) const", asFUNCTIONPR(operator/, (ezTime, ezTime), ezTime), asCALL_CDECL_OBJFIRST));
}


//////////////////////////////////////////////////////////////////////////
// ezTransform
//////////////////////////////////////////////////////////////////////////

static void ezTransform_Construct1(void* memory, const ezVec3& v)
{
  new (memory) ezTransform(v);
}

static void ezTransform_Construct2(void* memory, const ezVec3& v, const ezQuat& r)
{
  new (memory) ezTransform(v, r);
}

static void ezTransform_Construct3(void* memory, const ezVec3& v, const ezQuat& r, const ezVec3& s)
{
  new (memory) ezTransform(v, r, s);
}

void ezAngelScriptEngineSingleton::Register_Transform()
{
  AS_CHECK(m_pEngine->RegisterObjectProperty("ezTransform", "ezVec3 m_vPosition", asOFFSET(ezTransform, m_vPosition)));
  AS_CHECK(m_pEngine->RegisterObjectProperty("ezTransform", "ezQuat m_qRotation", asOFFSET(ezTransform, m_qRotation)));
  AS_CHECK(m_pEngine->RegisterObjectProperty("ezTransform", "ezVec3 m_vScale", asOFFSET(ezTransform, m_vScale)));

  AS_CHECK(m_pEngine->RegisterObjectBehaviour("ezTransform", asBEHAVE_CONSTRUCT, "void f(ezVec3)", asFUNCTION(ezTransform_Construct1), asCALL_CDECL_OBJFIRST));
  AS_CHECK(m_pEngine->RegisterObjectBehaviour("ezTransform", asBEHAVE_CONSTRUCT, "void f(ezVec3, ezQuat)", asFUNCTION(ezTransform_Construct2), asCALL_CDECL_OBJFIRST));
  AS_CHECK(m_pEngine->RegisterObjectBehaviour("ezTransform", asBEHAVE_CONSTRUCT, "void f(ezVec3, ezQuat, ezVec3)", asFUNCTION(ezTransform_Construct3), asCALL_CDECL_OBJFIRST));

  // static functions
  {
    m_pEngine->SetDefaultNamespace("ezTransform");

    AS_CHECK(m_pEngine->RegisterGlobalFunction("ezTransform Make(ezVec3, ezQuat, ezVec3)", asFUNCTION(ezTransform::Make), asCALL_CDECL));
    AS_CHECK(m_pEngine->RegisterGlobalFunction("ezTransform MakeIdentity()", asFUNCTION(ezTransform::MakeIdentity), asCALL_CDECL));
    AS_CHECK(m_pEngine->RegisterGlobalFunction("ezTransform MakeFromMat4(ezMat4)", asFUNCTION(ezTransform::MakeFromMat4), asCALL_CDECL));
    AS_CHECK(m_pEngine->RegisterGlobalFunction("ezTransform MakeLocalTransform(ezTransform, ezTransform)", asFUNCTION(ezTransform::MakeLocalTransform), asCALL_CDECL));
    AS_CHECK(m_pEngine->RegisterGlobalFunction("ezTransform MakeGlobalTransform(ezTransform, ezTransform)", asFUNCTION(ezTransform::MakeGlobalTransform), asCALL_CDECL));

    m_pEngine->SetDefaultNamespace("");
  }

  AS_CHECK(m_pEngine->RegisterObjectMethod("ezTransform", "void SetIdentity()", asMETHOD(ezTransform, SetIdentity), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezTransform", "float GetMaxScale() const", asMETHOD(ezTransform, GetMaxScale), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezTransform", "bool HasMirrorScaling() const", asMETHOD(ezTransform, HasMirrorScaling), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezTransform", "bool ContainsUniformScale() const", asMETHOD(ezTransform, ContainsUniformScale), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezTransform", "bool IsValid() const", asMETHOD(ezTransform, IsValid), asCALL_THISCALL));

  AS_CHECK(m_pEngine->RegisterObjectMethod("ezTransform", "bool IsIdentical(ezTransform) const", asMETHOD(ezTransform, IsIdentical), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezTransform", "bool IsEqual(ezTransform, float) const", asMETHOD(ezTransform, IsEqual), asCALL_THISCALL));

  AS_CHECK(m_pEngine->RegisterObjectMethod("ezTransform", "void Invert()", asMETHOD(ezTransform, Invert), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezTransform", "ezTransform GetInverse() const", asMETHOD(ezTransform, GetInverse), asCALL_THISCALL));

  AS_CHECK(m_pEngine->RegisterObjectMethod("ezTransform", "ezVec3 TransformPosition(ezVec3) const", asMETHOD(ezTransform, TransformPosition), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezTransform", "ezVec3 TransformDirection(ezVec3) const", asMETHOD(ezTransform, TransformDirection), asCALL_THISCALL));

  AS_CHECK(m_pEngine->RegisterObjectMethod("ezTransform", "void opAddAssign(ezVec3)", asMETHOD(ezTransform, operator+=), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezTransform", "void opSubAssign(ezVec3)", asMETHOD(ezTransform, operator-=), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezTransform", "ezMat4 GetAsMat4() const", asMETHOD(ezTransform, GetAsMat4), asCALL_THISCALL));

  AS_CHECK(m_pEngine->RegisterObjectMethod("ezTransform", "ezVec3 opMul(ezVec3) const", asFUNCTIONPR(operator*, (const ezTransform&, const ezVec3&), const ezVec3), asCALL_CDECL_OBJFIRST));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezTransform", "ezTransform opMul_r(ezQuat) const", asFUNCTIONPR(operator*, (const ezQuat&, const ezTransform&), const ezTransform), asCALL_CDECL_OBJLAST));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezTransform", "ezTransform opMul(ezQuat) const", asFUNCTIONPR(operator*, (const ezTransform&, const ezQuat&), const ezTransform), asCALL_CDECL_OBJFIRST));

  AS_CHECK(m_pEngine->RegisterObjectMethod("ezTransform", "ezTransform opAdd(ezVec3) const", asFUNCTIONPR(operator+, (const ezTransform&, const ezVec3&), const ezTransform), asCALL_CDECL_OBJFIRST));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezTransform", "ezTransform opSub(ezVec3) const", asFUNCTIONPR(operator-, (const ezTransform&, const ezVec3&), const ezTransform), asCALL_CDECL_OBJFIRST));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezTransform", "ezTransform opMul(ezTransform) const", asFUNCTIONPR(operator*, (const ezTransform&, const ezTransform&), const ezTransform), asCALL_CDECL_OBJFIRST));

  AS_CHECK(m_pEngine->RegisterObjectMethod("ezTransform", "bool opEquals(ezTransform) const", asFUNCTIONPR(operator==, (const ezTransform&, const ezTransform&), bool), asCALL_CDECL_OBJFIRST));
}

//////////////////////////////////////////////////////////////////////////
// ezMat3
//////////////////////////////////////////////////////////////////////////

void ezAngelScriptEngineSingleton::Register_Mat3()
{
  // static functions
  {
    m_pEngine->SetDefaultNamespace("ezMat3");
    m_pEngine->SetDefaultNamespace("");
  }

  // TODO AngelScript: Register ezMat3
}

//////////////////////////////////////////////////////////////////////////
// ezMat4
//////////////////////////////////////////////////////////////////////////

void ezAngelScriptEngineSingleton::Register_Mat4()
{
  // static functions
  {
    m_pEngine->SetDefaultNamespace("ezMat4");
    m_pEngine->SetDefaultNamespace("");
  }

  // TODO AngelScript: Register ezMat4
}

//////////////////////////////////////////////////////////////////////////
// ezWorld
//////////////////////////////////////////////////////////////////////////

static void ezGameObjectDesc_Construct(void* memory)
{
  new (memory) ezGameObjectDesc();
}

void ezAngelScriptEngineSingleton::Register_World()
{
  {
    AS_CHECK(m_pEngine->RegisterObjectType("ezGameObjectDesc", sizeof(ezGameObjectDesc), asOBJ_VALUE | asOBJ_POD | asGetTypeTraits<ezGameObjectDesc>()));

    AS_CHECK(m_pEngine->RegisterObjectProperty("ezGameObjectDesc", "bool m_bActiveFlag", asOFFSET(ezGameObjectDesc, m_bActiveFlag)));
    AS_CHECK(m_pEngine->RegisterObjectProperty("ezGameObjectDesc", "bool m_bDynamic", asOFFSET(ezGameObjectDesc, m_bDynamic)));
    AS_CHECK(m_pEngine->RegisterObjectProperty("ezGameObjectDesc", "uint16 m_uiTeamID", asOFFSET(ezGameObjectDesc, m_uiTeamID)));
    AS_CHECK(m_pEngine->RegisterObjectProperty("ezGameObjectDesc", "ezHashedString m_sName", asOFFSET(ezGameObjectDesc, m_sName)));
    AS_CHECK(m_pEngine->RegisterObjectProperty("ezGameObjectDesc", "ezGameObjectHandle m_hParent", asOFFSET(ezGameObjectDesc, m_hParent)));
    AS_CHECK(m_pEngine->RegisterObjectProperty("ezGameObjectDesc", "ezVec3 m_LocalPosition", asOFFSET(ezGameObjectDesc, m_LocalPosition)));
    AS_CHECK(m_pEngine->RegisterObjectProperty("ezGameObjectDesc", "ezQuat m_LocalRotation", asOFFSET(ezGameObjectDesc, m_LocalRotation)));
    AS_CHECK(m_pEngine->RegisterObjectProperty("ezGameObjectDesc", "ezVec3 m_LocalScaling", asOFFSET(ezGameObjectDesc, m_LocalScaling)));
    AS_CHECK(m_pEngine->RegisterObjectProperty("ezGameObjectDesc", "float m_LocalUniformScaling", asOFFSET(ezGameObjectDesc, m_LocalUniformScaling)));
    // AS_CHECK(m_pEngine->RegisterObjectProperty("ezGameObjectDesc", "ezTagSet m_Tags", asOFFSET(ezGameObjectDesc, m_Tags)));
    AS_CHECK(m_pEngine->RegisterObjectProperty("ezGameObjectDesc", "uint32 m_uiStableRandomSeed", asOFFSET(ezGameObjectDesc, m_uiStableRandomSeed)));

    AS_CHECK(m_pEngine->RegisterObjectBehaviour("ezGameObjectDesc", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(ezGameObjectDesc_Construct), asCALL_CDECL_OBJFIRST));
  }

  AS_CHECK(m_pEngine->RegisterObjectMethod("ezWorld", "ezStringView GetName()", asMETHOD(ezWorld, GetName), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezWorld", "ezGameObjectHandle CreateObject(ezGameObjectDesc& in)", asMETHODPR(ezWorld, CreateObject, (const ezGameObjectDesc& desc), ezGameObjectHandle), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezWorld", "ezGameObjectHandle CreateObject(ezGameObjectDesc& in, ezGameObject& out)", asMETHODPR(ezWorld, CreateObject, (const ezGameObjectDesc& desc, ezGameObject*&), ezGameObjectHandle), asCALL_THISCALL));

  AS_CHECK(m_pEngine->RegisterObjectMethod("ezWorld", "void DeleteObjectDelayed(const ezGameObjectHandle& in, bool emptyParents = true)", asMETHOD(ezWorld, DeleteObjectDelayed), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezWorld", "bool IsValidObject(const ezGameObjectHandle& in)", asMETHOD(ezWorld, IsValidObject), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezWorld", "bool TryGetObject(ezGameObjectHandle& in, const ezGameObject& out)", asMETHODPR(ezWorld, TryGetObject, (const ezGameObjectHandle&, ezGameObject*&), bool), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezWorld", "bool TryGetObject(ezGameObjectHandle& in, const ezGameObject& out) const", asMETHODPR(ezWorld, TryGetObject, (const ezGameObjectHandle&, const ezGameObject*&) const, bool), asCALL_THISCALL));

  // TryGetObjectWithGlobalKey
  // GetOrCreateModule

  AS_CHECK(m_pEngine->RegisterObjectMethod("ezWorld", "bool IsValidComponent(ezComponentHandle& in)", asMETHOD(ezWorld, IsValidComponent), asCALL_THISCALL));

  // TryGetComponent

  AS_CHECK(m_pEngine->RegisterObjectMethod("ezWorld", "void SendMessage(ezGameObjectHandle& in, ezMessage& inout)", asMETHODPR(ezWorld, SendMessage, (const ezGameObjectHandle&, ezMessage&), void), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezWorld", "void SendMessageRecursive(ezGameObjectHandle& in, ezMessage& inout)", asMETHODPR(ezWorld, SendMessageRecursive, (const ezGameObjectHandle&, ezMessage&), void), asCALL_THISCALL));

  // PostMessage
  // PostMessageRecursive

  AS_CHECK(m_pEngine->RegisterObjectMethod("ezWorld", "void SendMessage(ezComponentHandle& in, ezMessage& inout)", asMETHODPR(ezWorld, SendMessage, (const ezComponentHandle&, ezMessage&), void), asCALL_THISCALL));

  // PostMessage

  AS_CHECK(m_pEngine->RegisterObjectMethod("ezWorld", "ezClock@ GetClock()", asMETHODPR(ezWorld, GetClock, (), ezClock&), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezWorld", "const ezClock@ GetClock() const", asMETHODPR(ezWorld, GetClock, () const, const ezClock&),
    asCALL_THISCALL));

  AS_CHECK(m_pEngine->RegisterObjectMethod("ezWorld", "ezRandom@ GetRandomNumberGenerator()", asMETHOD(ezWorld, GetRandomNumberGenerator), asCALL_THISCALL));

  // TODO AngelScript: Register ezWorld
}

//////////////////////////////////////////////////////////////////////////
// ezClock
//////////////////////////////////////////////////////////////////////////

void ezAngelScriptEngineSingleton::Register_Clock()
{
  // static functions
  {
    m_pEngine->SetDefaultNamespace("ezClock");
    m_pEngine->SetDefaultNamespace("");
  }

  // TODO AngelScript: Register ezClock
}

//////////////////////////////////////////////////////////////////////////
// ezStringBase
//////////////////////////////////////////////////////////////////////////

template <typename T>
void RegisterStringBase(asIScriptEngine* pEngine, const char* szType)
{
  AS_CHECK(pEngine->RegisterObjectMethod(szType, "bool StartsWith(ezStringView) const", asMETHOD(T, StartsWith), asCALL_THISCALL));
  AS_CHECK(pEngine->RegisterObjectMethod(szType, "bool StartsWith_NoCase(ezStringView) const", asMETHOD(T, StartsWith_NoCase), asCALL_THISCALL));
  AS_CHECK(pEngine->RegisterObjectMethod(szType, "bool EndsWith(ezStringView) const", asMETHOD(T, EndsWith), asCALL_THISCALL));
  AS_CHECK(pEngine->RegisterObjectMethod(szType, "bool EndsWith_NoCase(ezStringView) const", asMETHOD(T, EndsWith_NoCase), asCALL_THISCALL));

  // FindSubString
  // FindSubString_NoCase
  // FindLastSubString
  // FindLastSubString_NoCase
  // FindWholeWord
  // FindWholeWord_NoCase

  AS_CHECK(pEngine->RegisterObjectMethod(szType, "int Compare(ezStringView) const", asMETHOD(T, Compare), asCALL_THISCALL));
  AS_CHECK(pEngine->RegisterObjectMethod(szType, "int Compare_NoCase(ezStringView) const", asMETHOD(T, Compare_NoCase), asCALL_THISCALL));
  AS_CHECK(pEngine->RegisterObjectMethod(szType, "int CompareN(ezStringView, uint32) const", asMETHOD(T, CompareN), asCALL_THISCALL));
  AS_CHECK(pEngine->RegisterObjectMethod(szType, "int CompareN_NoCase(ezStringView, uint32) const", asMETHOD(T, CompareN_NoCase), asCALL_THISCALL));

  AS_CHECK(pEngine->RegisterObjectMethod(szType, "uint32 GetElementCount() const", asMETHOD(T, GetElementCount), asCALL_THISCALL));
  AS_CHECK(pEngine->RegisterObjectMethod(szType, "bool IsEmpty() const", asMETHOD(T, IsEmpty), asCALL_THISCALL));
  AS_CHECK(pEngine->RegisterObjectMethod(szType, "bool IsEqual(ezStringView) const", asMETHOD(T, IsEqual), asCALL_THISCALL));
  AS_CHECK(pEngine->RegisterObjectMethod(szType, "bool IsEqual_NoCase(ezStringView) const", asMETHOD(T, IsEqual_NoCase), asCALL_THISCALL));
  AS_CHECK(pEngine->RegisterObjectMethod(szType, "bool IsEqualN(ezStringView, uint32) const", asMETHOD(T, IsEqualN), asCALL_THISCALL));
  AS_CHECK(pEngine->RegisterObjectMethod(szType, "bool IsEqualN_NoCase(ezStringView, uint32) const", asMETHOD(T, IsEqualN_NoCase), asCALL_THISCALL));

  AS_CHECK(pEngine->RegisterObjectMethod(szType, "bool HasAnyExtension() const", asMETHOD(T, HasAnyExtension), asCALL_THISCALL));
  AS_CHECK(pEngine->RegisterObjectMethod(szType, "bool HasExtension(ezStringView) const", asMETHOD(T, HasExtension), asCALL_THISCALL));

  AS_CHECK(pEngine->RegisterObjectMethod(szType, "ezStringView GetFileExtension(bool full = false) const", asMETHOD(T, GetFileExtension), asCALL_THISCALL));
  AS_CHECK(pEngine->RegisterObjectMethod(szType, "ezStringView GetFileName() const", asMETHOD(T, GetFileName), asCALL_THISCALL));
  AS_CHECK(pEngine->RegisterObjectMethod(szType, "ezStringView GetFileNameAndExtension() const", asMETHOD(T, GetFileNameAndExtension), asCALL_THISCALL));
  AS_CHECK(pEngine->RegisterObjectMethod(szType, "ezStringView GetFileDirectory() const", asMETHOD(T, GetFileDirectory), asCALL_THISCALL));
  AS_CHECK(pEngine->RegisterObjectMethod(szType, "bool IsAbsolutePath() const", asMETHOD(T, IsAbsolutePath), asCALL_THISCALL));
  AS_CHECK(pEngine->RegisterObjectMethod(szType, "bool IsRelativePath() const", asMETHOD(T, IsRelativePath), asCALL_THISCALL));
  AS_CHECK(pEngine->RegisterObjectMethod(szType, "bool IsRootedPath() const", asMETHOD(T, IsRootedPath), asCALL_THISCALL));
  AS_CHECK(pEngine->RegisterObjectMethod(szType, "bool GetRootedPathRootName() const", asMETHOD(T, GetRootedPathRootName), asCALL_THISCALL));
}

//////////////////////////////////////////////////////////////////////////
// ezStringView
//////////////////////////////////////////////////////////////////////////

static int ezStringView_opCmp(ezStringView lhs, ezStringView rhs)
{
  if (lhs < rhs)
    return -1;
  if (rhs < lhs)
    return +1;

  return 0;
}

static void ezStringView_opAssignStr(ezStringView* lhs, const std::string& rhs)
{
  *lhs = rhs.c_str();
}

static void StdString_opAssignStringView(std::string* lhs, ezStringView rhs)
{
  lhs->assign(rhs.GetStartPointer(), rhs.GetElementCount());
}

void ezAngelScriptEngineSingleton::Register_StringView()
{
  RegisterStringBase<ezStringView>(m_pEngine, "ezStringView");

  AS_CHECK(m_pEngine->RegisterObjectMethod("ezStringView", "void Shrink(uint32, uint32)", asMETHOD(ezStringView, Shrink), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezStringView", "ezStringView GetShrunk(uint32 front, uint32 back = 0) const", asMETHOD(ezStringView, GetShrunk), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezStringView", "ezStringView GetSubString(uint32, uint32) const", asMETHOD(ezStringView, GetSubString), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezStringView", "void ChopAwayFirstCharacterUtf8()", asMETHOD(ezStringView, ChopAwayFirstCharacterUtf8), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezStringView", "void ChopAwayFirstCharacterAscii()", asMETHOD(ezStringView, ChopAwayFirstCharacterAscii), asCALL_THISCALL));
  // Trim
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezStringView", "bool TrimWordStart(ezStringView)", asMETHOD(ezStringView, TrimWordStart), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezStringView", "bool TrimWordEnd(ezStringView)", asMETHOD(ezStringView, TrimWordEnd), asCALL_THISCALL));

  AS_CHECK(m_pEngine->RegisterObjectMethod("ezStringView", "bool opEquals(ezStringView) const", asFUNCTIONPR(operator==, (ezStringView, ezStringView), bool), asCALL_CDECL_OBJFIRST));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezStringView", "int opCmp(ezStringView) const", asFUNCTIONPR(ezStringView_opCmp, (ezStringView, ezStringView), int), asCALL_CDECL_OBJFIRST));

  AS_CHECK(m_pEngine->RegisterObjectMethod("ezStringView", "void opAssign(const string& in)", asFUNCTION(ezStringView_opAssignStr), asCALL_CDECL_OBJFIRST));
  AS_CHECK(m_pEngine->RegisterObjectBehaviour("ezStringView", asBEHAVE_CONSTRUCT, "void f(const string& in)", asFUNCTION(ezStringView_opAssignStr), asCALL_CDECL_OBJFIRST));

  AS_CHECK(m_pEngine->RegisterObjectMethod("string", "void opAssign(ezStringView)", asFUNCTION(StdString_opAssignStringView), asCALL_CDECL_OBJFIRST));
  AS_CHECK(m_pEngine->RegisterObjectBehaviour("string", asBEHAVE_CONSTRUCT, "void f(const ezStringView)", asFUNCTION(StdString_opAssignStringView), asCALL_CDECL_OBJFIRST));
}



//////////////////////////////////////////////////////////////////////////
// ezString
//////////////////////////////////////////////////////////////////////////

static void ezString_Construct(void* memory)
{
  new (memory) ezString();
}

static void ezString_Destruct(void* memory)
{
  ezString* p = (ezString*)memory;
  p->~ezString();
}

static int ezString_opCmp(const ezString& lhs, const ezString& rhs)
{
  if (lhs < rhs)
    return -1;
  if (rhs < lhs)
    return +1;

  return 0;
}

static void ezString_opAssignStr(ezString* lhs, const std::string& rhs)
{
  *lhs = rhs.c_str();
}
static void ezString_opAssignString(ezString* lhs, const ezString& rhs)
{
  *lhs = rhs;
}

static void ezString_opAssignStringView(ezString* lhs, ezStringView rhs)
{
  *lhs = rhs;
}

static void ezString_opAssignStringBuilder(ezString* lhs, const ezStringBuilder& rhs)
{
  *lhs = rhs;
}

static void StdString_opAssignString(std::string* lhs, const ezString& rhs)
{
  lhs->assign(rhs.GetData(), rhs.GetElementCount());
}

void ezAngelScriptEngineSingleton::Register_String()
{
  RegisterStringBase<ezString>(m_pEngine, "ezString");

  AS_CHECK(m_pEngine->RegisterObjectMethod("ezString", "ezStringView GetView() const", asMETHOD(ezString, GetView), asCALL_THISCALL));
  // AS_CHECK(m_pEngine->RegisterObjectMethod("ezString", "uint32 GetCharacterCount() const", asMETHOD(ezString, GetCharacterCount), asCALL_THISCALL));

  // TODO AngelScript: string equals operator
  // AS_CHECK(m_pEngine->RegisterObjectMethod("ezString", "bool opEquals(const ezString& in) const", asFUNCTIONPR(operator==, (const ezString&, const ezString&), bool), asCALL_CDECL_OBJFIRST));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezString", "int opCmp(const ezString& in) const", asFUNCTIONPR(ezString_opCmp, (const ezString&, const ezString&), int), asCALL_CDECL_OBJFIRST));

  AS_CHECK(m_pEngine->RegisterObjectBehaviour("ezString", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(ezString_Construct), asCALL_CDECL_OBJFIRST));
  AS_CHECK(m_pEngine->RegisterObjectBehaviour("ezString", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(ezString_Destruct), asCALL_CDECL_OBJFIRST));

  AS_CHECK(m_pEngine->RegisterObjectMethod("ezString", "void opAssign(const string& in)", asFUNCTION(ezString_opAssignStr), asCALL_CDECL_OBJFIRST));
  AS_CHECK(m_pEngine->RegisterObjectBehaviour("ezString", asBEHAVE_CONSTRUCT, "void f(const string& in)", asFUNCTION(ezString_opAssignStr), asCALL_CDECL_OBJFIRST));

  AS_CHECK(m_pEngine->RegisterObjectMethod("ezString", "void opAssign(const ezString& in)", asFUNCTION(ezString_opAssignString), asCALL_CDECL_OBJFIRST));
  AS_CHECK(m_pEngine->RegisterObjectBehaviour("ezString", asBEHAVE_CONSTRUCT, "void f(const ezString& in)", asFUNCTION(ezString_opAssignString), asCALL_CDECL_OBJFIRST));

  AS_CHECK(m_pEngine->RegisterObjectMethod("ezString", "void opAssign(ezStringView)", asFUNCTION(ezString_opAssignStringView), asCALL_CDECL_OBJFIRST));
  AS_CHECK(m_pEngine->RegisterObjectBehaviour("ezString", asBEHAVE_CONSTRUCT, "void f(const ezStringView)", asFUNCTION(ezString_opAssignStringView), asCALL_CDECL_OBJFIRST));

  AS_CHECK(m_pEngine->RegisterObjectMethod("ezString", "void opAssign(const ezStringBuilder& in)", asFUNCTION(ezString_opAssignStringBuilder), asCALL_CDECL_OBJFIRST));
  AS_CHECK(m_pEngine->RegisterObjectBehaviour("ezString", asBEHAVE_CONSTRUCT, "void f(const ezStringBuilder& in)", asFUNCTION(ezString_opAssignStringBuilder), asCALL_CDECL_OBJFIRST));

  AS_CHECK(m_pEngine->RegisterObjectMethod("string", "void opAssign(const ezString& in)", asFUNCTION(StdString_opAssignString), asCALL_CDECL_OBJFIRST));
  AS_CHECK(m_pEngine->RegisterObjectBehaviour("string", asBEHAVE_CONSTRUCT, "void f(const ezString& in)", asFUNCTION(StdString_opAssignString), asCALL_CDECL_OBJFIRST));
}

//////////////////////////////////////////////////////////////////////////
// ezStringBuilder
//////////////////////////////////////////////////////////////////////////

static void ezStringBuilder_Construct(void* memory)
{
  new (memory) ezStringBuilder();
}

static void ezStringBuilder_Destruct(void* memory)
{
  ezStringBuilder* p = (ezStringBuilder*)memory;
  p->~ezStringBuilder();
}

void ezAngelScriptEngineSingleton::Register_StringBuilder()
{
  RegisterStringBase<ezStringBuilder>(m_pEngine, "ezStringBuilder");

  AS_CHECK(m_pEngine->RegisterObjectBehaviour("ezStringBuilder", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(ezStringBuilder_Construct), asCALL_CDECL_OBJFIRST));
  AS_CHECK(m_pEngine->RegisterObjectBehaviour("ezStringBuilder", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(ezStringBuilder_Destruct), asCALL_CDECL_OBJFIRST));

  AS_CHECK(m_pEngine->RegisterObjectMethod("ezStringBuilder", "ezStringView GetView() const", asMETHOD(ezStringBuilder, GetView), asCALL_THISCALL));

  // TODO AngelScript: Register ezStringBuilder
}

//////////////////////////////////////////////////////////////////////////
// ezTempHashedString
//////////////////////////////////////////////////////////////////////////

static void ezTempHashedString_Construct(void* memory)
{
  new (memory) ezTempHashedString();
}

static void ezTempHashedString_ConstructView(void* memory, ezStringView view)
{
  new (memory) ezTempHashedString(view);
}

static void ezTempHashedString_ConstructHS(void* memory, const ezHashedString& hs)
{
  new (memory) ezTempHashedString(hs);
}

static void ezTempHashedString_AssignStringView(ezTempHashedString* pStr, ezStringView view)
{
  *pStr = view;
}

static void ezTempHashedString_AssignHS(ezTempHashedString* pStr, const ezHashedString& hs)
{
  *pStr = hs;
}

void ezAngelScriptEngineSingleton::Register_TempHashedString()
{
  AS_CHECK(m_pEngine->RegisterObjectBehaviour("ezTempHashedString", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(ezTempHashedString_Construct), asCALL_CDECL_OBJFIRST));
  AS_CHECK(m_pEngine->RegisterObjectBehaviour("ezTempHashedString", asBEHAVE_CONSTRUCT, "void f(const ezStringView)", asFUNCTION(ezTempHashedString_ConstructView), asCALL_CDECL_OBJFIRST));
  AS_CHECK(m_pEngine->RegisterObjectBehaviour("ezTempHashedString", asBEHAVE_CONSTRUCT, "void f(const ezHashedString& in)", asFUNCTION(ezTempHashedString_ConstructHS), asCALL_CDECL_OBJFIRST));

  AS_CHECK(m_pEngine->RegisterObjectMethod("ezTempHashedString", "void opAssign(ezStringView)", asFUNCTION(ezTempHashedString_AssignStringView), asCALL_CDECL_OBJFIRST));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezTempHashedString", "void opAssign(const ezHashedString& in)", asFUNCTION(ezTempHashedString_AssignHS), asCALL_CDECL_OBJFIRST));

  AS_CHECK(m_pEngine->RegisterObjectMethod("ezTempHashedString", "bool opEquals(ezTempHashedString) const", asMETHODPR(ezTempHashedString, operator==, (const ezTempHashedString&) const, bool), asCALL_THISCALL));

  AS_CHECK(m_pEngine->RegisterObjectMethod("ezTempHashedString", "bool IsEmpty() const", asMETHOD(ezTempHashedString, IsEmpty), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezTempHashedString", "void Clear()", asMETHOD(ezTempHashedString, Clear), asCALL_THISCALL));
}

//////////////////////////////////////////////////////////////////////////
// ezHashedString
//////////////////////////////////////////////////////////////////////////

static void ezHashedString_Construct(void* memory)
{
  new (memory) ezHashedString();
}

// static void ezHashedString_ConstructView(void* memory, ezStringView view)
//{
//    new (memory) ezHashedString(view); // Assign(view);
// }

static void ezHashedString_ConstructHS(void* memory, const ezHashedString& hs)
{
  new (memory) ezHashedString(hs);
}

// static void ezHashedString_AssignStringView(ezHashedString* pStr, ezStringView view)
//{
//   pStr->Assign(view);
// }

static bool ezHashedString_EqualsStringView(ezHashedString* pStr, ezStringView view)
{
  return *pStr == view;
}

void ezAngelScriptEngineSingleton::Register_HashedString()
{
  AS_CHECK(m_pEngine->RegisterObjectBehaviour("ezHashedString", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(ezHashedString_Construct), asCALL_CDECL_OBJFIRST));
  // AS_CHECK(m_pEngine->RegisterObjectBehaviour("ezHashedString", asBEHAVE_CONSTRUCT, "void f(const ezStringView)", asFUNCTION(ezHashedString_ConstructView), asCALL_CDECL_OBJFIRST));
  AS_CHECK(m_pEngine->RegisterObjectBehaviour("ezHashedString", asBEHAVE_CONSTRUCT, "void f(const ezHashedString& in)", asFUNCTION(ezHashedString_ConstructHS), asCALL_CDECL_OBJFIRST));

  // AS_CHECK(m_pEngine->RegisterObjectMethod("ezHashedString", "void opAssign(ezStringView)", asFUNCTION(ezHashedString_AssignStringView), asCALL_CDECL_OBJFIRST));

  AS_CHECK(m_pEngine->RegisterObjectMethod("ezHashedString", "bool IsEmpty() const", asMETHOD(ezHashedString, IsEmpty), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezHashedString", "void Clear()", asMETHOD(ezHashedString, Clear), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezHashedString", "void Assign(ezStringView)", asMETHODPR(ezHashedString, Assign, (ezStringView), void), asCALL_THISCALL));

  AS_CHECK(m_pEngine->RegisterObjectMethod("ezHashedString", "bool opEquals(const ezHashedString& in) const", asMETHODPR(ezHashedString, operator==, (const ezHashedString&) const, bool), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezHashedString", "bool opEquals(const ezTempHashedString& in) const", asMETHODPR(ezHashedString, operator==, (const ezTempHashedString&) const, bool), asCALL_THISCALL));
  AS_CHECK(m_pEngine->RegisterObjectMethod("ezHashedString", "bool opEquals(ezStringView) const", asFUNCTION(ezHashedString_EqualsStringView), asCALL_CDECL_OBJFIRST));

  AS_CHECK(m_pEngine->RegisterObjectMethod("ezHashedString", "ezStringView GetView() const", asMETHOD(ezHashedString, GetView), asCALL_THISCALL));
}

//////////////////////////////////////////////////////////////////////////
// ezColor
//////////////////////////////////////////////////////////////////////////

static void ezColor_ConstructRGBA(void* memory, float r, float g, float b, float a)
{
  new (memory) ezColor(r, g, b, a);
}

static void ezColor_ConstructGamma(void* memory, ezColorGammaUB col)
{
  new (memory) ezColor(col);
}

void ezAngelScriptEngineSingleton::Register_Color()
{
  AS_CHECK(m_pEngine->RegisterObjectProperty("ezColor", "float r", asOFFSET(ezColor, r)));
  AS_CHECK(m_pEngine->RegisterObjectProperty("ezColor", "float g", asOFFSET(ezColor, g)));
  AS_CHECK(m_pEngine->RegisterObjectProperty("ezColor", "float b", asOFFSET(ezColor, b)));
  AS_CHECK(m_pEngine->RegisterObjectProperty("ezColor", "float a", asOFFSET(ezColor, a)));

  // static functions
  {
    m_pEngine->SetDefaultNamespace("ezColor");

    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor AliceBlue", (void*)&ezColor::AliceBlue));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor AntiqueWhite", (void*)&ezColor::AntiqueWhite));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor Aqua", (void*)&ezColor::Aqua));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor Aquamarine", (void*)&ezColor::Aquamarine));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor Azure", (void*)&ezColor::Azure));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor Beige", (void*)&ezColor::Beige));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor Bisque", (void*)&ezColor::Bisque));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor Black", (void*)&ezColor::Black));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor BlanchedAlmond", (void*)&ezColor::BlanchedAlmond));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor Blue", (void*)&ezColor::Blue));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor BlueViolet", (void*)&ezColor::BlueViolet));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor Brown", (void*)&ezColor::Brown));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor BurlyWood", (void*)&ezColor::BurlyWood));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor CadetBlue", (void*)&ezColor::CadetBlue));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor Chartreuse", (void*)&ezColor::Chartreuse));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor Chocolate", (void*)&ezColor::Chocolate));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor Coral", (void*)&ezColor::Coral));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor CornflowerBlue", (void*)&ezColor::CornflowerBlue));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor Cornsilk", (void*)&ezColor::Cornsilk));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor Crimson", (void*)&ezColor::Crimson));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor Cyan", (void*)&ezColor::Cyan));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor DarkBlue", (void*)&ezColor::DarkBlue));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor DarkCyan", (void*)&ezColor::DarkCyan));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor DarkGoldenRod", (void*)&ezColor::DarkGoldenRod));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor DarkGray", (void*)&ezColor::DarkGray));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor DarkGrey", (void*)&ezColor::DarkGrey));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor DarkGreen", (void*)&ezColor::DarkGreen));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor DarkKhaki", (void*)&ezColor::DarkKhaki));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor DarkMagenta", (void*)&ezColor::DarkMagenta));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor DarkOliveGreen", (void*)&ezColor::DarkOliveGreen));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor DarkOrange", (void*)&ezColor::DarkOrange));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor DarkOrchid", (void*)&ezColor::DarkOrchid));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor DarkRed", (void*)&ezColor::DarkRed));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor DarkSalmon", (void*)&ezColor::DarkSalmon));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor DarkSeaGreen", (void*)&ezColor::DarkSeaGreen));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor DarkSlateBlue", (void*)&ezColor::DarkSlateBlue));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor DarkSlateGray", (void*)&ezColor::DarkSlateGray));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor DarkSlateGrey", (void*)&ezColor::DarkSlateGrey));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor DarkTurquoise", (void*)&ezColor::DarkTurquoise));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor DarkViolet", (void*)&ezColor::DarkViolet));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor DeepPink", (void*)&ezColor::DeepPink));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor DeepSkyBlue", (void*)&ezColor::DeepSkyBlue));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor DimGray", (void*)&ezColor::DimGray));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor DimGrey", (void*)&ezColor::DimGrey));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor DodgerBlue", (void*)&ezColor::DodgerBlue));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor FireBrick", (void*)&ezColor::FireBrick));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor FloralWhite", (void*)&ezColor::FloralWhite));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor ForestGreen", (void*)&ezColor::ForestGreen));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor Fuchsia", (void*)&ezColor::Fuchsia));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor Gainsboro", (void*)&ezColor::Gainsboro));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor GhostWhite", (void*)&ezColor::GhostWhite));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor Gold", (void*)&ezColor::Gold));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor GoldenRod", (void*)&ezColor::GoldenRod));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor Gray", (void*)&ezColor::Gray));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor Grey", (void*)&ezColor::Grey));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor Green", (void*)&ezColor::Green));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor GreenYellow", (void*)&ezColor::GreenYellow));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor HoneyDew", (void*)&ezColor::HoneyDew));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor HotPink", (void*)&ezColor::HotPink));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor IndianRed", (void*)&ezColor::IndianRed));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor Indigo", (void*)&ezColor::Indigo));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor Ivory", (void*)&ezColor::Ivory));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor Khaki", (void*)&ezColor::Khaki));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor Lavender", (void*)&ezColor::Lavender));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor LavenderBlush", (void*)&ezColor::LavenderBlush));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor LawnGreen", (void*)&ezColor::LawnGreen));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor LemonChiffon", (void*)&ezColor::LemonChiffon));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor LightBlue", (void*)&ezColor::LightBlue));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor LightCoral", (void*)&ezColor::LightCoral));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor LightCyan", (void*)&ezColor::LightCyan));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor LightGoldenRodYellow", (void*)&ezColor::LightGoldenRodYellow));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor LightGray", (void*)&ezColor::LightGray));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor LightGrey", (void*)&ezColor::LightGrey));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor LightGreen", (void*)&ezColor::LightGreen));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor LightPink", (void*)&ezColor::LightPink));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor LightSalmon", (void*)&ezColor::LightSalmon));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor LightSeaGreen", (void*)&ezColor::LightSeaGreen));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor LightSkyBlue", (void*)&ezColor::LightSkyBlue));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor LightSlateGray", (void*)&ezColor::LightSlateGray));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor LightSlateGrey", (void*)&ezColor::LightSlateGrey));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor LightSteelBlue", (void*)&ezColor::LightSteelBlue));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor LightYellow", (void*)&ezColor::LightYellow));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor Lime", (void*)&ezColor::Lime));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor LimeGreen", (void*)&ezColor::LimeGreen));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor Linen", (void*)&ezColor::Linen));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor Magenta", (void*)&ezColor::Magenta));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor Maroon", (void*)&ezColor::Maroon));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor MediumAquaMarine", (void*)&ezColor::MediumAquaMarine));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor MediumBlue", (void*)&ezColor::MediumBlue));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor MediumOrchid", (void*)&ezColor::MediumOrchid));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor MediumPurple", (void*)&ezColor::MediumPurple));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor MediumSeaGreen", (void*)&ezColor::MediumSeaGreen));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor MediumSlateBlue", (void*)&ezColor::MediumSlateBlue));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor MediumSpringGreen", (void*)&ezColor::MediumSpringGreen));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor MediumTurquoise", (void*)&ezColor::MediumTurquoise));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor MediumVioletRed", (void*)&ezColor::MediumVioletRed));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor MidnightBlue", (void*)&ezColor::MidnightBlue));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor MintCream", (void*)&ezColor::MintCream));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor MistyRose", (void*)&ezColor::MistyRose));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor Moccasin", (void*)&ezColor::Moccasin));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor NavajoWhite", (void*)&ezColor::NavajoWhite));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor Navy", (void*)&ezColor::Navy));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor OldLace", (void*)&ezColor::OldLace));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor Olive", (void*)&ezColor::Olive));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor OliveDrab", (void*)&ezColor::OliveDrab));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor Orange", (void*)&ezColor::Orange));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor OrangeRed", (void*)&ezColor::OrangeRed));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor Orchid", (void*)&ezColor::Orchid));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor PaleGoldenRod", (void*)&ezColor::PaleGoldenRod));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor PaleGreen", (void*)&ezColor::PaleGreen));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor PaleTurquoise", (void*)&ezColor::PaleTurquoise));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor PaleVioletRed", (void*)&ezColor::PaleVioletRed));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor PapayaWhip", (void*)&ezColor::PapayaWhip));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor PeachPuff", (void*)&ezColor::PeachPuff));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor Peru", (void*)&ezColor::Peru));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor Pink", (void*)&ezColor::Pink));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor Plum", (void*)&ezColor::Plum));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor PowderBlue", (void*)&ezColor::PowderBlue));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor Purple", (void*)&ezColor::Purple));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor RebeccaPurple", (void*)&ezColor::RebeccaPurple));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor Red", (void*)&ezColor::Red));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor RosyBrown", (void*)&ezColor::RosyBrown));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor RoyalBlue", (void*)&ezColor::RoyalBlue));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor SaddleBrown", (void*)&ezColor::SaddleBrown));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor Salmon", (void*)&ezColor::Salmon));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor SandyBrown", (void*)&ezColor::SandyBrown));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor SeaGreen", (void*)&ezColor::SeaGreen));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor SeaShell", (void*)&ezColor::SeaShell));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor Sienna", (void*)&ezColor::Sienna));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor Silver", (void*)&ezColor::Silver));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor SkyBlue", (void*)&ezColor::SkyBlue));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor SlateBlue", (void*)&ezColor::SlateBlue));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor SlateGray", (void*)&ezColor::SlateGray));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor SlateGrey", (void*)&ezColor::SlateGrey));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor Snow", (void*)&ezColor::Snow));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor SpringGreen", (void*)&ezColor::SpringGreen));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor SteelBlue", (void*)&ezColor::SteelBlue));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor Tan", (void*)&ezColor::Tan));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor Teal", (void*)&ezColor::Teal));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor Thistle", (void*)&ezColor::Thistle));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor Tomato", (void*)&ezColor::Tomato));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor Turquoise", (void*)&ezColor::Turquoise));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor Violet", (void*)&ezColor::Violet));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor Wheat", (void*)&ezColor::Wheat));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor White", (void*)&ezColor::White));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor WhiteSmoke", (void*)&ezColor::WhiteSmoke));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor Yellow", (void*)&ezColor::Yellow));
    AS_CHECK(m_pEngine->RegisterGlobalProperty("const ezColor YellowGreen", (void*)&ezColor::YellowGreen));

    AS_CHECK(m_pEngine->RegisterGlobalFunction("ezColor MakeNaN()", asFUNCTION(ezColor::MakeNaN), asCALL_CDECL));
    AS_CHECK(m_pEngine->RegisterGlobalFunction("ezColor MakeZero()", asFUNCTION(ezColor::MakeZero), asCALL_CDECL));
    AS_CHECK(m_pEngine->RegisterGlobalFunction("ezColor MakeRGBA(float,float,float,float)", asFUNCTION(ezColor::MakeRGBA), asCALL_CDECL));
    AS_CHECK(m_pEngine->RegisterGlobalFunction("ezColor MakeFromKelvin(uint32)", asFUNCTION(ezColor::MakeFromKelvin), asCALL_CDECL));
    AS_CHECK(m_pEngine->RegisterGlobalFunction("ezColor MakeHSV(float,float,float)", asFUNCTION(ezColor::MakeHSV), asCALL_CDECL));

    m_pEngine->SetDefaultNamespace("");
  }

  // Constructors
  {
    AS_CHECK(m_pEngine->RegisterObjectBehaviour("ezColor", asBEHAVE_CONSTRUCT, "void f(float, float, float, float a = 1.0f)", asFUNCTION(ezColor_ConstructRGBA), asCALL_CDECL_OBJFIRST));
    AS_CHECK(m_pEngine->RegisterObjectBehaviour("ezColor", asBEHAVE_CONSTRUCT, "void f(ezColorGammaUB)", asFUNCTION(ezColor_ConstructGamma), asCALL_CDECL_OBJFIRST));
  }

  // Operators
  {
    AS_CHECK(m_pEngine->RegisterObjectMethod("ezColor", "void opAssign(const ezColorGammaUB& in)", asMETHODPR(ezColor, operator=, (const ezColorGammaUB&), void), asCALL_THISCALL));
    AS_CHECK(m_pEngine->RegisterObjectMethod("ezColor", "void opAddAssign(const ezColor& in)", asMETHODPR(ezColor, operator+=, (const ezColor&), void), asCALL_THISCALL));
    AS_CHECK(m_pEngine->RegisterObjectMethod("ezColor", "void opSubAssign(const ezColor& in)", asMETHODPR(ezColor, operator-=, (const ezColor&), void), asCALL_THISCALL));
    AS_CHECK(m_pEngine->RegisterObjectMethod("ezColor", "void opMulAssign(const ezColor& in)", asMETHODPR(ezColor, operator*=, (const ezColor&), void), asCALL_THISCALL));
    AS_CHECK(m_pEngine->RegisterObjectMethod("ezColor", "void opMulAssign(float)", asMETHODPR(ezColor, operator*=, (float), void), asCALL_THISCALL));
    AS_CHECK(m_pEngine->RegisterObjectMethod("ezColor", "void opDivAssign(float)", asMETHODPR(ezColor, operator/=, (float), void), asCALL_THISCALL));
    AS_CHECK(m_pEngine->RegisterObjectMethod("ezColor", "void opMulAssign(const ezMat4& in)", asMETHODPR(ezColor, operator*=, (const ezMat4&), void), asCALL_THISCALL));


    AS_CHECK(m_pEngine->RegisterObjectMethod("ezColor", "ezColor opAdd(const ezColor& in) const", asFUNCTIONPR(operator+, (const ezColor&, const ezColor&), const ezColor), asCALL_CDECL_OBJFIRST));
    AS_CHECK(m_pEngine->RegisterObjectMethod("ezColor", "ezColor opSub(const ezColor& in) const", asFUNCTIONPR(operator-, (const ezColor&, const ezColor&), const ezColor), asCALL_CDECL_OBJFIRST));
    AS_CHECK(m_pEngine->RegisterObjectMethod("ezColor", "ezColor opMul(const ezColor& in) const", asFUNCTIONPR(operator*, (const ezColor&, const ezColor&), const ezColor), asCALL_CDECL_OBJFIRST));

    AS_CHECK(m_pEngine->RegisterObjectMethod("ezColor", "ezColor opMul(float) const", asFUNCTIONPR(operator*, (const ezColor&, float), const ezColor), asCALL_CDECL_OBJFIRST));
    AS_CHECK(m_pEngine->RegisterObjectMethod("ezColor", "ezColor opMul_r(float) const", asFUNCTIONPR(operator*, (float, const ezColor&), const ezColor), asCALL_CDECL_OBJFIRST));
    AS_CHECK(m_pEngine->RegisterObjectMethod("ezColor", "ezColor opDiv(float) const", asFUNCTIONPR(operator/, (const ezColor&, float), const ezColor), asCALL_CDECL_OBJFIRST));
    AS_CHECK(m_pEngine->RegisterObjectMethod("ezColor", "ezColor opMul_r(const ezMat4& in) const", asFUNCTIONPR(operator*, (const ezMat4&, const ezColor&), const ezColor), asCALL_CDECL_OBJFIRST));

    AS_CHECK(m_pEngine->RegisterObjectMethod("ezColor", "bool opEquals(const ezColor& in) const", asFUNCTIONPR(operator==, (const ezColor&, const ezColor&), bool), asCALL_CDECL_OBJFIRST));
  }

  // Methods
  {
    AS_CHECK(m_pEngine->RegisterObjectMethod("ezColor", "void SetRGB(float, float, float)", asMETHOD(ezColor, SetRGB), asCALL_THISCALL));
    AS_CHECK(m_pEngine->RegisterObjectMethod("ezColor", "void SetRGBA(float, float, float, float a = 1.0f)", asMETHOD(ezColor, SetRGBA), asCALL_THISCALL));
    AS_CHECK(m_pEngine->RegisterObjectMethod("ezColor", "void GetHSV(float& out, float& out, float& out) const", asMETHOD(ezColor, GetHSV), asCALL_THISCALL));
    AS_CHECK(m_pEngine->RegisterObjectMethod("ezColor", "ezVec4 GetAsVec4() const", asMETHOD(ezColor, GetAsVec4), asCALL_THISCALL));
    AS_CHECK(m_pEngine->RegisterObjectMethod("ezColor", "bool IsNormalized() const", asMETHOD(ezColor, IsNormalized), asCALL_THISCALL));
    AS_CHECK(m_pEngine->RegisterObjectMethod("ezColor", "float CalcAverageRGB() const", asMETHOD(ezColor, CalcAverageRGB), asCALL_THISCALL));
    AS_CHECK(m_pEngine->RegisterObjectMethod("ezColor", "float GetSaturation() const", asMETHOD(ezColor, GetSaturation), asCALL_THISCALL));
    AS_CHECK(m_pEngine->RegisterObjectMethod("ezColor", "float GetLuminance() const", asMETHOD(ezColor, GetLuminance), asCALL_THISCALL));
    AS_CHECK(m_pEngine->RegisterObjectMethod("ezColor", "ezColor GetInvertedColor() const", asMETHOD(ezColor, GetInvertedColor), asCALL_THISCALL));
    AS_CHECK(m_pEngine->RegisterObjectMethod("ezColor", "ezColor GetComplementaryColor() const", asMETHOD(ezColor, GetComplementaryColor), asCALL_THISCALL));
    AS_CHECK(m_pEngine->RegisterObjectMethod("ezColor", "void ScaleRGB(float)", asMETHOD(ezColor, ScaleRGB), asCALL_THISCALL));
    AS_CHECK(m_pEngine->RegisterObjectMethod("ezColor", "void ScaleRGBA(float)", asMETHOD(ezColor, ScaleRGBA), asCALL_THISCALL));
    AS_CHECK(m_pEngine->RegisterObjectMethod("ezColor", "float ComputeHdrMultiplier() const", asMETHOD(ezColor, ComputeHdrMultiplier), asCALL_THISCALL));
    AS_CHECK(m_pEngine->RegisterObjectMethod("ezColor", "float ComputeHdrExposureValue() const", asMETHOD(ezColor, ComputeHdrExposureValue), asCALL_THISCALL));
    AS_CHECK(m_pEngine->RegisterObjectMethod("ezColor", "void ApplyHdrExposureValue(float)", asMETHOD(ezColor, ApplyHdrExposureValue), asCALL_THISCALL));
    AS_CHECK(m_pEngine->RegisterObjectMethod("ezColor", "void NormalizeToLdrRange()", asMETHOD(ezColor, NormalizeToLdrRange), asCALL_THISCALL));
    AS_CHECK(m_pEngine->RegisterObjectMethod("ezColor", "ezColor GetDarker(float) const", asMETHOD(ezColor, GetDarker), asCALL_THISCALL));
    AS_CHECK(m_pEngine->RegisterObjectMethod("ezColor", "bool IsNaN() const", asMETHOD(ezColor, IsNaN), asCALL_THISCALL));
    AS_CHECK(m_pEngine->RegisterObjectMethod("ezColor", "bool IsValid() const", asMETHOD(ezColor, IsValid), asCALL_THISCALL));
    AS_CHECK(m_pEngine->RegisterObjectMethod("ezColor", "bool IsIdenticalRGB(const ezColor& in) const", asMETHOD(ezColor, IsIdenticalRGB), asCALL_THISCALL));
    AS_CHECK(m_pEngine->RegisterObjectMethod("ezColor", "bool IsIdenticalRGBA(const ezColor& in) const", asMETHOD(ezColor, IsIdenticalRGBA), asCALL_THISCALL));
    AS_CHECK(m_pEngine->RegisterObjectMethod("ezColor", "bool IsEqualRGB(const ezColor& in, float eps) const", asMETHOD(ezColor, IsEqualRGB), asCALL_THISCALL));
    AS_CHECK(m_pEngine->RegisterObjectMethod("ezColor", "bool IsEqualRGBA(const ezColor& in, float eps) const", asMETHOD(ezColor, IsEqualRGBA), asCALL_THISCALL));
    AS_CHECK(m_pEngine->RegisterObjectMethod("ezColor", "ezColor WithAlpha(float) const", asMETHOD(ezColor, WithAlpha), asCALL_THISCALL));
  }
}

//////////////////////////////////////////////////////////////////////////
// ezColorGammaUB
//////////////////////////////////////////////////////////////////////////

void ezColorGamma_ConstructRGBA(void* memory, ezUInt8 r, ezUInt8 g, ezUInt8 b, ezUInt8 a)
{
  new (memory) ezColorGammaUB(r, g, b, a);
}

void ezColorGamma_ConstructColor(void* memory, const ezColor& col)
{
  new (memory) ezColorGammaUB(col);
}

void ezAngelScriptEngineSingleton::Register_ColorGammaUB()
{
  AS_CHECK(m_pEngine->RegisterObjectProperty("ezColorGammaUB", "uint8 r", asOFFSET(ezColorGammaUB, r)));
  AS_CHECK(m_pEngine->RegisterObjectProperty("ezColorGammaUB", "uint8 g", asOFFSET(ezColorGammaUB, g)));
  AS_CHECK(m_pEngine->RegisterObjectProperty("ezColorGammaUB", "uint8 b", asOFFSET(ezColorGammaUB, b)));
  AS_CHECK(m_pEngine->RegisterObjectProperty("ezColorGammaUB", "uint8 a", asOFFSET(ezColorGammaUB, a)));

  // Constructors
  {
    AS_CHECK(m_pEngine->RegisterObjectBehaviour("ezColorGammaUB", asBEHAVE_CONSTRUCT, "void f(uint8, uint8, uint8, uint8 a = 255)", asFUNCTION(ezColorGamma_ConstructRGBA), asCALL_CDECL_OBJFIRST));
    AS_CHECK(m_pEngine->RegisterObjectBehaviour("ezColorGammaUB", asBEHAVE_CONSTRUCT, "void f(const ezColor& in)", asFUNCTION(ezColorGamma_ConstructColor), asCALL_CDECL_OBJFIRST));
  }

  // Operators
  {
    AS_CHECK(m_pEngine->RegisterObjectMethod("ezColorGammaUB", "void opAssign(const ezColor& in)", asMETHODPR(ezColorGammaUB, operator=, (const ezColor&), void), asCALL_THISCALL));
  }

  // Methods
  {
    AS_CHECK(m_pEngine->RegisterObjectMethod("ezColorGammaUB", "ezColor ToLinearFloat() const", asMETHOD(ezColorGammaUB, ToLinearFloat), asCALL_THISCALL));
  }
}


//////////////////////////////////////////////////////////////////////////
// ezRandom
//////////////////////////////////////////////////////////////////////////

void ezAngelScriptEngineSingleton::Register_Random()
{
  // static functions
  {
    m_pEngine->SetDefaultNamespace("ezRandom");
    m_pEngine->SetDefaultNamespace("");
  }

  // TODO AngelScript: Register ezRandom
}
