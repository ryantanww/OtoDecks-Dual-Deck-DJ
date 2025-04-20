#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include <vector>
#include <string>

class PlaylistComponent  : public Component,
                           public TableListBoxModel,
                           public Button::Listener,
                           public FileDragAndDropTarget,
                           public Timer,
                           public TextEditor::Listener
{
public:
    PlaylistComponent(DJAudioPlayer* player);
    ~PlaylistComponent() override;

    void paint (Graphics&) override;
    void resized() override;
    
    int getNumRows() override;
    void paintRowBackground(Graphics & g, int rowNumber, int width, int height, bool rowIsSelected) override;
    void paintCell(Graphics & g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;

    Component* refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component *existingComponentToUpdate) override;

    bool isInterestedInFileDrag(const StringArray& files) override;
    void filesDropped(const StringArray& files, int x, int y) override;

    void buttonClicked(Button* button) override;

    void textEditorTextChanged(TextEditor&) override;
    
    void timerCallback() override;

    using ButtonClickCallback = std::function<void(int, int)>;
    void setButtonClickCallback(ButtonClickCallback callback);

    

    std::vector<URL> tracks;
    //std::vector<std::string> tracks;
    
private:

    TableListBox playlistTable;

    //creates a text file named PlaylistSession.txt, which stores the current playlist audio file URLs so that the playlist data persists when reopening
    //std::ifstream readSession("PlaylistSession.txt");
    File playlistSession = File::getSpecialLocation(File::userApplicationDataDirectory).getChildFile("PlaylistSession.txt");

    Array<File> fileSelections;

    
    bool duplicates = false;

    TextButton loadPlaylistButton{ "LOAD AUDIO IN PLAYLIST" };    
    FileChooser fChooser{ "Select a file..." };

    TextEditor searchPlaylist;
    Label searchLabel;

    DJAudioPlayer* player;

    ButtonClickCallback buttonClickCallback;
    int rowClicked = 0;
    int colClicked = 0;


    void previewTrack();
    void deleteTrack();
    void createSession();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
