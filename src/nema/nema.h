#pragma once

#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#if defined __cplusplus
extern "C" {
#endif

#include "mtk.h"

// constexpr size_t MAX_NEMA_MSG_SIZE = 100;
#define MAX_NEMA_MSG_SIZE 100

// parsers
#define NEMA_SUCCESS 0
#define NEMA_INVALID_MSG_ID -1
#define NEMA_MEMORY_ALLOCATION_FAIL -2
#define NEMA_INCOMPLETE_MSG -3
// checksum
#define NEMA_INVALID_MSG -1 // missing $ at start
#define NEMA_MISSING_CHECKSUM -2
#define NEMA_INVALID_CHECKSUM_FORMAT -3 // checksum: also -4
#define NEMA_WRONG_CHECKSUM -5          // checksum != string contents
// time
#define NEMA_INVALID_INPUT -1 // time: null ptr
#define NEMA_EMPTY_TIME_FIELD -2
#define NEMA_TIME_FIELD_TOO_SHORT -3
#define NEMA_TIME_INVALID_HOURS -4
#define NEMA_TIME_INVALID_MINUTES -5
#define NEMA_TIME_INVALID_SECONDS -6
#define NEMA_TIME_INVALID_MILLISECONDS -7

// Function to validate the checksum of an NMEA message
// Returns 0 if valid, non-zero if invalid or error
int validate_nmea_checksum(const char *nmea_msg);

// Define the struct to hold parsed GPS time
typedef struct {
  uint8_t hours;         // Hours (0-23)
  uint8_t minutes;       // Minutes (0-59)
  uint8_t seconds;       // Seconds (0-59)
  uint16_t milliseconds; // Milliseconds (0-999)
} gps_time_t;

// Function to parse GPS time from an NMEA time field (HHMMSS.sss)
int parse_gps_time(const char *time_str, gps_time_t *time);

// Define the struct to hold GGA data
typedef struct {
  char time[10];          // UTC time (HHMMSS.SSS)
  double latitude;        // Latitude in decimal degrees
  char lat_dir;           // Latitude direction (N/S)
  double longitude;       // Longitude in decimal degrees
  char lon_dir;           // Longitude direction (E/W)
  int fix_quality;        // Fix quality (0=invalid, 1=GPS, 2=DGPS)
  int satellites;         // Number of satellites
  float hdop;             // Horizontal dilution of precision
  float altitude;         // Altitude above mean sea level (meters)
  char alt_unit;          // Altitude unit (M=meters)
  float geoid_separation; // Height of geoid above WGS84 ellipsoid
  char geoid_unit;        // Geoid separation unit (M=meters)
} gga_t;

int parse_gga(const char *gga_msg, gga_t *data);
void print_gga(gga_t gga);

// Define the struct to hold RMC data
typedef struct {
  char time[10];    // UTC time (HHMMSS)
  char status;      // Status (A=active, V=void)
  double latitude;  // Latitude in decimal degrees
  char lat_dir;     // Latitude direction (N/S)
  double longitude; // Longitude in decimal degrees
  char lon_dir;     // Longitude direction (E/W)
  float speed;      // Speed over ground (knots)
  float course;     // Course over ground (degrees, true)
  char date[7];     // Date (DDMMYY)
  float mag_var;    // Magnetic variation (degrees)
  char mag_var_dir; // Magnetic variation direction (E/W)
} rmc_t;

// Function to parse an RMC message
int parse_rmc(const char *rmc_msg, rmc_t *data);
void print_rmc(rmc_t rmc);

// Define the struct to hold GSA data
typedef struct {
  char mode;          // Mode (M=manual, A=automatic)
  int fix_type;       // Fix type (1=no fix, 2=2D, 3=3D)
  int satellites[12]; // PRN numbers of satellites used in fix (up to 12)
  float pdop;         // Position dilution of precision
  float hdop;         // Horizontal dilution of precision
  float vdop;         // Vertical dilution of precision
} gsa_t;

// Function to parse a GSA message
int parse_gsa(const char *gsa_msg, gsa_t *data);
void print_gsa(gsa_t gsa);

#if defined __cplusplus
}
#endif