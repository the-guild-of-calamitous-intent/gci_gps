# Calculating Error

## Position Error

GPS covariance matrix: [hdop^2 hdop^2 vdop^2]

- **DOP (Dilution of Precision):** A unitless factor that quantifies how satellite geometry impacts position accuracy. Lower DOP values indicate better geometry (more accurate positioning).
  - **HDOP:** Affects horizontal position (latitude and longitude).
  - **VDOP:** Affects vertical position (altitude).
  - **PDOP:** Affects 3D position (latitude, longitude, and altitude combined).
- **UERE (User Equivalent Range Error):** An estimate of the error in the pseudorange measurements (in meters), typically including errors from satellite clocks, ephemeris, ionosphere, troposphere, and receiver noise. UERE varies depending on the GPS system and conditions (e.g., 1–5 meters for standard GPS, lower for differential GPS).
- **Position Error:** The estimated error in position is calculated by multiplying the appropriate DOP value by UERE:
  - Horizontal Error = HDOP × UERE
  - Vertical Error = VDOP × UERE
  - 3D Error = PDOP × UERE
  These errors are typically expressed as standard deviations (1-sigma, ~68% confidence). For higher confidence levels (e.g., 95% or 2-sigma), multiply the error by a scaling factor (e.g., 2 for 95%).

### Calculate Error

Obtain DOP Values:

1. Extract HDOP, PDOP, and VDOP from the GSA or GGA NMEA messages (as parsed in your previous code).
2. Example: From a GSA message like $GPGSA,A,3,19,28,14,18,,,,,,,1.7,1.0,1.3*35, you get:
  - PDOP = 1.7
  - HDOP = 1.0
  - VDOP = 1.3

3. Estimate UERE:
  - UERE depends on the GPS system and environment:
    - Standard GPS: ~2–5 meters (civilian, non-differential).
    - Differential GPS (DGPS): ~0.5–2 meters.
    - Modern GNSS (GPS+GLONASS+Galileo): ~1–3 meters.
    - High-precision (RTK): ~0.01–0.1 meters.

  For a rough estimate, assume UERE = 2 meters for modern GNSS without differential corrections, or use a measured value if available (e.g., from receiver specifications or environmental conditions). If you have additional data (e.g., from a receiver’s signal-to-noise ratio or augmentation systems like WAAS), you can refine UERE.

Calculate Errors:

1. **Horizontal Error (1-sigma):** HDOP × UERE (meters, affects latitude/longitude).
2. **Vertical Error (1-sigma):** VDOP × UERE (meters, affects altitude).
3. **3D Error (1-sigma):** PDOP × UERE (meters, total position error).

For 95% confidence (2-sigma), multiply each by 2.

Interpret Results:

The errors represent the expected standard deviation of position measurements. For example, a horizontal error of 2 meters means ~68% of position estimates are within ±2 meters of the true position horizontally.
