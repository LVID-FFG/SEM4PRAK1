#pragma once
using namespace std;
class Employee {
private:
    string id;
    string name;
    string position;
    string department;
    bool active;
    string passId;  // номер пропуска

public:
    Employee() : active(true) {}
    
    Employee(const string& id, const string& name, const string& position, const string& department) : id(id), name(name)
                                                    ,position(position), department(department), active(true), passId(id) {}
    
    string getId() const { return id; }
    string getName() const { return name; }
    string getPosition() const { return position; }
    string getDepartment() const { return department; }
    bool isActive() const { return active; }
    string getPassId() const { return passId; }
    
    void setPosition(const string& newPosition) { position = newPosition; }
    void setDepartment(const string& newDepartment) { department = newDepartment; }
    void setActive(bool status) { active = status; }
    void setPassId(const string& newPassId) { passId = newPassId; }
};