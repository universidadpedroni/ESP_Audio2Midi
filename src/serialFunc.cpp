#include "SerialFunc.h"
#include "midiNotesChart.h"

void SerialFuncClass::serialSendFFT(double *vReal, int numSamples){
  
    Serial.println(ENCABEZADO_TRAMA);
    for(int i = 0; i < numSamples; i++)
    {
        Serial.println((int)vReal[i]);
        
        
    }
    Serial.println(FIN_TRAMA);
    
}


void SerialFuncClass::serialInfo(double freq, double mag, int notaMidi, int interval) {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    Serial.printf("Freq: %.2f Hz, Mag: %.2f V, Nota: %d, %s\n", freq, mag, notaMidi, NOTAS[notaMidi]);
  }
}

void SerialFuncClass::serialInfo(double freq, double mag, int notaMidi) {
  Serial.printf("Freq: %.2f Hz, Mag: %.2f V, Nota: %d, %s\n", freq, mag, notaMidi, NOTAS[notaMidi]);
}

void SerialFuncClass::sendMIDI(HardwareSerial &SerialMidi, int cmd, int data1, int data2, int MIDI_CHANNEL)
{
  // plays a MIDI note or Control change..  Doesn't check to see that
  // cmd is greater than 127, or that data values are  less than 127:
  // Examples: 
  // cmd: NOTE_ON, data1: pitch, data2: 0 -127
  // cmd: NOTE_OFF, data1: pitch, data2: 0
  // cmd: CONTROL_CHANGE, data1: SUSTAIN, data2: 0 - 127
  cmd = cmd | char(MIDI_CHANNEL - 1);    // merge channel number
  
	SerialMidi.write(cmd);
	SerialMidi.write(data1);
	SerialMidi.write(data2);
}

SerialFuncClass SerialFunc;
