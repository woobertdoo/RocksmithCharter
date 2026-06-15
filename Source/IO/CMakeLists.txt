cmake_minimum_required(VERSION 3.24)

add_library(IO FileCodes.h GPExtractor.h GPExtractor.cpp GPParser.h GPParser.cpp)

target_include_directories(IO
    PUBLIC
        ${CMAKE_SOURCE_DIR}/RocksmithCharter_artefacts/JuceLibraryCode
        ${CMAKE_CURRENT_SOURCE_DIR}
        ../Chart
)

target_link_libraries(IO PUBLIC UTIL)
