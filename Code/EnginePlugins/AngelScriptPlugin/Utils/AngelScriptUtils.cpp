#include <AngelScriptPlugin/AngelScriptPluginPCH.h>

#include <AngelScript/include/angelscript.h>
#include <AngelScriptPlugin/Utils/AngelScriptUtils.h>
#include <Foundation/Types/Variant.h>

void ezAngelScriptUtils::WriteAsProperty(int iPropertyTypeID, void* pPropertyAddress, asIScriptEngine* pEngine, const ezVariant& value)
{
  void* pProp = pPropertyAddress;

  switch (iPropertyTypeID)
  {
    case asTYPEID_BOOL:
      *static_cast<ezInt8*>(pProp) = value.ConvertTo<bool>() ? 1 : 0;
      return;
    case asTYPEID_INT8:
      *static_cast<ezInt8*>(pProp) = value.ConvertTo<ezInt8>();
      return;
    case asTYPEID_INT16:
      *static_cast<ezInt16*>(pProp) = value.ConvertTo<ezInt16>();
      return;
    case asTYPEID_INT32:
      *static_cast<ezInt32*>(pProp) = value.ConvertTo<ezInt32>();
      return;
    case asTYPEID_INT64:
      *static_cast<ezInt64*>(pProp) = value.ConvertTo<ezInt64>();
      return;
    case asTYPEID_UINT8:
      *static_cast<ezUInt8*>(pProp) = value.ConvertTo<ezUInt8>();
      return;
    case asTYPEID_UINT16:
      *static_cast<ezUInt16*>(pProp) = value.ConvertTo<ezUInt16>();
      return;
    case asTYPEID_UINT32:
      *static_cast<ezUInt32*>(pProp) = value.ConvertTo<ezUInt32>();
      return;
    case asTYPEID_UINT64:
      *static_cast<ezUInt64*>(pProp) = value.ConvertTo<ezUInt64>();
      return;
    case asTYPEID_FLOAT:
      *static_cast<float*>(pProp) = value.ConvertTo<float>();
      return;
    case asTYPEID_DOUBLE:
      *static_cast<double*>(pProp) = value.ConvertTo<double>();
      return;
  }

  if (const asITypeInfo* pInfo = pEngine->GetTypeInfoById(iPropertyTypeID))
  {
    ezString sTypeName = pInfo->GetName();

    if (sTypeName == "string")
    {
      *static_cast<std::string*>(pProp) = value.ConvertTo<ezString>();
      return;
    }
    else if (sTypeName == "ezAngle")
    {
      *static_cast<ezAngle*>(pProp) = value.ConvertTo<ezAngle>();
      return;
    }
    else if (sTypeName == "ezTime")
    {
      *static_cast<ezTime*>(pProp) = value.ConvertTo<ezTime>();
      return;
    }
    else if (sTypeName == "ezColor")
    {
      *static_cast<ezColor*>(pProp) = value.ConvertTo<ezColor>();
      return;
    }
    else if (sTypeName == "ezColorGammaUB")
    {
      *static_cast<ezColorGammaUB*>(pProp) = value.ConvertTo<ezColorGammaUB>();
      return;
    }
    else if (sTypeName == "ezVec2")
    {
      *static_cast<ezVec2*>(pProp) = value.ConvertTo<ezVec2>();
      return;
    }
    else if (sTypeName == "ezVec3")
    {
      *static_cast<ezVec3*>(pProp) = value.ConvertTo<ezVec3>();
      return;
    }
    else if (sTypeName == "ezVec4")
    {
      *static_cast<ezVec4*>(pProp) = value.ConvertTo<ezVec4>();
      return;
    }
    else if (sTypeName == "ezQuat")
    {
      *static_cast<ezQuat*>(pProp) = value.ConvertTo<ezQuat>();
      return;
    }
    else if (sTypeName == "ezTransform")
    {
      *static_cast<ezTransform*>(pProp) = value.ConvertTo<ezTransform>();
      return;
    }
    else if (sTypeName == "ezString")
    {
      *static_cast<ezString*>(pProp) = value.ConvertTo<ezString>();
      return;
    }
  }

  // currently unsupported type for exposed parameter
  EZ_ASSERT_NOT_IMPLEMENTED;
}

