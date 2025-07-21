////////////////////////////////////////////////
//  The MIT License (MIT)
//  Copyright (c) 2023 Kevin Walchko
//  see LICENSE for full details
////////////////////////////////////////////////
#include "nema.h"
#include <stdlib.h>
#include <string.h>

// Function to parse GPS time from an NMEA time field (HHMMSS.sss)
int parse_gps_time(const char *time_str, gps_time_t *time) {
  // Check for valid input
  if ((time_str == NULL) || (time == NULL)) {
    return NEMA_INVALID_INPUT; // Invalid input
  }

  // Check if the string is empty
  if (time_str[0] == '\0') {
    return NEMA_EMPTY_TIME_FIELD; // Empty time field
  }

  // Initialize output struct
  time->hours        = 0;
  time->minutes      = 0;
  time->seconds      = 0;
  time->milliseconds = 0;

  // Expected format: HHMMSS.sss (e.g., "123519.000")
  // Minimum length for HHMMSS is 6 characters
  size_t len = strlen(time_str);
  if (len < 6) {
    return NEMA_TIME_FIELD_TOO_SHORT; // Invalid time format (too short)
  }

  // Parse hours, minutes, seconds
  char temp[7] = {0};         // Buffer for HHMMSS
  strncpy(temp, time_str, 6); // Copy first 6 characters (HHMMSS)

  // Validate and extract hours
  if ((temp[0] >= '0') && (temp[0] <= '2') && (temp[1] >= '0') && (temp[1] <= '9')) {
    time->hours = (temp[0] - '0') * 10 + (temp[1] - '0');
  }
  else {
    return NEMA_TIME_INVALID_HOURS; // Invalid hours
  }
  if (time->hours > 23) return NEMA_TIME_INVALID_HOURS;

  // Validate and extract minutes
  if (temp[2] >= '0' && temp[2] <= '5' && temp[3] >= '0' && temp[3] <= '9') {
    time->minutes = (temp[2] - '0') * 10 + (temp[3] - '0');
  }
  else {
    return NEMA_TIME_INVALID_MINUTES; // Invalid minutes
  }
  if (time->minutes > 59) return NEMA_TIME_INVALID_MINUTES;

  // Validate and extract seconds
  if (temp[4] >= '0' && temp[4] <= '5' && temp[5] >= '0' && temp[5] <= '9') {
    time->seconds = (temp[4] - '0') * 10 + (temp[5] - '0');
  }
  else {
    return NEMA_TIME_INVALID_SECONDS; // Invalid seconds
  }
  if (time->seconds > 59) return NEMA_TIME_INVALID_SECONDS;

  // Parse milliseconds if present
  if (len > 7 && time_str[6] == '.') {          // Check for decimal point
    char ms_str[4] = {0};                       // Buffer for up to 3 digits of milliseconds
    int ms_len     = len - 7 > 3 ? 3 : len - 7; // Limit to 3 digits
    strncpy(ms_str, time_str + 7, ms_len);
    time->milliseconds = atoi(ms_str);
  }
  if (time->milliseconds > 999) return NEMA_TIME_INVALID_MILLISECONDS;

  // Validate ranges
  // if (time->hours > 23 || time->minutes > 59 || time->seconds > 59 || time->milliseconds > 999) {
  //   return NEMA_TIME_OUT_OF_RANGE; // Out of range
  // }

  return 0; // Success
}