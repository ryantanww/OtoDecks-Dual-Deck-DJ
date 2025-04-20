#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DJAudioPlayer.h"
#include "DeckGUI.h"
#include "PlaylistComponent.h"

class MainComponent   : public AudioAppComponent
{
public:
    MainComponent();
    ~MainComponent();

    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    void paint (Graphics& g) override;
    void resized() override;


private:
    int deck1 = 1;
    int deck2 = 2;
    AudioFormatManager formatManager;
    AudioThumbnailCache thumbCache{100};

    DJAudioPlayer player1{formatManager};
    DeckGUI deckGUI1{deck1, &player1, formatManager, thumbCache, &playlistComponent};

    DJAudioPlayer player2{formatManager};
    DeckGUI deckGUI2{deck2, &player2, formatManager, thumbCache, &playlistComponent};

    MixerAudioSource mixerSource; 
    
    DJAudioPlayer playlistPlayer{formatManager};
    PlaylistComponent playlistComponent{&playlistPlayer};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
