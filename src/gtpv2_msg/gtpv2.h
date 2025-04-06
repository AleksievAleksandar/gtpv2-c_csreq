#ifndef GTPv2_H
#define GTPv2_H

#include <iostream>

struct Header
{
    // GTPv2 Header (8 bytes)
    #define HEADER_SIZE 8
    Header();

    void set_flags(uint8_t p_flags);
    void set_msg_type(uint8_t p_msg_type);

    void set_msg_length(uint16_t p_length);
    uint16_t get_msg_length() const;

    void set_teid(uint32_t p_teid);
    uint32_t get_teid() const;

    uint8_t m_flags;
    uint8_t m_msg_type;
    uint8_t m_msg_length[2];
    uint8_t m_teid[4];

} __attribute__((packed));

struct Sequence_number
{
    // Sequence Number & Spare (4 bytes)
    Sequence_number();
    void set_sequence_number(uint8_t p_seq_num);

    uint8_t m_sequence_number[3];
private:
    uint8_t m_spare;

} __attribute__((packed));

struct IMSI
{
    // IMSI (International Mobile Subscriber Identity) IE (12 bytes)
    IMSI();
    void set_imsi_ie_type(uint8_t p_imsi_ie_type);
    void set_imsi(uint64_t p_imsi);
    void print_IMSI();

    uint8_t m_imsi_ie_type;
    uint8_t m_length[2];
private:
    uint8_t m_spare;
public:
    uint8_t m_imsi[8];

} __attribute__((packed));

struct MSISDN
{
    // IMSI (International Mobile Subscriber Identity) IE (12 bytes)
    MSISDN();
    void set_msisdn_ie_type(uint8_t p_msisdn_ie_type);
    void set_msisdn(uint64_t p_msisdn);
    void print_MSISDN();

    uint8_t m_msisdn_ie_type;
    uint8_t m_length[2];
private:
    uint8_t m_spare;
public:
    uint8_t m_msisdn[8];

} __attribute__((packed));

struct MEI
{
    // IMSI (International Mobile Subscriber Identity) IE (12 bytes)
    MEI();
    void set_mei_ie_type(uint8_t p_mei_ie_type);
    void set_mei(uint64_t p_mei);
    void print_MEI();

    uint8_t m_mei_ie_type;
    uint8_t m_length[2];
private:
    uint8_t m_spare;
public:
    uint8_t m_mei[8];

} __attribute__((packed));

struct Serving_Network
{
    // Serving Network (3GPP) IE (17 bytes)
    Serving_Network();
    void set_serving_net(void* p_serving_net);

    uint8_t m_serving_net_ie_type;
    uint8_t m_serving_net_length[2];
private:
    uint8_t m_spare;
public:
    uint8_t m_serving_net[13];
} __attribute__((packed));

struct RAT
{
    // RAT Type (Radio Access Technology) IE (6 bytes)
    RAT();

    uint8_t m_rat_ie_type;
    uint8_t m_rat_length[2];
private:
    uint8_t m_spare;
public:
    uint8_t m_rat[2];

} __attribute__((packed));

struct Flags
{
    // Indication Flags (6 bytes)
    Flags();
    void set_flags(void* p_flags);

    uint8_t m_flags_ie_type;
    uint8_t m_flags[5];

} __attribute__((packed));

struct F_TEID
{
    // F-TEID for Control Plane (13 bytes)
    F_TEID();
    void set_interface_and_teid(void* p_interface_and_teid);
    void set_ip(uint8_t p_ip[4]);
    void set_ip(const char* ip_str);
    std::string get_ip_as_string() const;
    void print_IP() const;

    uint8_t m_f_teid_ie_type;
    uint8_t m_f_teid_length[2];
    uint8_t m_flags;
    uint8_t m_interface_type_and_teid[5];
    uint8_t m_ip[4];

} __attribute__((packed));

struct APN
{
    // APN (Access Point Name) IE (32 bytes)
    APN();

