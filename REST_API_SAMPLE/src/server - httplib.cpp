#include "server.h"
#include <iostream>

using json = nlohmann::json;

const std::string CONFIG_FILE = "config.json";  // �����ļ�·��

RESTServer::RESTServer()
{
    loadConfigFromFile();  // ���� JSON ����
    setupRoutes();
}

void RESTServer::loadConfigFromFile()
{
    std::ifstream file(CONFIG_FILE);
    if (file) {
        try {
            file >> configStorage;
            std::cout << "Loaded config from " << CONFIG_FILE << std::endl;
        }
        catch (const std::exception& e) {
            std::cerr << "Error loading config: " << e.what() << std::endl;
        }
    }
    else {
        std::cerr << "Config file not found, using default settings." << std::endl;

        // ����ļ������ڣ�ʹ��Ĭ������
        configStorage["BankWidthDetect"] = {
            {"description", "This is a Bank Width Detection task."},
            {"status", "success"},
            {
                "results", {
                    {1, {{"snCode", "SN001"}, {"result", "Passed"}, {"score", 98.7}}},
                    {2, {{"snCode", "SN002"}, {"result", "Failed"}, {"score", 45.3}}}
                }
            }
        };

        configStorage["ImageQualityDetect"] = {
            {"description", "This is an Image Quality Detection task."},
            {"status", "success"},
            {
                "results", {
                    {3, {{"snCode", "SN003"}, {"result", "Passed"}, {"score", 91.2}}}
                }
            }
        };

        configStorage["PixelAnalysis"] = {
            {"description", "This is a Pixel Analysis task."},
            {"status", "running"},
            {"results", json::object()}  // Ŀǰ�޽��
        };

        configStorage["SystemVersion"] = "1.0.0";
        configStorage["MaxClients"] = 100;
    }
}

void RESTServer::saveConfigToFile()
{
    std::ofstream file(CONFIG_FILE);
    if (file) {
        file << configStorage.dump(4);  // ��ʽ��д��
        std::cout << "Config saved to " << CONFIG_FILE << std::endl;
    }
    else {
        std::cerr << "Failed to save config to " << CONFIG_FILE << std::endl;
    }
}

void RESTServer::setupRoutes()
{
    // ��ȡ��������
    svr.Get("/Config", [this](const httplib::Request&, httplib::Response & res) {
        json responseJson;
        for (const auto&[key, value] : configStorage) {
            responseJson[key] = value;
        }
        res.set_content(responseJson.dump(), "application/json");
    });

    // �޸����ò�����
    svr.Post("/Config", [this](const httplib::Request & req, httplib::Response & res) {
        try {
            json newConfig = json::parse(req.body);
            configStorage = newConfig;
            saveConfigToFile();  // ���浽�ļ�
            res.set_content(R"({"status": "success"})", "application/json");
        }
        catch (const std::exception& e) {
            res.status = 400;
            res.set_content(R"({"error": "Invalid JSON"})", "application/json");
        }
    });
}

void RESTServer::start(const std::string& address, int port)
{
    std::cout << "Starting server at " << address << ":" << port << std::endl;
    svr.listen(address.c_str(), port);
}
