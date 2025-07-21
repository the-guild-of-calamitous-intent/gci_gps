#include "gci_gps.h"
#include <scruffy/scruffy.h>

TEST(NMEATest, ParseGGA_ValidMessage) {
  gga_t gga_data;
  const char *msg =
      "$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47";
  EXPECT_EQ(0, parse_gga(msg, &gga_data));
  // EXPECT_STREQ("123519", gga_data.time);
  // EXPECT_NEAR(48.117300, gga_data.latitude, 0.0001);
  EXPECT_EQ('N', gga_data.lat_dir);
  // EXPECT_NEAR(11.516667, gga_data.longitude, 0.0001);
  EXPECT_EQ('E', gga_data.lon_dir);
  EXPECT_EQ(1, gga_data.fix_quality);
  EXPECT_EQ(8, gga_data.satellites);
  EXPECT_FLOAT_EQ(0.9f, gga_data.hdop);
  EXPECT_FLOAT_EQ(545.4f, gga_data.altitude);
  EXPECT_EQ('M', gga_data.alt_unit);
  EXPECT_FLOAT_EQ(46.9f, gga_data.geoid_separation);
  EXPECT_EQ('M', gga_data.geoid_unit);
}

TEST(NMEATest, ParseGGA_EmptyFields) {
  gga_t gga_data;
  const char *msg = "$GPGGA,123519.00,,,,,0,00,99.9,,M,,M,,*56";
  EXPECT_EQ(0, parse_gga(msg, &gga_data));
  EXPECT_STREQ("123519.00", gga_data.time);
  EXPECT_DOUBLE_EQ(0.0, gga_data.latitude);
  EXPECT_EQ('\0', gga_data.lat_dir);
  EXPECT_DOUBLE_EQ(0.0, gga_data.longitude);
  EXPECT_EQ('\0', gga_data.lon_dir);
  EXPECT_EQ(0, gga_data.fix_quality);
  EXPECT_EQ(0, gga_data.satellites);
  EXPECT_FLOAT_EQ(99.9f, gga_data.hdop);
  EXPECT_FLOAT_EQ(0.0f, gga_data.altitude);
  EXPECT_EQ('M', gga_data.alt_unit);
  EXPECT_FLOAT_EQ(0.0f, gga_data.geoid_separation);
  EXPECT_EQ('M', gga_data.geoid_unit);
}

TEST(NMEATest, ParseGGA_GOOD_2) {
  gga_t gga_data;
  const char *msg = "$GNGGA,001043.00,4404.14036,N,12118.85961,W,1,12,0.98,"
                    "1113.0,M,-21.3,M*47";
  EXPECT_EQ(0, parse_gga(msg, &gga_data));
  EXPECT_STREQ("001043.00", gga_data.time);
  EXPECT_DOUBLE_EQ(44.069006, gga_data.latitude);
  EXPECT_EQ('N', gga_data.lat_dir);
  EXPECT_FLOAT_EQ(-121.31432683f, (float)gga_data.longitude);
  EXPECT_EQ('W', gga_data.lon_dir);
  EXPECT_EQ(1, gga_data.fix_quality);
  EXPECT_EQ(12, gga_data.satellites);
  EXPECT_FLOAT_EQ(0.98f, gga_data.hdop);
  EXPECT_FLOAT_EQ(1113.0f, gga_data.altitude);
  EXPECT_EQ('M', gga_data.alt_unit);
  EXPECT_FLOAT_EQ(-21.3f, gga_data.geoid_separation);
  EXPECT_EQ('M', gga_data.geoid_unit);
}

TEST(NMEATest, ParseGGA_InvalidMessage) {
  gga_t gga_data;
  const char *msg = "$GPXXX,123519,,,,,0,00,99.9,,,M,,M,,*56";
  EXPECT_EQ(NEMA_INVALID_MSG_ID, parse_gga(msg, &gga_data));
}

TEST(NMEATest, ParseGGA_IncompleteMessage) {
  gga_t gga_data;
  const char *msg = "$GPGGA,123519,4807.038,N,01131.000,E,1,08*47";
  EXPECT_EQ(NEMA_INCOMPLETE_MSG, parse_gga(msg, &gga_data));
}

// Test parse_rmc
TEST(NMEATest, ParseRMC_ValidMessage) {
  rmc_t rmc_data;
  const char *msg =
      "$GPRMC,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W*6A";
  EXPECT_EQ(0, parse_rmc(msg, &rmc_data));
  EXPECT_STREQ("123519", rmc_data.time);
  EXPECT_EQ('A', rmc_data.status);
  // EXPECT_NEAR(48.117300, rmc_data.latitude, 0.0001);
  EXPECT_EQ('N', rmc_data.lat_dir);
  // EXPECT_NEAR(11.516667, rmc_data.longitude, 0.0001);
  EXPECT_EQ('E', rmc_data.lon_dir);
  EXPECT_FLOAT_EQ(22.4f, rmc_data.speed);
  EXPECT_FLOAT_EQ(84.4f, rmc_data.course);
  EXPECT_STREQ("230394", rmc_data.date);
  EXPECT_FLOAT_EQ(-3.1f, rmc_data.mag_var);
  EXPECT_EQ('W', rmc_data.mag_var_dir);
}

