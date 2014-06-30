#include "protocols/controller/controller_handler.h"

#include "base/logger.h"

#include "net/server.h"

void ControllerHandler::DidReceive(char* data, int /*size*/) {
  m_data.append(data);

  if (!m_data.empty()) {
    if (m_data[m_data.length() - 1] == '\n') {
      Logger::Log(Logger::INFO, "Message from controller: %s", m_data.c_str());

      if (m_data == "Unload all plugins\r\n") {
        m_server->UnloadAllPlugins();
      }

      m_data.clear();
    }
  }
}
