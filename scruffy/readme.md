# Scruffy Tests

You should be able to build and run the tests:

```bash
build$ ./scruffy/test-all 
[==========] Starting tests
[----------] Global test environment set-up.
[ RUN      ] NMEATest.ParseGGA_ValidMessage
[       OK ] 10 / 10 tests passed (7 usec)
[----------]
[ RUN      ] NMEATest.ParseGGA_EmptyFields
[       OK ] 23 / 23 tests passed (3 usec)
[----------]
[ RUN      ] NMEATest.ParseGGA_GOOD_2
[       OK ] 36 / 36 tests passed (71 usec)
[----------]
[ RUN      ] NMEATest.ParseGGA_InvalidMessage
[       OK ] 37 / 37 tests passed (2 usec)
[----------]
[ RUN      ] NMEATest.ParseGGA_IncompleteMessage
[       OK ] 38 / 38 tests passed (3 usec)
[----------]
[ RUN      ] NMEATest.ParseRMC_ValidMessage
[       OK ] 48 / 48 tests passed (2 usec)
[----------]
[ RUN      ] NMEATest.ParseRMC_EmptyFields
[       OK ] 60 / 60 tests passed (2 usec)
[----------]
[ RUN      ] NMEATest.ParseRMC_InvalidMessage
[       OK ] 61 / 61 tests passed (65 usec)
[----------]
[ RUN      ] NMEATest.ParseRMC_IncompleteMessage
[       OK ] 62 / 62 tests passed (3 usec)
[----------]
[ RUN      ] NMEATest.ParseGSA_ValidMessage
[       OK ] 80 / 80 tests passed (2 usec)
[----------]
[ RUN      ] NMEATest.ParseGSA_EmptyFields
[       OK ] 98 / 98 tests passed (2 usec)
[----------]
[ RUN      ] NMEATest.ParseGSA_InvalidMessage
[       OK ] 99 / 99 tests passed (1 usec)
[----------]
[ RUN      ] NMEATest.ParseGSA_IncompleteMessage
[       OK ] 100 / 100 tests passed (2 usec)
[----------]
[ RUN      ] NMEATest.ParseGPSTime_ValidWithMilliseconds
[       OK ] 105 / 105 tests passed (2 usec)
[----------]
[ RUN      ] NMEATest.ParseGPSTime_ValidWithoutMilliseconds
[       OK ] 110 / 110 tests passed (2 usec)
[----------]
[ RUN      ] NMEATest.ParseGPSTime_Empty
[       OK ] 111 / 111 tests passed (2 usec)
[----------]
[ RUN      ] NMEATest.ParseGPSTime_InvalidFormat
[       OK ] 112 / 112 tests passed (1 usec)
[----------]
[ RUN      ] NMEATest.ParseGPSTime_InvalidHours
[       OK ] 113 / 113 tests passed (1 usec)
[----------]
[ RUN      ] NMEATest.ParseGPSTime_OutOfRange
[       OK ] 114 / 114 tests passed (1 usec)
[----------]
[ RUN      ] NMEATest.ValidateChecksum_Valid
[       OK ] 115 / 115 tests passed (1 usec)
[----------]
[ RUN      ] NMEATest.ValidateChecksum_InvalidChecksum
[       OK ] 116 / 116 tests passed (1 usec)
[----------]
[ RUN      ] NMEATest.ValidateChecksum_Mismatch
[       OK ] 117 / 117 tests passed (1 usec)
[----------]
[ RUN      ] NMEATest.ValidateChecksum_NoChecksum
[       OK ] 118 / 118 tests passed (68 usec)
[----------]
[ RUN      ] NMEATest.ValidateChecksum_InvalidInput
[       OK ] 119 / 119 tests passed (1 usec)
[----------]
[----------] Global test environment tear-down
[==========] 119 out of 119 total tests passed (603 usec total)
```