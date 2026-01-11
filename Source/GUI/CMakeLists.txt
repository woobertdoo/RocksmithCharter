cmake_minimum_required(VERSION 3.24)

add_library(GUI MainComponent.h MainComponent.cpp TimelinePanel.h TimelinePanel.cpp EditionPanel.h EditionPanel.cpp PreviewPanel.h PreviewPanel.cpp)

target_include_directories(GUI 
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/RocksmithCharter_artefacts/JuceLibraryCode
)

target_link_libraries(GUI PUBLIC IO)
