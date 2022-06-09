#cmake version
cmake_minimum_required(VERSION 3.5)

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_C_COMPILER /home/nanhe/RV1109/prebuilts/gcc/linux-x86/arm/gcc-arm-8.3-2019.03-x86_64-arm-linux-gnueabihf/bin/arm-linux-gnueabihf-gcc)
set(CMAKE_CXX_COMPILER /home/nanhe/RV1109/prebuilts/gcc/linux-x86/arm/gcc-arm-8.3-2019.03-x86_64-arm-linux-gnueabihf/bin/arm-linux-gnueabihf-g++)

#project name
project(alsa_example)

file(GLOB SOURCES  "src/*.c")
#lib dentry
link_directories(${PROJECT_SOURCE_DIR}/lib)
#add executable
message(nanhe ${PROJECT_SOURCE_DIR})
add_executable(alsa_example ${SOURCES})

#include
target_include_directories(alsa_example
    PUBLIC
    $(PROJECT_SOURCE_NAME)/include
)
#link library
target_link_libraries(alsa_example -lasound)
