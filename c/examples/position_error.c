#include <stdio.h>

// Define a struct to hold position error results
typedef struct {
  float horizontal_error; // Horizontal error (meters)
  float vertical_error;   // Vertical error (meters)
  float total_error;      // 3D error (meters)
} position_error_t;

// Function to calculate position error from DOP and UERE
// confidence_level: 1 for 1-sigma (~68%), 2 for 2-sigma (~95%), etc.
int calculate_position_error(float hdop, float pdop, float vdop, float uere,
                             float confidence_level, position_error_t *error) {
  // Validate inputs
  if (hdop < 0 || pdop < 0 || vdop < 0 || uere < 0 || confidence_level <= 0) {
    return -1; // Invalid input
  }
  if (!error) {
    return -2; // Null output pointer
  }

  // Calculate errors (scaled by confidence level)
  error->horizontal_error = hdop * uere * confidence_level;
  error->vertical_error   = vdop * uere * confidence_level;
  error->total_error      = pdop * uere * confidence_level;

  return 0; // Success
}

// Example usage
int main() {
  // Example values from a GSA message
  float hdop             = 1.0; // Horizontal DOP
  float pdop             = 1.7; // Position DOP
  float vdop             = 1.3; // Vertical DOP
  float uere             = 2.0; // User Equivalent Range Error (meters)
  float confidence_level = 2.0; // 2-sigma (~95% confidence)

  position_error_t error = {0};

  int result = calculate_position_error(hdop, pdop, vdop, uere, confidence_level, &error);
  if (result == 0) {
    printf("Position Error (95%% confidence):\n");
    printf("Horizontal Error: %.2f meters\n", error.horizontal_error);
    printf("Vertical Error: %.2f meters\n", error.vertical_error);
    printf("3D Error: %.2f meters\n", error.total_error);
  }
  else {
    printf("Error calculating position error: %d\n", result);
  }

  return 0;
}