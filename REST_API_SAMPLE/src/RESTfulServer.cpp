#include "RESTfulServer.h"
#include "DataBaseManager.h"
#include "DetectResult.h"
#include <iostream>

const std::string CONFIG_FILE = "config.json";

RESTfulServer::RESTfulServer()
{
    loadConfigFromFile();
    setupRoutes();
}

void RESTfulServer::loadConfigFromFile()
{
    std::ifstream file(CONFIG_FILE);
    if (file) {
        try {
            nlohmann::json jsonData;
            file >> jsonData;

            for (auto&[key, value] : jsonData.items()) {
                configStorage[key] = value;
            }

            std::cout << "Loaded config from " << CONFIG_FILE << std::endl;
        }
        catch (const std::exception& e) {
            std::cerr << "Error loading config: " << e.what() << std::endl;
        }
    }
    else {
        //std::cerr << "Config file not found, using default settings." << std::endl;

        //// 如果文件不存在或加载失败，使用默认设置
        //configStorage["BankWidthDetect"] = {
        //    {"description", "This is a Bank Width Detection task."},
        //    {"status", "success"},
        //    {
        //        "results", {
        //            {1, {{"snCode", "SN001"}, {"result", "Passed"}, {"score", 98.7}}},
        //            {2, {{"snCode", "SN002"}, {"result", "Failed"}, {"score", 45.3}}}
        //        }
        //    }
        //};
    }
}

void RESTfulServer::saveConfigToFile()
{
    std::ofstream file(CONFIG_FILE);
    if (file) {
        nlohmann::json jsonConfig(configStorage);
        file << jsonConfig.dump(4);
        std::cout << "Config saved to " << CONFIG_FILE << std::endl;
    }
    else {
        std::cerr << "Failed to save config to " << CONFIG_FILE << std::endl;
    }
}

void RESTfulServer::setupRoutes()
{
    CROW_ROUTE(app, "/all").methods(crow::HTTPMethod::Get)
    ([this]() {
        return crow::response(200, nlohmann::json(configStorage).dump());
    });

    CROW_ROUTE(app, "/tasks").methods(crow::HTTPMethod::Get)
    ([this]() {
        nlohmann::json responseJson;
        for (const auto&[key, value] : configStorage) {
            if (key != "SystemVersion" && key != "MaxClients") {
                responseJson[key] = value;
            }
        }
        return crow::response(200, responseJson.dump());
    });

    CROW_ROUTE(app, "/tasks/<string>").methods(crow::HTTPMethod::Get)
    ([this](const std::string & taskName) {
        if (configStorage.find(taskName) != configStorage.end()) {
            return crow::response(200, configStorage[taskName].dump());
        }
        return crow::response(404, R"({"error": "Task not found"})");
    });

    CROW_ROUTE(app, "/tasks/<string>").methods(crow::HTTPMethod::Post)
    ([this](const crow::request & req, const std::string & taskName) {
        try {
            nlohmann::json newConfig = nlohmann::json::parse(req.body);
            if (configStorage.find(taskName) != configStorage.end()) {
                configStorage[taskName] = newConfig;
                saveConfigToFile();
                return crow::response(200, R"({"status": "success"})");
            }
            return crow::response(404, R"({"error": "Task not found"})");
        }
        catch (const std::exception&) {
            return crow::response(400, R"({"error": "Invalid JSON"})");
        }
    });

    CROW_ROUTE(app, "/tasks/<string>/results").methods(crow::HTTPMethod::Get)
    ([this](const std::string & taskName) {
        if (configStorage.find(taskName) != configStorage.end()) {
            DataBaseManager::GetInstance()->Connect(DataBaseManager::sqlName);
            auto vec = DataBaseManager::GetInstance()->sqlite.query<DBTestResult>();

            nlohmann::json resultArray = nlohmann::json::array();
            for (auto &[resultJsonStr, id] : vec) {
                try {
                    nlohmann::json resultJson = nlohmann::json::parse(resultJsonStr);
                    resultJson["id"] = id;
                    resultArray.push_back(resultJson);
                }
                catch (const nlohmann::json::exception& e) {
                    return crow::response(500, R"({"error": "JSON parsing failed"})"_json);
                }
            }

            // 直接返回 JSON
            return crow::response{ resultArray.dump() };
        }
        return crow::response(404, R"({"error": "No results found"})");
    });

    CROW_ROUTE(app, "/tasks/<string>/results/<string>").methods(crow::HTTPMethod::Get)
    ([this](const std::string & taskName, const std::string & snCode) {
        if (configStorage.find(taskName) != configStorage.end() && configStorage[taskName].contains("results")) {
            for (const auto& result : configStorage[taskName]["results"]) {
                if (result["snCode"] == snCode) {
                    return crow::response(200, result.dump());
                }
            }
        }
        return crow::response(404, R"({"error": "Result not found"})");
    });

    CROW_ROUTE(app, "/system").methods(crow::HTTPMethod::Get)
    ([this]() {
        nlohmann::json responseJson{
            {"SystemVersion", configStorage["SystemVersion"]},
            {"MaxClients", configStorage["MaxClients"]}
        };
        return crow::response(200, responseJson.dump());
    });

    CROW_ROUTE(app, "/system").methods(crow::HTTPMethod::Post)
    ([this](const crow::request & req) {
        try {
            nlohmann::json newConfig = nlohmann::json::parse(req.body);
            if (newConfig.contains("SystemVersion")) {
                configStorage["SystemVersion"] = newConfig["SystemVersion"];
            }
            if (newConfig.contains("MaxClients")) {
                configStorage["MaxClients"] = newConfig["MaxClients"];
            }
            saveConfigToFile();
            return crow::response(200, R"({"status": "success"})");
        }
        catch (const std::exception&) {
            return crow::response(400, R"({"error": "Invalid JSON"})");
        }
    });
}

void RESTfulServer::start(int port)
{
    std::cout << "Starting Crow server on port " << port << std::endl;
    app.port(port).multithreaded().run();
}
