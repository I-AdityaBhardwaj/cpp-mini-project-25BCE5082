#include <emscripten/bind.h>
#include <vector>
#include <string>
#include <map>
#include <sstream>

using namespace std;
using namespace emscripten;

class Student {
public:
    string regNo;
    string name;
    map<string, char> attendance;

    Student(string r, string n) {
        regNo = r;
        name = n;
    }

    float getPercentage() {
        if (attendance.size() == 0) return 0;

        int present = 0;
        for (auto &p : attendance) {
            if (p.second == 'P') present++;
        }

        return (present * 100.0) / attendance.size();
    }
};

class AttendanceManager {
private:
    vector<Student> students;

public:

    // Add student
    string addStudent(string reg, string name) {
        for (auto &s : students) {
            if (s.regNo == reg) {
                return "{\"ok\":false,\"msg\":\"Duplicate student\"}";
            }
        }

        students.emplace_back(reg, name);
        return "{\"ok\":true,\"msg\":\"Student added\"}";
    }

    // Mark attendance
    string markAttendance(string reg, string date, string status) {
        for (auto &s : students) {
            if (s.regNo == reg) {

                if (status != "P" && status != "A") {
                    return "{\"ok\":false,\"msg\":\"Only P/A allowed\"}";
                }

                s.attendance[date] = status[0];
                return "{\"ok\":true,\"msg\":\"Attendance marked\"}";
            }
        }

        return "{\"ok\":false,\"msg\":\"Student not found\"}";
    }

    // Summary
    string getStudentSummary(string reg) {
        for (auto &s : students) {
            if (s.regNo == reg) {

                int total = s.attendance.size();
                int present = 0;

                for (auto &p : s.attendance) {
                    if (p.second == 'P') present++;
                }

                int absent = total - present;
                float perc = s.getPercentage();

                stringstream ss;
                ss << "{"
                   << "\"ok\":true,"
                   << "\"totalDays\":" << total << ","
                   << "\"present\":" << present << ","
                   << "\"absent\":" << absent << ","
                   << "\"percentage\":" << perc
                   << "}";

                return ss.str();
            }
        }

        return "{\"ok\":false,\"msg\":\"Student not found\"}";
    }

    // All students
    string getAllStudentsJSON() {
        stringstream ss;
        ss << "[";

        for (int i = 0; i < students.size(); i++) {
            auto &s = students[i];
            float p = s.getPercentage();

            ss << "{"
               << "\"regNo\":\"" << s.regNo << "\","
               << "\"name\":\"" << s.name << "\","
               << "\"percentage\":" << p << ","
               << "\"shortage\":" << (p < 75 ? "true" : "false")
               << "}";

            if (i != students.size() - 1) ss << ",";
        }

        ss << "]";
        return ss.str();
    }

    int getStudentCount() {
        return students.size();
    }

    float getClassAverage() {
        if (students.size() == 0) return 0;

        float total = 0;
        for (auto &s : students) {
            total += s.getPercentage();
        }

        return total / students.size();
    }
};



// 🔥 IMPORTANT: EMBIND BINDING
EMSCRIPTEN_BINDINGS(my_module) {

    class_<AttendanceManager>("AttendanceManager")
        .constructor<>()
        .function("addStudent", &AttendanceManager::addStudent)
        .function("markAttendance", &AttendanceManager::markAttendance)
        .function("getStudentSummary", &AttendanceManager::getStudentSummary)
        .function("getAllStudentsJSON", &AttendanceManager::getAllStudentsJSON)
        .function("getStudentCount", &AttendanceManager::getStudentCount)
        .function("getClassAverage", &AttendanceManager::getClassAverage);
}