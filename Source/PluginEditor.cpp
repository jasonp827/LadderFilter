/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
LadderFilterAudioProcessorEditor::LadderFilterAudioProcessorEditor (LadderFilterAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
    
    //FilterMenu
    filterMenu.addListener(this);
    filterMenu.setJustificationType(Justification::centred);
    filterMenu.addItem("LPF12", 1);
    filterMenu.addItem("BPF12", 2);
    filterMenu.addItem("HPF12", 3);
    filterMenu.addItem("LPF24", 4);
    filterMenu.addItem("BPF24", 5);
    filterMenu.addItem("HPF24", 6);
    filterMenu.setBounds(50, 175, 100, 50);
    addAndMakeVisible(&filterMenu);
    
    
    
    //Filter Slider
    cutoffSlider.addListener(this);
    cutoffSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    cutoffSlider.setBounds(50, 30, 100, 100);
    cutoffSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 30);
    cutoffSlider.setRange(20.f, 20000.0f, 1.f);
    cutoffSlider.isSymmetricSkew();
    cutoffSlider.setSkewFactor(0.35);
    cutoffSlider.setValue(20000.0f);
    addAndMakeVisible(cutoffSlider);
    
    cutoffLabel.setText("Cutoff", dontSendNotification);
    cutoffLabel.attachToComponent(&cutoffSlider, false);
    cutoffLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(cutoffLabel);
    
    //Resonance Slider
    resonanceSlider.addListener(this);
    resonanceSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    resonanceSlider.setBounds(150, 30, 100, 100);
    resonanceSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 30);
    resonanceSlider.setRange(0.0f, 1.0f, 0.01f);
    resonanceSlider.setValue(0.0f);
    addAndMakeVisible(resonanceSlider);
    
    resonanceLabel.setText("Res", dontSendNotification);
    resonanceLabel.attachToComponent(&resonanceSlider, false);
    resonanceLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(resonanceLabel);
    
    
    // Drive Slider
    driveSlider.addListener(this);
    driveSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    driveSlider.setBounds(250, 30, 100, 100);
    driveSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 30);
    driveSlider.setRange(1.f, 3.0f, 0.01f);
    driveSlider.setValue(1.0f);
    addAndMakeVisible(driveSlider);
    
    driveLabel.setText("Drive", dontSendNotification);
    driveLabel.attachToComponent(&driveSlider, false);
    driveLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(driveLabel);
    
    startTimer(50);
    
    
}

LadderFilterAudioProcessorEditor::~LadderFilterAudioProcessorEditor()
{
}

//==============================================================================
void LadderFilterAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (Colours::black);

}

void LadderFilterAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

void LadderFilterAudioProcessorEditor::sliderValueChanged(Slider* slider)
{
    if (slider == &cutoffSlider)
    {*audioProcessor.cutoffValue = cutoffSlider.getValue();}
    
    if (slider == &resonanceSlider)
    {*audioProcessor.resonanceValue = resonanceSlider.getValue();}
    
    if (slider == &driveSlider)
    {*audioProcessor.driveValue = driveSlider.getValue();}
}


void LadderFilterAudioProcessorEditor::comboBoxChanged(ComboBox* comboBoxThatHasChanged)
{
    if (comboBoxThatHasChanged == &filterMenu)
    {
        switch(filterMenu.getSelectedId())
        {
            case 1: *audioProcessor.filterModeChoice = filterMenu.getSelectedId();
                audioProcessor.filterMode = juce::dsp::LadderFilterMode::LPF12;
                break;
            case 2: *audioProcessor.filterModeChoice = filterMenu.getSelectedId();
                audioProcessor.filterMode = juce::dsp::LadderFilterMode::BPF12;
                break;
            case 3: *audioProcessor.filterModeChoice = filterMenu.getSelectedId();
                audioProcessor.filterMode = juce::dsp::LadderFilterMode::HPF12;
                break;
            case 4: *audioProcessor.filterModeChoice = filterMenu.getSelectedId();
                audioProcessor.filterMode = juce::dsp::LadderFilterMode::LPF24;
                break;
            case 5: *audioProcessor.filterModeChoice = filterMenu.getSelectedId();
                audioProcessor.filterMode = juce::dsp::LadderFilterMode::BPF24;
                break;
            case 6: *audioProcessor.filterModeChoice = filterMenu.getSelectedId();
                audioProcessor.filterMode = juce::dsp::LadderFilterMode::HPF24;
                break;
        }
    }
}

void LadderFilterAudioProcessorEditor::timerCallback()
{
    //This function will update the shown parameter on the GUI to represent values being automated
    cutoffSlider.setValue(*audioProcessor.cutoffValue, dontSendNotification);
    resonanceSlider.setValue(*audioProcessor.resonanceValue, dontSendNotification);
    driveSlider.setValue(*audioProcessor.driveValue, dontSendNotification);
    filterMenu.setSelectedId(*audioProcessor.filterModeChoice, dontSendNotification);
    //This function updates toggleButton everytime it is automated

}

void LadderFilterAudioProcessorEditor::valueChanged(int newValue)
{
    filterMenu.setSelectedId(newValue);
}
