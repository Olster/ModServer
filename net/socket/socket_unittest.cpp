#include "net/socket/socket.h"
#include "gtest/gtest.h"

class TestSocket : public Socket {
 public:
  virtual bool Open(int* err = NULL) override {
    return OpenHelper(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  }
};

TEST(SocketTest, OpenClose) {
  TestSocket sock;

  EXPECT_EQ(sock.handle(), Socket::kInvalidSocket);

  ASSERT_TRUE(sock.Open());
  EXPECT_NE(sock.handle(), Socket::kInvalidSocket);

  ASSERT_TRUE(sock.Close());
  EXPECT_EQ(sock.handle(), Socket::kInvalidSocket);
}
