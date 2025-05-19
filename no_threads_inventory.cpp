#include <iostream>
#include <vector>
#include <ctime>

using namespace std;

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));
    vector<int> stock(1000);
    int low_stock_count = 0;

    for (int i = 0; i < 1000; ++i) {
        stock[i] = rand() % 101;
    }

    for (int i = 0; i < 1000; ++i) {
        if (stock[i] < 10) {
            cout << "[ALERT] Product " << i << ": " << stock[i] << " units left\n";
            ++low_stock_count;
        }
    }

    cout << "Total low-stock items: " << low_stock_count << "\n";
    cout << "Jhevan P. Baque";
    return 0;
}
