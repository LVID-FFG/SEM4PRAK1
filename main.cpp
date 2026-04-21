#include <iostream>
#include <string>
#include <limits>
#include "presentation/AdminUI.h"
#include "presentation/TurnUI.h"
using namespace std;

/**
 * Главное консольное меню — позволяет выбирать, от имени какого пользователя
 * работает оператор: сотрудник у турникета, охранник/администратор.
 * 
 * Это  слой представления системы, а просто способ управления моделью.
 */
class MainMenu {
private:
    TurnstileUI turnstileUI;
    AdminWebUI adminUI;
    
    void printMainMenu() {
        cout << endl << "--- ВЫБОР ИНТЕРФЕЙСА ---" << endl;
        cout << "1. Режим турникета (сотрудник проходит)" << endl;
        cout << "2. Режим администратора (управление системой)" << endl;
        cout << "0. Выход" << endl;
        cout << "Выберите: ";
    }
    
public:
    void run() {
        while (true) {
            printMainMenu();
            
            int choice;
            cin >> choice;
            
            switch (choice) {
                case 1:
                    turnstileUI.run();
                    break;
                case 2:
                    adminUI.run();
                    break;
                case 0:
                    cout << "Выход из системы..." << endl;
                    return;
                default:
                    cout << "Неверный выбор." << endl;
            }
        }
    }
};

int main() {
    MainMenu menu;
    menu.run();
    return 0;
}