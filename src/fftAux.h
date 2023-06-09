#ifndef FFTAUX_H
#define FFTAUX_H

#include <ArduinoFFT.h>
class fftAux {
  private:
    arduinoFFT FFT;
    const uint16_t SAMPLES;
    const double SamplingFreq;

  public:
    double* vReal;
    double* vImag;
    fftAux(uint16_t numSamples, double samplingFreq);
    ~fftAux();
    void ComputeFFT();
    double FindPeak();
    double FindMag();
    int findNote(double freq);
    int findGuitarNote(double freq);
    void DCRemoval();
};


#endif