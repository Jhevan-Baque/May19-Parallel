#include <iostream>
#include <thread>
#include <mutex>
#include <fstream>
#include <ctime>

using namespace std;

const int TOTAL_ROOMS = 50;
int rooms[TOTAL_ROOMS];
int successful_bookings = 0;
mutex room_mutex;

void save_to_file(const string& filename) {
    lock_guard<mutex> lock(room_mutex);
    ofstream out(filename);
    if (out) {
        for (int i = 0; i < TOTAL_ROOMS; ++i) {
            out << rooms[i] << " ";
        }
        out << "\n" << successful_bookings << "\n";
        cout << "Saved room data to file.\n";
    } else {
        cout << "Failed to open file for saving.\n";
    }
}

void load_from_file(const string& filename) {
    lock_guard<mutex> lock(room_mutex);
    ifstream in(filename);
    if (in) {
        for (int i = 0; i < TOTAL_ROOMS; ++i) {
            in >> rooms[i];
        }
        in >> successful_bookings;
        cout << "Loaded room data from file.\n";
    } else {
        cout << "No saved data found, starting fresh.\n";
        for (int i = 0; i < TOTAL_ROOMS; ++i) {
            rooms[i] = 0;
        }
        successful_bookings = 0;
    }
}

void reset_rooms(int value = 0) {
    lock_guard<mutex> lock(room_mutex);
    for (int i = 0; i < TOTAL_ROOMS; ++i) {
        rooms[i] = value;
    }
    successful_bookings = 0;
}

void book_rooms(int attempts) {
    for (int i = 0; i < attempts; ++i) {
        int room_to_book = rand() % TOTAL_ROOMS;
        lock_guard<mutex> lock(room_mutex);
        if (rooms[room_to_book] == 0) {
            rooms[room_to_book] = 1;
            ++successful_bookings;
            cout << "Booked room: " << room_to_book << endl;
        } else {
            cout << "Failed to book room: " << room_to_book << " (already taken)" << endl;
        }
    }
}

void TestCase1() {
    reset_rooms(1);
    successful_bookings = TOTAL_ROOMS;
    for (int i = 0; i < TOTAL_ROOMS; ++i) {
        cout << "Booked room: " << i << endl;
    }
    cout << "Total bookings: " << successful_bookings << "/" << TOTAL_ROOMS << "\n";
}

void TestCase2() {
    reset_rooms(1);
    successful_bookings = 0;
    book_rooms(100);
    cout << "Total bookings: " << successful_bookings << "/100\n";
}

void TestCase3() {
    reset_rooms(0);
    successful_bookings = 0;
    srand(42);
    book_rooms(100);
    cout << "Total bookings: " << successful_bookings << "/100\n";
}

void TestCase4() {
    reset_rooms(0);
    successful_bookings = 0;
    const int rooms_per_thread = TOTAL_ROOMS / 4;
    thread threads[4];
    for (int i = 0; i < 4; ++i) {
        int start_index = i * rooms_per_thread;
        int end_index = (i == 3) ? (TOTAL_ROOMS - 1) : (start_index + rooms_per_thread - 1);
        threads[i] = thread([start_index, end_index]() {
            for (int room = start_index; room <= end_index; ++room) {
                lock_guard<mutex> lock(room_mutex);
                if (rooms[room] == 0) {
                    rooms[room] = 1;
                    ++successful_bookings;
                    cout << "Booked room: " << room << endl;
                } else {
                    cout << "Failed to book room: " << room << " (already taken)" << endl;
                }
            }
        });
    }
    for (int i = 0; i < 4; ++i) {
        threads[i].join();
    }
    cout << "Total bookings: " << successful_bookings << "/" << TOTAL_ROOMS << "\n";
}

void display_room_status() {
    lock_guard<mutex> lock(room_mutex);
    cout << "\n--- Room Booking Status ---\n";
    for (int i = 0; i < TOTAL_ROOMS; ++i) {
        cout << "Room " << i << ": " << (rooms[i] == 1 ? "Booked" : "Available") << '\n';
    }
    cout << "Total successful bookings: " << successful_bookings << "/" << TOTAL_ROOMS << "\n";
}

int main() {
    srand(time(0));
    load_from_file("rooms.txt");

    while (true) {
        cout << "\n--- Hotel Reservation System ---\n";
        cout << "1: Run Test Case 1 (All Rooms Booked)\n";
        cout << "2: Run Test Case 2 (Booking Attempts on Full Hotel)\n";
        cout << "3: Run Test Case 3 (Random Bookings with Seed 42)\n";
        cout << "4: Run Test Case 4 (Parallel Bookings with Threads)\n";
        cout << "5: Display Room Status\n";
        cout << "6: Exit\n";
        cout << "Select an option: ";

        int choice;
        cin >> choice;

        switch (choice) {
            case 1: TestCase1(); break;
            case 2: TestCase2(); break;
            case 3: TestCase3(); break;
            case 4: TestCase4(); break;
            case 5: display_room_status(); break;
            case 6:
                save_to_file("rooms.txt");
                return 0;
            default:
                cout << "Invalid option! Please try again.\n";
        }
    }
    return 0;
}
