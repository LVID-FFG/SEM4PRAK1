#pragma once
#include <unordered_map>
#include <vector>
#include <optional>
#include <iostream>
#include "../business/models/Employee.h"
#include "../business/models/AccessPoint.h"
#include "../business/models/AccessEvent.h"
#include "../business/models/User.h"
using namespace std;

class Database {
private:
    unordered_map<string, Employee> employees;
    unordered_map<string, AccessPoint> accessPoints;
    vector<AccessEvent> events;
    unordered_map<string, User> users;
    
    Database() { // изначальные тестовые данные в БД
        cout << "[БАЗА ДАННЫХ] Инициализация..." << endl;
        
        employees["EMP001"] = Employee("EMP001", "Иванов Иван Иванович", "Инженер", "Цех №1");
        employees["EMP002"] = Employee("EMP002", "Петров Петр Петрович", "Начальник смены", "Цех №2");
        employees["EMP003"] = Employee("EMP003", "Сидорова Анна Сергеевна", "Бухгалтер", "Бухгалтерия");
        
        accessPoints["TURN_01"] = AccessPoint("TURN_01", "Центральная проходная", "Главный вход", AccessPoint::Type::TURN);
        accessPoints["TURN_02"] = AccessPoint("TURN_02", "Западная проходная", "Западный вход", AccessPoint::Type::TURN);
        accessPoints["DOOR_01"] = AccessPoint("DOOR_01", "Вход в цех №1", "Цех №1, южный вход", AccessPoint::Type::DOOR);
        
        users["admin"] = User("admin", "admin", "administrator");
        
        cout << "[БАЗА ДАННЫХ] Загружено сотрудников: " << employees.size() << endl;
        cout << "[БАЗА ДАННЫХ] Загружено точек доступа: " << accessPoints.size() << endl;
        cout << "[БАЗА ДАННЫХ] Загружено пользователей: " << users.size() << endl;
    }
    
    Database(const Database&) = delete;
    Database& operator=(const Database&) = delete;
    
public:
    static Database& getInstance() {
        static Database instance;
        return instance;
    }
    
    bool saveEmployee(const Employee& employee) {
        if (employees.find(employee.getId()) != employees.end()) {
            cout << "[БАЗА ДАННЫХ] Ошибка: сотрудник с таким ID уже существует" << endl;
            return false;
        }
        employees[employee.getId()] = employee;
        cout << "[БАЗА ДАННЫХ] Запись добавлена. Всего сотрудников: " << employees.size() << endl;
        return true;
    }
    
    optional<Employee> findEmployeeById(const string& id) {
        auto it = employees.find(id);
        if (it != employees.end()) {
            cout << "[БАЗА ДАННЫХ] Запись найдена: " << it->second.getName() << endl;
            return it->second;
        }
        cout << "[БАЗА ДАННЫХ] Запись не найдена" << endl;
        return nullopt;
    }
    
    vector<Employee> getAllEmployees() {
        vector<Employee> result;
        for (const auto& pair : employees) {
            result.push_back(pair.second);
        }
        cout << "[БАЗА ДАННЫХ] Возвращено " << result.size() << " записей" << endl;
        return result;
    }
    
    bool toggleEmployeeStatus(const string& id) {
        auto it = employees.find(id);
        if (it == employees.end()) {
            cout << "[БАЗА ДАННЫХ] Сотрудник не найден" << endl;
            return false;
        }
        bool newStatus = !it->second.isActive();
        it->second.setActive(newStatus);
        cout << "[БАЗА ДАННЫХ] Статус изменен на: " << (newStatus ? "Активен" : "Заблокирован") << endl;
        return true;
    }
    
    bool saveAccessPoint(const AccessPoint& point) {
        if (accessPoints.find(point.getId()) != accessPoints.end()) {
            cout << "[БАЗА ДАННЫХ] Ошибка: точка доступа с таким ID уже существует" << endl;
            return false;
        }
        accessPoints[point.getId()] = point;
        cout << "[БАЗА ДАННЫХ] Запись добавлена. Всего точек: " << accessPoints.size() << endl;
        return true;
    }
    
    optional<AccessPoint> findAccessPointById(const string& id) {
        auto it = accessPoints.find(id);
        if (it != accessPoints.end()) {
            cout << "[БАЗА ДАННЫХ] Запись найдена: " << it->second.getName() << endl;
            return it->second;
        }
        cout << "[БАЗА ДАННЫХ] Запись не найдена" << endl;
        return nullopt;
    }
    
    vector<AccessPoint> getAllAccessPoints() {
        vector<AccessPoint> result;
        for (const auto& pair : accessPoints) {
            result.push_back(pair.second);
        }
        cout << "[БАЗА ДАННЫХ] Возвращено " << result.size() << " записей" << endl;
        return result;
    }
    
    bool toggleAccessPointStatus(const string& id) {
        auto it = accessPoints.find(id);
        if (it == accessPoints.end()) {
            cout << "[БАЗА ДАННЫХ] Точка доступа не найдена" << endl;
            return false;
        }
        bool newStatus = !it->second.isActive();
        it->second.setActive(newStatus);
        cout << "[БАЗА ДАННЫХ] Статус изменен на: " << (newStatus ? "Работает" : "На обслуживании") << endl;
        return true;
    }
    
    void saveEvent(const AccessEvent& event) {
        events.push_back(event);
        cout << "[БАЗА ДАННЫХ] Событие сохранено. Всего записей: " << events.size() << endl;
    }
    
    vector<AccessEvent> getAllEvents() {
        cout << "[БАЗА ДАННЫХ] Возвращено " << events.size() << " записей" << endl;
        return events;
    }
    
    optional<User> findUserByLogin(const string& login) {
        auto it = users.find(login);
        if (it != users.end()) {
            cout << "[БАЗА ДАННЫХ] Пользователь найден: " << login << endl;
            return it->second;
        }
        cout << "[БАЗА ДАННЫХ] Пользователь не найден: " << login << endl;
        return nullopt;
    }
    
    bool saveUser(const User& user) {
        if (users.find(user.getLogin()) != users.end()) {
            cout << "[БАЗА ДАННЫХ] Ошибка: пользователь с таким логином уже существует" << endl;
            return false;
        }
        users[user.getLogin()] = user;
        cout << "[БАЗА ДАННЫХ] Пользователь добавлен. Всего пользователей: " << users.size() << endl;
        return true;
    }
};