/*
  ==============================================================================

    This file was auto-generated by the Introjucer!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PluginEditor::PluginEditor (PluginProcessor& p)
    : slAudioProcessorEditor (p), processor (p)
{
    addAndMakeVisible (&scope);
    
    for (slParameter* pp : p.getPluginParameters())
    {
        ParamComponent* pc;
        
        if (pp->isOnOff())
            pc = new Switch (pp);
        else
            pc = new Knob (pp);
        
        addAndMakeVisible (pc);
        controls.add (pc);
    }
    
    setGridSize (7, 2);
    
    scope.setNumChannels (p.getTotalNumInputChannels());
    
    for (auto pp : processor.getPluginParameters())
        pp->addListener (this);
    
    updateScope();
}

PluginEditor::~PluginEditor()
{
    for (auto pp : processor.getPluginParameters())
        pp->removeListener (this);
}

//==============================================================================
void PluginEditor::resized()
{
    slAudioProcessorEditor::resized();

    scope.setBounds (getGridArea (1, 0, 3, 2));
    
    componentForId (PARAM_SAMPLES_PER_PIXEL)->setBounds (getGridArea (4, 0));
    componentForId (PARAM_VERTICAL_ZOOM)->setBounds (getGridArea (4, 1));
    componentForId (PARAM_TRIGGER_CHANNEL)->setBounds (getGridArea (5, 0));
    componentForId (PARAM_TRIGGER_MODE)->setBounds (getGridArea (5, 1));
    componentForId (PARAM_TRIGGER_LEVEL)->setBounds (getGridArea (6, 0));
}

void PluginEditor::updateScope()
{
    scope.setNumSamplesPerPixel (processor.parameterIntValue (PARAM_SAMPLES_PER_PIXEL));
    scope.setVerticalZoomFactor (processor.parameterValue (PARAM_VERTICAL_ZOOM));
    scope.setTriggerChannel (processor.parameterIntValue (PARAM_TRIGGER_CHANNEL));
    scope.setTriggerMode ((drow::TriggeredScope::TriggerMode)processor.parameterIntValue (PARAM_TRIGGER_MODE));
    scope.setTriggerLevel (processor.parameterValue (PARAM_TRIGGER_LEVEL));
}
