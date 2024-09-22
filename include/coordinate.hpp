#ifndef COORDINATE_HPP
#define COORDINATE_HPP

#include <hpx/serialization/serialize.hpp>

struct coordinate
{
private:
  double lat;
  double lon;

public:
  // Default constructor for serialization
  coordinate();
  coordinate(double lat, double lon);

  double get_latitude() const;
  double get_longitude() const;

  // Serialization method (only in header, since it's templated)
  template <typename Archive>
  void serialize(Archive &ar, unsigned int version)
  {
    ar & lat & lon;
  }
};

#endif // COORDINATE_HPP
