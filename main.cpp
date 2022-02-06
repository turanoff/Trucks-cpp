#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <regex>
#include <string>
#include <vector>
#include <unistd.h>

using namespace std;


string base_path = "C:\\Users\\User\\Desktop\\ConsoleApplication1\\";
string separator = " | ";


// Глобальные переменные
struct Vehicle {
    int id;
    string model;
    string plate;
    string trailer_plate;
    float fuel_rate;
};
struct Driver {
    int id;
    string name;
    int rate;
};
struct Trip {
    int id;
    int days;
    string name;
    int dist;
    float fuel;
    float weight;
    int driver_id;
    int vehicle_id;
};
string vehicles_title;
vector<Vehicle> all_vehicles;
vector<string> all_vehicles_str;
string drivers_title;
vector<Driver> all_drivers;
vector<string> all_drivers_str;
string trips_title;
vector<Trip> all_trips;
vector<string> all_trips_str;


// Читать файл в массив строк
vector<string> read_file(string path) {
    ifstream fin(path);
    string line;
    vector<string> lines;
    stringstream buffer;
    buffer << fin.rdbuf();
    while (getline(buffer, line, '\n')) {
       lines.push_back(line);
    }
    return lines;
}


vector<Vehicle> strings_to_vehicles(vector<string> lines) {
    string cell;
    vector<Vehicle> vehicles;

    for (string line : lines) {
        stringstream line_buf(line);
        vector<string> cells;
        while (getline(line_buf, cell, ',')) {cells.push_back(cell);}
        Vehicle vehicle = {stoi(cells[0]), cells[1], cells[2], cells[3], stof(cells[4])};
        vehicles.push_back(vehicle);
    }
    return vehicles;
}

vector<Driver> strings_to_drivers(vector<string> lines) {
    string cell;
    vector<Driver> drivers;

    for (string line : lines) {
        stringstream line_buf(line);
        vector<string> cells;
        while (getline(line_buf, cell, ',')) {cells.push_back(cell);}
        Driver driver = {stoi(cells[0]), cells[1], stoi(cells[2])};
        drivers.push_back(driver);
    }
    return drivers;
}

vector<Trip> strings_to_trips(vector<string> lines) {
    string cell;
    vector<Trip> trips;

    for (string line : lines) {
        stringstream line_buf(line);
        vector<string> cells;
        while (getline(line_buf, cell, ',')) {cells.push_back(cell);}
        Trip trip = {stoi(cells[0]), stoi(cells[1]), cells[2], stoi(cells[3]),
                     stof(cells[4]), stof(cells[5]), stoi(cells[6]), stoi(cells[7])};
        trips.push_back(trip);
    }
    return trips;
}


// Читать все таблицы из файлов в память
void read_tables() {
    all_vehicles_str = read_file(base_path + "vehicles.csv");
    vehicles_title = all_vehicles_str[0];
    all_vehicles_str.erase(all_vehicles_str.begin());
    all_vehicles = strings_to_vehicles(all_vehicles_str);

    all_drivers_str = read_file(base_path + "drivers.csv");
    drivers_title = all_drivers_str[0];
    all_drivers_str.erase(all_drivers_str.begin());
    all_drivers = strings_to_drivers(all_drivers_str);

    all_trips_str = read_file(base_path + "trips.csv");
    trips_title = all_trips_str[0];
    all_trips_str.erase(all_trips_str.begin());
    all_trips = strings_to_trips(all_trips_str);
}


void show_menu() {
    cout << "Меню" << endl;
    cout << "1. Таблица Автомобили" << endl;
    cout << "2. Таблица Водители" << endl;
    cout << "3. Таблица Рейсы" << endl;
    cout << "4. Поиск по автомобилям и водителям" << endl;
    cout << "5. Расчет перерасхода/экономии топлива" << endl;
    cout << "6. Расчет получки водителя" << endl;
    cout << "0. Выход" << endl;
    cout << endl;
}


void show_vehicles(vector<Vehicle> vehicles) {
    cout << regex_replace(vehicles_title, regex(","), separator) << endl;
    for (Vehicle vehicle : vehicles) {
        cout << setw(2) << vehicle.id << separator;
        cout << setw(12) << vehicle.model << separator;
        cout << setw(15) << vehicle.plate << separator;
        cout << setw(20) << vehicle.trailer_plate << separator;
        cout << setw(21) << vehicle.fuel_rate << endl;
    }
}

void show_drivers(vector<Driver> drivers) {
    cout << regex_replace(drivers_title, regex(","), separator) << endl;
    for (Driver driver : drivers) {
        cout << setw(2) << driver.id << separator;
        cout << setw(34) << driver.name << separator;
        cout << setw(2) << driver.rate << endl;
    }
}

void show_trips(vector<Trip> trips) {
    cout << regex_replace(trips_title, regex(","), separator) << endl;
    for (Trip trip : trips) {
        cout << setw(2) << trip.id << separator;
        cout << setw(17) << trip.days << separator;
        cout << setw(10) << trip.name << separator;
        cout << setw(11) << trip.dist << separator;
        cout << setw(14) << trip.fuel << separator;
        cout << setw(9) << trip.weight << separator;
        cout << setw(11) << trip.driver_id << separator;
        cout << setw(2) << trip.vehicle_id << endl;
    }
}


