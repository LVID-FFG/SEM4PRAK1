#pragma once
#include "../../data/Database.h"
using namespace std;
//Сервис аутентификации
class AuthService {
private:
    Database& db;
    
    AuthService() : db(Database::getInstance()) {}
    
public:
    AuthService(const AuthService&) = delete;
    AuthService& operator=(const AuthService&) = delete;
    
    static AuthService& getInstance() {
        static AuthService instance;
        return instance;
    }
    
    bool authenticate(const string& login, const string& password) {
        cout << "[СЕРВЕР ПРИЛОЖЕНИЙ] Запрос аутентификации пользователя: " << login << endl;
        
        // Получение данных пользователя из БД
        auto user = db.findUserByLogin(login);
        
        if (!user.has_value()) {
            cout << "[СЕРВЕР ПРИЛОЖЕНИЙ] Пользователь не найден" << endl;
            return false;
        }
        
        // для простоты сравниваем с хранимым значением (без хэширования)
        bool valid = (user.value().getPassword() == password);
        
        if (valid) {
            cout << "[СЕРВЕР ПРИЛОЖЕНИЙ] Аутентификация успешна" << endl;
        } else {
            cout << "[СЕРВЕР ПРИЛОЖЕНИЙ] Неверный пароль" << endl;
        }
        
        return valid;
    }
};