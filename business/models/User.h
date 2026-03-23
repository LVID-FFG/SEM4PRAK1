#pragma once
using namespace std;
class User {
private:
    string login;
    string passwordHash;  // для упрощения хранится сам пароль
    string role;
    bool active;

public:
    User() : active(true) {}
    
    User(const string& login, const string& password, const string& role) : login(login), passwordHash(password), role(role), active(true) {}
    
    string getLogin() const { return login; }
    string getPassword() const { return passwordHash; }
    string getRole() const { return role; }
    bool isActive() const { return active; }
    
    void setPassword(const string& newPassword) { passwordHash = newPassword; }
    void setActive(bool status) { active = status; }
};