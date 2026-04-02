#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class BreakoutAudioProcessorEditor : public juce::AudioProcessorEditor,
                                     private juce::Timer
{
public:
    BreakoutAudioProcessorEditor (BreakoutAudioProcessor&);
    ~BreakoutAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    void timerCallback() override;

private:
    BreakoutAudioProcessor& audioProcessor;
    
    juce::TextButton resetButton;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BreakoutAudioProcessorEditor)
};