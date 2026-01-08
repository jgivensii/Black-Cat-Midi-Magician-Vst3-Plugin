#include "PluginProcessor.h"
#include "PluginEditor.h"

AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor(AudioPluginAudioProcessor& p)
    : AudioProcessorEditor(&p), processorRef(p)
{
    // Preset selector
    presetSelector.addItem("Triplet Bounce (Travis Scott Inspired)", 1);
    presetSelector.addItem("Sparce Syncopation (Lil Baby Inspired)", 2);
    presetSelector.addItem("Straight 16ths (Tyga Inspired)", 3);
    presetSelector.addItem("Call and Response (Reddie Rich Inspired)", 4);
    presetSelector.addItem("Swinged Loops (Gunna Inspired)", 5);
    presetSelector.addItem("Minimalist Trap (Future Inspired)", 6);
    presetSelector.addItem("Drill Bounce (Lil Durk Inspired)", 7);
    presetSelector.addItem("Staggered Pulse (DaBaby Inspired)", 8);
    presetSelector.addItem("Lofi Trap Drift (Drake Inspired)", 9);
    presetSelector.addItem("Double-Time spark (Offset Inspired)", 10);
    addAndMakeVisible(presetSelector);

    // Scale selector
    scaleSelector.addItem("A Minor", 1);
    scaleSelector.addItem("E Minor", 2);
    scaleSelector.addItem("F Minor", 3);
    scaleSelector.addItem("G Minor", 4);
    scaleSelector.addItem("D Minor", 5);
    addAndMakeVisible(scaleSelector);

    presetSelector.onChange = [this]()
        {
            processorRef.setSelectedPreset(presetSelector.getSelectedId() - 1);
        };

    scaleSelector.onChange = [this]()
        {
            processorRef.setSelectedScale(scaleSelector.getSelectedId() - 1);
        };

    // Generate button
    generateButton.setButtonText("Generate");
    generateButton.onClick = [this]()
        {
            // Update piano roll with the buffer from MelodyEngine
            pianoRoll.setMidiBuffer(processorRef.latestMidiBuffer);

            // Also dump events into text editor
          /*  midiDisplay.clear();
            for (const auto metadata : processorRef.latestMidiBuffer)
            {
                const auto& msg = metadata.getMessage();
                if (msg.isNoteOn())
                    midiDisplay.insertTextAtCaret("NoteOn: " + juce::String(msg.getNoteNumber()) + "\n");
                else if (msg.isNoteOff())
                    midiDisplay.insertTextAtCaret("NoteOff: " + juce::String(msg.getNoteNumber()) + "\n");
            }*/
        };
    addAndMakeVisible(generateButton);

    // Clear button
    clearButton.setButtonText("Clear");
    clearButton.onClick = [this]()
        {
            processorRef.clearMidiPattern();
            midiDisplay.clear();
            pianoRoll.setMidiBuffer(juce::MidiBuffer()); // clear piano roll too
        };
    addAndMakeVisible(clearButton);

    // Display area (text)
   /* midiDisplay.setMultiLine(true);
    midiDisplay.setReadOnly(true);
    midiDisplay.setColour(juce::TextEditor::backgroundColourId, juce::Colours::darkgrey);
    midiDisplay.setColour(juce::TextEditor::textColourId, juce::Colours::white);
    addAndMakeVisible(midiDisplay);*/

    // Piano roll
    addAndMakeVisible(pianoRoll);

    // Set initial size
    setSize(600, 600);
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor() {}

void AudioPluginAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);

    g.setColour(juce::Colours::purple);
    g.setFont(18.0f);
    g.drawFittedText("Midi Magician!", getLocalBounds().removeFromTop(40),
        juce::Justification::centred, 1);
}

void AudioPluginAudioProcessorEditor::resized()
{
    presetSelector.setBounds(20, 60, 200, 30);
    scaleSelector.setBounds(240, 60, 200, 30);

    generateButton.setBounds(20, 110, 150, 40);
    clearButton.setBounds(190, 110, 150, 40);

    midiDisplay.setBounds(20, 170, 220, 200);
    pianoRoll.setBounds(75, 170, 350, 300); // side-by-side with text
}

