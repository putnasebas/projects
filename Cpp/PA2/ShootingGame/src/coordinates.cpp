#include "coordinates.h"

using namespace std;

Coordinates::Coordinates(size_t X, size_t Y)
        : x(X), y(Y) {}

bool Coordinates::operator<(const Coordinates &compare) const {
    if (y == compare.y) {
        return x < compare.x;
    }
    return y < compare.y;
}

ostream &operator<<(ostream &os, const Coordinates &data) {
    os << data.x << "," << data.y;
    return os;
}

bool Coordinates::operator==(const Coordinates &compare) const {
    return x == compare.x && y == compare.y;
}

bool Coordinates::operator!=(const Coordinates &compare) const {
    return !(*this == compare);
}