#include "SoftwareSerial.h"

// 미리 정의된 데이터. 필요에 따라 이 값들을 변경할 수 있습니다.

SoftwareSerial::SoftwareSerial(int receivePin, int transmitPin) : _index(0) { }

void SoftwareSerial::begin(long speed) { }

int SoftwareSerial::available() {
    return BUFFER_SIZE - _index;
}

char SoftwareSerial::read() {
    if (_index < BUFFER_SIZE) {
        return PREDEFINED_DATA[_index++];
    }
    return -1;
}