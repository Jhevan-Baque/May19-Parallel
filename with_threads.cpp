#include <iostream>
#include <thread>
using namespace std;

void PrintWords(int value) {
    string Words[4] = {"Be Brave", "Fortitude", "Excellence", "Uprightness"};
    for (int i = 1; i < 801; i++) {
        cout << " " << Words[value] << " ";
        this_thread::sleep_for(chrono::milliseconds(200));
    }
}

void ThreadedPrint() {
    thread t1(PrintWords, 0);
    thread t2(PrintWords, 1);
    thread t3(PrintWords, 2);
    thread t4(PrintWords, 3);

    t1.join();
    t2.join();
    t3.join();
    t4.join();
    cout << endl << "Jhevan P. Baque";
}

int main() {
    ThreadedPrint();
    return 0;
}
