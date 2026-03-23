#pragma once
#include <vector>
#include <iostream>
#include "../models/Employee.h"
#include "../../data/Database.h"
using namespace std;
//Сервис управления сотрудниками
class EmployeeManagementService {
private:
    Database& db;
    
    EmployeeManagementService() : db(Database::getInstance()) {}
    
public:
    EmployeeManagementService(const EmployeeManagementService&) = delete;
    EmployeeManagementService& operator=(const EmployeeManagementService&) = delete;
    
    static EmployeeManagementService& getInstance() {
        static EmployeeManagementService instance;
        return instance;
    }
    
    bool addEmployee(const string& id, const string& name, const string& position, const string& department) {
        cout << endl << "[СЕРВЕР ПРИЛОЖЕНИЙ] Добавление сотрудника" << endl;
        Employee employee(id, name, position, department);
        bool result = db.saveEmployee(employee);
        if (result) {
            cout << "[СЕРВЕР ПРИЛОЖЕНИЙ] Сотрудник сохранен в БД" << endl;
        } else {
            cout << "[СЕРВЕР ПРИЛОЖЕНИЙ] Ошибка: сотрудник с таким ID уже существует" << endl;
        }
        return result;
    }
    
    vector<Employee> getAllEmployees() {
        cout << "[СЕРВЕР ПРИЛОЖЕНИЙ] Запрос списка сотрудников из БД" << endl;
        auto result = db.getAllEmployees();
        cout << "[СЕРВЕР ПРИЛОЖЕНИЙ] Получено " << result.size() << " записей" << endl;
        return result;
    }
    
    bool toggleEmployeeStatus(const string& id) {
        cout << "[СЕРВЕР ПРИЛОЖЕНИЙ] Изменение статуса сотрудника" << endl;
        return db.toggleEmployeeStatus(id);
    }
    
    optional<Employee> findEmployeeById(const string& id) {
        return db.findEmployeeById(id);
    }
};