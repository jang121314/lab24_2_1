#include "SoftwareSerial.h"

// �̸� ���ǵ� ������. �ʿ信 ���� �� ������ ������ �� �ֽ��ϴ�.

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