void search(string term) {
    vector<string> found_vehicles_str;
    for (string vehicle : all_vehicles_str) {
        if (vehicle.find(term) != string::npos) {
            found_vehicles_str.push_back(vehicle);
        }
    }
    vector<string> found_drivers_str;
    for (string driver : all_drivers_str) {
        if (driver.find(term) != string::npos) {
            found_drivers_str.push_back(driver);
        }
    }

    if (!found_vehicles_str.empty()) {
        cout << "Найдены автомобили:" << endl;
        vector<Vehicle> found_vehicles = strings_to_vehicles(found_vehicles_str);
        show_vehicles(found_vehicles);
        cout << endl;
    }
    if (!found_drivers_str.empty()) {
        cout << "Найдены водители:" << endl;
        vector<Driver> found_drivers = strings_to_drivers(found_drivers_str);
        show_drivers(found_drivers);
        cout << endl;
    }
    if (found_vehicles_str.empty() && found_drivers_str.empty()) {
        cout << endl << "Ничего не найдено" << endl;
    }
}


float calc_over_cons(Vehicle vehicle, Trip trip) {
    float teor_cons = (vehicle.fuel_rate + 0.4 * trip.weight) * trip.dist / 100;
    float over_cons = trip.fuel - teor_cons;
    return over_cons;
}


void show_trip_salary(int trip_id) {
    Trip trip = all_trips[trip_id-1];
    Vehicle vehicle = all_vehicles[trip.vehicle_id-1];
    Driver driver = all_drivers[trip.driver_id-1];

    float over_cons = calc_over_cons(vehicle, trip);
    float salary = trip.days * driver.rate - over_cons * 0.8;
    
    cout << "Водитель " << driver.name << ". Рейс " << trip.name;
    cout << ". Получка €" << salary << endl;
}


void ask_for_menu() {
    cout << endl;
    cout << "Нажмите Enter чтобы вернуться в меню" << endl << endl;
    cin.get();
    cin.ignore();
    show_menu();
}


void calc_salary() {
    string menu_num;
    cout << endl << endl << "Расчет получки за рейс" << endl;
    cout << "1. По конкретному рейсу" << endl;
    cout << "2. По всем рейсам" << endl;
    cout << "0. Назад" << endl << endl;

    do {
        cout << "Введите номер пункта меню и нажмите Enter: ";
        cin >> menu_num;

        if (menu_num == "1") {
            string trip_num;
            cout << endl;
            show_trips(all_trips);
            cout << endl << "Введите номер рейса: ";
            cin >> trip_num;
            cout << endl;
            show_trip_salary(stoi(trip_num));
        } else if (menu_num == "2") {
            cout << endl << endl;
            for (Trip trip : all_trips) {
                show_trip_salary(trip.id);
            }
        } else if (menu_num == "0") {
            cout << endl << endl;
            show_menu();
            break;
        } else {
            cout << "Ввод не распознан" << endl;
        }
    } while (1);
}


void show_over_cons(int driver_id) {
    float total_over_cons;
    for (Trip trip : all_trips) {
        if (trip.driver_id == driver_id) {
            Vehicle vehicle = all_vehicles[trip.vehicle_id-1];
            total_over_cons += calc_over_cons(vehicle, trip);
        }
    }
    Driver driver = all_drivers[driver_id-1];
    cout << "Водитель " << driver.name << ". Общий перерасход за все рейсы ";
    cout << fixed << setprecision(2) << total_over_cons << " л." << endl;
}


int main() {
    string menu_num;
    setlocale(LC_ALL, "Russian");
    setlocale(LC_CTYPE, "rus");
    read_tables();
	
	vector<string> picture = read_file(base_path + "pic.txt");
	for (string lin : picture) {
       cout << lin << endl;
    }
	sleep(5);
    	
    show_menu();

    do {
        cout << "Введите номер пункта меню и нажмите Enter: ";
        cin >> menu_num;

        if (menu_num == "1") {
            cout << endl << "Автомобили" << endl;
            show_vehicles(all_vehicles);
            ask_for_menu();
        }
        else if (menu_num == "2") {
            cout << endl << "Водители" << endl;
            show_drivers(all_drivers);
            ask_for_menu();
        }
        else if (menu_num == "3") {
            cout << endl << "Рейсы" << endl;
            show_trips(all_trips);
            ask_for_menu();
        }
        else if (menu_num == "4") {
            cout << endl << "Введите строку для поиска: ";
            string term;
            cin >> term;
            cout << endl << endl;
            search(term);
            ask_for_menu();
        }
        else if (menu_num == "5") {
            string driver_num;
            cout << endl << endl;
            show_drivers(all_drivers);
            cout << endl << "Выберите водителя: ";
            cin >> driver_num;
            cout << endl << endl;
            show_over_cons(stoi(driver_num));
            ask_for_menu();
        }
        else if (menu_num == "6") {
            calc_salary();
        }
        else if (menu_num == "0") {
            cout << "Выход" << endl;
            exit(EXIT_SUCCESS);
        }
        else {
            cout << "Ввод не распознан" << endl;
        }
    } while (1);

    return 0;
}
