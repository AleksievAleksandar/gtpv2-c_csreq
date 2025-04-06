#include "gtpv2.h"

#include <cstring>
#include <cassert>
#include <arpa/inet.h>


///////////////////////////////////// Header ////////////////////////////////////////////////////////////
Header::Header()
    : m_flags{ 0x48 }                    // Flags: Version(2) | P(0) | T(1) | Spare(000) = 01001000
    , m_msg_type{ 0x20 }                 // Message Type: Create Session Request (0x20)
    , m_msg_length{ 0x00, 0xCA }         // Message Length: 202 bytes
    , m_teid{ 0x00, 0x00, 0x00, 0x00 }   // TEID (Tunnel Endpoint Identifier) = 0 (Sender TEID)
{
    static_assert(sizeof(*this) == HEADER_SIZE);
}

void Header::set_flags(uint8_t p_flags)
{
    m_flags = p_flags;
}

void Header::set_msg_type(uint8_t p_msg_type)
{
    m_msg_type = p_msg_type;
}

void Header::set_msg_length(uint16_t p_length)
{
    uint16_t l_transform_length{ htons(p_length) };
    memcpy(m_msg_length, &l_transform_length, sizeof(m_msg_length));
}

uint16_t Header::get_msg_length() const
{
    return ntohs((*(uint16_t*)&m_msg_length));
}

void Header::set_teid(uint32_t p_teid)
{
    memcpy(m_teid, &p_teid, sizeof(m_teid));
}

