#define CPPHTTPLIB_OPENSSL_SUPPORT

#include "city_resolver_component.hpp"
#include <nlohmann/json.hpp>
#include <httplib.h>
#include <iostream>
#include <optional>

std::optional<coordinate> city_resolver_component::resolve_city_to_coordinate(const std::string &city)
{
  httplib::SSLClient client("nominatim.openstreetmap.org");
  client.set_follow_location(true); // Follow redirects

  auto res = client.Get(("/search?q=" + city + "&format=json&limit=1").c_str());

  if (res && res->status == 200)
  {
    try
    {
      auto json_response = nlohmann::json::parse(res->body);
      if (!json_response.empty())
      {
        // Extract the addresstype field
        std::string addresstype = json_response[0]["addresstype"].get<std::string>();

        // Check if the addresstype indicates a populated area
        if (addresstype == "city" || addresstype == "town" ||
            addresstype == "village" || addresstype == "hamlet" ||
            addresstype == "suburb" || addresstype == "neighbourhood")
        {
          double lat = std::stod(json_response[0]["lat"].get<std::string>());
          double lon = std::stod(json_response[0]["lon"].get<std::string>());
          return std::make_optional<coordinate>(lat, lon);
        }
      }
    }
    catch (const std::exception &e)
    {
      std::cerr << "Error parsing response: " << e.what() << std::endl;
    }
  }

  return std::nullopt; // Return std::nullopt if no valid place was found
}

typedef hpx::components::component<city_resolver_component> city_resolver_component_type;
HPX_REGISTER_COMPONENT(city_resolver_component_type, city_resolver_component);
HPX_REGISTER_ACTION(city_resolver_component::resolve_city_to_coordinate_action);
