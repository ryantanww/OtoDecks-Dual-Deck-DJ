#include <JuceHeader.h>
#include "EQProcessor.h"


EQProcessor::EQProcessor()
                        : 
                          lowFrequency(200.0f),
                          midFrequency(1000.0f), 
                          highFrequency(5000.0f)
{

}

EQProcessor::~EQProcessor()
{

}

void EQProcessor::prepareToPlay(double sampleRate, int maximumExpectedSamplesPerBlock)
{
    // Allocate resources, set up filters, etc.
    currentSampleRate = sampleRate;

    lowPassFilter.coefficients = dsp::IIR::Coefficients<float>::makeLowPass(currentSampleRate, lowFrequency);
    bandPassFilter.coefficients = dsp::IIR::Coefficients<float>::makeBandPass(currentSampleRate, midFrequency);
    highPassFilter.coefficients = dsp::IIR::Coefficients<float>::makeHighPass(currentSampleRate, highFrequency);
}

void EQProcessor::releaseResources()
{
    // Release any allocated resources.
}

void EQProcessor::processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    // Apply EQ processing to audio samples in the buffer.
    int numChannels = buffer.getNumChannels();
    int numSamples = buffer.getNumSamples();

    dsp::AudioBlock<float> audioBlock(buffer);
    dsp::ProcessContextReplacing<float> context(audioBlock);
    lowPassFilter.process(context);
    bandPassFilter.process(context);
    highPassFilter.process(context);
}

double EQProcessor::getTailLengthSeconds() const { return 0.0; }
bool EQProcessor::acceptsMidi() const { return false; }
bool EQProcessor::producesMidi() const { return false; }
AudioProcessorEditor* EQProcessor::createEditor() { return nullptr; }
bool EQProcessor::hasEditor() const { return false; }
int EQProcessor::getNumPrograms() { return 1; }
int EQProcessor::getCurrentProgram() { return 0; }
void EQProcessor::setCurrentProgram(int index) {}
const String EQProcessor::getProgramName(int index) { return "EQ Processor"; }
void EQProcessor::changeProgramName(int index, const String& newName) {}
void EQProcessor::setStateInformation(const void* data, int sizeInBytes) {}
void EQProcessor::getStateInformation(MemoryBlock& destData) {}

const String EQProcessor::getName() const
{
    return "EQ Processor";
}


void EQProcessor::setLowFrequency(float frequency)
{
    lowFrequency = frequency;
}

void EQProcessor::setMidFrequency(float frequency)
{
    midFrequency = frequency;
}

void EQProcessor::setHighFrequency(float frequency)
{
    highFrequency = frequency;
}



