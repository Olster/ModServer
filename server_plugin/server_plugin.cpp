#include "server_plugin/server_plugin.h"

#include <cassert>

#include "base/dynamic_lib.h"
#include "net/ip_endpoint.h"

ServerPlugin::ServerPlugin() {}
ServerPlugin::~ServerPlugin() {}

DynamicPlugin::DynamicPlugin(std::shared_ptr<DynamicLib> lib)
 : m_lib(lib) {}

DynamicPlugin::~DynamicPlugin() {}

// Checks if the dynamic lib is a suitable plugin.
bool DynamicPlugin::IsValid() {
  if (!m_lib) {
    return false;
  }

  const char* exportedNames[] = {
    "GetPluginName",
    "SocketType",
    "DefaultPort",
    "NewHandler"
  };

  // NOTE(Olster): Have to use unsafe array size method here. ARR_SIZE fails.
  for (int i = 0; i < sizeof(exportedNames)/sizeof(exportedNames[0]); i++) {
    if (!m_lib->GetProc(exportedNames[i])) {
      return false;
    }
  }

  return true;
}

unsigned short DynamicPlugin::port() {
  typedef unsigned short (*DefaultPortFn)();
  DefaultPortFn DefaultPort = (DefaultPortFn)m_lib->GetProc("DefaultPort");

  return DefaultPort();
}

SockType DynamicPlugin::sock_type() {
  assert(m_lib);

  typedef SockType (*SocketTypeFn)();
  SocketTypeFn SocketType = (SocketTypeFn)m_lib->GetProc("SocketType");

  return SocketType();
}

ProtocolHandler* DynamicPlugin::NewProtocolHandler() {
  assert(m_lib);

  typedef ProtocolHandler* (*NewHandlerFn)();
  NewHandlerFn NewHandler = (NewHandlerFn)m_lib->GetProc("NewHandler");

  return NewHandler();
}

std::string DynamicPlugin::name() {
  assert(m_lib);

  typedef void (*GetPluginNameFn)(char* buf, int size);
  GetPluginNameFn GetPluginName = (GetPluginNameFn)m_lib->GetProc("GetPluginName");

  char name[256] = "";
  GetPluginName(name, ARR_SIZE(name));

  return name;
}
