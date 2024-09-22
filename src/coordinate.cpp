#include "coordinate.hpp"

// Default constructor
coordinate::coordinate() : lat(0.0), lon(0.0) {}

// Constructor with parameters
coordinate::coordinate(double lat, double lon) : lat(lat), lon(lon) {}

// Getter for latitude
double coordinate::get_latitude() const
{
  return lat;
}

// Getter for longitude
double coordinate::get_longitude() const
{
  return lon;
}
