#pragma once

#include <EditorFramework/Assets/SimpleAssetDocument.h>
#include <ToolsFoundation/NodeObject/DocumentNodeManager.h>

class ezAngelScriptAssetDocument;


class ezAngelScriptParameter : public ezReflectedClass
{
  EZ_ADD_DYNAMIC_REFLECTION(ezAngelScriptParameter, ezReflectedClass);

public:
  bool m_bExpose = false;
  ezString m_sDeclaration;
  ezString m_sName;
  ezVariant m_DefaultValue;
};

class ezAngelScriptAssetProperties : public ezReflectedClass
{
  EZ_ADD_DYNAMIC_REFLECTION(ezAngelScriptAssetProperties, ezReflectedClass);

  ezString m_sScriptFile;
  ezString m_sClassName;

  ezDynamicArray<ezAngelScriptParameter> m_Parameters;
};

class ezAngelScriptAssetDocument : public ezSimpleAssetDocument<ezAngelScriptAssetProperties>
{
  EZ_ADD_DYNAMIC_REFLECTION(ezAngelScriptAssetDocument, ezSimpleAssetDocument<ezAngelScriptAssetProperties>);

public:
  ezAngelScriptAssetDocument(ezStringView sDocumentPath);

  void OpenExternalEditor();
  void SyncExposedParameters();

protected:
  virtual ezTransformStatus InternalTransformAsset(const char* szTargetFile, ezStringView sOutputTag, const ezPlatformProfile* pAssetProfile,
    const ezAssetFileHeader& AssetHeader, ezBitflags<ezTransformFlags> transformFlags) override;
  virtual ezTransformStatus InternalTransformAsset(ezStreamWriter& stream, ezStringView sOutputTag, const ezPlatformProfile* pAssetProfile, const ezAssetFileHeader& AssetHeader, ezBitflags<ezTransformFlags> transformFlags) override;

  void SyncInfos();

  virtual void UpdateAssetDocumentInfo(ezAssetDocumentInfo* pInfo) const override;
};
