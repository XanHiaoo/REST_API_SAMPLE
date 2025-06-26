#ifndef SERVER_H
#define SERVER_H

#include <crow.h>
#include <unordered_map>
#include <nlohmann/json.hpp>
#include <fstream>

class RESTfulServer {
public:
    RESTfulServer();
    void start(int port);

private:
    void setupRoutes();
    void loadConfigFromFile();
    void saveConfigToFile();

    std::unordered_map<std::string, nlohmann::json> configStorage;
    crow::SimpleApp app;
};

#endif  // SERVER_H
