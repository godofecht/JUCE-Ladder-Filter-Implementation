/**************************************************************************************************
 * Copyright (C) Abhishek Shivakumar - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Abhishek Shivakumar <abhishek.shivakumar@gmail.com>, April 2022
 *************************************************************************************************/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ShivakumarLadderFilterAudioProcessor::ShivakumarLadderFilterAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
parameters (*this, nullptr, "ShivakumarLadderFilter", createParameterLayout())
#endif
{ 
}

ShivakumarLadderFilterAudioProcessor::~ShivakumarLadderFilterAudioProcessor()
{
}

//==============================================================================
const juce::String ShivakumarLadderFilterAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ShivakumarLadderFilterAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool ShivakumarLadderFilterAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool ShivakumarLadderFilterAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double ShivakumarLadderFilterAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ShivakumarLadderFilterAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int ShivakumarLadderFilterAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ShivakumarLadderFilterAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String ShivakumarLadderFilterAudioProcessor::getProgramName (int index)
{
    return {};
}

void ShivakumarLadderFilterAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void ShivakumarLadderFilterAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = 2;
    ladderFilter.prepare (spec);
}

void ShivakumarLadderFilterAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ShivakumarLadderFilterAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void ShivakumarLadderFilterAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
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

    // Grab the parameter values here

    modeParameter = parameters.getRawParameterValue ("mode");
    driveParameter = parameters.getRawParameterValue ("drive");
    cutoffParameter = parameters.getRawParameterValue ("cutoff");
    resonanceParameter = parameters.getRawParameterValue ("resonance");

    if(drive != nullptr)
        ladderFilter.setDrive (*driveParameter);
    if(resonance != nullptr)
        ladderFilter.setResonance (*resonanceParameter);
    if (cutoff != nullptr)
    {
        ladderFilter.setCutoffFrequencyHz(*cutoffParameter);

        // Disable filter below 10Hz to prevent clicks and artefacting.
        if (*cutoffParameter < 10.0f)
        {
            ladderFilter.setEnabled (false);
        }
        else
        {
            ladderFilter.setEnabled (true);
        }

    }
    if (mode != nullptr)
    {
        modeValue = parameters.getParameter ("mode")->getValue();

        // When called from the editor, parameter->setValue() transmits a value between 0 and 1.
        // We need to scale the value back up by 10.0f
        modeValue = modeValue * 10.0f;
        ladderFilter.setMode (ModeList[(int) modeValue]);
    }

    juce::dsp::AudioBlock<float> block (buffer);
    juce::dsp::ProcessContextReplacing<float> context (block);
    ladderFilter.process (context);
}

//==============================================================================
bool ShivakumarLadderFilterAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* ShivakumarLadderFilterAudioProcessor::createEditor()
{
    return new ShivakumarLadderFilterAudioProcessorEditor (*this, parameters);
}

//==============================================================================
void ShivakumarLadderFilterAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void ShivakumarLadderFilterAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
 
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName (parameters.state.getType()))
            parameters.replaceState (juce::ValueTree::fromXml (*xmlState));
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ShivakumarLadderFilterAudioProcessor();
}