ezResult ezAngelScriptUtils::ReadAsProperty(int iPropertyTypeID, void* pPropertyAddress, asIScriptEngine* pEngine, ezVariant& out_Value)
{
  void* pProp = pPropertyAddress;

  switch (iPropertyTypeID)
  {
    case asTYPEID_BOOL:
      out_Value = (*static_cast<ezInt8*>(pProp) != 0) ? true : false;
      return EZ_SUCCESS;

    case asTYPEID_INT8:
      out_Value = *static_cast<ezInt8*>(pProp);
      return EZ_SUCCESS;

    case asTYPEID_INT16:
      out_Value = *static_cast<ezInt16*>(pProp);
      return EZ_SUCCESS;

    case asTYPEID_INT32:
      out_Value = *static_cast<ezInt32*>(pProp);
      return EZ_SUCCESS;

    case asTYPEID_INT64:
      out_Value = *static_cast<ezInt64*>(pProp);
      return EZ_SUCCESS;

    case asTYPEID_UINT8:
      out_Value = *static_cast<ezUInt8*>(pProp);
      return EZ_SUCCESS;

    case asTYPEID_UINT16:
      out_Value = *static_cast<ezUInt16*>(pProp);
      return EZ_SUCCESS;

    case asTYPEID_UINT32:
      out_Value = *static_cast<ezUInt32*>(pProp);
      return EZ_SUCCESS;

    case asTYPEID_UINT64:
      out_Value = *static_cast<ezUInt64*>(pProp);
      return EZ_SUCCESS;

    case asTYPEID_FLOAT:
      out_Value = *static_cast<float*>(pProp);
      return EZ_SUCCESS;

    case asTYPEID_DOUBLE:
      out_Value = *static_cast<double*>(pProp);
      return EZ_SUCCESS;
  }

  if (const asITypeInfo* pInfo = pEngine->GetTypeInfoById(iPropertyTypeID))
  {
    const ezString sTypeName = pInfo->GetName();

    if (sTypeName == "string")
    {
      out_Value = static_cast<std::string*>(pProp)->c_str();
      return EZ_SUCCESS;
    }
    else if (sTypeName == "ezAngle")
    {
      out_Value = *static_cast<ezAngle*>(pProp);
      return EZ_SUCCESS;
    }
    else if (sTypeName == "ezTime")
    {
      out_Value = *static_cast<ezTime*>(pProp);
      return EZ_SUCCESS;
    }
    else if (sTypeName == "ezColor")
    {
      out_Value = *static_cast<ezColor*>(pProp);
      return EZ_SUCCESS;
    }
    else if (sTypeName == "ezColorGammaUB")
    {
      out_Value = *static_cast<ezColorGammaUB*>(pProp);
      return EZ_SUCCESS;
    }
    else if (sTypeName == "ezVec2")
    {
      out_Value = *static_cast<ezVec2*>(pProp);
      return EZ_SUCCESS;
    }
    else if (sTypeName == "ezVec3")
    {
      out_Value = *static_cast<ezVec3*>(pProp);
      return EZ_SUCCESS;
    }
    else if (sTypeName == "ezVec4")
    {
      out_Value = *static_cast<ezVec4*>(pProp);
      return EZ_SUCCESS;
    }
    else if (sTypeName == "ezQuat")
    {
      out_Value = *static_cast<ezQuat*>(pProp);
      return EZ_SUCCESS;
    }
    else if (sTypeName == "ezTransform")
    {
      out_Value = *static_cast<ezTransform*>(pProp);
      return EZ_SUCCESS;
    }
    else if (sTypeName == "ezString")
    {
      out_Value = *static_cast<ezString*>(pProp);
      return EZ_SUCCESS;
    }
    else if (sTypeName == "ezStringView")
    {
      out_Value = *static_cast<ezStringView*>(pProp);
      return EZ_SUCCESS;
    }
    else if (sTypeName == "ezHashedString")
    {
      out_Value = *static_cast<ezHashedString*>(pProp);
      return EZ_SUCCESS;
    }
    else if (sTypeName == "ezTempHashedString")
    {
      out_Value = *static_cast<ezTempHashedString*>(pProp);
      return EZ_SUCCESS;
    }
  }

  // currently unsupported type for exposed parameter
  return EZ_FAILURE;
}

const char* ezAngelScriptUtils::VariantTypeToString(ezVariantType::Enum type)
{
  switch (type)
  {
    case ezVariantType::Bool:
      return "bool";
    case ezVariantType::Double:
      return "double";
    case ezVariantType::Float:
      return "float";
    case ezVariantType::Int8:
      return "int8";
    case ezVariantType::Int16:
      return "int16";
    case ezVariantType::Int32:
      return "int32";
    case ezVariantType::Int64:
      return "int64";
    case ezVariantType::UInt8:
      return "uint8";
    case ezVariantType::UInt16:
      return "uint16";
    case ezVariantType::UInt32:
      return "uint32";
    case ezVariantType::UInt64:
      return "uint64";
    case ezVariantType::Angle:
      return "ezAngle";
    case ezVariantType::Matrix3:
      return "ezMat3";
    case ezVariantType::Matrix4:
      return "ezMat4";
    case ezVariantType::Quaternion:
      return "ezQuat";
    case ezVariantType::Time:
      return "ezTime";
    case ezVariantType::Transform:
      return "ezTransform";
    case ezVariantType::Vector2:
      return "ezVec2";
    case ezVariantType::Vector3:
      return "ezVec3";
    case ezVariantType::Vector4:
      return "ezVec4";
    case ezVariantType::Color:
      return "ezColor";
    case ezVariantType::ColorGamma:
      return "ezColorGammaUB";

    case ezVariantType::String:
      return "ezString";
    case ezVariantType::HashedString:
      return "ezHashedString";
    case ezVariantType::StringView:
      return "ezStringView";
    case ezVariantType::TempHashedString:
      return "ezTempHashedString";

    default:
      break;
  }

  return nullptr;
}
