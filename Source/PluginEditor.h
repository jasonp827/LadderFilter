/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class LadderFilterAudioProcessorEditor  : public juce::AudioProcessorEditor,
                                          public juce::Slider::Listener,
                                          public juce::ComboBox::Listener,
                                          public juce::Timer

{
public:
    LadderFilterAudioProcessorEditor (LadderFilterAudioProcessor&);
    ~LadderFilterAudioProcessorEditor();

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    void sliderValueChanged(Slider* slider) override;
   // void buttonClicked(Button* button) override;
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override;
    void valueChanged(int newValue);

private:
    
    LadderFilterAudioProcessor& audioProcessor;
    
    ComboBox filterMenu;
    
    Slider cutoffSlider;
    Slider resonanceSlider;
    Slider driveSlider;
    
    Label cutoffLabel;
    Label resonanceLabel;
    Label driveLabel;
    
    void timerCallback() override;
    
   
    

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LadderFilterAudioProcessorEditor)
};
