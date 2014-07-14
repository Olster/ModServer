#include "server_plugin/plugin_loader.h"

#include <cassert>

#include "base/dynamic_lib.h"
#include "base/logger.h"

#include "server_plugin/server_plugin.h"

PluginLoader::~PluginLoader() {
  UnloadAll();
}

void PluginLoader::LoadAll(const std::string& pluginsFolder) {
  UNUSED(pluginsFolder);

  DynamicLib* httpLib = DynamicLib::Load("C://Users//Ivan//Documents//Visual Studio 2013//Projects//stHTTPs//Debug//plugins//http.dll");

  if (!httpLib) {
    Logger::Log(Logger::WARN, "No HTTP plugin found");
    return;
  }

  ServerPlugin* plugin = new ServerPlugin(httpLib);

  if (plugin->IsValid()) {
    AddPlugin(plugin);
  } else {
    delete plugin;
  }  
}

void PluginLoader::UnloadAll() {
  // NOTE(Olster): Removes controller as well!
  for (auto& plugin : m_plugins) {
    delete plugin;
  }
}

void PluginLoader::AddPlugin(ServerPlugin* plugin) {
  assert(plugin);

  m_plugins.push_back(plugin);
}

bool PluginLoader::HasLoadedPlugins() const {
  return !m_plugins.empty();
}

void PluginLoader::GetPlugins(std::list<ServerPlugin*>* plugins) {
  assert(plugins);
  *plugins = m_plugins;
}
