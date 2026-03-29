#include <iostream>
#include <vector>
#include <fstream>
#include <map>
using namespace std;

class Student {
private:
    string regNo;
    string name;
    vector<char> attendance; // P or A

public:
    Student(string r, string n) {
        regNo = r;
        name = n;
    }

    string getRegNo() { return regNo; }
    string getName() { return name; }

    void markAttendance(char status) {
        attendance.push_back(status);
    }

    float getAttendancePercentage() {
        if (attendance.size() == 0) return 0;

        int present = 0;
        for (char c : attendance) {
            if (c == 'P') present++;
        }
        return (present * 100.0) / attendance.size();
    }

    void displaySummary() {
        cout << "RegNo: " << regNo << ", Name: " << name << endl;
        cout << "Attendance: ";
        for (char c : attendance) cout << c << " ";
        cout << "\nPercentage: " << getAttendancePercentage() << "%\n";
    }

    vector<char> getAttendance() { return attendance; }

    void setAttendance(vector<char> att) {
        attendance = att;
    }
};

class AttendanceManager {
private:
    vector<Student> students;

public:
    void addStudent(string regNo, string name) {
        for (auto &s : students) {
            if (s.getRegNo() == regNo) {
                cout << "Duplicate student not allowed!\n";
                return;
            }
        }
        students.push_back(Student(regNo, name));
        cout << "Student added successfully.\n";
    }

    void markAttendance() {
        char status;
        for (auto &s : students) {
            cout << "Enter attendance for " << s.getName() << " (P/A): ";
            cin >> status;

            if (status != 'P' && status != 'A') {
                cout << "Invalid input! Only P/A allowed.\n";
                return;
            }
            s.markAttendance(status);
        }
    }

    void viewStudent(string regNo) {
        for (auto &s : students) {
            if (s.getRegNo() == regNo) {
                s.displaySummary();
                return;
            }
        }
        cout << "Student not found!\n";
    }

    void listShortage() {
        cout << "\n--- Students Below 75% ---\n";
        for (auto &s : students) {
            if (s.getAttendancePercentage() < 75) {
                cout << s.getName() << " (" << s.getRegNo() << ") - "
                     << s.getAttendancePercentage() << "%\n";
            }
        }
    }

    void classAverage() {
        float total = 0;
        for (auto &s : students) {
            total += s.getAttendancePercentage();
        }
        if (students.size() == 0) {
            cout << "No students available.\n";
            return;
        }
        cout << "Class Average Attendance: "
             << total / students.size() << "%\n";
    }

    void saveToFile() {
        ofstream file("attendance.txt");
        for (auto &s : students) {
            file << s.getRegNo() << "," << s.getName() << ",";

            vector<char> att = s.getAttendance();
            for (char c : att) file << c;

            file << endl;
        }
        file.close();
    }

    void loadFromFile() {
        ifstream file("attendance.txt");
        if (!file) return;

        students.clear();
        string regNo, name, att;

        while (getline(file, regNo, ',')) {
            getline(file, name, ',');
            getline(file, att);

            Student s(regNo, name);
            vector<char> attendance;

            for (char c : att) attendance.push_back(c);

            s.setAttendance(attendance);
            students.push_back(s);
        }
        file.close();
    }
};

int main() {
    AttendanceManager manager;
    manager.loadFromFile();

    int choice;
    string regNo, name;

    do {
        cout << "\n===== Attendance Manager =====\n";
        cout << "1. Add Student\n";
        cout << "2. Mark Attendance\n";
        cout << "3. View Student Summary\n";
        cout << "4. List Shortage Students (<75%)\n";
        cout << "5. Class Average Attendance\n";
        cout << "6. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            cout << "Enter RegNo: ";
            cin >> regNo;
            cout << "Enter Name: ";
            cin >> name;
            manager.addStudent(regNo, name);
            break;

        case 2:
            manager.markAttendance();
            break;

        case 3:
            cout << "Enter RegNo: ";
            cin >> regNo;
            manager.viewStudent(regNo);
            break;

        case 4:
            manager.listShortage();
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