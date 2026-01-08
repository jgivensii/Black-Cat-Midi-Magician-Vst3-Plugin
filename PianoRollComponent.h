#pragma once
#include "PluginProcessor.h"

// A simple piano roll visualization component.
// It paints rectangles for each note in a MidiBuffer.
class PianoRollComponent : public juce::Component,
    public juce::DragAndDropContainer
{
    public:
        PianoRollComponent() = default;
        ~PianoRollComponent() override = default;
        juce::MidiMessage msg;
        int samplePos;

        void mouseDown(const juce::MouseEvent & e) override;

        // Set the buffer to visualize
        void setMidiBuffer(const juce::MidiBuffer & buffer);

        // JUCE paint callback
        void paint(juce::Graphics & g) override;

        // Optional: resize handling if you want to adapt grid scaling
        void resized() override {}

    private:
        juce::MidiBuffer midiBuffer; // stores the notes to draw
        juce::File exportMidiFromRectangles(const juce::MidiBuffer& buffer);
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PianoRollComponent);
    
};