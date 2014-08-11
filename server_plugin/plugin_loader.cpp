#include "server_plugin/plugin_loader.h"

#include <cassert>

#include "base/dynamic_lib.h"
#include "base/logger.h"
#include "base/path.h"

#include "server_plugin/server_plugin.h"

namespace {
#if defined(WIN32)
const char* const kDllExt = ".dll";
#else
const char* const kDllExt = ".so";
#endif
}

PluginLoader::~PluginLoader() {
  UnloadAll();
}

void PluginLoader::LoadAll(const std::string& pluginsFolder) {
  Logger::Log(Logger::INFO, "Searching for plugins in %s",
              pluginsFolder.c_str());

  std::vector<Folder> subfolders;
  GetAllSubfolders(Folder(pluginsFolder), &subfolders);
  
  for (std::vector<Folder>::const_iterator it = subfolders.cbegin();
       it != subfolders.cend(); ++it) {
    Logger::Log(Logger::INFO, "Searching in subfolder %s",
                it->path().ToString());
    
    std::vector<File> files;
    
    Path::StringType wildcard = Path::StringType(PATH_LITERAL("*_plugin")) +
                                kDllExt;
    
    it->GetFilesWildcard(wildcard, &files);
    for (std::vector<File>::const_iterator fileIt = files.cbegin();
         fileIt != files.cend(); ++fileIt) {
      Logger::Log(Logger::INFO, "Trying file %s", fileIt->path().ToString());
      
      int err = 0;
      std::shared_ptr<DynamicLib> pluginDll(
        DynamicLib::Load(fileIt->path().ToString(), &err));
        
      if (!pluginDll) {
        Logger::Log(Logger::WARN, "Can't load shared library, err: %d", err);
        continue;
      }
      
      std::shared_ptr<DynamicPlugin> plugin(new DynamicPlugin(pluginDll));
      if (!plugin->IsValid()) {
        Logger::Log(Logger::WARN, "Not a valid plugin");
        continue;
      }
      
      AddPlugin(plugin);
    }
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
