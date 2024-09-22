cmake_minimum_required(VERSION 3.15)
project(WeatherComponent)

# Set C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add third-party directories to search path
add_subdirectory(third-party/json)
add_subdirectory(third-party/curl)

# Include directories for nlohmann/json and curl
include_directories(third-party/json/include)
include_directories(third-party/curl/include)

# Link curl library
find_package(CURL REQUIRED)
include_directories(${CURL_INCLUDE_DIR})

# Add the weather component executable
add_executable(weather_component main.cpp weather_component.cpp)

# Link against curl and nlohmann/json (nlohmann/json is header-only)
target_link_libraries(weather_component ${CURL_LIBRARIES})
