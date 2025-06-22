cmake_minimum_required(VERSION 3.31)
project(BejeweledPokmon)

set(CMAKE_CXX_STANDARD 26)

include_directories(D:/CLion/mingw64/include)
find_package(SFML 3.0.0 REQUIRED COMPONENTS System Window Graphics Audio Network) # 检查你的 SFML 版本
set(SFML_DIR "D:/CLion/mingw64/lib/cmake/SFML/")

add_executable(BejeweledPokmon main.cpp
        C_RandomNum/C_RandomNum.cpp
        C_RandomNum/C_RandomNum.h)

target_link_libraries(BejeweledPokmon PRIVATE SFML::Graphics SFML::Audio SFML::Network SFML::System SFML::Window)