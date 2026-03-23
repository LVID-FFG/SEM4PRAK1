#include <iostream>
#include <vector>
#include "../business/services/EmployeeManagementService.h"
#include "../business/services/AccessPointManagementService.h"
#include "../business/services/ReportService.h"
#include "../business/services/AuthService.h"
using namespace std;

/*  Веб-приложение для охраны и администратора состоит из:
    Модуль аутентификации: вызов сервиса аутентификации
    Модуль мониторинга: отображение событий в реальном времени
    Модуль управления сотрудниками: управление сотрудниками
    Модуль управления доступом: назначение прав
    Модуль отчетов: формирование статистики
*/

class AdminWebUI {
private:
    EmployeeManagementService& employeeService;
    AccessPointManagementService& pointService;
    ReportService reportService;
    AuthService& authService;
    bool authenticated;
    string currentUser;
    //Модуль аутентификации
    bool login() {
        cout << endl << "[АДМИН-ПАНЕЛЬ] Вход в систему" << endl;
        cout << "Логин: ";
        string login;
        cin >> login;
        
        cout << "Пароль: ";
        string password;
        cin >> password;
        
        bool success = authService.authenticate(login, password);
        
        if (success) {
            cout << "[АДМИН-ПАНЕЛЬ] Аутентификация успешна. Добро пожаловать!" << endl;
            authenticated = true;
            currentUser = login;
            return true;
        } else {
            cout << "[АДМИН-ПАНЕЛЬ] Ошибка аутентификации." << endl;
            authenticated = false;
            return false;
        }
    }
    //Модуль мониторинга
    void showMonitoring() {
        cout << endl << "МОНИТОРИНГ ПРОХОДОВ" << endl;
        
        auto events = reportService.getAllEvents();
        if (events.empty()) {
            cout << "Нет событий." << endl;
        } else {
            for (const auto& event : events) {
                cout << "[" << event.getTimestamp() << "] "
                          << "Сотрудник: " << event.getEmployeeId()
                          << " | Точка: " << event.getAccessPointId()
                          << " | Результат: ДОСТУП " << (event.isGranted() ? "РАЗРЕШЕН" : "ЗАПРЕЩЕН");
                if (!event.getReason().empty()) {
                    cout << " | Причина: " << event.getReason();
                }
                cout << endl << "";
            }
        }
        cout << endl << "Нажмите Enter для продолжения...";
        cin.ignore();
        cin.get();
    }
    //Модуль управления сотрудниками
    void manageEmployees() {
        while (true) {
            cout << endl << "УПРАВЛЕНИЕ СОТРУДНИКАМИ" << endl;
            cout << "1. Добавить сотрудника" << endl;
            cout << "2. Показать всех сотрудников" << endl;
            cout << "3. Заблокировать/разблокировать сотрудника" << endl;
            cout << "4. Назад" << endl;
            cout << "Выберите: ";
            
            int choice;
            cin >> choice;
            
            switch (choice) {
                case 1: addEmployee(); break;
                case 2: showAllEmployees(); break;
                case 3: toggleEmployeeStatus(); break;
                case 4: return;
                default: cout << "Неверный выбор." << endl;
            }
        }
    }
    
    void addEmployee() {
        cout << endl << "ДОБАВЛЕНИЕ СОТРУДНИКА" << endl;
        
        string id, name, position, department;
        
        cout << "ID сотрудника: ";
        cin >> id;
        
        cout << "ФИО: ";
        cin.ignore();
        getline(cin, name);
        
        cout << "Должность: ";
        getline(cin, position);
        
        cout << "Отдел: ";
        getline(cin, department);
        
        if (employeeService.addEmployee(id, name, position, department)) {
            cout << endl << "[СИСТЕМА] Сотрудник успешно добавлен." << endl;
        } else {
            cout << endl << "[СИСТЕМА] Ошибка: сотрудник с таким ID уже существует." << endl;
        }
    }
    
    void showAllEmployees() {
        cout << endl << "СПИСОК СОТРУДНИКОВ" << endl;
        auto employees = employeeService.getAllEmployees();
        
        if (employees.empty()) {
            cout << "Нет добавленных сотрудников." << endl;
            return;
        }
        
        for (const auto& emp : employees) {
            cout << "ID: " << emp.getId() 
                      << " | " << emp.getName()
                      << " | " << emp.getPosition()
                      << " | " << emp.getDepartment()
                      << " | Статус: " << (emp.isActive() ? "Активен" : "Заблокирован")
                      << endl << "";
        }
        cout << endl << "Нажмите Enter для продолжения...";
        cin.ignore();
        cin.get();
    }
    
    void toggleEmployeeStatus() {
        cout << endl << "ИЗМЕНЕНИЕ СТАТУСА СОТРУДНИКА" << endl;
        string id;
        cout << "Введите ID сотрудника: ";
        cin >> id;
        
        if (employeeService.toggleEmployeeStatus(id)) {
            cout << "[СИСТЕМА] Статус сотрудника изменен." << endl;
        } else {
            cout << "[СИСТЕМА] Сотрудник не найден." << endl;
        }
    }
    //Модуль управления точками доступа
    void manageAccessPoints() {
        while (true) {
            cout << endl << "УПРАВЛЕНИЕ ТОЧКАМИ ДОСТУПА" << endl;
            cout << "1. Добавить точку доступа" << endl;
            cout << "2. Показать все точки доступа" << endl;
            cout << "3. Включить/отключить точку доступа" << endl;
            cout << "4. Назад" << endl;
            cout << "Выберите: ";
            
            int choice;
            cin >> choice;
            
            switch (choice) {
                case 1: addAccessPoint(); break;
                case 2: showAllAccessPoints(); break;
                case 3: toggleAccessPointStatus(); break;
                case 4: return;
                default: cout << "Неверный выбор." << endl;
            }
        }
    }
    
