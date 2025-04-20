#include "MainComponent.h"

MainComponent::MainComponent()
{
    setSize (800, 600);

    if (RuntimePermissions::isRequired (RuntimePermissions::recordAudio)
        && ! RuntimePermissions::isGranted (RuntimePermissions::recordAudio))
    {
        RuntimePermissions::request (RuntimePermissions::recordAudio,
                                     [&] (bool granted) { if (granted)  setAudioChannels (2, 2); });
    }  
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (0, 2);
    } 

    if (formatManager.getNumKnownFormats() == 0)
    {
        formatManager.registerBasicFormats();
    }
    

    addAndMakeVisible(deckGUI1); 
    addAndMakeVisible(deckGUI2);
    
    addAndMakeVisible(playlistComponent);

    playlistComponent.setButtonClickCallback([this](int rowNum, int colNum)
        {
            if (colNum == 4)  // Assuming columnId 4 corresponds to deck 1
            {
                deckGUI1.loadFromPlaylist(deck1, rowNum);
            }
            else if (colNum == 5)  // Assuming columnId 5 corresponds to deck 2
            {
                deckGUI2.loadFromPlaylist(deck2, rowNum);
            }
        });

}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    player1.prepareToPlay(samplesPerBlockExpected, sampleRate);
    player2.prepareToPlay(samplesPerBlockExpected, sampleRate);
    playlistPlayer.prepareToPlay(samplesPerBlockExpected, sampleRate);
    
    mixerSource.prepareToPlay(samplesPerBlockExpected, sampleRate);

    mixerSource.addInputSource(&player1, false);
    mixerSource.addInputSource(&player2, false);
    mixerSource.addInputSource(&playlistPlayer, false);

}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    mixerSource.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
    player1.releaseResources();
    player2.releaseResources();
    playlistPlayer.releaseResources();

    mixerSource.releaseResources();
}

void MainComponent::paint (Graphics& g)
{
    g.fillAll (Colours::darkcyan);
}

void MainComponent::resized()
{
    double rowH = getHeight() / 3;

    deckGUI1.setBounds(0, rowH, getWidth()/2, rowH*2);
    deckGUI2.setBounds(getWidth() / 2, rowH, getWidth()/2, rowH*2);
    playlistComponent.setBounds(0, 0, getWidth(), rowH);
}