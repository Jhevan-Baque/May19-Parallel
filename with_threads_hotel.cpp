#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <cstring>
#include <fstream>
#include <algorithm>
#include <random>
using namespace std;

const int TOTAL_ROOMS = 50;
int rooms[TOTAL_ROOMS];
mutex room_mutex;
int successful_bookings = 0;

// File Handling
void save_to_file(const string& filename) {
    lock_guard<mutex> lock(room_mutex);
    ofstream out(filename, ios::binary);
    if (out) {
        out.write((char*)rooms, sizeof(rooms));
        out.write((char*)&successful_bookings, sizeof(successful_bookings));
        cout << "Saved room data to file.\n";
    } else {
        cout << "Failed to open file for saving.\n";
    }
    out.close();
}

void load_from_file(const string& filename) {
    lock_guard<mutex> lock(room_mutex);
    ifstream in(filename, ios::binary);
    if (in) {
        in.read((char*)rooms, sizeof(rooms));
        in.read((char*)&successful_bookings, sizeof(successful_bookings));
        cout << "Loaded room data from file.\n";
    } else {
        cout << "Failed to open file for loading.\n";
    }
    in.close();
}


void reset_rooms(int value = 0) {
   lock_guard<mutex> lock(room_mutex);
   memset(rooms, value, sizeof(rooms));
   successful_bookings = 0;
}

void book_rooms(int thread_id, int attempts) {
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


// Test Case


void TestCase1() {
    reset_rooms(0);
    for (int i = 0; i < TOTAL_ROOMS; ++i) {
        lock_guard<mutex> lock(room_mutex);
        rooms[i] = 1;
        ++successful_bookings;
        cout << "Booked room: " << i << endl;
    }
    cout << "Total bookings: " << successful_bookings << "/" << TOTAL_ROOMS << "\n";
}

void TestCase2() {
    reset_rooms(1);
    for (int i = 0; i < 100; ++i) {
        int room_to_book = rand() % TOTAL_ROOMS;
        lock_guard<mutex> lock(room_mutex);
        if (rooms[room_to_book] == 0) {
            rooms[room_to_book] = 1;
            ++successful_bookings;
            cout << "Booked room: " << room_to_book << endl;
        } else {
            cout << "Failed to book room: " << room_to_book << " (already taken)\n";
        }
    }
    cout << "Total bookings: " << successful_bookings << "/100\n";
}

void TestCase3() {
    reset_rooms(0);
    srand(42);
    for (int i = 0; i < 100; ++i) {
        int room_to_book = rand() % TOTAL_ROOMS;
        lock_guard<mutex> lock(room_mutex);
        if (rooms[room_to_book] == 0) {
            rooms[room_to_book] = 1;
            ++successful_bookings;
            cout << "Booked room: " << room_to_book << endl;
        } else {
            cout << "Failed to book room: " << room_to_book << " (already taken)\n";
        }
    }
    cout << "Total bookings: " << successful_bookings << "/100\n";
}

void TestCase4() {
    reset_rooms(0);
    srand(42);

    vector<int> room_indices(TOTAL_ROOMS);
    for (int i = 0; i < TOTAL_ROOMS; ++i) {
        room_indices[i] = i;
    }

    random_shuffle(room_indices.begin(), room_indices.end());

    vector<thread> threads;
    for (int i = 0; i < 4; ++i) {
        threads.emplace_back([i, &room_indices]() {
            for (int j = i * 25 / 2; j < (i + 1) * 25 / 2; ++j) {
                int room_to_book = room_indices[j];
                lock_guard<mutex> lock(room_mutex);
                if (rooms[room_to_book] == 0) {
                    rooms[room_to_book] = 1;
                    ++successful_bookings;
                    cout << "Booked room: " << room_to_book << endl;
                } else {
                    cout << "Failed to book room: " << room_to_book << " (already taken)" << endl;
                }
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    cout << "Total bookings: " << successful_bookings << "/50\n";
}

void display_room_status() {
    lock_guard<mutex> lock(room_mutex);
    cout << "\n--- Room Booking Status ---\n";
    for (int i = 0; i < TOTAL_ROOMS; ++i) {
        cout << "Room " << i << ": " << (rooms[i] == 1 ? "Booked" : "Available") << '\n';
    }
    cout << "Total successful bookings: " << successful_bookings << "/50\n";
}


int main() {
    while (true) {
        cout << "\n--- Hotel Reservation System ---\n";
        cout << "1: Run Test Case 1 (All Rooms Available)\n";
        cout << "2: Run Test Case 2 (Fully Booked Hotel)\n";
        cout << "3: Run Test Case 3 (Random Bookings Seed=42)\n";
        cout << "4: Run Test Case 4 (Parallel Thread Safety)\n";
        cout << "5: Save Current Room Data to File\n";
        cout << "6: Load Room Data from File\n";
        cout << "7: Exit\n";
        cout << "Select an option: ";

        int choice;
        cin >> choice;

        switch (choice) {
            case 1: TestCase1(); break;
            case 2: TestCase2(); break;
            case 3: TestCase3(); break;
            case 4: TestCase4(); break;
            case 5: save_to_file("rooms.dat"); break;
            case 6:
                load_from_file("rooms.dat");
                display_room_status();
                break;
            case 7: return 0;
            default: cout << "Invalid option!\n";
        }
    }

    return 0;
}
