#ifndef PLUGIN_LOADER_H_
#define PLUGIN_LOADER_H_

#include <list>
#include <map>

#include "base/build_required.h"

class ServerPlugin;

struct PluginData {
  typedef void(*FreePluginFn)(ServerPlugin*);
  FreePluginFn freeFn;

  ServerPlugin* plugin;
};

class PluginLoader {
 public:
  PluginLoader() {}
  ~PluginLoader();

  void LoadAll();
  void UnloadAll();

  void AddPlugin(PluginData* pluginData, void* dynamicLib = NULL);

  bool HasLoadedPlugins() const;

  void GetPlugins(std::list<ServerPlugin*>& plugins);
 private:
  std::map<PluginData*, void*> m_pluginsLibs;

  DISALLOW_COPY_AND_ASSIGN(PluginLoader);
  DISALLOW_MOVE(PluginLoader);
};

#endif // PLUGIN_LOADER_H_
