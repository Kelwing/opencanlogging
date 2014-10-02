cmake_minimum_required (VERSION 2.6)

project (opencanlogging)

add_executable(
    opencanlogging
    main.cpp
    )

target_link_libraries(
    opencanlogging
    boost_system
    boost_program_options
    jsoncpp # for config parsing
    )


