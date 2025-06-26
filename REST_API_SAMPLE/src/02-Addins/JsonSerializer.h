#pragma once
#include <nlohmann/json.hpp>
#include "01-Interface/ISerializer.h"

template <typename T> class JsonSerializer : public ISerializer<T> {
public:
    std::string serialize(const T &obj) override
    {
        nlohmann::json j = obj;
        return j.dump();
    }

    T deserialize(const std::string &data) override
    {
        nlohmann::json j = nlohmann::json::parse(data);
        return j.get<T>();
    }
};
