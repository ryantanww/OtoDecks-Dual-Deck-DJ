#pragma once

#include <JuceHeader.h>

class EQProcessor : public AudioProcessor
{
public:
    EQProcessor();
    ~EQProcessor() override;

    void prepareToPlay(double sampleRate, int maximumExpectedSamplesPerBlock) override;
    void releaseResources() override;
    void processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages) override;

    double getTailLengthSeconds() const override;
    bool acceptsMidi() const override;
    bool producesMidi() const override;

    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const String getProgramName(int index) override;
    void changeProgramName(int index, const String& newName) override;
    void setStateInformation(const void* data, int sizeInBytes) override;
    void getStateInformation(MemoryBlock& destData) override;

    const String getName() const override;

    void setLowFrequency(float frequency);
    void setMidFrequency(float frequency);
    void setHighFrequency(float frequency);

private:

    float lowFrequency;
    float midFrequency;
    float highFrequency;
    double currentSampleRate;

    dsp::IIR::Filter<float> lowPassFilter;
    dsp::IIR::Filter<float> bandPassFilter;
    dsp::IIR::Filter<float> highPassFilter;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EQProcessor)
};
