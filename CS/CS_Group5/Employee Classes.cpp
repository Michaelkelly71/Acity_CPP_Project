#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <windows.h>
#include <memory>

using namespace std;

// Helper functions
int getConsoleWidth() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int width = 80;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    }
    return width;
}

void displayCentered(const string &text, bool moveCursorForInput = false) {
    int width = getConsoleWidth();
    int padding = (width - text.size()) / 2;
    cout << setw(padding) << "" << left << text;  // Left-align text after padding

    if (moveCursorForInput) {
        COORD cursorPosition;
        cursorPosition.X = padding + text.size();
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
            cursorPosition.Y = csbi.dwCursorPosition.Y;
        }
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPosition);
    } else {
        cout << endl;
    }
}

void gotoXY(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// Employee Classes
class Employee {
protected:
    string name;
    int id;
    double baseSalary;

public:
    Employee(const string &name, int id, double baseSalary)
        : name(name), id(id), baseSalary(baseSalary) {}

    virtual double calculateSalary() const = 0; // Pure virtual function
    virtual void displayInfo() const {
        cout << "Name: " << name << endl;
        cout << "ID: " << id << endl;
    }
};

class SalariedEmployee : public Employee {
public:
    SalariedEmployee(const string &name, int id, double baseSalary)
        : Employee(name, id, baseSalary) {}

    double calculateSalary() const override {
        return baseSalary + calculateBonus() - calculateDeductions();
    }

private:
    double calculateBonus() const { return baseSalary * 0.1; }
    double calculateDeductions() const { return baseSalary * 0.05; }
};

class HourlyEmployee : public Employee {
private:
    int hoursWorked;
    double hourlyRate;

public:
    HourlyEmployee(const string &name, int id, double hourlyRate, int hoursWorked)
        : Employee(name, id, 0), hourlyRate(hourlyRate), hoursWorked(hoursWorked) {}

    double calculateSalary() const override {
        return (hoursWorked * hourlyRate) + calculateBonus() - calculateDeductions();
    }

private:
    double calculateBonus() const { return (hoursWorked * hourlyRate) * 0.05; }
    double calculateDeductions() const { return (hoursWorked * hourlyRate) * 0.03; }
};

// File operations
bool createAccount(const string &username, const string &password) {
    ofstream file("data.txt", ios::app); // Open file in append mode
    if (file.is_open()) {
        // Write account data with a specific delimiter
        file << "                ACCOUNT                   " << endl;
        file << "------------------------------------------" << endl;
        file << username << " " << password << endl;
        file.close();
        return true;
    }
    return false;
}

bool login(const string &username, const string &password) {
    ifstream file("data.txt");
    string user, pass, line;
    bool found = false;

    while (getline(file, line)) {
        if (line == "ACCOUNT") { // Look for account block
            if (file >> user >> pass) {
                if (user == username && pass == password) {
                    found = true;
                    break;
                }
            }
        }
    }
    return found;
}

void savePayrollData(const Employee &employee) {
    ofstream file("data.txt", ios::app); // Open file in append mode
    if (file.is_open()) {
        // Write payroll data with a specific delimiter
        file << "PAYROLL:" << endl;
        file << fixed << setprecision(2);
        file << "Employee Salary: " << employee.calculateSalary() << endl;
        file.close();
    }
}
