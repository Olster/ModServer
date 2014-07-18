#include "server_plugin/plugin_loader.h"

#include <cassert>

#include "base/dynamic_lib.h"
#include "base/logger.h"

#include "server_plugin/server_plugin.h"

PluginLoader::~PluginLoader() {
  UnloadAll();
}

void PluginLoader::LoadAll(const std::string& pluginsFolder) {
  DynamicLib* httpLib = DynamicLib::Load(pluginsFolder + "http.dll");

  if (!httpLib) {
    Logger::Log(Logger::WARN, "No HTTP plugin found");
    return;
  }

  std::unique_ptr<DynamicPlugin> plugin(new DynamicPlugin(httpLib));
  if (plugin->IsValid()) {
    AddPlugin(plugin.release());
  }
}

void PluginLoader::UnloadAll() {
  // NOTE(Olster): Removes controller as well!
  for (auto& plugin : m_plugins) {
    delete plugin;
  }

  m_plugins.clear();
}

void PluginLoader::AddPlugin(ServerPlugin* plugin) {
  assert(plugin);
  m_plugins.push_back(plugin);
}

bool PluginLoader::HasLoadedPlugins() const {
  return !m_plugins.empty();
}

const std::list<ServerPlugin*>& PluginLoader::GetPlugins() {
  return m_plugins;
}
