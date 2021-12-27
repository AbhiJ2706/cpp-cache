#include <iostream>

#include "cache.h"

using std::cout;
using std::endl;


void hit_or_miss(bool h) {
    if (h) cout << "hit!" << endl;
    else cout << "miss!" << endl;
}

int main() {
    //set associative
    Cache x{2, 8, 4};
    hit_or_miss(x.receiveEntry("00001001", 24));
    x.update();
    hit_or_miss(x.receiveEntry("11111111", 376));
    x.update();
    hit_or_miss(x.receiveEntry("00101101", 32));
    x.update();
    hit_or_miss(x.receiveEntry("11001001", 188));
    x.update();
    hit_or_miss(x.receiveEntry("00001001", 0));
    x.update();
    cout << x << endl;
    hit_or_miss(x.receiveEntry("10001001", 1024));
    x.update();
    cout << x << endl;

    //direct-mapped
    Cache y{1, 16, 1};
    hit_or_miss(y.receiveEntry("00001001", 24));
    y.update();
    hit_or_miss(y.receiveEntry("11111111", 376));
    y.update();
    hit_or_miss(y.receiveEntry("00101101", 32));
    y.update();
    hit_or_miss(y.receiveEntry("11001001", 188));
    y.update();
    hit_or_miss(y.receiveEntry("00001001", 0));
    y.update();
    cout << y << endl;

    //fully associative
    Cache z{16, 1, 1};
    hit_or_miss(z.receiveEntry("00001001", 24));
    z.update();
    hit_or_miss(z.receiveEntry("11111111", 376));
    z.update();
    hit_or_miss(z.receiveEntry("00101101", 32));
    z.update();
    hit_or_miss(z.receiveEntry("11001001", 188));
    z.update();
    hit_or_miss(z.receiveEntry("00001001", 24));
    z.update();
    cout << z << endl;
}