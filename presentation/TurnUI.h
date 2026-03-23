#pragma once
#include <iostream>
#include <vector>
#include "../business/services/AccessService.h"
#include "../business/services/AccessPointManagementService.h"
using namespace std;
/*  Программное обеспечение турникета состоит из:
    Модуль считывания данных: получение информации с пропуска
    Модуль формирования и отправки запроса: подготовка и отправка запроса на сервер
    Модуль принятия решения: анализ ответа и управление турникетом
 */
class TurnstileUI {
private:
    AccessDecisionService& decisionService;
    AccessPointManagementService& pointService;
    string turnstileId;
    vector<pair<string, string>> availableTurnstiles;
    
    void loadAvailableTurnstiles() {
        availableTurnstiles = pointService.getTurnstilesList();
    }
    
    bool selectTurnstile() {
        loadAvailableTurnstiles();
        
        cout << endl << "[ТУРНИКЕТ] Доступные турникеты:" << endl;
        for (size_t i = 0; i < availableTurnstiles.size(); i++) {
            cout << "  " << (i + 1) << ". " << availableTurnstiles[i].second 
                 << " (ID: " << availableTurnstiles[i].first << ")" << endl;
        }
        
        cout << "Выберите номер турникета (0 - отмена): ";
        int choice;
        cin >> choice;
        
        if (choice == 0) {
            return false;
        }
        
        if (choice < 1 || choice > (int)availableTurnstiles.size()) {
            cout << "[ТУРНИКЕТ] Неверный выбор." << endl;
            return false;
        }
        
        turnstileId = availableTurnstiles[choice - 1].first;
        cout << "[ТУРНИКЕТ] Выбран турникет: " << availableTurnstiles[choice - 1].second 
             << " (ID: " << turnstileId << ")" << endl;
        return true;
    }
    //Модуль считывания данных
    string readCredential() {
        cout << endl << "[ТУРНИКЕТ] Считайте пропуск..." << endl;
        cout << "Введите ID сотрудника (или 'cancel' для отмены, 'change' для смены турникета): ";
        
        string input;
        cin >> input;
        
        if (input == "cancel") {
            return "";
        }
        if (input == "change") {
            return "CHANGE_TURN";
        }
        
        cout << "[ТУРНИКЕТ] Считан идентификатор: " << input << endl << "";
        return input;
    }
    //Модуль формирования и отправки запроса
    bool sendRequestToServer(const string& employeeId) {
        cout << endl << "";
        cout << "[ТУРНИКЕТ] Формирование запроса..." << endl;
        cout << "[ТУРНИКЕТ] Установка HTTPS-соединения с сервером..." << endl;
        cout << "[ТУРНИКЕТ] Отправка запроса..." << endl;
        cout << "[ТУРНИКЕТ] Ожидание ответа...\n" << endl;
        
        bool result = decisionService.checkAccess(employeeId, turnstileId);
        
        cout << "[ТУРНИКЕТ] Получен ответ от сервера" << endl;
        cout << "[ТУРНИКЕТ] Закрытие соединения" << endl;
        cout << "[ТУРНИКЕТ] Ответ сервера: " << (result ? "GRANTED" : "DENIED") << endl << "";
        return result;
    }
    //Модуль принятия решения и управления исполнительным механизмом
    void executeDecision(bool granted) {
        if (granted) {
            cout << "[ТУРНИКЕТ] Решение: ОТКРЫТЬ проход" << endl;
            cout << "[ТУРНИКЕТ] Отправка команды исполнительному механизму..." << endl;
            cout << "[ТУРНИКЕТ] Исполнительный механизм: открытие турникета" << endl;
            cout << "[ТУРНИКЕТ] ТУРНИКЕТ ОТКРЫТ" << endl;
        } else {
            cout << "[ТУРНИКЕТ] Решение: ЗАКРЫТЬ проход" << endl;
            cout << "[ТУРНИКЕТ] Исполнительный механизм: турникет остается закрытым" << endl;
            cout << "[ТУРНИКЕТ] ДОСТУП ЗАПРЕЩЕН. Обратитесь к охране." << endl;
        }
    }

public:
    TurnstileUI() : decisionService(AccessDecisionService::getInstance()),
                    pointService(AccessPointManagementService::getInstance()),
                    turnstileId("") {}
    
    void run() {
        cout << "   РЕЖИМ ТУРНИКЕТА" << endl;
        
        if (!selectTurnstile()) {
            cout << "[ТУРНИКЕТ] Возврат в главное меню." << endl;
            return;
        }
        
        while (true) {
            string employeeId = readCredential();
            if (employeeId.empty()) {
                cout << "[ТУРНИКЕТ] Операция отменена. Возврат в главное меню." << endl;
                return;
            }
            if (employeeId == "CHANGE_TURN") {
                if (!selectTurnstile()) {
                    return;
                }
                continue;
            }
            
            bool granted = sendRequestToServer(employeeId);
            executeDecision(granted);
            
            cout << endl << "[ТУРНИКЕТ] Готов к следующему сотруднику" << endl;
        }
    }
};