TEST(NMEATest, ParseRMC_EmptyFields) {
  rmc_t rmc_data;
  const char *msg = "$GPRMC,123519,V,,,,,0.0,0.0,230394,,,A*4D";
  EXPECT_EQ(0, parse_rmc(msg, &rmc_data));
  EXPECT_STREQ("123519", rmc_data.time);
  EXPECT_EQ('V', rmc_data.status);
  EXPECT_DOUBLE_EQ(0.0, rmc_data.latitude);
  EXPECT_EQ('\0', rmc_data.lat_dir);
  EXPECT_DOUBLE_EQ(0.0, rmc_data.longitude);
  EXPECT_EQ('\0', rmc_data.lon_dir);
  EXPECT_FLOAT_EQ(0.0f, rmc_data.speed);
  EXPECT_FLOAT_EQ(0.0f, rmc_data.course);
  EXPECT_STREQ("230394", rmc_data.date);
  EXPECT_FLOAT_EQ(0.0f, rmc_data.mag_var);
  EXPECT_EQ('\0', rmc_data.mag_var_dir);
}

TEST(NMEATest, ParseRMC_InvalidMessage) {
  rmc_t rmc_data;
  const char *msg =
      "$GPXXX,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W*6A";
  EXPECT_EQ(NEMA_INVALID_MSG_ID, parse_rmc(msg, &rmc_data));
}

TEST(NMEATest, ParseRMC_IncompleteMessage) {
  rmc_t rmc_data;
  const char *msg = "$GPRMC,123519,A,4807.038,N,01131.000,E*6A";
  EXPECT_EQ(NEMA_INCOMPLETE_MSG, parse_rmc(msg, &rmc_data));
}

// Test parse_gsa
TEST(NMEATest, ParseGSA_ValidMessage) {
  gsa_t gsa_data;
  const char *msg = "$GPGSA,A,3,19,28,14,18,,,,,,,,,1.7,1.0,1.3*35";
  EXPECT_EQ(NEMA_SUCCESS, parse_gsa(msg, &gsa_data));
  EXPECT_EQ('A', gsa_data.mode);
  EXPECT_EQ(3, gsa_data.fix_type);
  EXPECT_EQ(19, gsa_data.satellites[0]);
  EXPECT_EQ(28, gsa_data.satellites[1]);
  EXPECT_EQ(14, gsa_data.satellites[2]);
  EXPECT_EQ(18, gsa_data.satellites[3]);
  for (int i = 4; i < 12; i++) {
    EXPECT_EQ(0, gsa_data.satellites[i]);
  }
  EXPECT_FLOAT_EQ(1.7f, gsa_data.pdop);
  EXPECT_FLOAT_EQ(1.0f, gsa_data.hdop);
  EXPECT_FLOAT_EQ(1.3f, gsa_data.vdop);
}

TEST(NMEATest, ParseGSA_EmptyFields) {
  gsa_t gsa_data;
  const char *msg = "$GPGSA,M,1,,,,,,,,,,,,,1.0,0.8,0.6*3C";
  EXPECT_EQ(NEMA_SUCCESS, parse_gsa(msg, &gsa_data));
  EXPECT_EQ('M', gsa_data.mode);
  EXPECT_EQ(1, gsa_data.fix_type);
  for (int i = 0; i < 12; i++) {
    EXPECT_EQ(0, gsa_data.satellites[i]);
  }
  EXPECT_FLOAT_EQ(1.0f, gsa_data.pdop);
  EXPECT_FLOAT_EQ(0.8f, gsa_data.hdop);
  EXPECT_FLOAT_EQ(0.6f, gsa_data.vdop);
}

TEST(NMEATest, ParseGSA_InvalidMessage) {
  gsa_t gsa_data;
  const char *msg = "$GPXXX,A,3,19,28,14,18,,,,,,,1.7,1.0,1.3*35";
  EXPECT_EQ(NEMA_INVALID_MSG_ID, parse_gsa(msg, &gsa_data));
}

TEST(NMEATest, ParseGSA_IncompleteMessage) {
  gsa_t gsa_data;
  const char *msg = "$GPGSA,A,3,19,28,14,18,,,,,,,1.7*35";
  EXPECT_EQ(NEMA_INCOMPLETE_MSG, parse_gsa(msg, &gsa_data));
}

