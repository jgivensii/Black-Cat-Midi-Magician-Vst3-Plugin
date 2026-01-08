#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "PianoRollComponent.h"
#include <cmath>

    void PianoRollComponent::setMidiBuffer(const juce::MidiBuffer& buffer)
    {
        midiBuffer = buffer;
        repaint();
    }

    void PianoRollComponent::paint(juce::Graphics& g)
    {
        g.fillAll(juce::Colours::darkgrey);
        juce::MidiBuffer::Iterator it(midiBuffer);
        

        while (it.getNextEvent(msg, samplePos))
        {
            if (msg.isNoteOn())
            {
                int pitch = msg.getNoteNumber();
                int startSample = samplePos;   // use samplePos here, not metadata.samplePosition

                // Find matching note-off
                int endSample = startSample;
                juce::MidiBuffer::Iterator it2(midiBuffer);
                juce::MidiMessage msg2;
                int pos2;
                while (it2.getNextEvent(msg2, pos2))
                {
                    if (msg2.isNoteOff() && msg2.getNoteNumber() == pitch)
                    {
                        endSample = pos2;
                        break;
                    }
                }

                int y = (getHeight() - (pitch - 60) * 5)-getHeight()/2;
                int x = startSample / 250;
                int width = std::abs((endSample - startSample)) / 500;
                int height = 5;

                g.setColour(juce::Colours::green);
                g.fillRect(x, y, width, height);

            }
        }
    }

    void PianoRollComponent::mouseDown(const juce::MouseEvent& e)
    {
        juce::File tempMidi = exportMidiFromRectangles(midiBuffer);

        juce::StringArray files;
        files.add(tempMidi.getFullPathName());

        performExternalDragDropOfFiles(files, true); // only two args
    }


    juce::File PianoRollComponent::exportMidiFromRectangles(const juce::MidiBuffer& buffer)
    {
        juce::MidiFile midi;
        midi.setTicksPerQuarterNote(9450);

        // Convert MidiBuffer into MidiMessageSequence
        juce::MidiMessageSequence seq;
        for (const auto metadata :buffer)
        {
            const auto& msg = metadata.getMessage();
            seq.addEvent(msg, metadata.samplePosition);
        }

        midi.addTrack(seq);

        juce::File tempMidi = juce::File::createTempFile("exported.mid");
        juce::FileOutputStream outStream(tempMidi);

        if (outStream.openedOk())
        {
            midi.writeTo(outStream);
            outStream.flush();
        }

        return tempMidi;
    }