    void addAccessPoint() {
        cout << endl << "ДОБАВЛЕНИЕ ТОЧКИ ДОСТУПА" << endl;
        
        string id, name, location;
        int type;
        
        cout << "ID точки: ";
        cin >> id;
        
        cout << "Название: ";
        cin.ignore();
        getline(cin, name);
        
        cout << "Расположение: ";
        getline(cin, location);
        
        cout << "Тип (1-турникет, 2-дверь, 3-шлагбаум): ";
        cin >> type;
        
        AccessPoint::Type pointType;
        switch(type) {
            case 1: pointType = AccessPoint::Type::TURN; break;
            case 2: pointType = AccessPoint::Type::DOOR; break;
            case 3: pointType = AccessPoint::Type::BARRIER; break;
            default: pointType = AccessPoint::Type::TURN;
        }
        
        if (pointService.addAccessPoint(id, name, location, pointType)) {
            cout << endl << "[СИСТЕМА] Точка доступа успешно добавлена." << endl;
        } else {
            cout << endl << "[СИСТЕМА] Ошибка: точка с таким ID уже существует." << endl;
        }
    }
    
    void showAllAccessPoints() {
        cout << endl << "СПИСОК ТОЧЕК ДОСТУПА" << endl;
        auto points = pointService.getAllAccessPoints();
        
        if (points.empty()) {
            cout << "Нет добавленных точек доступа." << endl;
            return;
        }
        
        for (const auto& point : points) {
            cout << "ID: " << point.getId() 
                      << " | " << point.getName()
                      << " | " << point.getLocation()
                      << " | Тип: " << point.getTypeString()
                      << " | Статус: " << (point.isActive() ? "Работает" : "На обслуживании")
                      << endl << "";
        }
        cout << endl << "Нажмите Enter для продолжения...";
        cin.ignore();
        cin.get();
    }
    
    void toggleAccessPointStatus() {
        cout << endl << "ИЗМЕНЕНИЕ СТАТУСА ТОЧКИ ДОСТУПА" << endl;
        string id;
        cout << "Введите ID точки: ";
        cin >> id;
        
        if (pointService.toggleAccessPointStatus(id)) {
            cout << "[СИСТЕМА] Статус точки доступа изменен." << endl;
        } else {
            cout << "[СИСТЕМА] Точка доступа не найдена." << endl;
        }
    }
    //Модуль отчетов
    void showReports() {
        cout << endl << "ФОРМИРОВАНИЕ ОТЧЕТОВ" << endl;
        
        auto stats = reportService.getStatistics();
        
        cout << endl << "СТАТИСТИКА ПРОХОДОВ:" << endl;
        cout << "Всего попыток прохода: " << stats.totalAttempts << endl << "";
        cout << "Успешных проходов: " << stats.grantedAttempts << endl << "";
        cout << "Запрещенных попыток: " << stats.deniedAttempts << endl << "";
        cout << "Процент успешных: " << stats.successRate << "%" << endl;
        
        cout << endl << "ПОСЛЕДНИЕ СОБЫТИЯ (последние 5):" << endl;
        auto events = reportService.getAllEvents();
        int count = 0;
        for (auto it = events.rbegin(); it != events.rend() && count < 5; ++it, ++count) {
            cout << "[" << it->getTimestamp() << "] " << it->getEmployeeId() << " -> " << it->getAccessPointId() << " : ДОСТУП " 
            << (it->isGranted() ? "РАЗРЕШЕН" : "ЗАПРЕЩЕН") << endl << "";
        }
        
        cout << endl << "Нажмите Enter для продолжения...";
        cin.ignore();
        cin.get();
    }
    
    void printAdminMenu() {
        cout << endl << "АДМИН МЕНЮ" << endl;
        cout << "1. Мониторинг проходов" << endl;
        cout << "2. Управление сотрудниками" << endl;
        cout << "3. Управление точками доступа" << endl;
        cout << "4. Отчеты и статистика" << endl;
        cout << "0. Выйти из режима администратора" << endl;
        cout << "Выберите: ";
    }
    
public:
    AdminWebUI() : employeeService(EmployeeManagementService::getInstance()),
                   pointService(AccessPointManagementService::getInstance()),
                   authService(AuthService::getInstance()),
                   authenticated(false) {}
    
    void run() {
        if (!login()) {
            cout << "Доступ запрещен. Возврат в главное меню." << endl;
            return;
        }
        
        while (authenticated) {
            printAdminMenu();
            
            int choice;
            cin >> choice;
            
            switch (choice) {
                case 1: showMonitoring(); break;
                case 2: manageEmployees(); break;
                case 3: manageAccessPoints(); break;
                case 4: showReports(); break;
                case 0: 
                    cout << "[АДМИН-ПАНЕЛЬ] Выход из системы." << endl;
                    authenticated = false;
                    return;
                default:
                    cout << "Неверный выбор." << endl;
            }
        }
    }
};