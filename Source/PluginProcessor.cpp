/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
LadderFilterAudioProcessor::LadderFilterAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    addParameter(cutoffValue = new juce::AudioParameterFloat("cutoffValue", "Cutoff", 20.0f, 20000.0f, 20000.0f));
    addParameter(resonanceValue = new juce::AudioParameterFloat("resonanceValue", "Res", 0.0f, 1.0f, 0.0f));
    addParameter(driveValue = new juce::AudioParameterFloat("driveValue", "Drive", 1.0f, 3.0f, 1.0f));
    addParameter(filterModeChoice = new juce::AudioParameterChoice("filterModeChoice", "Mode", filterList, 1));
}
                 
LadderFilterAudioProcessor::~LadderFilterAudioProcessor()
{
}

//==============================================================================
const juce::String LadderFilterAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool LadderFilterAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool LadderFilterAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool LadderFilterAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double LadderFilterAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int LadderFilterAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int LadderFilterAudioProcessor::getCurrentProgram()
{
    return 0;
}

void LadderFilterAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String LadderFilterAudioProcessor::getProgramName (int index)
{
    return {};
}

void LadderFilterAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void LadderFilterAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
   // ladderFilter.setMode(LPF24);
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = 2;
    ladderFilter.prepare(spec);
    ladderFilter.reset();
//    ladderFilter.setCutoffFrequencyHz(20000.f);
//    ladderFilter.setResonance(0.0f);
//    ladderFilter.setDrive(1.0f);
    
}

void LadderFilterAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool LadderFilterAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void LadderFilterAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    
    ladderFilter.setCutoffFrequencyHz(*cutoffValue);
    ladderFilter.setResonance(*resonanceValue);
    ladderFilter.setDrive(*driveValue);
    ladderFilter.setMode(filterMode);
    //changeFilterMode(filterModeChoice);
    
    juce::dsp::AudioBlock<float> sampleBlock (buffer);
    ladderFilter.process(juce::dsp::ProcessContextReplacing<float>(sampleBlock));


    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        
        // ..do something to the data...
    }
}

//==============================================================================
bool LadderFilterAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* LadderFilterAudioProcessor::createEditor()
{
    return new LadderFilterAudioProcessorEditor (*this);
}

//==============================================================================
void LadderFilterAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    std::unique_ptr<juce::XmlElement> xml(new juce::XmlElement("LadderFilterParams"));
    xml->setAttribute("cutoffValue", static_cast<double>(*cutoffValue));
    xml->setAttribute("resonanceValue", static_cast<double>(*resonanceValue));
    xml->setAttribute("driveValue", static_cast<double>(*driveValue));
    xml->setAttribute("filterModeChoice", static_cast<int>(*filterModeChoice));
    copyXmlToBinary(*xml, destData);
}

void LadderFilterAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if(xmlState != nullptr)
    {
        if(xmlState->hasTagName("LadderFilterParams"))
        {
            *cutoffValue = xmlState->getDoubleAttribute("cutoffValue", 1000.0);
            *resonanceValue = xmlState->getDoubleAttribute("resonanceValue", 0.0);
            *driveValue = xmlState->getDoubleAttribute("driveValue", 1.2);
            *filterModeChoice = xmlState->getIntAttribute("filterModeChoice");
        }
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new LadderFilterAudioProcessor();
}
