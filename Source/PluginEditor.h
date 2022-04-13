/**************************************************************************************************
 * Copyright (C) Abhishek Shivakumar - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Abhishek Shivakumar <abhishek.shivakumar@gmail.com>, April 2022
 *************************************************************************************************/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/



class ShivakumarLadderFilterAudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::Button::Listener
{
public:
    ShivakumarLadderFilterAudioProcessorEditor (ShivakumarLadderFilterAudioProcessor&, juce::AudioProcessorValueTreeState&);
    ~ShivakumarLadderFilterAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    ShivakumarLadderFilterAudioProcessor& audioProcessor;

    juce::AudioProcessorValueTreeState& valueTreeState;
    
    juce::Slider cutoffFrequencySlider, resonanceSlider, driveSlider;
    
    juce::Label cutoffFrequencyLabel, resonanceLabel, driveLabel;
    juce::TextButton buttonLPF12, buttonHPF12, buttonBPF12, buttonLPF24, buttonHPF24, buttonBPF24;

    typedef juce::AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
    typedef juce::AudioProcessorValueTreeState::ButtonAttachment ButtonAttachment;

    std::unique_ptr<SliderAttachment> cutoffAttachment, resonanceAttachment, driveAttachment;

    juce::OwnedArray <juce::TextButton> buttonArray; 

    // Toggles all button states to off
    void resetButtons()
    {
        for (auto button : buttonArray)
        {
            button->setToggleState (false, juce::dontSendNotification);
        }
    }

    // This function is called when a button is clicked
    // and it sets the modeValue to a number between 0.0f and 0.5f.
    // The modeValue is used to set the filter type.
    void buttonClicked (juce::Button* b) override
    {
        resetButtons();
        for (int buttonIndex = 0; buttonIndex < buttonArray.size(); ++buttonIndex)
        {
            if (buttonArray[buttonIndex] == b)
            {
                audioProcessor.parameters.getParameter ("mode")->setValue((float) buttonIndex * 0.1f); b->setToggleState (true, juce::dontSendNotification); 
            };
        }
    }
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ShivakumarLadderFilterAudioProcessorEditor)
};
