#include "fftAux.h"

fftAux::fftAux(uint16_t numSamples, double samplingFreq)
    : SAMPLES(numSamples), SamplingFreq(samplingFreq) {
  vReal = new double[SAMPLES];
  vImag = new double[SAMPLES];
}

fftAux::~fftAux() {
  delete[] vReal;
  delete[] vImag;
}

void fftAux::ComputeFFT() {
  FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
  FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);
}

double fftAux::FindPeak() {
  double x;
  double v;
  FFT.MajorPeak(vReal, SAMPLES, SamplingFreq, &x, &v);
  return x;
}

double fftAux::FindMag() {
  double x;
  double v;
  FFT.MajorPeak(vReal, SAMPLES, SamplingFreq, &x, &v);
  return v;
}

int fftAux::findNote(double freq) {
  int notaMidi = 69 + static_cast<int>(12.0 * log2(freq / 440));
  return notaMidi;
}

// Tiró fruta el Chatgpt
int fftAux::findGuitarNote(double freq){
  int notaMidi = 0;
  if (freq <= 82.41) {
    notaMidi = 40 + static_cast<int>(12.0 * log2(freq / 82.41));
  } else if (freq <= 110.00) {
    notaMidi = 45 + static_cast<int>(12.0 * log2(freq / 110.00));
  } else if (freq <= 146.83) {
    notaMidi = 50 + static_cast<int>(12.0 * log2(freq / 146.83));
  } else if (freq <= 196.00) {
    notaMidi = 55 + static_cast<int>(12.0 * log2(freq / 196.00));
  } else if (freq <= 246.94) {
    notaMidi = 59 + static_cast<int>(12.0 * log2(freq / 246.94));
  } else if (freq <= 329.63) {
    notaMidi = 64 + static_cast<int>(12.0 * log2(freq / 329.63));
  }
  return notaMidi;
}


void fftAux::DCRemoval(){
  
  FFT.DCRemoval(vReal,SAMPLES);
  
}

