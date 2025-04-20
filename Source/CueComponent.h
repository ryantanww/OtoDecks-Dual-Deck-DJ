#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"

class CueComponent  : public Component,
                      public TableListBoxModel,
                      public Button::Listener
{
public:
    CueComponent();
    ~CueComponent() override;

    void paint(Graphics&) override;
    void resized() override;

    int getNumRows() override;
    void paintRowBackground(Graphics& g, int rowNumber, int width, int height, bool rowIsSelected) override;
    void paintCell(Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;

    Component* refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component* existingComponentToUpdate) override;

    void buttonClicked(Button* button) override;
    void getURL(URL audioURL);
    

private:

    TableListBox cueTable;
    AudioFormatManager formatManager;

    File cueSession = File::getSpecialLocation(File::userApplicationDataDirectory).getChildFile("CueSession.txt");

    URL getCueURL;
    std::vector<URL> cues;
    std::vector<String> cueTitles;
    std::vector<String> cuedTimeText;
    std::vector<double> cuedTime;
    double cueLength;
    String cueTime;
    
    bool duplicates = false;

    TextButton addCueButton{ "Add Cue" };
    FileChooser fChooser{ "Select a file..." };

    TextEditor searchPlaylist;
    Label searchLabel;

    DJAudioPlayer player{formatManager};

    int rowClicked = 0;
    int colClicked = 0;



    void deleteCue();
    void jumpCue();
    void createSession();


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CueComponent)
};
