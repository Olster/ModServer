#include "server_plugin/plugin_loader.h"
#include "server_plugin/server_plugin.h"

#include "base/logger.h"

// TODO(Olster): Add layer for multiplatform code.
#include <windows.h>
#include <tchar.h>

PluginLoader::~PluginLoader() {
  UnloadAll();
}

void PluginLoader::LoadAll() {
  HMODULE http = ::LoadLibrary(_T("C://Users//Ivan//Documents//Visual Studio 2013//Projects//stHTTPs//Debug//plugins//http.dll"));

  if (!http) {
    Logger::Log(Logger::WARN, "No HTTP plugin found");
    return;
  }

  typedef ServerPlugin* (*NewPluginInstanceFn)();
  NewPluginInstanceFn NewInstance = (NewPluginInstanceFn)
    ::GetProcAddress(http, "NewPluginInstance");

  if (!NewInstance) {
    ::FreeLibrary(http);
    return;
  }
  
  PluginData::FreePluginFn freePlugin = (PluginData::FreePluginFn)
    ::GetProcAddress(http, "FreePluginInstance");

  if (!freePlugin) {
    ::FreeLibrary(http);
    return;
  }

  PluginData* pluginData = new PluginData;
  pluginData->freeFn = freePlugin;
  pluginData->plugin = NewInstance();

  AddPlugin(pluginData, http);
}

void PluginLoader::UnloadAll() {
  // TODO(Olster): Removes controller as well!
  for (auto& pluginDataLib : m_pluginsLibs) {
    pluginDataLib.first->freeFn(pluginDataLib.first->plugin);
    ::FreeLibrary(reinterpret_cast<HMODULE>(pluginDataLib.second));

    delete pluginDataLib.first;
  }
}

void PluginLoader::AddPlugin(PluginData* pluginData, void* dynamicLib) {
  m_pluginsLibs.insert(std::make_pair(pluginData, dynamicLib));
}

bool PluginLoader::HasLoadedPlugins() const {
  return !m_pluginsLibs.empty();
}

void PluginLoader::GetPlugins(std::list<ServerPlugin*>& plugins) {
  for (auto& pair : m_pluginsLibs) {
    plugins.push_back(pair.first->plugin);
  }
}
