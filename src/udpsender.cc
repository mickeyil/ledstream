#include "udpsender.h"

UDPSender::UDPSender(const char *hostname, int port, int mtu_) :
    mtu(mtu_)
{ 
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        printf("ERROR opening socket");
        return;
    }
    
    server = gethostbyname(hostname);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host as %s\n", hostname);
        exit(0);
    }

    bzero((char *) &serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
        (char *)&serveraddr.sin_addr.s_addr, server->h_length);
    serveraddr.sin_port = htons(port);

    buf.resize(mtu);
}

UDPSender::~UDPSender()
{
    close(sockfd);
}

size_t UDPSender::send(int strip_id, int frame_id, uint8_t* colordata,
    size_t size, int pixel_id)
{
    // available size for LED data when discounting header size
    size_t max_dsize = mtu - sizeof(ledburn_t);
    size_t dsize = std::min(size, max_dsize);

    size_t num_segments = size / dsize;
    if (num_segments * dsize < size) {
        num_segments += 1;
    }

    // populate header
    ledburn_t header;
    strncpy(header.header, "LedBurn", 7);
    header.header[7] = 0;       // protocol version
    header.frame_id = static_cast<uint32_t>(frame_id);
    header.num_segments = static_cast<uint32_t>(num_segments);
    header.strip_id = static_cast<uint16_t>(strip_id);
    header.pixel_id = static_cast<uint16_t>(pixel_id); // offset

    // send segments as different UDP packets
    for (size_t i = 0; i < num_segments; i++) {
        // reset buffer
        memset(&buf[0], 0, sizeof(buf[0]) * buf.size());

        // copy header
        memcpy(&buf[0], &header, sizeof(header));
        (reinterpret_cast<ledburn_t*>(&buf[0]))->segment_id = i;

        // copy data
        size_t offset = dsize * i;
        memcpy(&buf[sizeof(ledburn_t)], &colordata[offset], dsize);

        // send data from buffer to socket
        int n = sendto(sockfd, &buf[0], buf.size(), 0, (struct sockaddr *) 
            &serveraddr, sizeof(serveraddr));
        if (n < 0) {
            printf("error sending packet.");
            return 0;
        }
    }
    return num_segments;
}
