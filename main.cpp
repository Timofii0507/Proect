#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <Windows.h>

using namespace std;

struct Task {
    string title;
    int priority;
    string description;
    string date;
    string time;

    Task(string t, int p, string d, string dt, string tm) {
        title = t;
        priority = p;
        description = d;
        date = dt;
        time = tm;
    }

    void print() const {
        cout << "�����: " << title << endl;
        cout << "��������: " << priority << endl;
        cout << "����: " << description << endl;
        cout << "����: " << date << endl;
        cout << "���: " << time << endl;
    }
};

vector<Task> tasks;

bool isLeapYear(int year) {
    return (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
}

bool validateDate(string date) {
    if (date.length() != 10) {
        return false;
    }

    for (char c : date) {
        if (!isdigit(c) && c != '-') {
            return false;
        }
    }

    int year = stoi(date.substr(0, 4));
    int month = stoi(date.substr(5, 2));
    int day = stoi(date.substr(8, 2));

    if (year < 1900 || year > 2100) {
        return false;
    }

    if (month < 1 || month > 12) {
        return false;
    }

    switch (month) {
    case 2:
        if (isLeapYear(year)) {
            if (day < 1 || day > 29) {
                return false;
            }
        }
        else {
            if (day < 1 || day > 28) {
                return false;
            }
        }
        break;
    case 4:
    case 6:
    case 9:
    case 11:
        if (day < 1 || day > 30) {
            return false;
        }
        break;
    default:
        if (day < 1 || day > 31) {
            return false;
        }
    }

    return true;
}

bool validateTime(string time) {
    if (time.length() != 5) {
        return false;
    }

    for (char c : time) {
        if (!isdigit(c) && c != ':') {
            return false;
        }
    }

    int hours = stoi(time.substr(0, 2));
    int minutes = stoi(time.substr(3, 2));

    if (hours < 0 || hours > 23) {
        return false;
    }

    if (minutes < 0 || minutes > 59) {
        return false;
    }

    return true;
}

void saveTasksToFile() {
    ofstream file("tasks.txt", ios::out);
    if (file.is_open()) {
        for (const Task& task : tasks) {
            file << task.title << "|" << task.priority << "|" << task.description << "|"
                << task.date << "|" << task.time << endl;
        }
        file.close();
    }
    else {
        cout << "�� ������� ������� ���� ��� ����������.\n";
    }
}


void loadTasksFromFile() {
    ifstream file("tasks.txt");
    tasks.clear();
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string t, d, dt, tm;
            int p;
            getline(ss, t, '|');
            ss >> p;
            ss.ignore();
            getline(ss, d, '|');
            getline(ss, dt, '|');
            getline(ss, tm, '|');
            Task loadedTask(t, p, d, dt, tm);
            tasks.push_back(loadedTask);
        }
        file.close();
    }
    else {
        cout << "�� ������� ������� ���� ��� ������������.\n";
    }
}

void addTask() {
    string title, date, description, time;
    int priority;
    cout << "������ ����� ������: ";
    getline(cin, title);
    cout << "������ �������� ������ (�� 1 �� 5): ";
    cin >> priority;
    if (priority < 1 || priority > 5) {
        cout << "�������: �������� ������� ���� � ������� �� 1 �� 5.\n";
        return;
    }

    cin.ignore();
    cout << "������ ���� ������: ";
    getline(cin, description);
    cout << "������ ���� ��������� ������ (� ������ ����-��-��): ";
    getline(cin, date);

    if (validateDate(date)) {
        cout << "������ ��� ��������� ������ (� ������ ��:��): ";
        getline(cin, time);

        if (validateTime(time)) {
            Task newTask(title, priority, description, date, time);
            tasks.push_back(newTask);
            cout << "������ ������ �� ������.\n";
        }
        else {
            cout << "��� ��������.\n";
        }
    }
    else {
        cout << "���� �������.\n";
    }
}

void deleteTask() {
    string title;
    cout << "������ ����� ������, ��� �� ������ ��������: ";
    getline(cin, title);
    for (int i = 0; i < tasks.size(); i++) {
        if (tasks[i].title == title) {
            tasks.erase(tasks.begin() + i);
            cout << "������ �������� � ������.\n";
            return;
        }
    }
    cout << "������ � ����� ������ �� ��������.\n";
}

void editTask() {
    string title, date, description, time;
    int priority;
    cout << "������ ����� ������, ��� �� ������ ����������: ";
    getline(cin, title);
    for (int i = 0; i < tasks.size(); i++) {
        if (tasks[i].title == title) {
            cout << "������ ���� ����� ������: ";
            getline(cin, title);
            cout << "������ ����� �������� ������ (�� 1 �� 5): ";
            cin >> priority;
            cin.ignore();
            cout << "������ ����� ���� ������: ";
            getline(cin, description);
            cout << "������ ���� ���� ��������� ������ (� ������ ����-��-��): ";
            getline(cin, date);

            if (validateDate(date)) {
                cout << "������ ����� ��� ��������� ������ (� ������ ��:��): ";
                getline(cin, time);
                if (validateTime(time)) {
                    tasks[i] = Task(title, priority, description, date, time);
                    cout << "������ ������������.\n";
                }
                else {
                    cout << "��� ��������.\n";
                }
                return;
            }
            else {
                cout << "���� �������.\n";
                return;
            }
        }
    }
    cout << "������ � ����� ������ �� ��������.\n";
}

