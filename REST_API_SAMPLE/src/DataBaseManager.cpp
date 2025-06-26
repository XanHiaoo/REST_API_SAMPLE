#include "DataBaseManager.h"
const char* DataBaseManager::sqlName = "RestFulApiTest.sqlite3";
DataBaseManager* DataBaseManager::m_SingleInstance = nullptr;
std::mutex DataBaseManager::m_Mutex;

DataBaseManager* DataBaseManager::GetInstance()
{

    if (m_SingleInstance == nullptr) {
        std::unique_lock<std::mutex> lock(m_Mutex);
        if (m_SingleInstance == nullptr) {
            volatile auto temp = new (std::nothrow) DataBaseManager();
            m_SingleInstance = temp;
        }
    }
    return m_SingleInstance;
}
