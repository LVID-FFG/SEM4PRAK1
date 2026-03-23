#pragma once
#include <vector>
#include "../models/AccessEvent.h"
#include "../../data/Database.h"
using namespace std;
//Сервис формирования отчетов - формирует отчёты и расчитывает статистику
class ReportService {
private:
    Database& db;
    
public:
    ReportService() : db(Database::getInstance()) {}
    

    vector<AccessEvent> getAllEvents() {
        return db.getAllEvents();
    }
    
    //Структура для хранения статистических данных
    struct Statistics {
        int totalAttempts;
        int grantedAttempts;
        int deniedAttempts;
        double successRate;
    };
    
    //Расчет статистики по всем событиям
    Statistics getStatistics() {
        auto events = db.getAllEvents();
        Statistics stats = {0, 0, 0, 0.0};
        stats.totalAttempts = events.size();
        
        for (const auto& event : events) {
            if (event.isGranted()) {
                stats.grantedAttempts++;
            } else {
                stats.deniedAttempts++;
            }
        }
        
        if (stats.totalAttempts > 0) {
            stats.successRate = (double)stats.grantedAttempts / stats.totalAttempts * 100;
        }
        
        return stats;
    }
};