uint32_t Header::get_teid() const
{
    return (*(uint32_t*)&m_teid);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////// Sequence_number ////////////////////////////////////////////////////////////
Sequence_number::Sequence_number()
    : m_sequence_number{ 0x00, 0x00, 0x02 }  // Sequence Number: 2
    , m_spare{ 0x00 }                        // Spare
{}

void Sequence_number::set_sequence_number(uint8_t p_seq_num)
{
    memcpy(m_sequence_number, &p_seq_num, sizeof(m_sequence_number));
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////// IMSI ////////////////////////////////////////////////////////////
IMSI::IMSI()
    : m_imsi_ie_type{ 0x01 }                                     // IMSI IE Type (0x01)
    , m_length{ 0x00, 0x08 }                                     // Length: 8 bytes
    , m_spare{ 0x00 }                                            // Spare
    , m_imsi{ 0x22, 0x02, 0x01, 0x12, 0x14, 0x64, 0x14, 0xf8 }   // IMSI value (encoded)
{
}

void IMSI::set_imsi_ie_type(uint8_t p_imsi_ie_type)
{
    m_imsi_ie_type = p_imsi_ie_type;
}

void IMSI::set_imsi(uint64_t p_imsi)
{
        // Convert the IMSI number to a string of digits
        char l_imsi_str[16] = { 0 }; // Max 15 digits + null terminator
        snprintf(l_imsi_str, sizeof(l_imsi_str), "%llu", static_cast<unsigned long long>(p_imsi));

        size_t l_len = strlen(l_imsi_str);
        memset(m_imsi, 0xFF, sizeof(m_imsi)); // Fill with padding (0xFF = 0xF | 0xF)

        for (size_t i = 0; i < l_len; i += 2)
        {
            uint8_t first_digit = l_imsi_str[i] - '0';
            uint8_t second_digit = (i + 1 < l_len) ? (l_imsi_str[i + 1] - '0') : 0xF;

            // Lower nibble: first digit, Upper nibble: second digit
            m_imsi[i / 2] = (second_digit << 4) | first_digit;
        }
}

void IMSI::print_IMSI()
{
    for (int32_t i = 0; i < 8; i++)
    {
        // Extract low and high nibbles (correct order for IMSI decoding)
        int32_t low_nibble = m_imsi[i] & 0x0F;         // First digit
        int32_t high_nibble = (m_imsi[i] & 0xF0) >> 4; // Second digit

        // Print low nibble first
        printf("%d", low_nibble);

        // Print high nibble (ignore if it's 0xF, which is padding)
        if (high_nibble != 0xF)
        {
            printf("%d", high_nibble);
        }
    }
    printf("\n");
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////// MSISDN ////////////////////////////////////////////////////////////
MSISDN::MSISDN()
    : m_msisdn_ie_type{ 0x4c }                                   // MSISDN IE Type (0x4c)
    , m_length{ 0x00, 0x08 }                                     // Length: 8 bytes
    , m_spare{ 0x00 }                                            // Spare
    , m_msisdn{ 0x79, 0x52, 0x95, 0x00, 0x00, 0x00, 0x00, 0xf0 } // MSISDN value
{}

void MSISDN::set_msisdn_ie_type(uint8_t p_msisdn_ie_type)
{
    m_msisdn_ie_type = p_msisdn_ie_type;
}

void MSISDN::set_msisdn(uint64_t p_msisdn)
{
    // Convert the MSISDN number to a string of digits
    char l_msisdn_str[16] = { 0 }; // Max 15 digits + null terminator
    snprintf(l_msisdn_str, sizeof(l_msisdn_str), "%llu", static_cast<unsigned long long>(p_msisdn));

    size_t l_len = strlen(l_msisdn_str);
    memset(m_msisdn, 0xFF, sizeof(m_msisdn)); // Fill with padding (0xFF = 0xF | 0xF)

    for (size_t i = 0; i < l_len; i += 2)
    {
        uint8_t first_digit = l_msisdn_str[i] - '0';
        uint8_t second_digit = (i + 1 < l_len) ? (l_msisdn_str[i + 1] - '0') : 0xF;

        // Lower nibble: first digit, Upper nibble: second digit
        m_msisdn[i / 2] = (second_digit << 4) | first_digit;
    }
}

void MSISDN::print_MSISDN()
{
    for (int32_t i = 0; i < 8; i++)
    {
        // Extract low and high nibbles (correct order for MSISDN decoding)
        int32_t low_nibble = m_msisdn[i] & 0x0F;         // First digit
        int32_t high_nibble = (m_msisdn[i] & 0xF0) >> 4; // Second digit

        // Print low nibble first
        printf("%d", low_nibble);

        // Print high nibble (ignore if it's 0xF, which is padding)
        if (high_nibble != 0xF)
        {
            printf("%d", high_nibble);
        }
    }
    printf("\n");
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////// MEI ////////////////////////////////////////////////////////////
MEI::MEI()
    : m_mei_ie_type{ 0x4b }                                   // MEI IE Type (0x4b)
    , m_length{ 0x00, 0x08 }                                  // Length: 8 bytes
    , m_spare{ 0x00 }                                         // Spare
    , m_mei{ 0x21, 0x43, 0x05, 0x01, 0x00, 0x00, 0x00, 0xf0 } // MEI value
{}

void MEI::set_mei_ie_type(uint8_t p_mei_ie_type)
{
    m_mei_ie_type = p_mei_ie_type;
}

void MEI::set_mei(uint64_t p_mei)
{
    // Convert the MEI number to a string of digits
    char l_mei_str[16] = { 0 }; // Max 15 digits + null terminator
    snprintf(l_mei_str, sizeof(l_mei_str), "%llu", static_cast<unsigned long long>(p_mei));

    size_t l_len = strlen(l_mei_str);
    memset(m_mei, 0xFF, sizeof(m_mei)); // Fill with padding (0xFF = 0xF | 0xF)

    for (size_t i = 0; i < l_len; i += 2)
    {
        uint8_t first_digit = l_mei_str[i] - '0';
        uint8_t second_digit = (i + 1 < l_len) ? (l_mei_str[i + 1] - '0') : 0xF;

        // Lower nibble: first digit, Upper nibble: second digit
        m_mei[i / 2] = (second_digit << 4) | first_digit;
    }
}

void MEI::print_MEI()
{
    for (int32_t i = 0; i < 8; i++)
    {
        // Extract low and high nibbles (correct order for MEI decoding)
        int32_t low_nibble = m_mei[i] & 0x0F;         // First digit
        int32_t high_nibble = (m_mei[i] & 0xF0) >> 4; // Second digit

        // Print low nibble first
        printf("%d", low_nibble);

        // Print high nibble (ignore if it's 0xF, which is padding)
        if (high_nibble != 0xF)
        {
            printf("%d", high_nibble);
        }
    }
    printf("\n");
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////// Serving_network ////////////////////////////////////////////////////////////
Serving_Network::Serving_Network()
    : m_serving_net_ie_type{ 0x56 } // Serving Network IE Type (0x56)
    , m_serving_net_length{ 0x00, 0x0d } // Length: 13 bytes
    , m_spare{ 0x00 } // Spare
    , m_serving_net{ 0x0c, 0x13, 0x01, 0x84, 0xff, 0xfe, 0x00, 0xff, 0x13, 0x01, 0x84, 0xd8, 0x0b } // MCC/MNC encoded
{}

void Serving_Network::set_serving_net(void* p_serving_net)
{
    memcpy(m_serving_net, p_serving_net, sizeof(m_serving_net));
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////// RAT ////////////////////////////////////////////////////////////
RAT::RAT()
    : m_rat_ie_type{ 0x53 } // RAT Type IE Type (0x53)
    , m_rat_length{ 0x00, 0x03 } // Length: 3 bytes
    , m_spare{ 0x00 } // Spare
    , m_rat{ 0x22, 0xf2 } // RAT Type Value
{}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////// FLAGS ////////////////////////////////////////////////////////////
Flags::Flags()
    : m_flags_ie_type{ 0x10 } // Indication IE Type (0x10)
    , m_flags{ 0x52, 0x00, 0x01, 0x00, 0x06 } // Flags (various indicators)
{}

void Flags::set_flags(void* p_flags)
{
    memcpy(m_flags, p_flags, sizeof(m_flags));
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////// F_TEID ////////////////////////////////////////////////////////////
F_TEID::F_TEID()
    : m_f_teid_ie_type{ 0x57 }                                   // F-TEID IE Type (0x57)
    , m_f_teid_length{ 0x00, 0x09 }                              // Length: 9 bytes
    , m_flags{ 0x00 }                                            // Spare
    , m_interface_type_and_teid{ 0x86, 0x00, 0xff, 0x00, 0x01 }  // Interface Type and TEID
    , m_ip{ 0xc0, 0xa8, 0x60, 0xef }                             // IPv4 Address (192.168.96.239)
{}

void F_TEID::set_interface_and_teid(void* p_interface_and_teid)
{
    memcpy(m_interface_type_and_teid, p_interface_and_teid, sizeof(m_interface_type_and_teid));
}

void F_TEID::set_ip(uint8_t p_ip[4])
{
    memcpy(m_ip, p_ip, sizeof(m_ip));
}

void F_TEID::set_ip(const char* ip_str)
{
    inet_pton(AF_INET, ip_str, m_ip); // Converts string to binary IPv4
}

std::string F_TEID::get_ip_as_string() const
{
    char buffer[INET_ADDRSTRLEN]; // Enough to hold "255.255.255.255\0"
    inet_ntop(AF_INET, m_ip, buffer, INET_ADDRSTRLEN);
    return std::string(buffer);
}

void F_TEID::print_IP() const
{
    // IPv4 address starts at byte 5
    printf("IPv4 as raw uint32_t: %u\n", *(uint32_t*)&m_ip);
    printf("IPv4 Address: %d.%d.%d.%d\n", m_ip[0], m_ip[1], m_ip[2], m_ip[3]);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////// APN ////////////////////////////////////////////////////////////
APN::APN()
    : m_apn_ie_type{ 0x47 }                                          // APN IE Type (0x47)
    , m_apn_length{ 0x00, 0x1c }                                     // Length: 28 bytes
    , m_spare{ 0x00 }                                                // Spare
    , m_apn{ 0x08, 0x69, 0x6e, 0x74, 0x65, 0x72, 0x6e, 0x65, 0x74 }  // "internet"
    , m_mnc{ 0x06, 0x6d, 0x6e, 0x63, 0x30, 0x30, 0x31 }              // "mnc001"
    , m_mcc{ 0x06, 0x6d, 0x63, 0x63, 0x32, 0x32, 0x32 }              // "mcc222"
    , m_gprs{ 0x04, 0x67, 0x70, 0x72, 0x73 }                         // "gprs"
{}

void APN::set_apn(void* p_apn)
{
    memcpy(m_apn, p_apn, sizeof(m_apn));
}

void APN::set_mnc(void* p_mnc)
{
    memcpy(m_mnc, p_mnc, sizeof(m_mnc));
}

void APN::set_mcc(void* p_mcc)
{
    memcpy(m_mcc, p_mcc, sizeof(m_mcc));
}

void APN::set_gprs(void* p_gprs)
{
    memcpy(m_gprs, p_gprs, sizeof(m_gprs));
}

void APN::print_APN()
{
    for (size_t i = 1; i <= m_apn[0]; i++)
    {
        printf("%c", m_apn[i]);
    }
    printf(".");

    for (size_t i = 1; i <= m_mnc[0]; i++)
    {
        printf("%c", m_mnc[i]);
    }
    printf(".");

    for (size_t i = 1; i <= m_mcc[0]; i++)
    {
        printf("%c", m_mcc[i]);
    }
    printf(".");

    for (size_t i = 1; i <= m_gprs[0]; i++)
    {
        printf("%c", m_gprs[i]);
    }

    printf("\n");
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////// Selection_Mode ////////////////////////////////////////////////////////////
Selection_Mode::Selection_Mode()
    : m_ie_type {0x80} // Selection Mode IE Type (0x80)
    , m_value {0x00, 0x01, 0x00, 0x00}
{}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////// PDN_Type ////////////////////////////////////////////////////////////
PDN_Type::PDN_Type()
    : m_ie_type{ 0x63 } // PDN Type IE Type (0x63)
    , m_value{ 0x00, 0x01, 0x00, 0x01 } // IPv4
{}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////// PDN_Addr_alloc ////////////////////////////////////////////////////////////
PDN_Addr_alloc::PDN_Addr_alloc()
    : m_ie_type{ 0x4f }                  // PAA IE Type (0x4f)
    , m_length{ 0x00, 0x05 }             // Length: 5 bytes
    , m_spare{ 0x00 }                    // Spare
    , m_type{ 0x01 }                     // IPv4 Type
    , m_addr{ 0x00, 0x00, 0x00, 0x00 }   // IPv4 Address (0.0.0.0)
{}

void PDN_Addr_alloc::set_type(void* p_type)
{
    m_type = *(uint8_t*)p_type;
}

void PDN_Addr_alloc::set_addr(void* p_addr)
{
    memcpy(m_addr, p_addr, sizeof(m_addr));
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////// APN_Restriction ////////////////////////////////////////////////////////////
APN_Restriction::APN_Restriction()
    : m_ie_type {0x7f} // Recovery IE Type (0x7f)
    , m_length {0x00, 0x01}
    , m_flags {0x00}
    , m_restrictions {0x00}
{}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////// Aggregate_Max_Bit_rate /////////////////////////////////////////////////////////
Aggregate_Max_Bit_Rate::Aggregate_Max_Bit_Rate()
    : m_ie_type{ 0x48 } // AMBR IE Type (0x48)
    , m_length{ 0x00, 0x08 } // Length: 8 bytes
    , m_spare{ 0x00 } // Spare
    , m_uplink{ 0x11, 0x11, 0x11, 0x11 } // UL-AMBR
    , m_downlink{ 0x22, 0x22, 0x22, 0x22 } // DL-AMBR
{}

void Aggregate_Max_Bit_Rate::set_uplink(void* p_uplink)
{
    memcpy(m_uplink, p_uplink, sizeof(m_uplink));
}

void Aggregate_Max_Bit_Rate::set_downlink(void* p_downlink)
{
    memcpy(m_downlink, p_downlink, sizeof(m_downlink));
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////// EBI ////////////////////////////////////////////////////////////
EBI::EBI()
    : m_ie_type {0x49} // EBI IE Type (0x49)
    , m_length {0x00, 0x01} // Length: 1 byte
    , m_flags {0x00} // Spare
    , m_id {0x05} // EBI Value
{}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////// Bearer_Level_Quality_of_Service //////////////////////////////////////////////////
Bearer_Level_Quality_of_Service::Bearer_Level_Quality_of_Service()
    : m_ie_type {0x50} // Bearer QoS IE Type (0x50)
    , m_length {0x00, 0x16} // Length: 22 bytes
    , m_flags {0x00} // Flags
    , m_pci {0x49} //PCI
    , m_label {0xff} // Label
    , m_mbr_upl {0x00, 0x00, 0x00, 0x00, 0x00} // Max bit rate for uplink
    , m_mbr_down {0x00, 0x00, 0x00, 0x00, 0x00} // Max bit rate for downlink
    , m_guaranteed_mbr_upl {0x00, 0x00, 0x00, 0x00, 0x00} // Guaranteed bit rate for uplink
    , m_guaranteed_mbr_down {0x00, 0x00, 0x00, 0x00, 0x00} // Guaranteed bit rate for downlink
{}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////// Bearer_Ctx //////////////////////////////////////////////////
Bearer_Ctx::Bearer_Ctx()
    : m_ie_type {0x5d} // Bearer Context IE Type (0x5d)
    , m_length {0x00, 0x2c} // Length: 44 bytes
    , m_flags {0x00}
{
    m_f_teid.m_f_teid_ie_type = 0x57;
    m_f_teid.m_f_teid_length[0] = 0x00;
    m_f_teid.m_f_teid_length[1] = 0x09;
    m_f_teid.m_flags = 0x02;

    uint8_t l_interface[] {0x84, 0x00, 0xaf, 0xff, 0x01};
    uint8_t l_ip[] {0xc0, 0xa8, 0x30, 0x3b};

    m_f_teid.set_interface_and_teid(l_interface);
    m_f_teid.set_ip(l_ip);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////// GTPv2 //////////////////////////////////////////////////
GTPv2::GTPv2()
{
    uint16_t l_msg_length_without_header {
          sizeof(Header) - 4 // left only TEID part
        + sizeof(Sequence_number)
        + sizeof(IMSI)
        + sizeof(MSISDN)
        + sizeof(MEI)
        + sizeof(Serving_Network)
        + sizeof(RAT)
        + sizeof(Flags)
        + sizeof(F_TEID)
        + sizeof(APN)
        + sizeof(Selection_Mode)
        + sizeof(PDN_Type)
        + sizeof(PDN_Addr_alloc)
        + sizeof(APN_Restriction)
        + sizeof(Aggregate_Max_Bit_Rate)
        + sizeof(Bearer_Ctx)
    };
    m_header.set_msg_length(l_msg_length_without_header);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////// Raw_Msgs //////////////////////////////////////////////////
unsigned char gtpMessage[] = {

    0x48, 0x20, 0x00, 0xca, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x01, 0x00, 0x08, 0x00,
    0x22, 0x02, 0x01, 0x12, 0x14, 0x64, 0x14, 0xf8, 0x4c, 0x00, 0x08, 0x00, 0x79, 0x52, 0x95, 0x00,
    0x00, 0x00, 0x00, 0xf0, 0x4b, 0x00, 0x08, 0x00, 0x21, 0x43, 0x05, 0x01, 0x00, 0x00, 0x00, 0xf0,
    0x56, 0x00, 0x0d, 0x00, 0x0c, 0x13, 0x01, 0x84, 0xff, 0xfe, 0x00, 0xff, 0x13, 0x01, 0x84, 0xd8,
    0x0b, 0x53, 0x00, 0x03, 0x00, 0x22, 0xf2, 0x10, 0x52, 0x00, 0x01, 0x00, 0x06, 0x57, 0x00, 0x09,
    0x00, 0x86, 0x00, 0xff, 0x00, 0x01, 0xc0, 0xa8, 0x60, 0xef, 0x47, 0x00, 0x1c, 0x00, 0x08, 0x69,
    0x6e, 0x74, 0x65, 0x72, 0x6e, 0x65, 0x74, 0x06, 0x6d, 0x6e, 0x63, 0x30, 0x30, 0x31, 0x06, 0x6d,
    0x63, 0x63, 0x32, 0x32, 0x32, 0x04, 0x67, 0x70, 0x72, 0x73, 0x80, 0x00, 0x01, 0x00, 0x00, 0x63,
    0x00, 0x01, 0x00, 0x01, 0x4f, 0x00, 0x05, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x7f, 0x00, 0x01,
    0x00, 0x00, 0x48, 0x00, 0x08, 0x00, 0x11, 0x11, 0x11, 0x11, 0x22, 0x22, 0x22, 0x22, 0x5d, 0x00,
    0x2c, 0x00, 0x49, 0x00, 0x01, 0x00, 0x05, 0x57, 0x00, 0x09, 0x02, 0x84, 0x00, 0xaf, 0xff, 0x01,
    0xc0, 0xa8, 0x30, 0x3b, 0x50, 0x00, 0x16, 0x00, 0x49, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

unsigned char gtpMessage2[] = {

    // GTPv2 Header (8 bytes)
    0x48, // Flags: Version(2) | P(0) | T(1) | Spare(000) = 01001000
    0x20, // Message Type: Create Session Request (0x20)
    0x00, 0xca, // Message Length: 202 bytes
    0x00, 0x00, 0x00, 0x00, // TEID (Tunnel Endpoint Identifier) = 0 (Sender TEID)

    // Sequence Number & Spare (4 bytes)
    0x00, 0x00, 0x02, // Sequence Number: 2
    0x00, // Spare

    // IMSI (International Mobile Subscriber Identity) IE (12 bytes)
    0x01, // IMSI IE Type (0x01)
    0x00, 0x08, // Length: 8 bytes
    0x00, // Spare
    0x22, 0x02, 0x01, 0x12, 0x14, 0x64, 0x14, 0xf8, // IMSI value (encoded)

    // MSISDN (Mobile Subscriber ISDN Number) IE (12 bytes)
    0x4c, // MSISDN IE Type (0x4c)
    0x00, 0x08, // Length: 8 bytes
    0x00, // Spare
    0x79, 0x52, 0x95, 0x00, 0x00, 0x00, 0x00, 0xf0, // MSISDN value

    // MEI (Mobile Equipment Identity) IE (12 bytes)
    0x4b, // MEI IE Type (0x4b)
    0x00, 0x08, // Length: 8 bytes
    0x00, // Spare
    0x21, 0x43, 0x05, 0x01, 0x00, 0x00, 0x00, 0xf0, // IMEI value

    // Serving Network (3GPP) IE (17 bytes)
    0x56, // Serving Network IE Type (0x56)
    0x00, 0x0d, // Length: 13 bytes
    0x00, // Spare
    0x0c, 0x13, 0x01, 0x84, 0xff, 0xfe, 0x00, 0xff, 0x13, 0x01, 0x84, 0xd8, 0x0b, // MCC/MNC encoded

    // RAT Type (Radio Access Technology) IE (6 bytes)
    0x53, // RAT Type IE Type (0x53)
    0x00, 0x03, // Length: 3 bytes
    0x00, // Spare
    0x22, 0xf2, // RAT Type Value

    // Indication Flags (6 bytes)
    0x10, // Indication IE Type (0x10)
    0x52, 0x00, 0x01, 0x00, 0x06, // Flags (various indicators)

    // F-TEID for Control Plane (9 bytes)
    0x57, // F-TEID IE Type (0x57)
    0x00, 0x09, // Length: 9 bytes
    0x00, // Spare
    0x86, 0x00, 0xff, 0x00, // Interface Type and TEID
    0x01, 0xc0, 0xa8, 0x60, 0xef, // IPv4 Address (192.168.96.239)

    // APN (Access Point Name) IE (28 bytes)
    0x47, // APN IE Type (0x47)
    0x00, 0x1c, // Length: 28 bytes
    0x00, 0x08, // Spare
    0x69, 0x6e, 0x74, 0x65, 0x72, 0x6e, 0x65, 0x74, // "internet"
    0x06, 0x6d, 0x6e, 0x63, 0x30, 0x30, 0x31, // "mnc001"
    0x06, 0x6d, 0x63, 0x63, 0x32, 0x32, 0x32, // "mcc222"
    0x04, 0x67, 0x70, 0x72, 0x73, // "gprs"

    // Selection Mode (2 bytes)
    0x80, // Selection Mode IE Type (0x80)
    0x00, 0x01, 0x00, 0x00,

    // PDN Type (2 bytes)
    0x63, // PDN Type IE Type (0x63)
    0x00, 0x01, 0x00, 0x01, // IPv4

    // PAA (PDN Address Allocation) IE (5 bytes)
    0x4f, // PAA IE Type (0x4f)
    0x00, 0x05, // Length: 5 bytes
    0x00, // Spare
    0x01, // IPv4 Type
    0x00, 0x00, 0x00, 0x00, // IPv4 Address (0.0.0.0)

    // Recovery (1 byte)
    0x7f, // Recovery IE Type (0x7f)
    0x00, 0x01, 0x00, 0x00,

    // AMBR (Aggregated Maximum Bitrate) IE (8 bytes)
    0x48, // AMBR IE Type (0x48)
    0x00, 0x08, // Length: 8 bytes
    0x00, // Spare
    0x11, 0x11, 0x11, 0x11, // UL-AMBR
    0x22, 0x22, 0x22, 0x22, // DL-AMBR

    // Bearer Contexts (44 bytes)
    0x5d, // Bearer Context IE Type (0x5d)
    0x00, 0x2c, // Length: 44 bytes
    0x00,

    // EBI (EPS Bearer ID) (2 bytes)
    0x49, // EBI IE Type (0x49)
    0x00, 0x01, // Length: 1 byte
    0x00, // Spare
    0x05, // EBI Value

    // F-TEID for Bearer (9 bytes)
    0x57, // F-TEID IE Type (0x57)
    0x00, 0x09, // Length: 9 bytes
    0x02, // Spare
    0x84, 0x00, 0xaf, 0xff, // Interface Type and TEID
    0x01, 0xc0, 0xa8, 0x30, 0x3b, // IPv4 Address (192.168.48.59)

    // Bearer QoS (22 bytes)
    0x50, // Bearer QoS IE Type (0x50)
    0x00, 0x16, // Length: 22 bytes
    0x00,
    0x49, 0xff, 0x00, 0x00, // QCI & ARP
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // MBR & GBR
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 // Extra Bytes
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
