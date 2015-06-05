#include "server_plugin/plugin_loader.h"

#include <cassert>

#include "base/dynamic_lib.h"
#include "base/logger.h"

#include "server_plugin/server_plugin.h"

namespace {
#if defined(WIN32)
const wchar_t* const kDllExt = L".dll";
#else
const char* const kDllExt = ".so";
#endif
}

PluginLoader::~PluginLoader() {
  UnloadAll();
}

void PluginLoader::LoadAll(const Path::StringType& pluginsFolder) {
  Log(INFO) << "Searching for plugins in " << pluginsFolder;

  std::vector<Folder> searchFolders;

  Folder searchRootFolder = Folder(Path(pluginsFolder));
  searchRootFolder.GetAllSubfolders(&searchFolders);

  // Search in root dir too.
  searchFolders.push_back(searchRootFolder);

  for (std::vector<Folder>::const_iterator it = searchFolders.cbegin();
       it != searchFolders.cend(); ++it) {
    Log(INFO) << "\tSearching in subfolder " << it->path().ToString();

    std::vector<File> files;
    Path::StringType wildcard = Path::StringType(PATH_LITERAL("*_plugin")) +
                                kDllExt;

    it->GetFilesWildcard(wildcard, &files);
    for (std::vector<File>::const_iterator fileIt = files.cbegin();
         fileIt != files.cend(); ++fileIt) {
      Log(INFO) << "Trying file " << fileIt->path().ToString();

      int err = 0;
      std::shared_ptr<DynamicLib> pluginDll(
        DynamicLib::Load(fileIt->path().ToString(), &err));

      if (!pluginDll) {
        Log(WARN) << "Can't load shared library, err: " << err;
        continue;
      }

      std::unique_ptr<DynamicPlugin> plugin(new DynamicPlugin(pluginDll));
      if (!plugin->IsValid()) {
        Log(WARN) << "Not a valid plugin";
        continue;
      }

      AddPlugin(plugin.release());
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
