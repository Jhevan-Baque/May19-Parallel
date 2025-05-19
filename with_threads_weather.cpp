#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <random>
#include <numeric>

using namespace std;

mutex mtx;
double totalSum = 0.0;
int totalCount = 0;

void processChunk(const vector<double>& chunk) {
    double sum = accumulate(chunk.begin(), chunk.end(), 0.0);
    
    lock_guard<mutex> lock(mtx);
    totalSum += sum;
    totalCount += chunk.size();
}

vector<double> generateTemperatures(int count) {
    vector<double> temps;
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dist(-50.0, 50.0);

    for (int i = 0; i < count; ++i) {
        temps.push_back(dist(gen));
    }
    return temps;
}

vector<double> getTestData(int option) {
    vector<double> data;
    
    switch(option) {
        case 1: // All 10°C
            data.resize(10000, 10.0);
            break;
        case 2: // Alternating -10°C, 10°C
            for (int i = 0; i < 10000; ++i) {
                data.push_back(i % 2 ? 10.0 : -10.0);
            }
            break;
        case 3: // Random Data (Seed=42)
            {
                mt19937 gen(42);
                uniform_real_distribution<> dist(-50.0, 50.0);
                for (int i = 0; i < 10000; ++i) {
                    data.push_back(dist(gen));
                }
            }
            break;
        case 4: // Empty
            break;
        default:
            cerr << "Invalid option" << endl;
    }
    return data;
}

int main() {
    cout << "Select test case:\n"
         << "1. All 10°C\n"
         << "2. Alternating -10°C, 10C°\n"
         << "3. Random Data (Seed=42)\n"
         << "4. Empty Input\n"
         << "Enter choice: ";
    
    int choice;
    cin >> choice;
    
    vector<double> temps = getTestData(choice);
    
    if (temps.empty()) {
        if (choice == 4) {
            cerr << "Error: No temperature data provided." << endl;
            return 1;
        }
        cerr << "Error: No temperature data generated." << endl;
        return 1;
    }

    const size_t chunkSize = temps.size() / 4;
    vector<thread> threads;

    for (size_t i = 0; i < 4; ++i) {
        size_t start = i * chunkSize;
        size_t end = (i == 3) ? temps.size() : start + chunkSize;
        vector<double> chunk(temps.begin() + start, temps.begin() + end);
        
        threads.emplace_back(processChunk, chunk);
    }

    for (auto& t : threads) {
        t.join();
    }

    if (totalCount == 0) {
        cerr << "Error: No temperature data processed." << endl;
        return 1;
    }

    double avg = totalSum / totalCount;
    
    cout << "Average temperature: " << avg << "°C" << endl;
    cout << "John Doe" << endl; // Replace with your name
    
    return 0;
}