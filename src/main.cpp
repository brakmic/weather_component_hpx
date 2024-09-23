#include <hpx/hpx_main.hpp>
#include <hpx/include/components.hpp>
#include <hpx/iostream.hpp>
#include <hpx/future.hpp>
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

  // Sequential approach
  // *****************
  // // Invoke the resolve_city_to_coordinate action
  // city_resolver_component::resolve_city_to_coordinate_action resolve_action;
  // std::optional<coordinate> coord_opt = resolve_action(city_resolver_id, city_name);

  // // Invoke the get_weather action, passing std::optional
  // weather_component::get_weather_action weather_action;
  // std::string result = weather_action(weather_component_id, coord_opt);
  // Output the result
  // hpx::cout << result << '\n'
  //           << std::flush;
  // *****************

  // Async approach that uses hpx::future and hpx::async
  // Invoke the resolve_city_to_coordinate action asynchronously using a future
  city_resolver_component::resolve_city_to_coordinate_action resolve_action;
  hpx::future<std::optional<coordinate>> city_resolve_future = hpx::async(resolve_action, city_resolver_id, city_name);

  // Chain the weather action to run after the city resolution completes using future continuation
  auto weather_future = city_resolve_future.then([weather_component_id](hpx::future<std::optional<coordinate>> f)
                                                 {
        std::optional<coordinate> coord_opt = f.get();  // Get the resolved coordinates

        // Invoke the get_weather action, passing the resolved coordinates
        weather_component::get_weather_action weather_action;
        return weather_action(weather_component_id, coord_opt); });

  // Wait for the weather future to complete and get the result
  std::string result = weather_future.get();

  // Output the result
  hpx::cout << result << '\n'
            << std::flush;

  return 0;
}
