cmake_minimum_required(VERSION 3.24)

add_library(Chart Chart.cpp Chart.h Note.cpp Note.h Bar.cpp Bar.h Beat.cpp Beat.h)

target_include_directories(Chart 
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/RocksmithCharter_artefacts/JuceLibraryCode
)

target_link_libraries(GUI PUBLIC IO)
