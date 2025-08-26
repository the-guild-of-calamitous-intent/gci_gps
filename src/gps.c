#include "gci_gps.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Function to determine message type and process it
int process_gps_message(gps_msgs_t *msgs, const char *data, size_t len) {
  int ret = 0;
  bool found_msg = false;
  if (data == NULL) return 0;
  int begin = 0;
  char *p = NULL;

  const char *msg = (const char *)data;
  for (int i=0; i<len; ++i) {
    if (found_msg == false) {
      if ((strncmp(&msg[i], "$GP", 3) == 0) || (strncmp(&msg[i], "$GN", 3) == 0)) {
        if (strncmp(&msg[i+3], "RMC", 3) == 0) {p = msgs->rmc; msgs->rmc_found = true;}
        else if (strncmp(&msg[i+3], "GSA", 3) == 0) {p = msgs->gsa; msgs->gsa_found = true;}
        else if (strncmp(&msg[i+3], "GGA", 3) == 0) {p = msgs->gga; msgs->gga_found = true;}
        else continue;
        found_msg = true;
        begin = i;
      }
    }
    else {
      if (msg[i] == '\r') {
        memcpy(p, &msg[begin], i-begin);
        found_msg = false;
        ret += 1;
      }
    }
  }

  // printf(">> Found: %d msgs\n", ret);

  return ret;
}