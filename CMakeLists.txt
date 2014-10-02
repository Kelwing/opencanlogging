cmake_minimum_required (VERSION 2.6)

project (opencanlogging)

SET(CMAKE_CXX_FLAGS "-std=c++0x")

add_executable(
    opencanlogging
    main.cpp
    obd.cpp
    )

target_link_libraries(
    opencanlogging
    boost_thread
    boost_system
    boost_program_options
    jsoncpp # for config parsing
    )


