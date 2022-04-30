/*
  =============================================================================

    This file contains the basic framework code for a JUCE plugin editor

  =============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "ChannelComponent.h"

//=============================================================================
ADHDAudioProcessorEditor::ADHDAudioProcessorEditor(ADHDAudioProcessor& p): AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    destroyDial.setRange(0.0, 1.0, 1. / 128);
    destroyDial.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    destroyDial.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    destroyDial.setLookAndFeel(&lkLeF);
    //LRButton.setToggleState(true, false);
    //linkButton.setToggleState(true, false);
    
    //ATTACHMENTS
    msAttachment.reset (new juce::AudioProcessorValueTreeState::ButtonAttachment (audioProcessor.treeState, "MIDSIDE", msLr));
    gainLAttachment.reset (new juce::AudioProcessorValueTreeState::SliderAttachment (audioProcessor.treeState, "GAINL", *channelL.getInGainDial()));
    gainRAttachment.reset (new juce::AudioProcessorValueTreeState::SliderAttachment (audioProcessor.treeState, "GAINR", *channelR.getInGainDial()));
    volumeLAttachment.reset (new juce::AudioProcessorValueTreeState::SliderAttachment (audioProcessor.treeState, "VOLUMEL", *channelL.getOutGainDial()));
    volumeRAttachment.reset (new juce::AudioProcessorValueTreeState::SliderAttachment (audioProcessor.treeState, "VOLUMER", *channelR.getOutGainDial()));
    dryWetLAttachment.reset (new juce::AudioProcessorValueTreeState::SliderAttachment (audioProcessor.treeState, "DRYWETL", *channelL.getDryWetDial()));
    dryWetRAttachment.reset (new juce::AudioProcessorValueTreeState::SliderAttachment (audioProcessor.treeState, "DRYWETR", *channelR.getDryWetDial()));
    freqLAttachment.reset (new juce::AudioProcessorValueTreeState::SliderAttachment (audioProcessor.treeState, "FREQL", *channelL.getFreqDial()));
    freqRAttachment.reset (new juce::AudioProcessorValueTreeState::SliderAttachment (audioProcessor.treeState, "FREQR", *channelR.getFreqDial()));
    qLAttachment.reset (new juce::AudioProcessorValueTreeState::SliderAttachment (audioProcessor.treeState, "QL", *channelL.getQFactorDial()));
    qRAttachment.reset (new juce::AudioProcessorValueTreeState::SliderAttachment (audioProcessor.treeState, "QR", *channelR.getQFactorDial()));
    
    eqLPLAttachment.reset (new juce::AudioProcessorValueTreeState::ButtonAttachment (audioProcessor.treeState, "EQLPL", *channelL.getFilterLP()));
    eqBPLAttachment.reset (new juce::AudioProcessorValueTreeState::ButtonAttachment (audioProcessor.treeState, "EQBPL", *channelL.getFilterBP()));
    eqHPLAttachment.reset (new juce::AudioProcessorValueTreeState::ButtonAttachment (audioProcessor.treeState, "EQHPL", *channelL.getFilterHP()));
     
    filterListLAttachment.reset (new juce::AudioProcessorValueTreeState::ComboBoxAttachment (audioProcessor.treeState, "EQSELECTL", *channelL.getFilterList()));
    filterListRAttachment.reset (new juce::AudioProcessorValueTreeState::ComboBoxAttachment (audioProcessor.treeState, "EQSELECTR", *channelR.getFilterList()));
    
    eqLPRAttachment.reset (new juce::AudioProcessorValueTreeState::ButtonAttachment (audioProcessor.treeState, "EQLPR", *channelR.getFilterLP()));
    eqBPRAttachment.reset (new juce::AudioProcessorValueTreeState::ButtonAttachment (audioProcessor.treeState, "EQBPR", *channelR.getFilterBP()));
    eqHPRAttachment.reset (new juce::AudioProcessorValueTreeState::ButtonAttachment (audioProcessor.treeState, "EQHPR", *channelR.getFilterHP()));
    
    eqOnLAttachment.reset (new juce::AudioProcessorValueTreeState::ButtonAttachment (audioProcessor.treeState, "EQONL", *channelL.getFilterOn()));
    eqOnRAttachment.reset (new juce::AudioProcessorValueTreeState::ButtonAttachment (audioProcessor.treeState, "EQONR", *channelR.getFilterOn()));
    channelOnLAttachment.reset (new juce::AudioProcessorValueTreeState::ButtonAttachment (audioProcessor.treeState, "CHANNELONL", *channelL.getChannelOn()));
    channelOnRAttachment.reset (new juce::AudioProcessorValueTreeState::ButtonAttachment (audioProcessor.treeState, "CHANNELONR", *channelR.getChannelOn()));


    destroyButton.setLookAndFeel(&dbLeF);
    linkButton.setLookAndFeel(&blLeF);
    //MSButton.setLookAndFeel(&bMSLeF);
    //LRButton.setLookAndFeel(&bLRLeF);
    msLr.setLookAndFeel(&bMSLRLeF);

    //ADD AND MAKE VISIBLE
    addAndMakeVisible(channelL);
    addAndMakeVisible(channelR);
    channelL.setBKLeF(&bkLeF);
    channelR.setBKLeF(&bkLeF);
    channelL.setLKLeF(&lkLeF);
    channelR.setLKLeF(&lkLeF);
    channelL.setOnButtonsLeF(&bOLeF);
    channelR.setOnButtonsLeF(&bOLeF);
    channelL.setLPButtonsLeF(&bLPLeF);
    channelR.setLPButtonsLeF(&bLPLeF);
    channelL.setBPButtonsLeF(&bBPLeF);
    channelR.setBPButtonsLeF(&bBPLeF);
    channelL.setHPButtonsLeF(&bHPLeF);
    channelR.setHPButtonsLeF(&bHPLeF);
    
    addAndMakeVisible(destroyDial);
    addAndMakeVisible(destroyButton);
    //addAndMakeVisible(LRButton);
    //addAndMakeVisible(MSButton);
    addAndMakeVisible(msLr);
    
    addAndMakeVisible(linkButton);

    //LISTENERS
    //destroyDial.addListener(this);
    

    setSize (1000, 400);
}

ADHDAudioProcessorEditor::~ADHDAudioProcessorEditor()
{
}

//=============================================================================
void ADHDAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.drawImageAt(juce::ImageCache::getFromMemory(BinaryData::GUI_definitivaBG_png, BinaryData::GUI_definitivaBG_pngSize), 0, 0);


    // (Our component is opaque, so we must completely fill the background with a solid colour)
    juce::Rectangle<int> channelAreaL(30, 30, 780, 160);
    juce::Rectangle<int> channelAreaR(30, 210, 780, 160);

    const int buttonDimension = 30; //px
    const int primaryDialDimension = 120; //px
    const int secondaryDialDimension = 80; //px

    int yOffset = 30;
    int xOffset = 30;

    //Buttons Bounds
    juce::Rectangle<int> linkButtonArea(109, 181, 22, 38);

    juce::Rectangle<int> channelOnArea(20+xOffset, 65 + yOffset, buttonDimension, buttonDimension);
    juce::Rectangle<int> filterOnArea(113 + xOffset, 20 + yOffset, buttonDimension, buttonDimension);
    juce::Rectangle<int> filterLPArea(147 + xOffset, 20 + yOffset, buttonDimension, buttonDimension);
    juce::Rectangle<int> filterBPArea(183 + xOffset, 20 + yOffset, buttonDimension, buttonDimension);
    juce::Rectangle<int> filterHPArea(217 + xOffset, 20 + yOffset, buttonDimension, buttonDimension);

    //Dials Bounds
    juce::Rectangle<int> freqDialArea(141 + xOffset, 59 + yOffset, secondaryDialDimension, secondaryDialDimension);
    juce::Rectangle<int> inGainDialArea(267 + xOffset, 17 + yOffset, primaryDialDimension, primaryDialDimension);
    juce::Rectangle<int> clipDialArea(388 + xOffset, 37 + yOffset, secondaryDialDimension, secondaryDialDimension);
    juce::Rectangle<int> toneDialArea(469 + xOffset, 37 + yOffset, secondaryDialDimension, secondaryDialDimension);
    juce::Rectangle<int> outGainDialArea(550 + xOffset, 17 + yOffset, primaryDialDimension, primaryDialDimension);
    juce::Rectangle<int> drywetDialArea(680 + xOffset, 37 + yOffset, secondaryDialDimension, secondaryDialDimension);

    juce::Rectangle<int> destroySectionArea(830, 30, 140, 340);

    xOffset = destroySectionArea.getX();
    yOffset = destroySectionArea.getY();

    juce::Rectangle<int> destroyButtonArea(20 + xOffset, 270 + yOffset, 100, 60);
    juce::Rectangle<int> destroyDialArea(30 + xOffset, 180 + yOffset, 80, 80);

    //juce::Rectangle<int> LRButtonArea(30 + xOffset, 20 + yOffset, 30, 30);
    //juce::Rectangle<int> MSButtonArea(78 + xOffset, 20 + yOffset, 30, 30);
    juce::Rectangle<int> msLrArea(30 + xOffset, 20 + yOffset, 78, 30);
    
    
    juce::Rectangle<int> inMeterArea(20 + xOffset, 60 + yOffset, 40, 120);
    juce::Rectangle<int> outMeterArea(80 + xOffset, 60 + yOffset, 40, 120);
    /**************************************/
    g.setColour(juce::Colours::red);
    g.drawRect(channelAreaL);
    g.drawRect(channelAreaR);
    g.drawRect(destroySectionArea);
    g.drawRect(linkButtonArea);

    g.setColour( juce::Colours::beige);
    g.drawRect(destroyButtonArea);
    g.drawRect(destroyDialArea);
    
    
    //g.drawRect(LRButtonArea);
    //g.drawRect(MSButtonArea);
    g.drawRect(msLrArea);
    
    
    g.drawRect(inMeterArea);
    g.drawRect(outMeterArea);

}

