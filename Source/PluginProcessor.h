/**************************************************************************************************
 * Copyright (C) Abhishek Shivakumar - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Abhishek Shivakumar <abhishek.shivakumar@gmail.com>, April 2022
 *************************************************************************************************/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class ShivakumarLadderFilterAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    ShivakumarLadderFilterAudioProcessor();
    ~ShivakumarLadderFilterAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

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
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    juce::ValueTree& getChildState (int index)
    {
        juce::Identifier id = juce::String (index);
        valueTree = parameters.state.getOrCreateChildWithName (id, nullptr);
        return valueTree;
    }
    
    void restorePluginStateFromValueTree (const juce::ValueTree& stateToLoad)
    {
        auto v = stateToLoad.createCopy();
        //restore plugin state here
    }
    
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout ()
    {

        juce::AudioProcessorValueTreeState::ParameterLayout params;


        params.add (std::make_unique<juce::AudioParameterFloat> ("mode", "mode", juce::NormalisableRange<float> (0.0f, 6.0f), 3.0f));
        params.add (std::make_unique<juce::AudioParameterFloat> ("cutoff", "cutoff", juce::NormalisableRange<float> (10.0f, 20000.0f), 10.0f));
        params.add (std::make_unique<juce::AudioParameterFloat> ("resonance", "resonance", juce::NormalisableRange<float> (0.0f, 1.0f), 0.22f));
        params.add (std::make_unique<juce::AudioParameterFloat> ("drive", "drive", juce::NormalisableRange<float> (1.0f, 2.0f), 1.0f));


        return params;
    }
    
    std::vector<juce::dsp::LadderFilterMode> ModeList { juce::dsp::LadderFilterMode::LPF12,
                                                        juce::dsp::LadderFilterMode::HPF12,
                                                        juce::dsp::LadderFilterMode::BPF12,
                                                        juce::dsp::LadderFilterMode::LPF24,
                                                        juce::dsp::LadderFilterMode::HPF24,
                                                        juce::dsp::LadderFilterMode::BPF24
        
    };
    
    juce::dsp::LadderFilter<float> ladderFilter;

    juce::AudioParameterFloat* cutoff;
    juce::AudioParameterFloat* resonance;
    juce::AudioParameterFloat* drive;
    juce::AudioParameterFloat* mode;
      
    juce::AudioProcessorValueTreeState parameters;
    juce::ValueTree valueTree;

private:

    float modeValue = 0.1f;

    std::atomic<float>* cutoffParameter = nullptr;
    std::atomic<float>* resonanceParameter = nullptr;
    std::atomic<float>* driveParameter = nullptr;
    std::atomic<float>* modeParameter = nullptr;



    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ShivakumarLadderFilterAudioProcessor)
};
