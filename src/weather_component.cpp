#define CPPHTTPLIB_OPENSSL_SUPPORT

#include "weather_component.hpp"
#include <httplib.h>
#include <nlohmann/json.hpp>
#include <sstream>
#include <iomanip>
#include <optional>

// Main method that retrieves weather data for the given coordinates
std::string weather_component::get_weather(const std::optional<coordinate> &coord_opt)
{
  // Check if we received a valid coordinate
  if (!coord_opt.has_value())
  {
    return "Error: City could not be resolved to valid coordinates.";
  }

  const coordinate &coord = coord_opt.value(); // Extract coordinate
  httplib::Client cli("https://api.open-meteo.com");

  // First API call: General weather data
  std::stringstream path;
  path << "/v1/forecast?latitude=" << std::fixed << std::setprecision(4) << coord.get_latitude()
       << "&longitude=" << std::fixed << std::setprecision(4) << coord.get_longitude()
       << "&current_weather=true";

  std::string result;

  if (auto res = cli.Get(path.str().c_str()))
  {
    if (res->status == 200)
    {
      auto data = nlohmann::json::parse(res->body);

      double windspeed = data["current_weather"]["windspeed"].get<double>();
      double temperature = data["current_weather"]["temperature"].get<double>();
      double winddirection = data["current_weather"]["winddirection"].get<double>();
      std::string time = data["current_weather"]["time"].get<std::string>();

      std::stringstream weather_info;
      weather_info << "Time: " << time << "\n"
                   << "Temperature: " << temperature << " °C\n"
                   << "Windspeed: " << windspeed << " km/h\n"
                   << "Wind Direction: " << winddirection << " degrees\n";

      result = weather_info.str();
    }
  }

  // Second API call: Additional weather data
  std::stringstream path2;
  path2 << "/v1/forecast?latitude=" << std::fixed << std::setprecision(4) << coord.get_latitude()
        << "&longitude=" << std::fixed << std::setprecision(4) << coord.get_longitude()
        << "&hourly=relative_humidity_2m,cloud_cover,visibility,pressure_msl";

  if (auto res2 = cli.Get(path2.str().c_str()))
  {
    if (res2->status == 200)
    {
      auto data2 = nlohmann::json::parse(res2->body);

      double humidity = data2["hourly"]["relative_humidity_2m"][0].get<double>();
      double pressure = data2["hourly"]["pressure_msl"][0].get<double>();
      double cloudcover = data2["hourly"]["cloud_cover"][0].get<double>();
      double visibility = data2["hourly"]["visibility"][0].get<double>();

      std::stringstream extra_info;
      extra_info << "Humidity: " << humidity << "%\n"
                 << "Pressure: " << pressure << " hPa\n"
                 << "Cloud Cover: " << cloudcover << "%\n"
                 << "Visibility: " << visibility << " km";

      result += extra_info.str();
    }
  }

  if (result.empty())
  {
    return "Failed to retrieve weather data.";
  }

  return result;
}

typedef hpx::components::component<weather_component> weather_component_type;
HPX_REGISTER_COMPONENT(weather_component_type, weather_component);
HPX_REGISTER_ACTION(weather_component::get_weather_action, weather_component_get_weather_action);
