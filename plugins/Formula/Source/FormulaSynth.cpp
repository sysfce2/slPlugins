/*
  ==============================================================================

    FormulaSynth.cpp
    Created: 7 Jun 2018 8:56:26am
    Author:  Roland Rabien

  ==============================================================================
*/

#include "FormulaSynth.h"
#include "FormulaVoice.h"

FormulaSynth::FormulaSynth()
{
    for (int i = 0; i < 32; i++)
        addVoice (new FormulaVoice (*this));
}

void FormulaSynth::setMPE (bool mpe)
{
    if (mpe)
    {
        MPEZoneLayout zones;
        zones.setLowerZone (15);
        instrument->setZoneLayout (zones);
        setPitchbendTrackingMode (MPEInstrument::lastNotePlayedOnChannel);
    }
    else
    {
        instrument->enableLegacyMode();
        setPitchbendTrackingMode (MPEInstrument::allNotesOnChannel);
    }
}

void FormulaSynth::setFormulas (String f1, String f2, String f3)
{
    for (int i = getNumVoices(); --i >= 0;)
        if (auto* voice = dynamic_cast<FormulaVoice*> (getVoice (i)))
            voice->setFormulas (f1, f2, f3);
}