////////////////////////////////////////////////
//  The MIT License (MIT)
//  Copyright (c) 2023 Kevin Walchko
//  see LICENSE for full details
////////////////////////////////////////////////
#pragma once

#if defined __cplusplus
extern "C" {
#endif

#include "nema/nema.h"
#include "ublox/ubx_parser.h"


typedef struct {
  bool rmc_found, gga_found, gsa_found, ublox_found;
  char rmc[MAX_NEMA_MSG_SIZE];
  char gga[MAX_NEMA_MSG_SIZE];
  char gsa[MAX_NEMA_MSG_SIZE];
  char ublox[MAX_NEMA_MSG_SIZE];
} gps_msgs_t;

int process_gps_message(gps_msgs_t *msgs, const char *data, size_t len);

#if defined __cplusplus
}
#endif