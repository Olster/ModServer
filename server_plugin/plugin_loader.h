#ifndef PLUGIN_LOADER_H_
#define PLUGIN_LOADER_H_

#include <list>

#include "base/build_required.h"

class ServerPlugin;

class PluginLoader {
 public:
  PluginLoader() {}
  ~PluginLoader();

  void LoadAll(const std::string& pluginsFolder);
  void UnloadAll();

  // Adds a plugin to the list of plugins.
  // Can be used if the plugin object was created in this proces.
  void AddPlugin(ServerPlugin* plugin);

  // Returns true if any plugins are loaded.
  bool HasLoadedPlugins() const;

  void GetPlugins(std::list<ServerPlugin*>* plugins);
 private:
  std::list<ServerPlugin*> m_plugins;

  DISALLOW_COPY_AND_ASSIGN(PluginLoader);
  DISALLOW_MOVE(PluginLoader);
};

#endif // PLUGIN_LOADER_H_
