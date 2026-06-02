cmake_minimum_required(VERSION 3.24)

add_library(CHART Chart.cpp Chart.h Note.cpp Note.h Bar.cpp Bar.h Beat.cpp Beat.h NoteBuilder.h NoteBuilder.cpp)

target_include_directories(CHART 
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/RocksmithCharter_artefacts/JuceLibraryCode
)

target_link_libraries(CHART PUBLIC IO UTIL)
