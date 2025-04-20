LIST(APPEND examples
    gga
    rmc
    gsa
    gps_time
    checksum
    position_error
)

foreach(app ${examples})
    add_executable(${app} ${app}.c)
    target_link_libraries(${app} PUBLIC ${PROJECT_NAME})
endforeach()