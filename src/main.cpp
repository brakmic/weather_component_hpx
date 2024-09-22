#include <hpx/hpx_main.hpp>
#include <hpx/include/components.hpp>
#include <hpx/iostream.hpp>
#include "weather_component.hpp"
#include "city_resolver_component.hpp"

int main(int argc, char *argv[])
{
  // Ensure city name is provided
  if (argc < 2)
  {
    hpx::cout << "Usage: " << argv[0] << " <city_name>\n"
              << std::flush;
    return 1;
  }

  std::string city_name = argv[1];

  // Create instances of components
  hpx::id_type city_resolver_id = hpx::new_<city_resolver_component>(hpx::find_here()).get();
  hpx::id_type weather_component_id = hpx::new_<weather_component>(hpx::find_here()).get();

  // Invoke the resolve_city_to_coordinate action
  city_resolver_component::resolve_city_to_coordinate_action resolve_action;
  std::optional<coordinate> coord_opt = resolve_action(city_resolver_id, city_name);

  // Invoke the get_weather action, passing std::optional
  weather_component::get_weather_action weather_action;
  std::string result = weather_action(weather_component_id, coord_opt);

  // Output the result
  hpx::cout << result << '\n'
            << std::flush;

  return 0;
}