    void set_apn(void* p_apn);
    void set_mnc(void* p_mnc);
    void set_mcc(void* p_mcc);
    void set_gprs(void* p_gprs);

    void print_APN();

    uint8_t m_apn_ie_type;
    uint8_t m_apn_length[2];
    uint8_t m_spare;
    uint8_t m_apn[9]; // first idx is the length
    uint8_t m_mnc[7]; // first idx is the length
    uint8_t m_mcc[7]; // first idx is the length
    uint8_t m_gprs[5]; // first idx is the length

} __attribute__((packed));

struct Selection_Mode
{
    // Selection Mode (2 bytes)
    Selection_Mode();

    uint8_t m_ie_type;
    uint8_t m_value[4];

} __attribute__((packed));

struct PDN_Type
{
    // PDN Type (2 bytes)
    PDN_Type();

    uint8_t m_ie_type;
    uint8_t m_value[4];

} __attribute__((packed));

struct PDN_Addr_alloc
{
    // PAA (PDN Address Allocation) IE (5 bytes)
    PDN_Addr_alloc();
    void set_type(void* p_type);
    void set_addr(void* p_addr);

    uint8_t m_ie_type;
    uint8_t m_length[2];
private:
    uint8_t m_spare;
public:
    uint8_t m_type;
    uint8_t m_addr[4];
} __attribute__((packed));

struct APN_Restriction
{
    // Recovery (1 byte)
    APN_Restriction();

    uint8_t m_ie_type;
    uint8_t m_length[2];
    uint8_t m_flags;
    uint8_t m_restrictions;

} __attribute__((packed));

struct Aggregate_Max_Bit_Rate
{
    // AMBR (Aggregated Maximum Bitrate) IE (8 bytes)
    Aggregate_Max_Bit_Rate();
    void set_uplink(void* p_uplink);
    void set_downlink(void* p_downlink);

    uint8_t m_ie_type;
    uint8_t m_length[2];
private:
    uint8_t m_spare;
public:
    uint8_t m_uplink[4];
    uint8_t m_downlink[4];

} __attribute__((packed));

struct EBI
{
    // EBI (EPS Bearer ID) (2 bytes)
    EBI();

    uint8_t m_ie_type;
    uint8_t m_length[2];
    uint8_t m_flags;
    uint8_t m_id;

} __attribute__((packed));

struct Bearer_Level_Quality_of_Service
{
    // Bearer QoS (22 bytes)
    Bearer_Level_Quality_of_Service();

    uint8_t m_ie_type;
    uint8_t m_length[2];
    uint8_t m_flags;
    uint8_t m_pci;
    uint8_t m_label;
    uint8_t m_mbr_upl[5];
    uint8_t m_mbr_down[5];
    uint8_t m_guaranteed_mbr_upl[5];
    uint8_t m_guaranteed_mbr_down[5];

} __attribute__((packed));

struct Bearer_Ctx
{
    // Bearer Contexts (44 bytes)
    Bearer_Ctx();

    uint8_t m_ie_type;
    uint8_t m_length[2];
    u_int8_t m_flags;

    EBI m_ebi;
    F_TEID m_f_teid;
    Bearer_Level_Quality_of_Service m_bearer_qos;

} __attribute__((packed));

struct GTPv2
{
    GTPv2();

    Header m_header;
    Sequence_number m_seq_num;
    IMSI m_imsi;
    MSISDN m_msisdn;
    MEI m_mei;
    Serving_Network m_serving_net;
    RAT m_rat;
    Flags m_flags;
    F_TEID m_f_teid;
    APN m_apn;
    Selection_Mode m_selection_mode;
    PDN_Type m_pdn;
    PDN_Addr_alloc m_pdn_alloc;
    APN_Restriction m_apn_restriction;
    Aggregate_Max_Bit_Rate m_bit_rate;
    Bearer_Ctx m_bearer_ctx;

} __attribute__((packed));


// Construct a valid GTPv2-C Create Session Request
extern unsigned char gtpMessage[206];

extern unsigned char gtpMessage2[206];

#endif