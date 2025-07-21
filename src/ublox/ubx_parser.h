#pragma once

#if defined __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

// UBX protocol constants
#define UBX_SYNC1 0xB5
#define UBX_SYNC2 0x62
#define UBX_HEADER_LENGTH 6   // Sync1 + Sync2 + Class + ID + Length (2 bytes)
#define UBX_CHECKSUM_LENGTH 2 // CK_A + CK_B

// UBX message class and ID for NAV-PVT
#define UBX_NAV_PVT_CLASS 0x01
#define UBX_NAV_PVT_ID 0x07
#define UBX_NAV_PVT_LENGTH 92 // Fixed payload length for NAV-PVT

// Structure to hold UBX message data
typedef struct {
  uint8_t msg_class;
  uint8_t msg_id;
  uint16_t payload_length;
  uint8_t *payload; // Pointer to payload data
  uint8_t ck_a;     // Checksum A
  uint8_t ck_b;     // Checksum B
} ubx_message_t;

// Structure for NAV-PVT message data
typedef struct {
  uint32_t iTOW;        // GPS time of week (ms)
  uint16_t year;        // Year (UTC)
  uint8_t month;        // Month (1-12, UTC)
  uint8_t day;          // Day (1-31, UTC)
  uint8_t hour;         // Hour (0-23, UTC)
  uint8_t min;          // Minute (0-59, UTC)
  uint8_t sec;          // Second (0-60, UTC)
  uint8_t valid;        // Validity flags
  uint32_t tAcc;        // Time accuracy estimate (ns)
  int32_t nano;         // Nanoseconds of second
  uint8_t fixType;      // GNSS fix type (0=no fix, 3=3D fix, etc.)
  uint8_t flags;        // Fix status flags
  uint8_t flags2;       // Additional flags
  uint8_t numSV;        // Number of satellites used
  int32_t lon;          // Longitude (degrees * 1e7)
  int32_t lat;          // Latitude (degrees * 1e7)
  int32_t height;       // Height above ellipsoid (mm)
  int32_t hMSL;         // Height above mean sea level (mm)
  uint32_t hAcc;        // Horizontal accuracy (mm)
  uint32_t vAcc;        // Vertical accuracy (mm)
  int32_t velN;         // North velocity (mm/s)
  int32_t velE;         // East velocity (mm/s)
  int32_t velD;         // Down velocity (mm/s)
  int32_t gSpeed;       // Ground speed (2D, mm/s)
  int32_t headMot;      // Heading of motion (degrees * 1e5)
  uint32_t sAcc;        // Speed accuracy (mm/s)
  uint32_t headAcc;     // Heading accuracy (degrees * 1e5)
  uint16_t pDOP;        // Position DOP (* 0.01)
  uint8_t reserved1[6]; // Reserved bytes
  int32_t headVeh;      // Heading of vehicle (degrees * 1e5)
  int16_t magDec;       // Magnetic declination (degrees * 1e2)
  uint16_t magAcc;      // Magnetic declination accuracy (degrees * 1e2)
} ubx_nav_pvt_t;

// Parser states
typedef enum {
  UBX_STATE_SYNC1,
  UBX_STATE_SYNC2,
  UBX_STATE_CLASS,
  UBX_STATE_ID,
  UBX_STATE_LENGTH1,
  UBX_STATE_LENGTH2,
  UBX_STATE_PAYLOAD,
  UBX_STATE_CK_A,
  UBX_STATE_CK_B
} ubx_parser_state_t;

// Parser context
typedef struct {
  ubx_parser_state_t state;
  ubx_message_t message;
  uint16_t payload_bytes_received;
  uint8_t ck_a; // Running checksum A
  uint8_t ck_b; // Running checksum B
} ubx_parser_t;

// Function prototypes
void ubx_parser_init(ubx_parser_t *parser);
bool ubx_parser_process_byte(ubx_parser_t *parser, uint8_t byte,
                             ubx_message_t *msg);
bool ubx_validate_checksum(const ubx_message_t *msg);
bool ubx_parse_nav_pvt(const ubx_message_t *msg, ubx_nav_pvt_t *nav_pvt);

#if defined __cplusplus
}
#endif