#ifndef SERVER_PLUGIN_SERVER_PLUGIN_H_
#define SERVER_PLUGIN_SERVER_PLUGIN_H_

#include <memory>
#include <string>

#include "base/build_required.h"
#include "plugin_api/plugin.h"

class ProtocolHandler;
class DynamicLib;

class ServerPlugin {
 public:
  ServerPlugin();
  virtual ~ServerPlugin();

  // Return true if plugin is valid.
  virtual bool IsValid() = 0;

  virtual unsigned short port() = 0;
  virtual SockType sock_type() = 0;

  virtual ProtocolHandler* NewProtocolHandler() = 0;

  virtual std::string name() = 0;

 private:
  DISALLOW_COPY_AND_ASSIGN(ServerPlugin);
  DISALLOW_MOVE(ServerPlugin);
};

class DynamicPlugin : public ServerPlugin {
 public:
  explicit DynamicPlugin(std::shared_ptr<DynamicLib> lib);
  virtual ~DynamicPlugin();

  // Checks if the dynamic lib is a suitable plugin.
  virtual bool IsValid() override;

  virtual unsigned short port() override;
  virtual SockType sock_type() override;

  virtual ProtocolHandler* NewProtocolHandler() override;

  virtual std::string name() override;

 private:
  std::shared_ptr<DynamicLib> m_lib;
};

#endif  // SERVER_PLUGIN_SERVER_PLUGIN_H_
