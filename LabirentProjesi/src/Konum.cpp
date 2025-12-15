#include "Konum.hpp"


// Parametreli kurucu: Koordinatları atar
Konum::Konum(int x, int y) {
    this->x = x;
    this->y = y;
}

// Parametresiz kurucu: Varsayılan olarak 0,0 atar
Konum::Konum() {
    this->x = 0;
    this->y = 0;
}