#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <ctime>

using namespace std;

mutex mtx;
int low_stock_count = 0;

void check_stock(const vector<int>& stock, int start, int end) {
    for (int i = start; i <= end; ++i) {
        if (stock[i] < 10) {
            lock_guard<mutex> lock(mtx);
            cout << "[ALERT] Product " << i << ": " << stock[i] << " units left\n";
            ++low_stock_count;
        }
    }
}

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));
    vector<int> stock(1000);

    for (int i = 0; i < 1000; ++i) {
        stock[i] = rand() % 101;
    }

    thread t1(check_stock, cref(stock), 0, 249);
    thread t2(check_stock, cref(stock), 250, 499);
    thread t3(check_stock, cref(stock), 500, 749);
    thread t4(check_stock, cref(stock), 750, 999);

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    cout << "Total low-stock items: " << low_stock_count << endl;
    cout << "Jhevan P. Baque";
    return 0;
}
