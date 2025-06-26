#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H
#include "ormpp/dbng.hpp"
#include <ormpp/sqlite.hpp>
struct DBTestResult {
    std::string resultJsonStr;
    int id;
};
REGISTER_AUTO_KEY(DBTestResult, id)
REFLECTION_WITH_NAME(DBTestResult, "t_detect_result", id, resultJsonStr)

class DataBaseManager {
private:

    static DataBaseManager* m_SingleInstance;
    static std::mutex m_Mutex;
public:
    static const char* sqlName;

    ormpp::dbng<ormpp::sqlite> sqlite;

    static DataBaseManager* DataBaseManager::GetInstance();


    bool Connect(const char* Name)
    {
        return sqlite.connect(Name);
    }

    bool Disconnect()
    {
        return sqlite.disconnect();
    }

};
#endif  // DATABASEMANAGER_H
