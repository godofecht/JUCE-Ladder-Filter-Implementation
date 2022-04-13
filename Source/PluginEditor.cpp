/**************************************************************************************************
 * Copyright (C) Abhishek Shivakumar - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Abhishek Shivakumar <abhishek.shivakumar@gmail.com>, April 2022
 *************************************************************************************************/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ShivakumarLadderFilterAudioProcessorEditor::ShivakumarLadderFilterAudioProcessorEditor (ShivakumarLadderFilterAudioProcessor& p, juce::AudioProcessorValueTreeState& vts)
: AudioProcessorEditor (&p), audioProcessor (p), valueTreeState (vts)
{
    buttonArray.addArray({ &buttonLPF12, &buttonHPF12, &buttonBPF12, &buttonLPF24, &buttonHPF24, &buttonBPF24 });

    // Setting plugin size
    setSize (600, 140);
    
    // GUI Component Initialization
    //===============================================================|
    addAndMakeVisible (cutoffFrequencySlider);
    addAndMakeVisible (resonanceSlider);
    addAndMakeVisible (driveSlider);
    
    addAndMakeVisible (cutoffFrequencyLabel);
    addAndMakeVisible (resonanceLabel);
    addAndMakeVisible (driveLabel);
    //===============================================================|
    //===============================================================|
    cutoffFrequencySlider.setRange (10.0f, 20000.0f, 0.01f);
    resonanceSlider.setRange (0.0f, 1.0f);
    driveSlider.setRange (1.0f, 2.0f);
    //===============================================================|
    cutoffFrequencyLabel.setText ("CUTOFF", juce::dontSendNotification);
    resonanceLabel.setText ("RESONANCE", juce::dontSendNotification);
    driveLabel.setText ("DRIVE", juce::dontSendNotification);
    //===============================================================|
    cutoffFrequencyLabel.attachToComponent (&cutoffFrequencySlider, true);
    resonanceLabel.attachToComponent (&resonanceSlider, true);
    driveLabel.attachToComponent (&driveSlider, true);
    //===============================================================|
    buttonLPF12.setButtonText ("LPF12");
    buttonHPF12.setButtonText ("HPF12");
    buttonBPF12.setButtonText ("BPF12");
    buttonHPF24.setButtonText ("LPF24");
    buttonLPF24.setButtonText ("HPF24");
    buttonBPF24.setButtonText ("BPF24");

    // Adding button listeners here so that clicks call buttonClicked (Button*)
    for (auto button : buttonArray)
    {
        addAndMakeVisible (button);
        button->addListener (this);
    }

    // Attaching sliders to filter parameter values so that their changes reflect on the
    // sliders and vice versa.
    cutoffAttachment.reset (new SliderAttachment (valueTreeState, "cutoff", cutoffFrequencySlider));
    resonanceAttachment.reset (new SliderAttachment (valueTreeState, "resonance", resonanceSlider));
    driveAttachment.reset (new SliderAttachment (valueTreeState, "drive", driveSlider));
}

ShivakumarLadderFilterAudioProcessorEditor::~ShivakumarLadderFilterAudioProcessorEditor()
{

}

//==============================================================================
void ShivakumarLadderFilterAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
}

void ShivakumarLadderFilterAudioProcessorEditor::resized()
{
    cutoffFrequencySlider.setBounds (100, 10, 450, 20);
    resonanceSlider.setBounds (100, 40, 450, 20);
    driveSlider.setBounds (100, 70, 450, 20);

    for (int buttonIndex = 0; buttonIndex < buttonArray.size(); ++buttonIndex)
    {

        buttonArray[buttonIndex]->setBounds (100 + 450 / 6 * buttonIndex, 120, 55, 20);
    }

}
