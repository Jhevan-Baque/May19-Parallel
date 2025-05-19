#include <iostream>
#include <ctime>

using namespace std;

const int PRODUCT_COUNT = 1000;
int stock[PRODUCT_COUNT];
int low_stock_count = 0;

int main() {
    srand(time(0));

    for (int i = 0; i < PRODUCT_COUNT; ++i) {
        stock[i] = rand() % 101;
    }

    for (int i = 0; i < PRODUCT_COUNT; ++i) {
        if (stock[i] < 10) {
            cout << "[ALERT] Product " << i << ": " << stock[i] << " units left\n";
            ++low_stock_count;
        }
    }

    cout << "Total low-stock items: " << low_stock_count << endl;
    cout << "Jhevan P. Baque" << endl;

    return 0;
}
