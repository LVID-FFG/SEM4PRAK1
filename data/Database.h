#pragma once
#include <iostream>
#include <optional>
#include "json.hpp"
#include "../business/models/Employee.h"
#include "../business/models/AccessPoint.h"
#include "../business/models/AccessEvent.h"
#include "../business/models/User.h"

using namespace std;
using json = nlohmann::json;

class Database {
private:
    json employeesData;
    json accessPointsData;
    json eventsData;
    json usersData;
    
    Database() {
        cout << "[БАЗА ДАННЫХ] Инициализация..." << endl;
        
        employeesData = json::array();
        Employee emp1("EMP001", "Иванов Иван Иванович", "Инженер", "Цех №1");
        employeesData.push_back({
            {"id", emp1.getId()},
            {"name", emp1.getName()},
            {"position", emp1.getPosition()},
            {"department", emp1.getDepartment()},
            {"active", emp1.isActive()},
            {"passId", emp1.getPassId()}
        });
        
        Employee emp2("EMP002", "Петров Петр Петрович", "Начальник смены", "Цех №2");
        employeesData.push_back({
            {"id", emp2.getId()},
            {"name", emp2.getName()},
            {"position", emp2.getPosition()},
            {"department", emp2.getDepartment()},
            {"active", emp2.isActive()},
            {"passId", emp2.getPassId()}
        });
        
        Employee emp3("EMP003", "Сидорова Анна Сергеевна", "Бухгалтер", "Бухгалтерия");
        employeesData.push_back({
            {"id", emp3.getId()},
            {"name", emp3.getName()},
            {"position", emp3.getPosition()},
            {"department", emp3.getDepartment()},
            {"active", emp3.isActive()},
            {"passId", emp3.getPassId()}
        });
        
        accessPointsData = json::array();
        AccessPoint ap1("TURN_01", "Центральная проходная", "Главный вход", AccessPoint::Type::TURN);
        accessPointsData.push_back({
            {"id", ap1.getId()},
            {"name", ap1.getName()},
            {"location", ap1.getLocation()},
            {"type", static_cast<int>(ap1.getType())},
            {"active", ap1.isActive()}
        });
        
        AccessPoint ap2("TURN_02", "Западная проходная", "Западный вход", AccessPoint::Type::TURN);
        accessPointsData.push_back({
            {"id", ap2.getId()},
            {"name", ap2.getName()},
            {"location", ap2.getLocation()},
            {"type", static_cast<int>(ap2.getType())},
            {"active", ap2.isActive()}
        });
        
        AccessPoint ap3("DOOR_01", "Вход в цех №1", "Цех №1, южный вход", AccessPoint::Type::DOOR);
        accessPointsData.push_back({
            {"id", ap3.getId()},
            {"name", ap3.getName()},
            {"location", ap3.getLocation()},
            {"type", static_cast<int>(ap3.getType())},
            {"active", ap3.isActive()}
        });
        
        eventsData = json::array();
        
        usersData = json::array();
        User admin("admin", "admin", "administrator");
        usersData.push_back({
            {"login", admin.getLogin()},
            {"password", admin.getPassword()},
            {"role", admin.getRole()},
            {"active", admin.isActive()}
        });
        
        cout << "[БАЗА ДАННЫХ] Загружено сотрудников: " << employeesData.size() << endl;
        cout << "[БАЗА ДАННЫХ] Загружено точек доступа: " << accessPointsData.size() << endl;
        cout << "[БАЗА ДАННЫХ] Загружено пользователей: " << usersData.size() << endl;
    }
    
    Database(const Database&) = delete;
    Database& operator=(const Database&) = delete;
    
public:
    static Database& getInstance() {
        static Database instance;
        return instance;
    }
    
    bool saveEmployee(const Employee& employee) {
        for (const auto& item : employeesData) {
            if (item["id"] == employee.getId()) {
                cout << "[БАЗА ДАННЫХ] Ошибка: сотрудник с таким ID уже существует" << endl;
                return false;
            }
        }
        employeesData.push_back({
            {"id", employee.getId()},
            {"name", employee.getName()},
            {"position", employee.getPosition()},
            {"department", employee.getDepartment()},
            {"active", employee.isActive()},
            {"passId", employee.getPassId()}
        });
        cout << "[БАЗА ДАННЫХ] Запись добавлена. Всего сотрудников: " << employeesData.size() << endl;
        return true;
    }
    
    optional<Employee> findEmployeeById(const string& id) {
        for (const auto& item : employeesData) {
            if (item["id"] == id) {
                Employee emp(item["id"], item["name"], item["position"], item["department"]);
                emp.setActive(item["active"]);
                emp.setPassId(item["passId"]);
                cout << "[БАЗА ДАННЫХ] Запись найдена: " << emp.getName() << endl;
                return emp;
            }
        }
        cout << "[БАЗА ДАННЫХ] Запись не найдена" << endl;
        return nullopt;
    }
    
    vector<Employee> getAllEmployees() {
        vector<Employee> result;
        for (const auto& item : employeesData) {
            Employee emp(item["id"], item["name"], item["position"], item["department"]);
            emp.setActive(item["active"]);
            emp.setPassId(item["passId"]);
            result.push_back(emp);
        }
        cout << "[БАЗА ДАННЫХ] Возвращено " << result.size() << " записей" << endl;
        return result;
    }
    
