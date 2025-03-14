#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SGW_C_IP "192.168.100.2" // SGW Control Plane IP
#define GTP_PORT 2123  // Standard GTP-C port

int main() {
    int sockfd;
    struct sockaddr_in serverAddr;
    char buffer[1024];

    // Create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        std::cerr << "Socket creation failed" << std::endl;
        return -1;
    }

    // Configure SGW-C address
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(GTP_PORT);
    inet_pton(AF_INET, SGW_C_IP, &serverAddr.sin_addr);

    // Construct a valid GTPv2-C Create Session Request
    unsigned char gtpMessage[] = {
        // GTPv2-C Header (8 bytes)
        0x48, 0x20, 0x00, 0x32,  // Flags, Msg Type (0x20 = Create Session Request), Length (0x32 = 50 bytes)
        0x12, 0x34, 0x56, 0x78,  // TEID (Tunnel Endpoint Identifier)
        0x00, 0x01, 0x00, 0x10,  // Sequence Number, Spare

        // IMSI (IE Type = 0x01, Length = 8)
        0x01, 0x00, 0x08, 0x00,  // IE Type (0x01), Length (8 bytes), Instance (0)
        0x21, 0x43, 0x65, 0x87, 0x09, 0xAB, 0xCD, 0xEF,  // IMSI value

        // APN (IE Type = 0x57, Length = 8)
        0x57, 0x00, 0x08, 0x00,  // IE Type (0x57), Length (8 bytes), Instance (0)
        'i', 'n', 't', 'e', 'r', 'n', 'e', 't',  // APN string ("internet")

        // PDN Type (IE Type = 0x79, Length = 1)
        0x79, 0x00, 0x01, 0x00,  // IE Type (0x79), Length (1 byte), Instance (0)
        0x02,  // PDN Type (IPv4)

        // SGW TEID for Uplink (IE Type = 0x50, Length = 4)
        0x50, 0x00, 0x04, 0x00,  // IE Type (0x50), Length (4 bytes), Instance (0)
        0x12, 0x34, 0x56, 0x79,  // TEID value

        // PAA (UE IP Address) (IE Type = 0x4F, Length = 5)
        0x4F, 0x00, 0x05, 0x00,  // IE Type (0x4F), Length (5 bytes), Instance (0)
        0x01,  // PDN Type (IPv4)
        0x0A, 0x0A, 0x00, 0x01   // UE IPv4 Address: 10.10.0.1
    };

    // Send GTP message to SGW-C
    if (sendto(sockfd, gtpMessage, sizeof(gtpMessage), 0,
               (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Failed to send GTP message" << std::endl;
        close(sockfd);
        return -1;
    }

    std::cout << "Create Session Request sent to SGW-C" << std::endl;

    // Receive response from SGW-C
    socklen_t addrLen = sizeof(serverAddr);
    int recvLen = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&serverAddr, &addrLen);
    if (recvLen > 0) {
        std::cout << "Received response from SGW-C" << std::endl;
    } else {
        std::cerr << "Failed to receive response" << std::endl;
    }

    // Close socket
    close(sockfd);
    return 0;
}
