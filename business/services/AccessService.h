#pragma once
#include <iostream>
#include "../models/Employee.h"
#include "../models/AccessPoint.h"
#include "../models/AccessEvent.h"
#include "../../data/Database.h"
using namespace std;
//Сервис управления доступом (принятия решения о доступе)
class AccessDecisionService {
private:
    Database& db;
    
    AccessDecisionService() : db(Database::getInstance()) {}
    
    bool evaluateAccess(const Employee& employee, const AccessPoint& point) {
        cout << "[СЕРВЕР ПРИЛОЖЕНИЙ] Выполнение проверки прав доступа..." << endl;
        cout << "[СЕРВЕР ПРИЛОЖЕНИЙ]   - Сотрудник: " << employee.getName() 
             << " (" << employee.getId() << ")" << endl;
        cout << "[СЕРВЕР ПРИЛОЖЕНИЙ]   - Должность: " << employee.getPosition() << endl << "";
        cout << "[СЕРВЕР ПРИЛОЖЕНИЙ]   - Отдел: " << employee.getDepartment() << endl << "";
        cout << "[СЕРВЕР ПРИЛОЖЕНИЙ]   - Точка доступа: " << point.getName() 
             << " (" << point.getId() << ")" << endl;
        cout << "[СЕРВЕР ПРИЛОЖЕНИЙ]   - Тип точки: " << point.getTypeString() << endl << "";
        
        cout << "[СЕРВЕР ПРИЛОЖЕНИЙ] Правило 1: проверка статуса сотрудника..." << endl;
        if (!employee.isActive()) {
            cout << "[СЕРВЕР ПРИЛОЖЕНИЙ]   Сотрудник неактивен" << endl;
            return false;
        }
        cout << "[СЕРВЕР ПРИЛОЖЕНИЙ]   Сотрудник активен" << endl;
        
        cout << "[СЕРВЕР ПРИЛОЖЕНИЙ] Правило 2: проверка статуса точки доступа..." << endl;
        if (!point.isActive()) {
            cout << "[СЕРВЕР ПРИЛОЖЕНИЙ]   Точка доступа на обслуживании" << endl;
            return false;
        }
        cout << "[СЕРВЕР ПРИЛОЖЕНИЙ]   Точка доступа активна" << endl;
        
        cout << "[СЕРВЕР ПРИЛОЖЕНИЙ] Все проверки пройдены успешно" << endl;
        return true;
    }

public:
    AccessDecisionService(const AccessDecisionService&) = delete;
    AccessDecisionService& operator=(const AccessDecisionService&) = delete;
    
    static AccessDecisionService& getInstance() {
        static AccessDecisionService instance;
        return instance;
    }
    
    bool checkAccess(const string& employeeId, const string& pointId) {
        cout << "[СЕРВЕР ПРИЛОЖЕНИЙ] ПОЛУЧЕН ЗАПРОС ОТ ТУРНИКЕТА" << endl;
        cout << "[СЕРВЕР ПРИЛОЖЕНИЙ] Параметры: employeeId=" << employeeId << ", pointId=" << pointId << endl << endl;
        
        cout << "[СЕРВЕР ПРИЛОЖЕНИЙ] Запрос данных о сотруднике..." << endl;
        auto employee = db.findEmployeeById(employeeId);
        
        cout << "[СЕРВЕР ПРИЛОЖЕНИЙ] Запрос данных о точке доступа..." << endl;
        auto point = db.findAccessPointById(pointId);
        
        if (!employee.has_value()) {
            cout << endl << "[СЕРВЕР ПРИЛОЖЕНИЙ] ОШИБКА: сотрудник не найден в БД" << endl;
            cout << "[СЕРВЕР ПРИЛОЖЕНИЙ] Сохранение события в журнал" << endl;
            AccessEvent event(employeeId, pointId, false, "Сотрудник не найден");
            db.saveEvent(event);
            return false;
        }
        
        if (!point.has_value()) {
            cout << endl << "[СЕРВЕР ПРИЛОЖЕНИЙ] ОШИБКА: точка доступа не найдена в БД" << endl;
            cout << "[СЕРВЕР ПРИЛОЖЕНИЙ] Сохранение события в журнал" << endl;
            AccessEvent event(employeeId, pointId, false, "Точка доступа не найдена");
            db.saveEvent(event);
            return false;
        }
        
        cout << endl << "[СЕРВЕР ПРИЛОЖЕНИЙ] Данные получены из слоя доступа:" << endl;
        cout << "[СЕРВЕР ПРИЛОЖЕНИЙ]   - Сотрудник: " << employee.value().getName() << endl << "";
        cout << "[СЕРВЕР ПРИЛОЖЕНИЙ]   - Точка: " << point.value().getName() << endl << "";
        
        bool granted = evaluateAccess(employee.value(), point.value());
        
        cout << endl << "[СЕРВЕР ПРИЛОЖЕНИЙ] Сохранение события в журнал" << endl;
        string reason = granted ? "" : "Недостаточно прав доступа";
        AccessEvent event(employeeId, pointId, granted, reason);
        db.saveEvent(event);
        cout << "[СЕРВЕР ПРИЛОЖЕНИЙ] Событие сохранено" << endl;
        
        cout << "[СЕРВЕР ПРИЛОЖЕНИЙ] ИТОГОВОЕ РЕШЕНИЕ: " 
             << (granted ? "ДОСТУП РАЗРЕШЕН" : "ДОСТУП ЗАПРЕЩЕН") << endl << "";
        
        return granted;
    }
};