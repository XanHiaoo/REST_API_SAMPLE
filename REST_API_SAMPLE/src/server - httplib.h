#ifndef SERVER_H
#define SERVER_H

#include <httplib.h>
#include <string>
#include <unordered_map>
#include <nlohmann/json.hpp>
#include <fstream>

class RESTServer {
public:
    RESTServer();
    void start(const std::string& address, int port);

private:
    void setupRoutes();
    void loadConfigFromFile();
    void saveConfigToFile();

    std::unordered_map<std::string, nlohmann::json> configStorage;
    httplib::Server svr;
};

#endif  // SERVER_H
