#include "net/ip_endpoint.h"

#include "gtest/gtest.h"

TEST(IpEndPointTest, ValidTest) {
  // IP and port as constructor params.
  IPEndPoint ep("127.0.0.1", 80);
  EXPECT_TRUE(ep.IsValid());

  // Invalid params.
  IPEndPoint ep4("127.0.0.1", 0);
  EXPECT_FALSE(ep4.IsValid());

  IPEndPoint ep5("NotAnIP", 80);
  EXPECT_FALSE(ep5.IsValid());
}

TEST(IpEndPointTest, SetDefault) {
  // Default c-tor.
  IPEndPoint ep;
  EXPECT_FALSE(ep.IsValid());

  ep.set_ip("127.0.0.1");
  ep.set_port(80);

  EXPECT_TRUE(ep.IsValid());
}

TEST(IpEndPointTest, SetGetParams) {
  IPEndPoint ep;

  char* ip = "127.0.0.1";
  ep.set_ip(ip);
  EXPECT_STREQ(ip, ep.ip());

  unsigned short port = 80;
  ep.set_port(port);
  EXPECT_EQ(port, ep.port());
}
