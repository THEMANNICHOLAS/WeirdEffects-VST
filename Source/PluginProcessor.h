/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//Struct for storing current parameter values
struct ChainSettings {
    float highCutFreq{ 0 }, lowCutFreq{ 0 };
    float gain{ 0 }, dryWet{ 0 };
    float reverb{ 0 };
};
ChainSettings getChainSettings(juce::AudioProcessorValueTreeState & Tree);


//==============================================================================
/**
*/
class WeirdEffectsAudioProcessor : public juce::AudioProcessor
{
public:
    //==============================================================================
    WeirdEffectsAudioProcessor();
    ~WeirdEffectsAudioProcessor() override;

    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    //Use this method as place to do pre-playblack and initalization

    void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    //Do not interrupt this process block as it can result in pops/bangs etc.

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;


    //User declared below...



    //Creates a Value tree used to attatch parameters to sliders/knobs and with extra properties.
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    juce::AudioProcessorValueTreeState valueTree{*this, nullptr ,"Parameters", createParameterLayout() };


private:

    using Filter = juce::dsp::IIR::Filter<float>;
    using GainProcessor = juce::dsp::Gain<float>;
    //using DryWetProcessor = juce::abst
    using ReverbProcessor = juce::dsp::Reverb;
    //Don't forget to include DryWetProcessor into the chain after GainProcessor
    using MonoChain = juce::dsp::ProcessorChain<Filter, Filter, GainProcessor, ReverbProcessor>;

    //Creates a stereo chain using MonoChain, right and left
    MonoChain leftChain, rightChain;

    enum ChainPosition {
        LowCut,
        HighCut,
        Gain,
        DryWet,
        Reverb,
    };




    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WeirdEffectsAudioProcessor)
};