    bool toggleEmployeeStatus(const string& id) {
        for (auto& item : employeesData) {
            if (item["id"] == id) {
                bool newStatus = !item["active"].get<bool>();
                item["active"] = newStatus;
                cout << "[БАЗА ДАННЫХ] Статус изменен на: " << (newStatus ? "Активен" : "Заблокирован") << endl;
                return true;
            }
        }
        cout << "[БАЗА ДАННЫХ] Сотрудник не найден" << endl;
        return false;
    }
    
    bool saveAccessPoint(const AccessPoint& point) {
        for (const auto& item : accessPointsData) {
            if (item["id"] == point.getId()) {
                cout << "[БАЗА ДАННЫХ] Ошибка: точка доступа с таким ID уже существует" << endl;
                return false;
            }
        }
        accessPointsData.push_back({
            {"id", point.getId()},
            {"name", point.getName()},
            {"location", point.getLocation()},
            {"type", static_cast<int>(point.getType())},
            {"active", point.isActive()}
        });
        cout << "[БАЗА ДАННЫХ] Запись добавлена. Всего точек: " << accessPointsData.size() << endl;
        return true;
    }
    
    optional<AccessPoint> findAccessPointById(const string& id) {
        for (const auto& item : accessPointsData) {
            if (item["id"] == id) {
                AccessPoint::Type type;
                switch(item["type"].get<int>()) {
                    case 0: type = AccessPoint::Type::TURN; break;
                    case 1: type = AccessPoint::Type::DOOR; break;
                    case 2: type = AccessPoint::Type::BARRIER; break;
                    default: type = AccessPoint::Type::TURN;
                }
                AccessPoint point(item["id"], item["name"], item["location"], type);
                point.setActive(item["active"]);
                cout << "[БАЗА ДАННЫХ] Запись найдена: " << point.getName() << endl;
                return point;
            }
        }
        cout << "[БАЗА ДАННЫХ] Запись не найдена" << endl;
        return nullopt;
    }
    
    vector<AccessPoint> getAllAccessPoints() {
        vector<AccessPoint> result;
        for (const auto& item : accessPointsData) {
            AccessPoint::Type type;
            switch(item["type"].get<int>()) {
                case 0: type = AccessPoint::Type::TURN; break;
                case 1: type = AccessPoint::Type::DOOR; break;
                case 2: type = AccessPoint::Type::BARRIER; break;
                default: type = AccessPoint::Type::TURN;
            }
            AccessPoint point(item["id"], item["name"], item["location"], type);
            point.setActive(item["active"]);
            result.push_back(point);
        }
        cout << "[БАЗА ДАННЫХ] Возвращено " << result.size() << " записей" << endl;
        return result;
    }
    
    bool toggleAccessPointStatus(const string& id) {
        for (auto& item : accessPointsData) {
            if (item["id"] == id) {
                bool newStatus = !item["active"].get<bool>();
                item["active"] = newStatus;
                cout << "[БАЗА ДАННЫХ] Статус изменен на: " << (newStatus ? "Работает" : "На обслуживании") << endl;
                return true;
            }
        }
        cout << "[БАЗА ДАННЫХ] Точка доступа не найдена" << endl;
        return false;
    }
    
    void saveEvent(const AccessEvent& event) {
        eventsData.push_back({
            {"id", event.getId()},
            {"employeeId", event.getEmployeeId()},
            {"accessPointId", event.getAccessPointId()},
            {"timestamp", event.getTimestamp()},
            {"granted", event.isGranted()},
            {"reason", event.getReason()}
        });
        cout << "[БАЗА ДАННЫХ] Событие сохранено. Всего записей: " << eventsData.size() << endl;
    }
    
    vector<AccessEvent> getAllEvents() {
        vector<AccessEvent> result;
        for (const auto& item : eventsData) {
            AccessEvent event(item["employeeId"], item["accessPointId"], item["granted"], item["reason"]);
            result.push_back(event);
        }
        cout << "[БАЗА ДАННЫХ] Возвращено " << result.size() << " записей" << endl;
        return result;
    }
    
    optional<User> findUserByLogin(const string& login) {
        for (const auto& item : usersData) {
            if (item["login"] == login) {
                User user(item["login"], item["password"], item["role"]);
                user.setActive(item["active"]);
                cout << "[БАЗА ДАННЫХ] Пользователь найден: " << login << endl;
                return user;
            }
        }
        cout << "[БАЗА ДАННЫХ] Пользователь не найден: " << login << endl;
        return nullopt;
    }
    
    bool saveUser(const User& user) {
        for (const auto& item : usersData) {
            if (item["login"] == user.getLogin()) {
                cout << "[БАЗА ДАННЫХ] Ошибка: пользователь с таким логином уже существует" << endl;
                return false;
            }
        }
        usersData.push_back({
            {"login", user.getLogin()},
            {"password", user.getPassword()},
            {"role", user.getRole()},
            {"active", user.isActive()}
        });
        cout << "[БАЗА ДАННЫХ] Пользователь добавлен. Всего пользователей: " << usersData.size() << endl;
        return true;
    }
};