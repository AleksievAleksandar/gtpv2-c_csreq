#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "gtpv2.h"

#define SMF_IP "192.168.96.231" // SMF IP
#define GTP_PORT 2123  // Standard GTP-C port

int main() {
    int sockfd;
    struct sockaddr_in serverAddr;
    struct sockaddr_in sgw;
    char buffer[1024];

    // Create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        std::cerr << "Socket creation failed" << std::endl;
        return -1;
    }

    memset(&sgw, 0, sizeof(sgw));
    sgw.sin_family = AF_INET;
    sgw.sin_port = htons(2123); // Local port
    sgw.sin_addr.s_addr = INADDR_ANY; // Your desired local IP

    // Bind the socket to the chosen IP and port
    if (bind(sockfd, (struct sockaddr*)&sgw, sizeof(sgw)) < 0)
    {
        perror("bind failed\n");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Configure SGW-C address
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(GTP_PORT);
    inet_pton(AF_INET, SMF_IP, &serverAddr.sin_addr);

    if (sizeof(gtpMessage) != sizeof(gtpMessage2)) //206 bytes
    {
        std::cerr << "ERROR -> the two msgs are with different sizes. gtpMessage: "
                                            << sizeof(gtpMessage) << " | gtpMessage2: " << sizeof(gtpMessage2) << '\n';
    }

    if (sizeof(gtpMessage2) != sizeof(GTPv2)) //206 bytes
    {
        std::cerr << "ERROR -> the two msgs are with different sizes. gtpMessage: "
                                            << sizeof(gtpMessage) << " | gtpMessage2: " << sizeof(gtpMessage2) << '\n';
    }

    GTPv2 l_gtp;
    uint8_t l_arr[206]{};
    memcpy(l_arr, &l_gtp, sizeof(gtpMessage2));
    for (uint32_t i = 0; i < sizeof(gtpMessage2); i++)
    {
        if (l_arr[i] != gtpMessage2[i])
        {
            printf("i = %u\n", i);
            printf("arr = %u | msg = %u\n", l_arr[i], gtpMessage2[i]);
            break;
        }
        
    }
    
    if (memcmp(gtpMessage2, l_arr, sizeof(GTPv2)))
    {
        printf("FUCK\n");
    }

    Header h;
    uint32_t teid {111116};
    h.set_teid(&teid);

    printf("Header: %lu length: %x\n", sizeof(h), h.m_msg_type);
    printf("Header: %lu length: %u\n", sizeof(h), h.get_teid());
    for (size_t i = 0; i < 4; i++)
    {
        printf("Header: %lu length: %x\n", sizeof(h), h.m_teid[i]);
    }

    // uint16_t a {512};
    // uint32_t a {66000};
    // h.set_msg_length(&a);
    printf("header msg length: %u\n", h.get_msg_length());
    
    IMSI imsi;
    imsi.print_IMSI();
    
    GTPv2 gtp;
    printf("Header: %lu\n", sizeof(gtp));
    printf("header msg length: %u\n", gtp.m_header.get_msg_length());
    
    F_TEID f_teid;
    f_teid.print_IP();

    APN apn;
    apn.print_APN();

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
