#include "DJAudioPlayer.h"

DJAudioPlayer::DJAudioPlayer(AudioFormatManager& _formatManager)
                            : formatManager(_formatManager)
{
    //formatManager.registerBasicFormats();
    currAudioURL = URL();
}
DJAudioPlayer::~DJAudioPlayer()
{

}

void DJAudioPlayer::prepareToPlay (int samplesPerBlockExpected, double sampleRate) 
{
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}
void DJAudioPlayer::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    if (readerSource.get() == nullptr)
    {
        bufferToFill.clearActiveBufferRegion();
        return;
    }

    resampleSource.getNextAudioBlock(bufferToFill);
    
}
void DJAudioPlayer::releaseResources()
{
    transportSource.releaseResources();
    resampleSource.releaseResources();
}

void DJAudioPlayer::loadURL(URL audioURL)
{
    currAudioURL = audioURL;
    auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));
    if (reader != nullptr) // good file!
    {       
        std::unique_ptr<AudioFormatReaderSource> newSource (new AudioFormatReaderSource (reader, true)); 
        transportSource.setSource (newSource.get(), 0, nullptr, reader->sampleRate);             
        readerSource.reset (newSource.release()); 
        
    }
}
void DJAudioPlayer::setGain(double gain)
{
    if (gain < 0 || gain > 1.0)
    {
        std::cout << "DJAudioPlayer::setGain gain should be between 0 and 1" << std::endl;
    }
    else 
    {
        transportSource.setGain(gain);
    }
}
void DJAudioPlayer::setSpeed(double ratio)
{
    if (ratio < 0 || ratio > 5.0)
    {
        std::cout << "DJAudioPlayer::setSpeed ratio should be between 0 and 5" << std::endl;
    }
    else 
    {
        resampleSource.setResamplingRatio(ratio);
    }
}
void DJAudioPlayer::setPosition(double posInSecs)
{
    transportSource.setPosition(posInSecs);
}

void DJAudioPlayer::setPositionRelative(double pos)
{
    if (pos < 0 || pos > 1.0)
    {
        std::cout << "DJAudioPlayer::setPositionRelative pos should be between 0 and 1" << std::endl;
    }
    else 
    {
        double posInSecs = transportSource.getLengthInSeconds() * pos;
        setPosition(posInSecs);
    }
}


void DJAudioPlayer::start()
{
    transportSource.start();
}

void DJAudioPlayer::stop()
{
    transportSource.stop();
}

double DJAudioPlayer::getPositionRelative()
{
    return transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();
}

double DJAudioPlayer::getCuePosition()
{
    return transportSource.getCurrentPosition();
}

void DJAudioPlayer::getAudioDetails(URL track)
{
    int length = 0;
    int hrs = 0;
    int mins = 0;
    int secs = 0;

    if (formatManager.getNumKnownFormats() == 0)
    {
        formatManager.registerBasicFormats();
    }
    auto* reader = formatManager.createReaderFor(track.createInputStream(false));
    if (reader != nullptr) // good file!
    {
        std::unique_ptr<AudioFormatReaderSource> newSource(new AudioFormatReaderSource(reader, true));
        transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
        readerSource.reset(newSource.release());
        length = transportSource.getLengthInSeconds();
        hrs = length / 3600;
        mins = ((length - hrs * 3600) / 60);
        secs = (length - hrs * 3600 - mins * 60);
        String secondsToTime = String::formatted("%02d:%02d:%02d", hrs, mins, secs);

        trackTitles.push_back(track.getFileName().toStdString());
        trackLength.push_back(secondsToTime);
    }
    searchTrackTitles = trackTitles;
    searchTrackLength = trackLength;
}


void DJAudioPlayer::getCueDetails(URL audioURL)
{
    double relLength = transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();
    int length = 0;
    int hrs = 0;
    int mins = 0;
    int secs = 0;
    std::cout << "DJAudioPlayer:: getCueLength: " << relLength << std::endl;
    if (formatManager.getNumKnownFormats() == 0)
    {
        formatManager.registerBasicFormats();
    }
    auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));
    if (reader != nullptr) // good file!
    {
        std::unique_ptr<AudioFormatReaderSource> newSource(new AudioFormatReaderSource(reader, true));
        transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
        readerSource.reset(newSource.release());
        length = transportSource.getCurrentPosition();

        hrs = length / 3600;
        mins = ((length - hrs * 3600) / 60);
        secs = (length - hrs * 3600 - mins * 60);

        String secondsToTime = String::formatted("%02d:%02d:%02d", hrs, mins, secs);
        std::cout << "DJAudioPlayer:: secondsToTime: " << secondsToTime << std::endl;
        cueTitles.push_back(audioURL.getFileName().toStdString());
        cuedTimeText.push_back(secondsToTime);
        cuedTime.push_back(relLength);
    }
}



