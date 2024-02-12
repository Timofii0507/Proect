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
        cout << "Назва: " << title << endl;
        cout << "Пріоритет: " << priority << endl;
        cout << "Опис: " << description << endl;
        cout << "Дата: " << date << endl;
        cout << "Час: " << time << endl;
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
        cout << "Не вдалося відкрити файл для збереження.\n";
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
        cout << "Не вдалося відкрити файл для завантаження.\n";
    }
}

void addTask() {
    string title, date, description, time;
    int priority;
    cout << "Введіть назву справи: ";
    getline(cin, title);
    cout << "Введіть пріоритет справи (від 1 до 5): ";
    cin >> priority;
    if (priority < 1 || priority > 5) {
        cout << "Помилка: пріоритет повинен бути у діапазоні від 1 до 5.\n";
        return;
    }

    cin.ignore();
    cout << "Введіть опис справи: ";
    getline(cin, description);
    cout << "Введіть дату виконання справи (у форматі рррр-мм-дд): ";
    getline(cin, date);

    if (validateDate(date)) {
        cout << "Введіть час виконання справи (у форматі гг:хх): ";
        getline(cin, time);

        if (validateTime(time)) {
            Task newTask(title, priority, description, date, time);
            tasks.push_back(newTask);
            cout << "Справу додано до списку.\n";
        }
        else {
            cout << "Час недійсний.\n";
        }
    }
    else {
        cout << "Дата недійсна.\n";
    }
}

void deleteTask() {
    string title;
    cout << "Введіть назву справи, яку ви хочете видалити: ";
    getline(cin, title);
    for (int i = 0; i < tasks.size(); i++) {
        if (tasks[i].title == title) {
            tasks.erase(tasks.begin() + i);
            cout << "Справу видалено зі списку.\n";
            return;
        }
    }
    cout << "Справи з такою назвою не знайдено.\n";
}

void editTask() {
    string title, date, description, time;
    int priority;
    cout << "Введіть назву справи, яку ви хочете редагувати: ";
    getline(cin, title);
    for (int i = 0; i < tasks.size(); i++) {
        if (tasks[i].title == title) {
            cout << "Введіть нову назву справи: ";
            getline(cin, title);
            cout << "Введіть новий пріоритет справи (від 1 до 5): ";
            cin >> priority;
            cin.ignore();
            cout << "Введіть новий опис справи: ";
            getline(cin, description);
            cout << "Введіть нову дату виконання справи (у форматі рррр-мм-дд): ";
            getline(cin, date);

            if (validateDate(date)) {
                cout << "Введіть новий час виконання справи (у форматі гг:хх): ";
                getline(cin, time);
                if (validateTime(time)) {
                    tasks[i] = Task(title, priority, description, date, time);
                    cout << "Справу відредаговано.\n";
                }
                else {
                    cout << "Час недійсний.\n";
                }
                return;
            }
            else {
                cout << "Дата недійсна.\n";
                return;
            }
        }
    }
    cout << "Справи з такою назвою не знайдено.\n";
}

void searchTask() {
    string title, date, description, time;
    int p, c;
    bool found = false;
    cout << "Виберіть критерій пошуку:\n";
    cout << "1. Назва\n";
    cout << "2. Пріоритет\n";
    cout << "3. Опис\n";
    cout << "4. Дата та час виконання\n";
    cin >> c;
    cin.ignore();
    switch (c) {
    case 1:
        cout << "Введіть назву справи: ";
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
        cout << "Введіть пріоритет справи (від 1 до 5): ";
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
        cout << "Введіть опис справи: ";
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
        cout << "Введіть дату виконання справи (у форматі рррр-мм-дд): ";
        getline(cin, date);
        cout << "Введіть час виконання справи (у форматі гг:хх): ";
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
        cout << "Невірний критерій пошуку.\n";
        return;
    }
    if (!found) {
        cout << "Справи за вибраним критерієм не знайдено.\n";
    }
}

void displayTasks() {
    string dt, dt1, dt2;
    int c;
    bool found = false;
    cout << "Виберіть період відображення:\n";
    cout << "1. На день\n";
    cout << "2. На тиждень\n";
    cout << "3. На місяць\n";
    cin >> c;
    cin.ignore();
    switch (c) {
    case 1:
        cout << "Введіть дату (у форматі рррр-мм-дд): ";
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
        cout << "Введіть початкову дату тижня (у форматі рррр-мм-дд): ";
        getline(cin, dt1);
        cout << "Введіть кінцеву дату тижня (у форматі рррр-мм-дд): ";
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
        cout << "Введіть дату (у форматі рррр-мм): ";
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
        cout << "Невірний період відображення.\n";
        return;
    }
    if (!found) {
        cout << "Справи за вибраним періодом не знайдено.\n";
    }
}

void sortTasks() {
    int c;
    cout << "Виберіть критерій сортування:\n";
    cout << "1. За пріоритетом\n";
    cout << "2. За датою та часом виконання\n";
    cin >> c;
    cin.ignore();
    switch (c) {
    case 1:
        sort(tasks.begin(), tasks.end(), [](const Task& a, const Task& b) {
            return a.priority > b.priority;
            });
        cout << "Список справ відсортовано за пріоритетом.\n";
        break;
    case 2:
        sort(tasks.begin(), tasks.end(), [](const Task& a, const Task& b) {
            return a.date + a.time < b.date + b.time;
            });
        cout << "Список справ відсортовано за датою та часом виконання.\n";
        break;
    default:
        cout << "Невірний критерій сортування.\n";
        return;
    }
}

void showMainMenu() {
    int choice;
    bool exit = false;
    while (!exit) {
        cout << "Виберіть дію:\n";
        cout << "1. Додати справу\n";
        cout << "2. Видалити справу\n";
        cout << "3. Редагувати справу\n";
        cout << "4. Пошук справи\n";
        cout << "5. Відображення списку справ\n";
        cout << "6. Сортування списку справ\n";
        cout << "7. Вихід\n";
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
            cout << "Невірна дія.\n";
            break;
        }
    }
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    loadTasksFromFile();
    cout << "Вітаю вас у додатку «Список справ»!\n";
    showMainMenu();
    cout << "Дякую за користування додатком «Список справ»!\n";
    saveTasksToFile();
    return 0;
}