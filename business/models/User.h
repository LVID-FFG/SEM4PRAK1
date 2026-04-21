#pragma once
using namespace std;

hash<string> hasher;

class User {
private:
    string login;
    size_t passwordHash;
    string role;
    bool active;

public:
    User() : active(true) {}
    
    User(const string& login, const string& password, const string& role) : login(login), passwordHash(hasher(password)), role(role), active(true) {}
    User(const string& login, size_t hash, const string& role, bool active) : login(login), passwordHash(hash), role(role), active(active) {}
    string getLogin() const { return login; }
    size_t getPassword() const { return passwordHash; }
    string getRole() const { return role; }
    bool isActive() const { return active; }
    
    void setPassword(const string newPassword) { passwordHash = hasher(newPassword); }
    void setActive(bool status) { active = status; }
};