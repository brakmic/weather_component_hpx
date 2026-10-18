cmake_minimum_required(VERSION 3.15)

project(weather_component_hpx LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# CMP0167 is a CMake 3.30 policy. Older CMake versions do not know it.
if(POLICY CMP0167)
  cmake_policy(SET CMP0167 NEW)
endif()

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(third-party/json/include)
include_directories(third-party/httplib)

# Find HPX and OpenSSL
find_package(HPX REQUIRED)
find_package(OpenSSL REQUIRED)

# Create the executable
add_executable(weather_component
                src/main.cpp
                src/weather_component.cpp
                src/city_resolver_component.cpp
                src/coordinate.cpp)

# Link HPX and OpenSSL
target_link_libraries(weather_component PRIVATE
                        HPX::hpx
                        HPX::wrap_main
                        HPX::iostreams_component
                        OpenSSL::SSL
                        OpenSSL::Crypto)
