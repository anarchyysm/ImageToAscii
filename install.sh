#!/bin/bash

BOLD="\033[1m"
RED="\033[31m"
GREEN="\033[32m"
YELLOW="\033[33m"
CYAN="\033[36m"
RESET="\033[0m"

echo -e "${CYAN}${BOLD}Choose Installation option:${RESET}"
echo -e "${YELLOW}${BOLD}1 - Just CLI${RESET}"
echo -e "${YELLOW}${BOLD}2 - CLI + GUI${RESET}"
read -p "Option (1/2): " OPTION


CMAKELISTS_FILE="CMakeLists.txt"

if [ "$OPTION" == "1" ]; then
    echo -e "${GREEN}${BOLD}Only CLI selected.${RESET}"
    cat > $CMAKELISTS_FILE <<EOL
cmake_minimum_required(VERSION 3.30)
project(ConvertImageToAscii)

find_package(OpenCV REQUIRED)
set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(\${OpenCV_INCLUDE_DIRS})
include_directories(AsciiConversor AsciiConversor/BinFiles)

add_executable(ConvertImageToAscii
    Main.cpp
    AsciiConversor/ImageProcessor.cpp
    AsciiConversor/VideoProcessor.cpp
)

target_link_libraries(ConvertImageToAscii PRIVATE \${OpenCV_LIBS})
EOL
elif [ "$OPTION" == "2" ]; then
    echo -e "${GREEN}${BOLD}Installation CLI + GUI selected.${RESET}"
    cat > $CMAKELISTS_FILE <<EOL
cmake_minimum_required(VERSION 3.30)
project(ConvertImageToAscii)

find_package(OpenCV REQUIRED)
find_package(Qt5 COMPONENTS Widgets REQUIRED)
set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(\${OpenCV_INCLUDE_DIRS})
include_directories(AsciiConversor)

add_executable(ConvertImageToAscii
    Main.cpp
    AsciiConversor/ImageProcessor.cpp
    AsciiConversor/VideoProcessor.cpp
)

target_link_libraries(ConvertImageToAscii PRIVATE \${OpenCV_LIBS})

add_subdirectory(ImageToAsciiGUI)
EOL
else
    echo -e "${RED}${BOLD}Invalid option! Please choose 1 or 2.${RESET}"
    exit 1
fi

echo -e "${CYAN}${BOLD}File $CMAKELISTS_FILE generated successfully!${RESET}"

mkdir -p build
cd build
cmake ..
make -j $(nproc)
