#ifndef WEATHER_COMPONENT_HPP
#define WEATHER_COMPONENT_HPP

#include <hpx/include/components.hpp>
#include "coordinate.hpp"
#include <optional>

class weather_component : public hpx::components::component_base<weather_component>
{
public:
  std::string get_weather(const std::optional<coordinate> &coord_opt);

  HPX_DEFINE_COMPONENT_ACTION(weather_component, get_weather);
};

HPX_REGISTER_ACTION_DECLARATION(weather_component::get_weather_action);

#endif // WEATHER_COMPONENT_HPP
