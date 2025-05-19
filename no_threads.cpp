#include <iostream>
using namespace std;

void PrintWord() {
    string Words[4] = {"Be Brave", "Fortitude", "Excellence", "Uprightness"};
    for (int i = 1; i < 801; i++) {
        cout << i << ". " << Words[0] << " ";
        cout << i << ". " << Words[1] << " ";
        cout << i << ". " << Words[2] << " ";
        cout << i << ". " << Words[3] << " ";
    }
    cout << endl << "Jhevan P. Baque";
}

int main() {
    PrintWord();
    return 0;
}
