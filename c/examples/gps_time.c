#include "gps.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Example usage
int main() {
  const char *sample_time = "123519.123"; // Example: 12:35:19.123 UTC
  gps_time_t time         = {0};          // Initialize struct

  printf("string: %s\n", sample_time);
  int result = parse_gps_time(sample_time, &time);
  if (result == 0) {
    printf("Hours: %02d\n", time.hours);
    printf("Minutes: %02d\n", time.minutes);
    printf("Seconds: %02d\n", time.seconds);
    printf("Milliseconds: %03d\n", time.milliseconds);
  }
  else {
    printf("Error parsing GPS time: %d\n", result);
  }

  // Test with empty field
  printf("\n\nTHIS SHOULD FAIL\n");
  result = parse_gps_time("", &time);
  if (result == 0) {
    printf("Hours: %02d\n", time.hours);
    printf("Minutes: %02d\n", time.minutes);
    printf("Seconds: %02d\n", time.seconds);
    printf("Milliseconds: %03d\n", time.milliseconds);
  }
  else {
    printf("Error parsing empty GPS time: %d\n", result);
  }

  return 0;
}