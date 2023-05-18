# cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE='/vcpkg/scripts/buildsystems/vcpkg.cmake'
# cmake --build build
# build/Debug/luatest.exe

# LUA headers are placed in:
# build\vcpkg_installed\x64-windows\include

cmake_minimum_required(VERSION 3.15)

project(luatest LANGUAGES C CXX)

# target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Lua REQUIRED)

add_executable(${PROJECT_NAME} main.cxx)

target_include_directories(${PROJECT_NAME} PRIVATE ${LUA_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE ${LUA_LIBRARIES})
