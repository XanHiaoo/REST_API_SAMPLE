#pragma once
#include <string>

template <typename T> class ISerializer {
public:
  virtual std::string serialize(const T &obj) = 0;
  virtual T deserialize(const std::string &data) = 0;
  virtual ~ISerializer() = default;
};
