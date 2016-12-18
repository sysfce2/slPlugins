/*
  ==============================================================================

    This file was auto-generated by the Introjucer!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
slToneAudioProcessor::slToneAudioProcessor()
{
    addPluginParameter (new slParameter (PARAM_ENABLE,       "Enable",       "",     0.0f,      1.0f, 1.0f,    1.0f, 1.0f));
    addPluginParameter (new slParameter (PARAM_FREQ,         "Frequency",    "Hz",   30.0f, 10000.0f, 0.0f, 1000.0f, 0.5f));
    addPluginParameter (new slParameter (PARAM_SINE_LEVEL,   "Sine",         "dB", -100.0f,     6.0f, 0.0f, -100.0f, 5.f));
    addPluginParameter (new slParameter (PARAM_TRI_LEVEL,    "Triangle",     "dB", -100.0f,     6.0f, 0.0f, -100.0f, 5.f));
    addPluginParameter (new slParameter (PARAM_SAW_LEVEL,    "Saw",          "dB", -100.0f,     6.0f, 0.0f, -100.0f, 5.f));
    addPluginParameter (new slParameter (PARAM_SQUARE_LEVEL, "Square",       "dB", -100.0f,     6.0f, 0.0f, -100.0f, 5.f));
    addPluginParameter (new slParameter (PARAM_NOISE_LEVEL,  "Noise",        "dB", -100.0f,     6.0f, 0.0f, -100.0f, 5.f));
    
    sine.setParameters     (ToneGen::Parameters (0, 0, 0, ToneGen::Sine));
    triangle.setParameters (ToneGen::Parameters (0, 0, 0, ToneGen::Triangle));
    saw.setParameters      (ToneGen::Parameters (0, 0, 0, ToneGen::Saw));
    square.setParameters   (ToneGen::Parameters (0, 0, 0, ToneGen::Square));
}

slToneAudioProcessor::~slToneAudioProcessor()
{
}

//==============================================================================
void slToneAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    scratch.setSize (1, samplesPerBlock);
    
    sine.setSampleRate (float (sampleRate));
    triangle.setSampleRate (float (sampleRate));
    saw.setSampleRate (float (sampleRate));
    square.setSampleRate (float (sampleRate));

    enableVal.reset (sampleRate, 0.05);
    sineVal.reset (sampleRate, 0.05);
    triangleVal.reset (sampleRate, 0.05);
    sawVal.reset (sampleRate, 0.05);
    squareVal.reset (sampleRate, 0.05);
}

void slToneAudioProcessor::releaseResources()
{
}

void slToneAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer&)
{
    int numSamples = buffer.getNumSamples();
    scratch.setSize (1, numSamples, false, false, true);
    
    float freq = getParameter (PARAM_FREQ)->getUserValue();
    
    sine.setParameters     (ToneGen::Parameters (freq, 0, 0, ToneGen::Sine));
    triangle.setParameters (ToneGen::Parameters (freq, 0, 0, ToneGen::Triangle));
    saw.setParameters      (ToneGen::Parameters (freq, 0, 0, ToneGen::Saw));
    square.setParameters   (ToneGen::Parameters (freq, 0, 0, ToneGen::Square));

    enableVal.setValue (getParameter (PARAM_ENABLE)->getUserValue());
    sineVal.setValue (Decibels::decibelsToGain (getParameter (PARAM_SINE_LEVEL)->getUserValue()));
    triangleVal.setValue (Decibels::decibelsToGain (getParameter (PARAM_TRI_LEVEL)->getUserValue()));
    sawVal.setValue (Decibels::decibelsToGain (getParameter (PARAM_SAW_LEVEL)->getUserValue()));
    squareVal.setValue (Decibels::decibelsToGain (getParameter (PARAM_SQUARE_LEVEL)->getUserValue()));
    noiseVal.setValue (Decibels::decibelsToGain (getParameter (PARAM_NOISE_LEVEL)->getUserValue()));

    float* scratchSamples = scratch.getWritePointer (0);

    sine.processBlock (scratchSamples, numSamples);
    applyGain (scratch, sineVal);
    buffer.addFrom (0, 0, scratchSamples, numSamples);
    
    triangle.processBlock (scratchSamples, numSamples);
    applyGain (scratch, triangleVal);
    buffer.addFrom (0, 0, scratchSamples, numSamples);
    
    saw.processBlock (scratchSamples, numSamples);
    applyGain (scratch, sawVal);
    buffer.addFrom (0, 0, scratchSamples, numSamples);
    
    square.processBlock (scratchSamples, numSamples);
    applyGain (scratch, squareVal);
    buffer.addFrom (0, 0, scratchSamples, numSamples);
    
    noise.copyNoiseToBuffer (scratchSamples, numSamples);
    applyGain (scratch, noiseVal);
    buffer.addFrom (0, 0, scratchSamples, numSamples);
    
    applyGain (buffer, enableVal);
    
    outputLevel.trackBuffer (buffer);
}

//==============================================================================
bool slToneAudioProcessor::hasEditor() const
{
    return true;
}

AudioProcessorEditor* slToneAudioProcessor::createEditor()
{
    return new slToneAudioProcessorEditor (*this);
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new slToneAudioProcessor();
}
