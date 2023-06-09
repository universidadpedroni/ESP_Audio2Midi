#ifndef SERIALFUNC_H
#define SERIALFUNC_H

#include <Arduino.h>

const int ENCABEZADO_TRAMA = -11;
const int FIN_TRAMA = -22;

class SerialFuncClass {
  public:
    void serialInfo(double freq, double mag, int notaMidi, int interval);
    void serialInfo(double freq, double mag, int notaMidi);
    void sendMIDI(HardwareSerial &SerialMidi, int cmd, int data1, int data2, int MIDI_CHANNEL);
    void serialSendFFT(double *vReal, int numSamples);
  private:
    unsigned long previousMillis;
};

extern SerialFuncClass SerialFunc;

#endif
