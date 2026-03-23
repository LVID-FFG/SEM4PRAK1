#pragma once
#include <vector>
#include <iostream>
#include "../models/AccessPoint.h"
#include "../../data/Database.h"
using namespace std;
//Сервис управления точками доступа
class AccessPointManagementService {
private:
    Database& db;
    
    AccessPointManagementService() : db(Database::getInstance()) {}
    
public:
    AccessPointManagementService(const AccessPointManagementService&) = delete;
    AccessPointManagementService& operator=(const AccessPointManagementService&) = delete;
    
    static AccessPointManagementService& getInstance() {
        static AccessPointManagementService instance;
        return instance;
    }
    
    bool addAccessPoint(const string& id, const string& name,
                        const string& location, AccessPoint::Type type) {
        cout << endl << "[СЕРВЕР ПРИЛОЖЕНИЙ] Добавление точки доступа" << endl;
        AccessPoint point(id, name, location, type);
        bool result = db.saveAccessPoint(point);
        if (result) {
            cout << "[СЕРВЕР ПРИЛОЖЕНИЙ] Точка доступа сохранена в БД" << endl;
        } else {
            cout << "[СЕРВЕР ПРИЛОЖЕНИЙ] Ошибка: точка с таким ID уже существует" << endl;
        }
        return result;
    }
    
    vector<AccessPoint> getAllAccessPoints() {
        cout << "[СЕРВЕР ПРИЛОЖЕНИЙ] Запрос списка точек доступа из БД" << endl;
        auto result = db.getAllAccessPoints();
        cout << "[СЕРВЕР ПРИЛОЖЕНИЙ] Получено " << result.size() << " записей" << endl;
        return result;
    }
    
    bool toggleAccessPointStatus(const string& id) {
        cout << "[СЕРВЕР ПРИЛОЖЕНИЙ] Изменение статуса точки доступа" << endl;
        return db.toggleAccessPointStatus(id);
    }
    
    optional<AccessPoint> findAccessPointById(const string& id) {
        return db.findAccessPointById(id);
    }
    
    vector<pair<string, string>> getTurnstilesList() {
        vector<pair<string, string>> result;
        auto allPoints = getAllAccessPoints();
        for (const auto& point : allPoints) {
            if (point.getType() == AccessPoint::Type::TURN) {
                result.push_back({point.getId(), point.getName()});
            }
        }
        return result;
    }
};