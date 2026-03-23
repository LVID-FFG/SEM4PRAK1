#pragma once
#include <ctime>
#include <iomanip>

using namespace std;

class AccessEvent {
private:
    string id;
    string employeeId;
    string accessPointId;
    string timestamp;
    bool granted;
    string reason;

    static string getCurrentTimestamp() {
        auto now = time(nullptr);
        stringstream ss;
        ss << put_time(localtime(&now), "%Y-%m-%d %H:%M:%S");
        return ss.str();
    }

public:
    AccessEvent(const string& employeeId, const string& accessPointId, bool granted, const string& reason = "")
        : employeeId(employeeId), accessPointId(accessPointId), timestamp(getCurrentTimestamp()), granted(granted), reason(reason) {
            static int counter = 0;
            id = to_string(++counter);
        }
    
    string getId() const { return id; }
    string getEmployeeId() const { return employeeId; }
    string getAccessPointId() const { return accessPointId; }
    string getTimestamp() const { return timestamp; }
    bool isGranted() const { return granted; }
    string getReason() const { return reason; }
};