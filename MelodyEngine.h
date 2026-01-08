#pragma once
#include "PluginProcessor.h"

class MelodyEngine
{
public:
    void setSampleRate(double sr) { sampleRate = sr; }
    void noteGeneration(int Userinput, int ScaleInput, juce::MidiBuffer& midiMessages);
    int startPosition(int index, double ppqBarStart, double ppqNow, double ppqPerQuarter);
    int endPosition(int index, double ppqBarStart, double ppqNow, double ppqPerQuarter);
    int notePitch(int Userinput, int ScaleInput);
    void setPlayHead(juce::AudioPlayHead* ph) { playHead = ph; }
    juce::AudioPlayHead* playHead = nullptr;
    double sampleRate = 44100.0;
   
};  