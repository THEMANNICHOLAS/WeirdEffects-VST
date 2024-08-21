/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
WeirdEffectsAudioProcessor::WeirdEffectsAudioProcessor()
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
}

WeirdEffectsAudioProcessor::~WeirdEffectsAudioProcessor()
{
}

//==============================================================================
const juce::String WeirdEffectsAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool WeirdEffectsAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool WeirdEffectsAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool WeirdEffectsAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double WeirdEffectsAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int WeirdEffectsAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int WeirdEffectsAudioProcessor::getCurrentProgram()
{
    return 0;
}

void WeirdEffectsAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String WeirdEffectsAudioProcessor::getProgramName (int index)
{
    return {};
}

void WeirdEffectsAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void WeirdEffectsAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void WeirdEffectsAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool WeirdEffectsAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
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

void WeirdEffectsAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    //This Code handles the Audio Buffer
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
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        // ..do something to the data...
    }


}

//==============================================================================
bool WeirdEffectsAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor * WeirdEffectsAudioProcessor::createEditor()
{

    return new juce::GenericAudioProcessorEditor(*this);
}

//==============================================================================
void WeirdEffectsAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void WeirdEffectsAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

 juce::AudioProcessorValueTreeState::ParameterLayout WeirdEffectsAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    
    //Creates Audio Parameter for volume. -32 is 32 decibles quieter, 0 is default, +6 is 6 more decibels.
    layout.add(std::make_unique<juce::AudioParameterFloat>
        (juce::ParameterID("Gain"),
         juce::String("Gain"), 
         juce::NormalisableRange<float>(-32.f, 6.f, .5f, 1.f), 0.f));

    //Creates Audio Parameter for dry/wet of entire effect. 0 being 0%, 100 being 100%, 50 being 50%, the default
    
    layout.add(std::make_unique<juce::AudioParameterFloat>
        (juce::ParameterID("Dry/Wet"),
         juce::String("Dry/Wet"), 
         juce::NormalisableRange<float>(0.f, 100.f, 1.f, 1.f), 50.f));
    

    //Creates Audio Parameter for controlling amount of reverb.
    
    layout.add(std::make_unique<juce::AudioParameterFloat>
        (juce::ParameterID("Reverb"),
         juce::String("Reverb"),
         juce::NormalisableRange<float>(0.f, 100.f, 1.f, 1.f), 0.f));
         

    //Creates LowCut frequency parameter for cutting the low end
    layout.add(std::make_unique<juce::AudioParameterFloat>
        (juce::ParameterID("LowCut Freq"),
         juce::String("LowCut Freq"),
         juce::NormalisableRange<float>(20.f, 20000.f,1.f, 1.f), 20.f));

    //Creates HighCut frequency parameter for cutting out high end
    layout.add(std::make_unique<juce::AudioParameterFloat>
        (juce::ParameterID("HighCut Freq"),
         juce::String("HighCut Freq"),
         juce::NormalisableRange<float>(20.f, 20000.f, 1.f, 1.f), 20000.f));

   

    return layout;
    
}


//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new WeirdEffectsAudioProcessor();
}
