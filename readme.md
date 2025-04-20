# NEMA Parser

> Yet another rewrite of my gciGps library

## AI Disclosure

Grok helped me write some of this code.

## Layout

- **c:** can be run on a desktop computer for testing and development
  - Build library and run test and development code here
- **gtests:** unit tests that can be run on a desktop computer
  - Run `gtests` here
- **pico:** library build and examples to be run on pi pico micro controller
  - Build library for pico
- **src:** source code for all of the above

## Mesages

| **Aspect**                | **GGA (Global Positioning System Fix Data)** | **GSA (GNSS DOP and Active Satellites)** | **RMC (Recommended Minimum Navigation Information)**  |
|---------------------------|----------------------------------------------|------------------------------------------|-------------------------------------------------------|
| **Purpose**               | Provides essential fix data, including position (latitude, longitude, altitude), time, and quality metrics.   | Provides satellite status, fix type, and Dilution of Precision (DOP) metrics for assessing accuracy.     | Provides minimal navigation data, including position, velocity, course, time, and date.              |
| **Primary Use Case**      | Determining precise 3D position and altitude for navigation, surveying, or mapping.                          | Assessing the quality of the position fix and satellite geometry for reliability checks.                 | Real-time tracking of position, speed, and course for marine, automotive, or aviation navigation.    |
| **Fields**                | 1. UTC Time (HHMMSS.sss)<br>2. Latitude (DDMM.MMMM)<br>3. Latitude Direction (N/S)<br>4. Longitude (DDDMM.MMMM)<br>5. Longitude Direction (E/W)<br>6. Fix Quality (0=invalid, 1=GPS, 2=DGPS, etc.)<br>7. Number of Satellites<br>8. HDOP<br>9. Altitude (meters)<br>10. Altitude Unit (M)<br>11. Geoid Separation (meters)<br>12. Geoid Unit (M)<br>13. Age of Differential Data<br>14. Differential Station ID | 1. Mode (M=manual, A=automatic)<br>2. Fix Type (1=no fix, 2=2D, 3=3D)<br>3–14. Satellite PRN Numbers (up to 12)<br>15. PDOP<br>16. HDOP<br>17. VDOP | 1. UTC Time (HHMMSS.sss)<br>2. Status (A=active, V=void)<br>3. Latitude (DDMM.MMMM)<br>4. Latitude Direction (N/S)<br>5. Longitude (DDDMM.MMMM)<br>6. Longitude Direction (E/W)<br>7. Speed Over Ground (knots)<br>8. Course Over Ground (degrees)<br>9. Date (DDMMYY)<br>10. Magnetic Variation (degrees)<br>11. Magnetic Variation Direction (E/W)<br>12. Mode Indicator (A=autonomous, D=differential, etc.) |
| **Number of Fields**      | 14                                                                                                          | 17                                                                                                      | 12                                                                                                  |
| **Position Data**         | Latitude, Longitude, Altitude                                                                               | None (focuses on satellite and DOP data)                                                                | Latitude, Longitude                                                                                 |
| **Time Data**             | UTC Time                                                                                                    | None                                                                                                    | UTC Time, Date                                                                                     |
| **Velocity Data**         | None                                                                                                        | None                                                                                                    | Speed Over Ground, Course Over Ground                                                              |
| **Quality Metrics**       | Fix Quality, Number of Satellites, HDOP                                                                     | Fix Type, Number of Satellites Used, PDOP, HDOP, VDOP                                                  | Status (Active/Void)                                                                               |
| **Altitude Information**  | Altitude, Geoid Separation                                                                                  | None                                                                                                    | None                                                                                               |
| **Satellite Information** | Number of Satellites in View                                                                                | Specific PRN Numbers of Satellites Used                                                                 | None                                                                                               |
| **Error Estimation**      | HDOP for horizontal error estimation                                                                        | PDOP, HDOP, VDOP for 3D, horizontal, and vertical error estimation                                      | None                                                                                               |
| **Typical Example**       | `$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47`                                        | `$GPGSA,A,3,19,28,14,18,,,,,,,1.7,1.0,1.3*35`                                                          | `$GPRMC,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W*6A`                             |
| **Strengths**             | Comprehensive position data, including altitude; useful for 3D navigation and surveying.                    | Detailed satellite and DOP data for assessing fix reliability; critical for error estimation.            | Combines position, velocity, and time/date; ideal for real-time navigation and tracking.             |
| **Limitations**           | Lacks velocity or course data; no PDOP/VDOP for full error analysis.                                        | No position or velocity data; requires GGA/RMC for context.                                             | No altitude or detailed satellite data; limited quality metrics.                                    |
| **Common Applications**   | Aviation, marine navigation, geospatial mapping, autonomous vehicles.                                       | Navigation systems requiring accuracy assessment, precision agriculture, autonomous systems.            | Marine navigation, vehicle tracking, autopilot systems, route logging.                              |


# MIT License

**Copyright (c) 2023 The Guild of Calamitous Intent**

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.