#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <vector>

struct ledburn_t {
  char header[8];
  uint32_t frame_id;
  uint32_t num_segments;
  uint32_t segment_id;
  uint16_t strip_id;
  uint16_t pixel_id;
};

class UDPSender
{
  public:
  UDPSender(const char *hostname, int port, int mtu_=500);
  ~UDPSender();

    // send LedBurn protocol UDP packets using the send method.
  size_t send(int strip_id, int frame_id, uint8_t* colordata, size_t size, 
    int pixel_id=1);

  private:
    int mtu;
    int sockfd;
    struct sockaddr_in serveraddr;
    struct hostent *server;
    std::vector<uint8_t> buf;
};
