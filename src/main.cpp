#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "gtpv2_msg/gtpv2.h"

#define SMF_IP "192.168.96.231" // SMF IP
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
    inet_pton(AF_INET, SMF_IP, &serverAddr.sin_addr);

    if (sizeof(gtpMessage) != sizeof(gtpMessage2))
    {
        std::cerr << "ERROR -> the two msgs are with different sizes. gtpMessage: "
                                            << sizeof(gtpMessage) << " | gtpMessage2: " << sizeof(gtpMessage2) << '\n';
    }

    Header h;
    uint32_t teid {111111};
    h.set_teid(&teid);
    printf("Header: %lu length: %x\n", sizeof(h), h.m_msg_type);
    for (size_t i = 0; i < 4; i++)
    {
        printf("Header: %lu length: %x\n", sizeof(h), h.m_teid[i]);
    }

    IMSI imsi;
    imsi.print_IMSI();
    
    GTPv2 gtp;
    printf("Header: %lu\n", sizeof(gtp));
    
    char arr[2];
    printf("arr: %lu\n", sizeof(arr));

#if 0
    // Send GTP message to SGW-C
    ssize_t sentBytes = sendto(sockfd, gtpMessage2, sizeof(gtpMessage2), 0,
                               (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    if (sentBytes < 0) {
        std::cerr << "Failed to send GTP message" << std::endl;
        close(sockfd);
        return -1;
    }

    std::cout << "Create Session Request sent to SMF. Sent bytes: " << sentBytes << std::endl;

    // Receive response from SGW-C
    socklen_t addrLen = sizeof(serverAddr);
    int recvLen = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&serverAddr, &addrLen);
    if (recvLen > 0) {
        std::cout << "Received response from SGW-C" << std::endl;
    } else {
        std::cerr << "Failed to receive response" << std::endl;
    }
#endif
    // Close socket
    close(sockfd);
    return 0;
}
