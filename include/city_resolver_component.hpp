#ifndef CITY_RESOLVER_COMPONENT_HPP
#define CITY_RESOLVER_COMPONENT_HPP

#include <hpx/include/components.hpp>
#include <unordered_map>
#include <optional>
#include "coordinate.hpp"

class city_resolver_component : public hpx::components::component_base<city_resolver_component>
{
public:
  // Resolve city to coordinates, returning std::optional
  std::optional<coordinate> resolve_city_to_coordinate(const std::string &city);

  HPX_DEFINE_COMPONENT_ACTION(city_resolver_component, resolve_city_to_coordinate);
};

HPX_REGISTER_ACTION_DECLARATION(city_resolver_component::resolve_city_to_coordinate_action);

#endif // CITY_RESOLVER_COMPONENT_HPP
