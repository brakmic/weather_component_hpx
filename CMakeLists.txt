cmake_minimum_required(VERSION 3.15)

# Set policies and suppress developer warnings
cmake_policy(SET CMP0167 NEW)
set(CMAKE_SUPPRESS_DEVELOPER_WARNINGS 1)

project(weather_component_hpx LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(third-party/json/include)
include_directories(third-party/httplib)

# Find HPX
find_package(HPX REQUIRED)

# Platform-specific handling for TBB
if(APPLE)
    # macOS: Use Homebrew-installed TBB
    set(TBB_ROOT "/usr/local/opt/tbb")  # Adjust this path based on your system
    include_directories("${TBB_ROOT}/include")
    link_directories("${TBB_ROOT}/lib")
    set(TBB_LIBRARY "${TBB_ROOT}/lib/libtbb.dylib")
else()
    # For non-macOS systems, use find_package to locate TBB
    find_package(TBB REQUIRED)
    set(TBB_LIBRARY TBB::TBB)
endif()

# Find OpenSSL
find_package(OpenSSL REQUIRED)

# Create the executable
add_executable(weather_component
                src/main.cpp
                src/weather_component.cpp
                src/city_resolver_component.cpp
                src/coordinate.cpp)

# Link HPX and other libraries
target_link_libraries(weather_component
                        HPX::hpx
                        HPX::wrap_main
                        HPX::iostreams_component
                        ${TBB_LIBRARY}
                        OpenSSL::SSL
                        OpenSSL::Crypto)
