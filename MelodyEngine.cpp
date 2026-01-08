#include "PluginProcessor.h"
#include "MelodyEngine.h"
#include "TrapstylePreset.h"
#include "ScaleManager.h"
#include <iostream>
#include <random>

TrapstylePreset styles; 


void MelodyEngine::noteGeneration(int Userinput, int ScaleInput, juce::MidiBuffer& midiMessages)
{
	auto chosenStyle = trapStyles[Userinput];
	double ppqBarStart = 0.0;
	double ppqNow = 0.0;
	double ppqPerQuarter = 1.0;
	for (int i = 0; i <= chosenStyle.rhythmPattern.size()-1; ++i)
	{
		if (chosenStyle.rhythmPattern[i] == 1)
		{
			int pitch = juce::jlimit(0, 127, notePitch(Userinput, ScaleInput));
			juce::uint8 velocity = (juce::uint8)juce::jlimit(0, 127, 100);
			juce::MidiBuffer generated;

			midiMessages.addEvent(
				juce::MidiMessage::noteOn(1, pitch, velocity),
			startPosition(i, ppqBarStart, ppqNow, ppqPerQuarter)
			);

			midiMessages.addEvent(
				juce::MidiMessage::noteOff(1, pitch, (juce::uint8)0),
				endPosition(i, ppqBarStart, ppqNow, ppqPerQuarter)
			);
			midiMessages.addEvents(generated, startPosition(i, ppqBarStart, ppqNow, ppqPerQuarter), endPosition(i, ppqBarStart, ppqNow, ppqPerQuarter), 0);
		}
	}

}

int MelodyEngine::startPosition(int index, double ppqBarStart, double ppqNow, double ppqPerQuarter)
{
	juce::AudioPlayHead::CurrentPositionInfo posInfo;
	if (auto ph = playHead)
		ph->getCurrentPosition(posInfo);
	double bpm = (posInfo.bpm > 0.0 ? posInfo.bpm : 120.0); // fallback

	double ppqPerSixteenth = ppqPerQuarter / 4.0;
	double samplesPerQuarterNote = sampleRate * 60.0 / bpm;
	int samplesPerSixteenth = (int)(samplesPerQuarterNote / 4.0);
	double stepPPQ = ppqBarStart + index * ppqPerSixteenth;
	int samplePos = (int)((stepPPQ - ppqNow) * samplesPerQuarterNote);
	samplePos = std::max(0, samplePos);
	return samplePos;
}

int MelodyEngine::endPosition(int index, double ppqBarStart, double ppqNow, double ppqPerQuarter)
{
	juce::AudioPlayHead::CurrentPositionInfo posInfo;
	if (auto ph = playHead)
		ph->getCurrentPosition(posInfo);

	double bpm = (posInfo.bpm > 0.0 ? posInfo.bpm : 120.0); // fallback
	double ppqPerSixteenth = ppqPerQuarter / 4.0;
	double samplesPerQuarterNote = sampleRate * 60.0 / bpm;
	int samplesPerSixteenth = (int)(samplesPerQuarterNote / 4.0);
	double stepPPQ = ppqBarStart + index * ppqPerSixteenth;
	int samplePos = (int)((stepPPQ - ppqNow) * samplesPerQuarterNote);
	double notelengthPPQ = ppqPerSixteenth;
	int noteLengthSamples = (int)(notelengthPPQ * samplesPerQuarterNote);
	samplePos = std::max(0, samplePos);
	return std::max(samplePos + noteLengthSamples, samplePos + 1);

}

int MelodyEngine::notePitch(int Userinput, int ScaleInput)
{
	auto ChosenScale = scales[ScaleInput];
	int BiasSize = trapStyles[Userinput].intervalBias.size();
	int rootNote = ChosenScale.rootMidiValue;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(0,BiasSize-1);
	int nextNote = trapStyles[Userinput].intervalBias[dist(gen)];

	return rootNote + nextNote;
}