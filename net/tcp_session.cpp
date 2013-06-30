#include "net/tcp_session.h"
#include "net/tcp_socket.h"

#ifdef DEBUG
#include <iostream>
#endif

namespace net {

TCPSession::TCPSession(net::TCPSocket* receiver, std::string resPath)
 : m_receiver(receiver), m_resPath(resPath) {

}

TCPSession::~TCPSession() {
  delete m_receiver;
  m_receiver = nullptr;
}

int TCPSession::Receive() {
  if (m_bAllDataSent) {
    int bytesReceived = m_receiver->Receive(m_request);

#ifdef DEBUG
if (bytesReceived > 0) {
  std::cout << "Received from " << m_receiver->GetHandle() << ":\n" << m_request << std::endl;
}
#endif

    if (bytesReceived <= 0) {
      return bytesReceived;
    }

    m_requestParser.Parse(m_request);
    if (m_requestParser.IsRequestValid()) {
      // If resource doesn't exist, give the 404 error
      bool resLoaded = FillResource(m_requestParser, m_resource);

      if (!resLoaded) {
        m_response = "HTTP/1.1 404 Not Found\nContent-type: text/html\nContent-length: " +
                   std::to_string(m_notFoundResponse.length()) + "\n\n" +
                   m_notFoundResponse + "\n\n";
      } else {
        m_response = "HTTP/1.1 200 OK\nContent-Type: " +
                    m_resource.GetMIMEType() + "\nContent-Length: " +
                    std::to_string(m_resource.GetData().length()) +
                    "\n\n" + m_resource.GetData() + "\n\n";
      }
    }

    return bytesReceived;
  }

  // |1| to keep the session open
  return 1;
}

int TCPSession::Send() {
  // If we have nothing to send, then go on
  if (!m_response.empty()) {
#ifdef DEBUG
std::cout << "Sending to " << m_receiver->GetHandle() << "\n" << /*m_response <<*/ std::endl;
#endif

    int bytesSent = m_receiver->Send(m_response);
//    if (bytesSent >= m_response.length()) {
//      m_bAllDataSent = true;
//      m_response.clear();
//    } else {
//      m_response = m_response.substr(bytesSent);
//      m_bAllDataSent = false;
//    }
    m_bAllDataSent = true;
    m_response.clear();

    return bytesSent;
  }

  return 0;
}

bool TCPSession::FillResource(const net::HTTPParser& parser, net::Resource& res) {
  if (parser.GetResourceURI() == "/") {
    // Asking for the site root
    return LoadResource("/index.html", res);
  }

  return LoadResource(parser.GetResourceURI(), res);
}

bool TCPSession::LoadResource(std::string filePath, net::Resource& res) {
  // Should return error code

  // TODO(Olster): This is not reliable, would fail if the
  // requested file is too big. Use chunked transfer encoding?

  FILE* file = nullptr;

  // TODO(Olster): The path to the folder containing resources will be set
  // when creating the HTTPServer as a parameter in a constructor.
  // Or make a configuration class and pass it as a parameter.

  filePath = m_resPath + filePath;

#ifdef DEBUG
std::cout << "\tOpening file " << filePath << std::endl;
#endif

  file = fopen(filePath.c_str(), "rb");

  if (!file) {
    return false;
  }

  char* buf = nullptr;

  fseek(file, 0, SEEK_END);
  int size = ftell(file);

  fseek(file, 0, SEEK_SET);

  buf = new char[size + 1];
  buf[size] = '\0';

  // Read EVERYTHING in one go: read chunk of the size |size|
  // once.
  // TODO(Olster): Check for error code after reading
  fread(buf, size, 1, file);

  fclose(file);
  file = nullptr;

  // NOTE(Olster): Can't do the assignment, cause binary data would be corrupt
  // the constructor that takes char as a parameter will delete all non-printed
  // characters
  res.SetData(std::string(buf, size - 1));

  delete [] buf;
  buf = nullptr;

  std::string::size_type extensionDot = filePath.find_last_of('.');
  std::string extension = filePath.substr(extensionDot + 1);

#ifdef DEBUG
std::cout << "\tInfo: Resource extension: " << extension << std::endl;
#endif

  // TODO(Olster): Build a map that returns MIME type instead of ifs

  // Text
  if (extension == "html" || extension == "htm") {
    res.SetMIMEType("text/html");
  }

  if (extension == "css") {
    res.SetMIMEType("text/css");
  }

  // Image
  if (extension == "ico ") {
    res.SetMIMEType("image/x-image");
  }

  if (extension == "png") {
    res.SetMIMEType("image/png");
  }

  if (extension == "gif") {
    res.SetMIMEType("image/gif");
  }

  if (extension == "jpg" || extension == "jpeg") {
    res.SetMIMEType("image/jpeg");
  }

  if (extension == "mp3") {
    res.SetMIMEType("audio/mpeg");
  }

  return true;
}

} // namespace net