// Test parse_gps_time
TEST(NMEATest, ParseGPSTime_ValidWithMilliseconds) {
  gps_time_t gps_time;
  const char *time_str = "123519.123";
  EXPECT_EQ(NEMA_SUCCESS, parse_gps_time(time_str, &gps_time));
  EXPECT_EQ(12, gps_time.hours);
  EXPECT_EQ(35, gps_time.minutes);
  EXPECT_EQ(19, gps_time.seconds);
  EXPECT_EQ(123, gps_time.milliseconds);
}

TEST(NMEATest, ParseGPSTime_ValidWithoutMilliseconds) {
  gps_time_t gps_time;
  const char *time_str = "123519";
  EXPECT_EQ(NEMA_SUCCESS, parse_gps_time(time_str, &gps_time));
  EXPECT_EQ(12, gps_time.hours);
  EXPECT_EQ(35, gps_time.minutes);
  EXPECT_EQ(19, gps_time.seconds);
  EXPECT_EQ(0, gps_time.milliseconds);
}

TEST(NMEATest, ParseGPSTime_Empty) {
  gps_time_t gps_time;
  const char *time_str = "";
  EXPECT_EQ(NEMA_MISSING_CHECKSUM, parse_gps_time(time_str, &gps_time));
}

TEST(NMEATest, ParseGPSTime_InvalidFormat) {
  gps_time_t gps_time;
  const char *time_str = "12:35:19";
  EXPECT_EQ(NEMA_TIME_INVALID_MINUTES, parse_gps_time(time_str, &gps_time));
}

TEST(NMEATest, ParseGPSTime_InvalidHours) {
  gps_time_t gps_time;
  const char *time_str = "253519.123";
  EXPECT_EQ(NEMA_TIME_INVALID_HOURS, parse_gps_time(time_str, &gps_time));
}

TEST(NMEATest, ParseGPSTime_OutOfRange) {
  gps_time_t gps_time;
  const char *time_str = "236060.123";
  EXPECT_EQ(NEMA_TIME_INVALID_MINUTES, parse_gps_time(time_str, &gps_time));
}

// Test validate_nmea_checksum
TEST(NMEATest, ValidateChecksum_Valid) {
  const char *msg =
      "$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47";
  EXPECT_EQ(0, validate_nmea_checksum(msg));
}

TEST(NMEATest, ValidateChecksum_InvalidChecksum) {
  const char *msg =
      "$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*XX";
  EXPECT_EQ(-4, validate_nmea_checksum(msg));
}

TEST(NMEATest, ValidateChecksum_Mismatch) {
  const char *msg =
      "$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*48";
  EXPECT_EQ(-5, validate_nmea_checksum(msg));
}

TEST(NMEATest, ValidateChecksum_NoChecksum) {
  const char *msg =
      "$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,";
  EXPECT_EQ(-2, validate_nmea_checksum(msg));
}

TEST(NMEATest, ValidateChecksum_InvalidInput) {
  const char *msg =
      "GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47";
  EXPECT_EQ(-1, validate_nmea_checksum(msg));
}

// Test calculate_position_error
// TEST_F(NMEATest, CalculatePositionError_Valid) {
//   float hdop = 1.0, pdop = 1.7, vdop = 1.3, uere = 2.0, confidence = 2.0;
//   EXPECT_EQ(0, calculate_position_error(hdop, pdop, vdop, uere, confidence,
//   &pos_error)); EXPECT_FLOAT_EQ(4.0, pos_error.horizontal_error); // 1.0
//   * 2.0 * 2.0 EXPECT_FLOAT_EQ(5.2, pos_error.vertical_error);   // 1.3 * 2.0
//   * 2.0 EXPECT_FLOAT_EQ(6.8, pos_error.total_error);      // 1.7 * 2.0 * 2.0
// }

// TEST_F(NMEATest, CalculatePositionError_ZeroConfidence) {
//   float hdop = 1.0, pdop = 1.7, vdop = 1.3, uere = 2.0, confidence = 0.0;
//   EXPECT_EQ(-1, calculate_position_error(hdop, pdop, vdop, uere, confidence,
//   &pos_error));
// }

// TEST_F(NMEATest, CalculatePositionError_NegativeDOP) {
//   float hdop = -1.0, pdop = 1.7, vdop = 1.3, uere = 2.0, confidence = 2.0;
//   EXPECT_EQ(-1, calculate_position_error(hdop, pdop, vdop, uere, confidence,
//   &pos_error));
// }

// TEST_F(NMEATest, CalculatePositionError_NullOutput) {
//   float hdop = 1.0, pdop = 1.7, vdop = 1.3, uere = 2.0, confidence = 2.0;
//   EXPECT_EQ(-2, calculate_position_error(hdop, pdop, vdop, uere, confidence,
//   nullptr));
// }

RUN_ALL();