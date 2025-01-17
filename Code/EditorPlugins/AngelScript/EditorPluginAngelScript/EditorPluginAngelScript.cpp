#include <EditorPluginAngelScript/EditorPluginAngelScriptPCH.h>

#include <EditorFramework/Actions/AssetActions.h>
#include <EditorFramework/Actions/ProjectActions.h>
#include <EditorPluginAngelScript/Actions/AngelScriptActions.h>
#include <GuiFoundation/Action/ActionMapManager.h>
#include <GuiFoundation/Action/CommandHistoryActions.h>
#include <GuiFoundation/Action/DocumentActions.h>
#include <GuiFoundation/Action/EditActions.h>
#include <GuiFoundation/Action/StandardMenus.h>

void OnLoadPlugin()
{
  ezAngelScriptActions::RegisterActions();

  // AngelScript
  {
    // Menu Bar
    {
      ezActionMapManager::RegisterActionMap("AngelScriptAssetMenuBar", "AssetMenuBar");
      ezEditActions::MapActions("AngelScriptAssetMenuBar", false, false);
    }

    // Tool Bar
    {
      ezActionMapManager::RegisterActionMap("AngelScriptAssetToolBar", "AssetToolbar");
      ezAngelScriptActions::MapActions("AngelScriptAssetToolBar");
    }
  }
}

void OnUnloadPlugin()
{
  ezAngelScriptActions::UnregisterActions();
}

EZ_PLUGIN_ON_LOADED()
{
  OnLoadPlugin();
}

EZ_PLUGIN_ON_UNLOADED()
{
  OnUnloadPlugin();
}
