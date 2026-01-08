#include "PianoRollComponent.h"

class AudioPluginAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    AudioPluginAudioProcessorEditor (AudioPluginAudioProcessor&);
    ~AudioPluginAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    AudioPluginAudioProcessor& processorRef;

    juce::ComboBox presetSelector;
    juce::ComboBox scaleSelector;

    juce::TextButton generateButton { "Generate MIDI" };
    juce::TextButton clearButton { "Clear MIDI" };

    juce::TextEditor midiDisplay;
    PianoRollComponent pianoRoll;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginAudioProcessorEditor)
};