void ADHDAudioProcessorEditor::resized()
{
   // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    // Link Button Bounds
    juce::Rectangle<int> linkButtonArea(109, 181, 22, 38);

    // Channels Bounds
    juce::Rectangle<int> channelAreaL(30, 30, 780, 160);
    juce::Rectangle<int> channelAreaR(30, 210, 780, 160);

    // Destroy Bounds
    juce::Rectangle<int> destroySectionArea(830, 30, 140, 340);
    
    const int xOffset = destroySectionArea.getX();
    const int yOffset = destroySectionArea.getY();

    juce::Rectangle<int> destroyButtonArea(20 + xOffset, 270 + yOffset, 100, 60);
    juce::Rectangle<int> destroyDialArea(30 + xOffset, 180 + yOffset, 80, 80);

    //juce::Rectangle<int> LRButtonArea(30 + xOffset, 20 + yOffset, 30, 30);
    //juce::Rectangle<int> MSButtonArea(78 + xOffset, 20 + yOffset, 30, 30);
    juce::Rectangle<int> msLrArea(30 + xOffset, 20 + yOffset, 78, 30);
    // Setting Bounds
    linkButton.setBounds(linkButtonArea);

    channelL.setBounds(channelAreaL);
    channelR.setBounds(channelAreaR);

    destroyButton.setBounds(destroyButtonArea);
    destroyDial.setBounds(destroyDialArea);
    //LRButton.setBounds(LRButtonArea);
    //MSButton.setBounds(MSButtonArea);
    msLr.setBounds(msLrArea);
}
