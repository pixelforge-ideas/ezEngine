#pragma once

#include <AngelScriptPlugin/AngelScriptPluginDLL.h>

#include <Foundation/Basics.h>
#include <Foundation/Types/VariantType.h>

class asIScriptEngine;
class ezVariant;

class EZ_ANGELSCRIPTPLUGIN_DLL ezAngelScriptUtils
{
public:
  static void WriteAsProperty(int iPropertyTypeID, void* pPropertyAddress, asIScriptEngine* pEngine, const ezVariant& value);
  static ezResult ReadAsProperty(int iPropertyTypeID, void* pPropertyAddress, asIScriptEngine* pEngine, ezVariant& out_Value);

  static const char* VariantTypeToString(ezVariantType::Enum type);
};
