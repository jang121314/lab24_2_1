
#ifndef SOFTWARE_SERIAL_H
#define SOFTWARE_SERIAL_H

class SoftwareSerial {
public:
    SoftwareSerial(int receivePin, int transmitPin);
    void begin(long speed);
    int available();
    char read();


private:
    static const int BUFFER_SIZE = 512;
    static const char PREDEFINED_DATA[BUFFER_SIZE];
    int _index;
};

#endif // SOFTWARE_SERIAL_H