# NEMA Messages

- [NEMA formats](https://gpsd.gitlab.io/gpsd/NMEA.html)

## GGA

This is one of the sentences commonly emitted by GPS units.

Time, Position and fix related data for a GPS receiver.

```
                                                      11
        1         2       3 4        5 6 7  8   9  10 |  12 13  14   15
        |         |       | |        | | |  |   |   | |   | |   |    |
 $--GGA,hhmmss.ss,ddmm.mm,a,ddmm.mm,a,x,xx,x.x,x.x,M,x.x,M,x.x,xxxx*hh<CR><LF>
```

Field Number:

1. UTC of this position report, hh is hours, mm is minutes, ss.ss is seconds.
2. Latitude, dd is degrees, mm.mm is minutes
3. N or S (North or South)
4. Longitude, dd is degrees, mm.mm is minutes
5. E or W (East or West)
6. GPS Quality Indicator (non null)
  - 0: fix not available,
  - 1: GPS fix,
  - 2: Differential GPS fix (values above 2 are 2.3 features)
  - 3: PPS fix
  - 4: Real Time Kinematic
  - 5: Float RTK
  - 6: estimated (dead reckoning)
  - 7: Manual input mode
  - 8: Simulation mode
7. Number of satellites in use, 00 - 12
8. Horizontal Dilution of precision (meters)
9. Antenna Altitude above/below mean-sea-level (geoid) (in meters)
10. Units of antenna altitude, meters
11. Geoidal separation, the difference between the WGS-84 earth ellipsoid and mean-sea-level (geoid), "-" means mean-sea-level below ellipsoid
12. Units of geoidal separation, meters
13. Age of differential GPS data, time in seconds since last SC104 type 1 or 9 update, null field when DGPS is not used
14. Differential reference station ID, 0000-1023
15. Checksum

The number of digits past the decimal point for Time, Latitude and Longitude is model dependent.

Example: `$GNGGA,001043.00,4404.14036,N,12118.85961,W,1,12,0.98,1113.0,M,-21.3,M*47`

> Note: Jackson Labs replaces the Quality indicator with GPSDO status.

Content: UTC time, latitude, longitude, fix quality, number of satellites, horizontal dilution of precision (HDOP), altitude, geoid separation.

`$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47`

```yaml
Time: 123519
Latitude: 48.117300 N
Longitude: 11.516667 E
Fix Quality: 1
Satellites: 8
HDOP: 0.9
Altitude: 545.4 M
Geoid Separation: 46.9 M
```

## RMC

Content: UTC time, status (active/void), latitude, longitude, speed over ground (SOG), course over ground (COG), date, magnetic variation.

This is one of the sentences commonly emitted by GPS units.
```
        1         2 3       4 5        6  7   8   9    10 11
        |         | |       | |        |  |   |   |    |  |
 $--RMC,hhmmss.ss,A,ddmm.mm,a,dddmm.mm,a,x.x,x.x,xxxx,x.x,a*hh<CR><LF>
NMEA 2.3:
 $--RMC,hhmmss.ss,A,ddmm.mm,a,dddmm.mm,a,x.x,x.x,xxxx,x.x,a,m*hh<CR><LF>
NMEA 4.1:
 $--RMC,hhmmss.ss,A,ddmm.mm,a,dddmm.mm,a,x.x,x.x,xxxx,x.x,a,m,s*hh<CR><LF>
```

Field Number:

1. UTC of position fix, hh is hours, mm is minutes, ss.ss is seconds.
2. Status, A = Valid, V = Warning
3. Latitude, dd is degrees. mm.mm is minutes.
4. N or S
5. Longitude, ddd is degrees. mm.mm is minutes.
6. E or W
7. Speed over ground, knots
8. Track made good, degrees true
9. Date, ddmmyy
10. Magnetic Variation, degrees
11. E or W
12. FAA mode indicator (NMEA 2.3 and later)
13. Nav Status (NMEA 4.1 and later) A=autonomous, D=differential, E=Estimated, M=Manual input mode N=not valid, S=Simulator, V = Valid
14. Checksum

A status of V means the GPS has a valid fix that is below an internal quality threshold, e.g. because the dilution of precision is too high or an elevation mask test failed.

The number of digits past the decimal point for Time, Latitude and Longitude is model dependent.

Example: 
  - `$GNRMC,001031.00,A,4404.13993,N,12118.86023,W,0.146,,100117,,,A*7B`
  - `$GPRMC,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W*6A`

```yaml
Time: 123519
Status: A
Latitude: 48.117300 N
Longitude: 11.516667 E
Speed: 22.4 knots
Course: 84.4 degrees
Date: 230394
Magnetic Variation: -3.1 W
```

## VTG

> I don't know how useful this is if I have the other messages

This is one of the sentences commonly emitted by GPS units.

```
         1  2  3  4  5  6  7  8 9
         |  |  |  |  |  |  |  | |
 $--VTG,x.x,T,x.x,M,x.x,N,x.x,K*hh<CR><LF>
NMEA 2.3:
 $--VTG,x.x,T,x.x,M,x.x,N,x.x,K,m*hh<CR><LF>
```

Field Number:
1. Course over ground, degrees True
2. T = True
3. Course over ground, degrees Magnetic
4. M = Magnetic
5. Speed over ground, knots
6. N = Knots
7. Speed over ground, km/h
8. K = Kilometers Per Hour
9. FAA mode indicator (NMEA 2.3 and later)
10. Checksum

Example: `$GPVTG,220.86,T,,M,2.550,N,4.724,K,A*34`

Content: Course over ground (true and magnetic), speed over ground (in knots and km/h).

## GSA
This is one of the sentences commonly emitted by GPS units.

```
        1 2 3                        14 15  16  17  18
        | | |                         |  |   |   |   |
 $--GSA,a,a,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x.x,x.x,x.x*hh<CR><LF>
```

Field Number:

1. Selection mode: M=Manual, forced to operate in 2D or 3D, A=Automatic, 2D/3D
2. Mode (1 = no fix, 2 = 2D fix, 3 = 3D fix)
3. ID of 1st satellite used for fix
4. ID of 2nd satellite used for fix
5. ID of 3rd satellite used for fix
6. ID of 4th satellite used for fix
7. ID of 5th satellite used for fix
8. ID of 6th satellite used for fix
9. ID of 7th satellite used for fix
10. ID of 8th satellite used for fix
11. ID of 9th satellite used for fix
12. ID of 10th satellite used for fix
13. ID of 11th satellite used for fix
14. ID of 12th satellite used for fix
15. PDOP
16. HDOP
17. VDOP
18. xx. Checksum

Example: `$GNGSA,A,3,80,71,73,79,69,,,,,,,,1.83,1.09,1.47*17`

Note: NMEA 4.1+ systems (u-blox 9, Quectel LCD79) may emit an extra field, System ID, just before the checksum.

1 = GPS L1C/A, L2CL, L2CM
2 = GLONASS L1 OF, L2 OF
3 = Galileo E1C, E1B, E5 bl, E5 bQ
4 = BeiDou B1I D1, B1I D2, B2I D1, B2I D12


Content: Mode (manual/automatic), fix type (1=no fix, 2=2D, 3=3D), satellites used in the fix, dilution of precision (DOP) values (PDOP, HDOP, VDOP).

`$GPGSA,A,3,19,28,14,18,,,,,,,1.7,1.0,1.3*35`

```yaml
Mode: A
Fix Type: 3
Satellites Used: 19 28 14 18 0 0 0 0 0 0 0 0
PDOP: 1.7
HDOP: 1.0
VDOP: 1.3
```

## Other Messages

- Comprehensive Data: Together, GGA, RMC, and VTG provide the essential navigation parameters: position, speed, course, and time. These are the foundation for most navigation tasks, from plotting a route to maintaining a heading.
- Reliability Assessment: GSA and GSV offer metadata about the fix quality and satellite constellation, enabling systems to gauge the trustworthiness of the position data. This is critical for safety-critical applications like aviation or marine navigation.
- Real-Time Decision Making: The combination of position (GGA, RMC), velocity (RMC, VTG), and accuracy metrics (GSA, GSV) supports real-time navigation decisions, such as course corrections, speed adjustments, or rerouting.
