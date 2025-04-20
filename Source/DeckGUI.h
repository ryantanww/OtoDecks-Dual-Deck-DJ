#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"
#include "MyLookAndFeel.h"
#include "PlaylistComponent.h"
#include "CueComponent.h"

class DeckGUI    : public Component,
                   public Button::Listener, 
                   public Slider::Listener,
                   public Timer,
                   public TableListBoxModel
{
public:
    DeckGUI(int index, DJAudioPlayer* player, 
            AudioFormatManager & 	formatManagerToUse,
            AudioThumbnailCache & 	cacheToUse,
            PlaylistComponent* playlistComponent);
    ~DeckGUI();

    void paint (Graphics&) override;
    void resized() override;

     /** implement Button::Listener */
    void buttonClicked (Button *) override;

    /** implement Slider::Listener */
    void sliderValueChanged (Slider *slider) override;

    void timerCallback() override;

    void loadFromPlaylist(int deckNum, int rowNum);


private:

    int deckIndex;
    int playlistRowClicked = 0;
    bool loopAudio = false;

    TextButton playButton{"PLAY"}, stopButton{"STOP"},
               previousButton{"BACK"}, nextButton{"NEXT"},
               unloopButton{"UNLOOP"}, loopButton{"LOOP"},
               cueButton{"CUE"};
  
    Slider volSlider, speedSlider, posSlider;

    Label volLabel, speedLabel;

    FileChooser fChooser{"Select a file..."};

    WaveformDisplay waveformDisplay;
    WaveformDisplay playbackWaveform;

    MyLookAndFeel myLookAndFeel;

    DJAudioPlayer* player;
    PlaylistComponent* playlistComponent;

    void EQSlidersComponent();

    CueComponent cueComponent;

    int getNumRows() override;
    void paintRowBackground(Graphics& g, int rowNumber, int width, int height, bool rowIsSelected) override;
    void paintCell(Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;
    Component* refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component* existingComponentToUpdate) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
