#include <iostream>
#include <vector>
#include <map>
#include <fstream>
using namespace std;

// ---------------- STUDENT CLASS ----------------
class Student {
private:
    string regNo;
    string name;
    map<string, char> attendance; // date -> P/A

public:
    // Constructor
    Student(string r, string n) {
        regNo = r;
        name = n;
    }

    string getRegNo() {
        return regNo;
    }

    string getName() {
        return name;
    }

    void markAttendance(string date, char status) {
        attendance[date] = status;
    }

    float getPercentage() {
        if (attendance.size() == 0) return 0;

        int present = 0;
        for (auto &entry : attendance) {
            if (entry.second == 'P') present++;
        }

        return (present * 100.0) / attendance.size();
    }

    void showSummary() {
        cout << "\nReg No: " << regNo;
        cout << "\nName: " << name;
        cout << "\nAttendance: ";

        for (auto &entry : attendance) {
            cout << "[" << entry.first << ":" << entry.second << "] ";
        }

        cout << "\nPercentage: " << getPercentage() << "%\n";
    }

    map<string, char> getAttendance() {
        return attendance;
    }

    void setAttendance(map<string, char> att) {
        attendance = att;
    }
};

// ---------------- MANAGER CLASS ----------------
class AttendanceManager {
private:
    vector<Student> students;

public:

    // Add Student
    void addStudent() {
        string reg, name;

        cout << "Enter RegNo: ";
        cin >> reg;
        cout << "Enter Name: ";
        cin >> name;

        // Check duplicate
        for (auto &s : students) {
            if (s.getRegNo() == reg) {
                cout << "Duplicate student not allowed!\n";
                return;
            }
        }

        students.push_back(Student(reg, name));
        cout << "Student added successfully.\n";
    }

    // Mark Attendance
    void markAttendance() {
        string reg, date;
        char status;

        cout << "Enter RegNo: ";
        cin >> reg;
        cout << "Enter Date: ";
        cin >> date;
        cout << "Enter Status (P/A): ";
        cin >> status;

        if (status != 'P' && status != 'A') {
            cout << "Invalid input! Only P/A allowed.\n";
            return;
        }

        for (auto &s : students) {
            if (s.getRegNo() == reg) {
                s.markAttendance(date, status);
                cout << "Attendance marked.\n";
                return;
            }
        }

        cout << "Student not found!\n";
    }

    // View Summary
    void viewSummary() {
        string reg;
        cout << "Enter RegNo: ";
        cin >> reg;

        for (auto &s : students) {
            if (s.getRegNo() == reg) {
                s.showSummary();
                return;
            }
        }

        cout << "Student not found!\n";
    }

    // Shortage List
    void shortageList() {
        cout << "\nStudents below 75%:\n";

        for (auto &s : students) {
            if (s.getPercentage() < 75) {
                cout << s.getName() << " (" << s.getRegNo() << ") - "
                     << s.getPercentage() << "%\n";
            }
        }
    }

    // Class Average
    void classAverage() {
        if (students.size() == 0) {
            cout << "No students available.\n";
            return;
        }

        float total = 0;
        for (auto &s : students) {
            total += s.getPercentage();
        }

        cout << "Class Average: "
             << total / students.size() << "%\n";
    }

    // Save to File
    void saveToFile() {
        ofstream file("data.txt");

        for (auto &s : students) {
            file << s.getRegNo() << "," << s.getName() << ",";

            map<string, char> att = s.getAttendance();

            for (auto &a : att) {
                file << a.first << ":" << a.second << "|";
            }

            file << endl;
        }

        file.close();
    }

    // Load from File
    void loadFromFile() {
        ifstream file("data.txt");
        if (!file) return;

        students.clear();

        string reg, name, data;

        while (getline(file, reg, ',')) {
            getline(file, name, ',');
            getline(file, data);

            Student s(reg, name);
            map<string, char> att;

            string temp = "";
            for (char c : data) {
                if (c == '|') {
                    int pos = temp.find(':');
                    string date = temp.substr(0, pos);
                    char status = temp[pos + 1];
                    att[date] = status;
                    temp = "";
                } else {
                    temp += c;
                }
            }

            s.setAttendance(att);
            students.push_back(s);
        }

        file.close();
    }
};

// ---------------- MAIN FUNCTION ----------------
int main() {
    AttendanceManager manager;
    manager.loadFromFile();

    int choice;

    do {
        cout << "\n===== Attendance Manager =====\n";
        cout << "1. Add Student\n";
        cout << "2. Mark Attendance\n";
        cout << "3. View Summary\n";
        cout << "4. Shortage List (<75%)\n";
        cout << "5. Class Average\n";
        cout << "6. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            manager.addStudent();
            break;
        case 2:
            manager.markAttendance();
            break;
        case 3:
            manager.viewSummary();
            break;
        case 4:
            manager.shortageList();
            break;
        case 5:
            manager.classAverage();
            break;
        case 6:
            manager.saveToFile();
            cout << "Data saved. Exiting...\n";
            break;
        default:
            cout << "Invalid choice!\n";
        }

    } while (choice != 6);

    return 0;
}