# Weather Component HPX

This project is an [HPX](https://hpx-docs.stellar-group.org/latest/html/index.html)-based C++ application that retrieves weather data for a given city using two APIs:
1. **City Resolution API ([Nominatim OpenStreetMap](https://nominatim.openstreetmap.org/))**: Resolves the city name to geographic coordinates (latitude and longitude).
2. **Weather Data API ([Open-Meteo](https://open-meteo.com/))**: Retrieves weather data for the provided coordinates, including windspeed, temperature, humidity, pressure, and more.

The project uses modern C++ features like `std::optional` to handle cases where the city cannot be resolved to coordinates. The client is unaware of internal handling and continues to function smoothly even when a city is not found. 

### Key Features:
- Uses HPX for distributed, asynchronous task handling.
- Resolves cities to geographic coordinates using the OpenStreetMap Nominatim API.
- Fetches weather data based on coordinates from the Open-Meteo API.
- Filters valid city results using the `addresstype` field from Nominatim to ensure only populated places like cities, towns, and villages are accepted.

### Project Structure:

```
weather_component_hpx/
├── CMakeLists.txt
├── LICENSE
├── include/
│   ├── city_resolver_component.hpp
│   ├── coordinate.hpp
│   └── weather_component.hpp
├── src/
│   ├── city_resolver_component.cpp
│   ├── coordinate.cpp
│   ├── main.cpp
│   └── weather_component.cpp
└── third-party/
    ├── httplib  (cpp-httplib as submodule)
    └── json     (nlohmann/json as submodule)
```

### Dependencies:
This project depends on the following libraries:
- **HPX**: High Performance ParalleX library for distributed systems.
- **cpp-httplib**: A lightweight C++ HTTP/HTTPS library (added as a git submodule).
- **nlohmann/json**: A header-only JSON library for C++ (added as a git submodule).
- **OpenSSL**: For SSL/TLS support in the HTTP client.
- **TBB**: Threading Building Blocks (required for HPX).

### Cloning the Repository

To properly clone the repository, including the required git submodules (`cpp-httplib` and `nlohmann/json`), run the following commands:

```bash
git clone --recurse-submodules https://github.com/brakmic/weather_component_hpx.git
```

If you already cloned the repository without the submodules, you can initialize and update them with:

```bash
git submodule init
git submodule update
```

### Build Instructions

Ensure that you have CMake and a C++23-compatible compiler installed. The project uses HPX and requires OpenSSL for secure HTTP requests.

#### Step 1: Install Dependencies
Make sure to have the following installed on your system:
- **HPX**: Install it via your package manager or build it from source.
- **OpenSSL**: Required for HTTPS support.
- **TBB**: Required by HPX (on macOS, it is expected to be installed via Homebrew).

#### Step 2: Configure and Build
To build the project, follow these steps:

1. **Create a build directory**:
   ```bash
   mkdir build && cd build
   ```

2. **Run CMake**:
   ```bash
   cmake ..
   ```

3. **Compile the project**:
   ```bash
   make -j$(nproc)
   ```

#### Step 3: Running the Application

Once the project is compiled, you can run the `weather_component` executable by providing a city name as a command-line argument:

```bash
./weather_component <city_name>
```

For example:

```bash
./weather_component "Bonn"
```

If the city is resolved successfully, the weather data for that city will be retrieved and displayed. If the city cannot be resolved (e.g., for invalid or non-existent city names), an error message will be returned.

### Example Output

When running the program with the city name "Bonn", the output will be something like:

```
Time: 2024-09-21T15:00:00Z
Temperature: 22.5 °C
Windspeed: 14.0 km/h
Wind Direction: 240 degrees
Humidity: 65%
Pressure: 1015 hPa
Cloud Cover: 20%
Visibility: 10 km
```

### License

This project is licensed under the MIT License. See the [LICENSE](./LICENSE) file for more details.