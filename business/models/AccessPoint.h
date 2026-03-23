#pragma once
using namespace std;
class AccessPoint {
public:
    enum class Type {
        TURN,  // турникет
        DOOR,          // дверь
        BARRIER        // шлагбаум
    };

private:
    string id;
    string name;
    string location;
    Type type;
    bool active;

public:
    AccessPoint() : active(true), type(Type::TURN) {}
    
    AccessPoint(const string& id, const string& name, const string& location, Type type)
        : id(id), name(name), location(location), type(type), active(true) {}
    
    string getId() const { return id; }
    string getName() const { return name; }
    string getLocation() const { return location; }
    Type getType() const { return type; }
    bool isActive() const { return active; }
    
    void setActive(bool status) { active = status; }
    
    string getTypeString() const {
        switch(type) {
            case Type::TURN: return "Турникет";
            case Type::DOOR: return "Дверь";
            case Type::BARRIER: return "Шлагбаум";
            default: return "Неизвестно";
        }
    }
};