void searchTask() {
    string title, date, description, time;
    int p, c;
    bool found = false;
    cout << "������� ������� ������:\n";
    cout << "1. �����\n";
    cout << "2. ��������\n";
    cout << "3. ����\n";
    cout << "4. ���� �� ��� ���������\n";
    cin >> c;
    cin.ignore();
    switch (c) {
    case 1:
        cout << "������ ����� ������: ";
        getline(cin, title);
        for (int i = 0; i < tasks.size(); i++) {
            if (tasks[i].title == title) {
                tasks[i].print();
                cout << "-----------------\n";
                found = true;
            }
        }
        break;
    case 2:
        cout << "������ �������� ������ (�� 1 �� 5): ";
        cin >> p;
        cin.ignore();
        for (int i = 0; i < tasks.size(); i++) {
            if (tasks[i].priority == p) {
                tasks[i].print();
                cout << "-----------------\n";
                found = true;
            }
        }
        break;
    case 3:
        cout << "������ ���� ������: ";
        getline(cin, description);
        for (int i = 0; i < tasks.size(); i++) {
            if (tasks[i].description == description) {
                tasks[i].print();
                cout << "-----------------\n";
                found = true;
            }
        }
        break;
    case 4:
        cout << "������ ���� ��������� ������ (� ������ ����-��-��): ";
        getline(cin, date);
        cout << "������ ��� ��������� ������ (� ������ ��:��): ";
        getline(cin, time);
        for (int i = 0; i < tasks.size(); i++) {
            if (tasks[i].date == date && tasks[i].time == time) {
                tasks[i].print();
                cout << "-----------------\n";
                found = true;
            }
        }
        break;
    default:
        cout << "������� ������� ������.\n";
        return;
    }
    if (!found) {
        cout << "������ �� �������� ������� �� ��������.\n";
    }
}

void displayTasks() {
    string dt, dt1, dt2;
    int c;
    bool found = false;
    cout << "������� ����� �����������:\n";
    cout << "1. �� ����\n";
    cout << "2. �� �������\n";
    cout << "3. �� �����\n";
    cin >> c;
    cin.ignore();
    switch (c) {
    case 1:
        cout << "������ ���� (� ������ ����-��-��): ";
        getline(cin, dt);
        for (int i = 0; i < tasks.size(); i++) {
            if (tasks[i].date == dt) {
                tasks[i].print();
                cout << "-----------------\n";
                found = true;
            }
        }
        break;
    case 2:
        cout << "������ ��������� ���� ����� (� ������ ����-��-��): ";
        getline(cin, dt1);
        cout << "������ ������ ���� ����� (� ������ ����-��-��): ";
        getline(cin, dt2);
        for (int i = 0; i < tasks.size(); i++) {
            if (tasks[i].date >= dt1 && tasks[i].date <= dt2) {
                tasks[i].print();
                cout << "-----------------\n";
                found = true;
            }
        }
        break;
    case 3:
        cout << "������ ���� (� ������ ����-��): ";
        getline(cin, dt);
        for (int i = 0; i < tasks.size(); i++) {
            if (tasks[i].date.substr(0, 7) == dt) {
                tasks[i].print();
                cout << "-----------------\n";
                found = true;
            }
        }
        break;
    default:
        cout << "������� ����� �����������.\n";
        return;
    }
    if (!found) {
        cout << "������ �� �������� ������� �� ��������.\n";
    }
}

void sortTasks() {
    int c;
    cout << "������� ������� ����������:\n";
    cout << "1. �� ����������\n";
    cout << "2. �� ����� �� ����� ���������\n";
    cin >> c;
    cin.ignore();
    switch (c) {
    case 1:
        sort(tasks.begin(), tasks.end(), [](const Task& a, const Task& b) {
            return a.priority > b.priority;
            });
        cout << "������ ����� ����������� �� ����������.\n";
        break;
    case 2:
        sort(tasks.begin(), tasks.end(), [](const Task& a, const Task& b) {
            return a.date + a.time < b.date + b.time;
            });
        cout << "������ ����� ����������� �� ����� �� ����� ���������.\n";
        break;
    default:
        cout << "������� ������� ����������.\n";
        return;
    }
}

void showMainMenu() {
    int choice;
    bool exit = false;
    while (!exit) {
        cout << "������� ��:\n";
        cout << "1. ������ ������\n";
        cout << "2. �������� ������\n";
        cout << "3. ���������� ������\n";
        cout << "4. ����� ������\n";
        cout << "5. ³���������� ������ �����\n";
        cout << "6. ���������� ������ �����\n";
        cout << "7. �����\n";
        cout << endl;
        cin >> choice;
        cin.ignore();
        switch (choice) {
        case 1:
            addTask();
            break;
        case 2:
            deleteTask();
            break;
        case 3:
            editTask();
            break;
        case 4:
            searchTask();
            break;
        case 5:
            displayTasks();
            break;
        case 6:
            sortTasks();
            break;
        case 7:
            exit = true;
            break;
        default:
            cout << "������ ��.\n";
            break;
        }
    }
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    loadTasksFromFile();
    cout << "³��� ��� � ������� ������� �����!\n";
    showMainMenu();
    cout << "����� �� ������������ �������� ������� �����!\n";
    saveTasksToFile();
    return 0;
}