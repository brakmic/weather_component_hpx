#define CPPHTTPLIB_OPENSSL_SUPPORT

#include "weather_component.hpp"
#include <httplib.h>
#include <nlohmann/json.hpp>
#include <format>
#include <optional>

// Retrieve weather data for the given coordinates
std::string weather_component::get_weather(const std::optional<coordinate> &coord_opt)
{
  // Return error message if no valid coordinate is found
  const coordinate &coord = coord_opt.value_or(coordinate{});
  if (!coord_opt)
  {
    return "Error: City could not be resolved to valid coordinates.";
  }

  httplib::Client cli("https://api.open-meteo.com");

  // Helper lambda for HTTP requests
  auto get_weather_data = [&](const std::string &path) -> std::optional<nlohmann::json>
  {
    if (auto res = cli.Get(path.c_str()); res && res->status == 200)
    {
      return nlohmann::json::parse(res->body);
    }
    return std::nullopt;
  };

  // First API call: General weather data
  std::string path = std::format(
      "/v1/forecast?latitude={:.4f}&longitude={:.4f}&current_weather=true",
      coord.get_latitude(), coord.get_longitude());

  std::string result;
  if (auto general_weather_data = get_weather_data(path); general_weather_data)
  {
    auto windspeed = (*general_weather_data)["current_weather"]["windspeed"].get<double>();
    auto temperature = (*general_weather_data)["current_weather"]["temperature"].get<double>();
    auto winddirection = (*general_weather_data)["current_weather"]["winddirection"].get<double>();
    auto time = (*general_weather_data)["current_weather"]["time"].get<std::string>();

    result = std::format(
        "Time: {}\nTemperature: {} °C\nWindspeed: {} km/h\nWind Direction: {} degrees\n",
        time, temperature, windspeed, winddirection);
  }

  // Second API call: Additional weather data
  std::string path2 = std::format(
      "/v1/forecast?latitude={:.4f}&longitude={:.4f}&hourly=relative_humidity_2m,cloud_cover,visibility,pressure_msl",
      coord.get_latitude(), coord.get_longitude());

  if (auto additional_weather_data = get_weather_data(path2); additional_weather_data)
  {
    auto humidity = (*additional_weather_data)["hourly"]["relative_humidity_2m"][0].get<double>();
    auto pressure = (*additional_weather_data)["hourly"]["pressure_msl"][0].get<double>();
    auto cloudcover = (*additional_weather_data)["hourly"]["cloud_cover"][0].get<double>();
    auto visibility = (*additional_weather_data)["hourly"]["visibility"][0].get<double>();

    result += std::format(
        "Humidity: {}%\nPressure: {} hPa\nCloud Cover: {}%\nVisibility: {} km",
        humidity, pressure, cloudcover, visibility);
  }

  return result.empty() ? "Failed to retrieve weather data." : result;
}

typedef hpx::components::component<weather_component> weather_component_type;
HPX_REGISTER_COMPONENT(weather_component_type, weather_component);
HPX_REGISTER_ACTION(weather_component::get_weather_action, weather_component_get_weather_action);
