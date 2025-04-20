#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class DJAudioPlayer : public AudioSource {
  public:

    DJAudioPlayer(AudioFormatManager& _formatManager);
    ~DJAudioPlayer();

    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    void loadURL(URL audioURL);
    void setGain(double gain);
    void setSpeed(double ratio);
    void setPosition(double posInSecs);
    void setPositionRelative(double pos);
    

    void start();
    void stop();


    /** get the relative position of the playhead */
    double getPositionRelative();
    double getCuePosition();

    void getAudioDetails(URL track);
    std::vector<String> trackTitles;
    std::vector<String> searchTrackTitles;
    std::vector<String> trackLength;
    std::vector<String> searchTrackLength;


    URL currAudioURL;
    void getCueDetails(URL audioURL);
    std::vector<String> cueTitles;
    std::vector<String> cuedTimeText;
    std::vector<double> cuedTime;


private:
    AudioFormatManager& formatManager;
    
    std::unique_ptr<AudioFormatReaderSource> readerSource;
    AudioTransportSource transportSource; 
    ResamplingAudioSource resampleSource{&transportSource, false, 2};
};





