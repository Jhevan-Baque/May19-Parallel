#include <iostream>
#include <random>
#include <vector>

using namespace std;

int main() {
    const int num_temperatures = 10000;
    vector<double> temperatures(num_temperatures);
    
    // Generate random temperatures between -50°C and 50°C
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(-50.0, 50.0);
    
    for (int i = 0; i < num_temperatures; ++i) {
        temperatures[i] = dis(gen);
    }
    
    // Calculate average
    double sum = 0.0;
    for (double temp : temperatures) {
        sum += temp;
    }
    double average = sum / num_temperatures;
    
    // Print result
    cout << "Average temperature: " << average << "°C" << endl;
    cout << "Mica" << endl;
    
    return 0;
}