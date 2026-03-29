#include <emscripten/bind.h>
#include <vector>
#include <string>
#include <map>
#include <sstream>

using namespace std;
using namespace emscripten;

// ---------------- STUDENT ----------------
class Student {
private:
    string regNo;
    string name;
    map<string, char> attendance;

public:
    Student(string r, string n) : regNo(r), name(n) {}

    string getRegNo() { return regNo; }
    string getName() { return name; }

    void markAttendance(string date, char status) {
        attendance[date] = status;
    }

    float getPercentage() {
        if (attendance.size() == 0) return 0;

        int present = 0;
        for (auto &p : attendance)
            if (p.second == 'P') present++;

        return (present * 100.0) / attendance.size();
    }

    map<string, char> getAttendance() { return attendance; }
};

// ---------------- MANAGER ----------------
class AttendanceManager {
private:
    vector<Student> students;

public:

    string addStudent(string reg, string name) {
        for (auto &s : students) {
            if (s.getRegNo() == reg)
                return "{\"ok\":false,\"msg\":\"Duplicate student\"}";
        }

        students.emplace_back(reg, name);
        return "{\"ok\":true,\"msg\":\"Student added\"}";
    }

    string markAttendance(string reg, string date, string status) {
        if (status != "P" && status != "A")
            return "{\"ok\":false,\"msg\":\"Only P/A allowed\"}";

        for (auto &s : students) {
            if (s.getRegNo() == reg) {
                s.markAttendance(date, status[0]);
                return "{\"ok\":true,\"msg\":\"Attendance marked\"}";
            }
        }

        return "{\"ok\":false,\"msg\":\"Student not found\"}";
    }

    string getStudentSummary(string reg) {
        for (auto &s : students) {
            if (s.getRegNo() == reg) {

                int total = s.getAttendance().size();
                int present = 0;

                for (auto &p : s.getAttendance())
                    if (p.second == 'P') present++;

                int absent = total - present;
                float perc = s.getPercentage();

                stringstream ss;
                ss << "{"
                   << "\"totalDays\":" << total << ","
                   << "\"present\":" << present << ","
                   << "\"absent\":" << absent << ","
                   << "\"percentage\":" << perc
                   << "}";

                return ss.str();
            }
        }

        return "{\"error\":\"Student not found\"}";
    }

    string getAllStudentsJSON() {
        stringstream ss;
        ss << "[";

        for (int i = 0; i < students.size(); i++) {
            auto &s = students[i];
            float p = s.getPercentage();

            ss << "{"
               << "\"regNo\":\"" << s.getRegNo() << "\","
               << "\"name\":\"" << s.getName() << "\","
               << "\"percentage\":" << p << ","
               << "\"shortage\":" << (p < 75 ? "true" : "false")
               << "}";

            if (i != students.size() - 1) ss << ",";
        }

        ss << "]";
        return ss.str();
    }

    float getClassAverage() {
        if (students.size() == 0) return 0;

        float total = 0;
        for (auto &s : students)
            total += s.getPercentage();

        return total / students.size();
    }
};

// ---------------- BINDINGS ----------------
EMSCRIPTEN_BINDINGS(my_module) {
    class_<AttendanceManager>("AttendanceManager")
        .constructor<>()
        .function("addStudent", &AttendanceManager::addStudent)
        .function("markAttendance", &AttendanceManager::markAttendance)
        .function("getStudentSummary", &AttendanceManager::getStudentSummary)
        .function("getAllStudentsJSON", &AttendanceManager::getAllStudentsJSON)
        .function("getClassAverage", &AttendanceManager::getClassAverage);
}