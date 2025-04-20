#include <JuceHeader.h>
#include "PlaylistComponent.h"



PlaylistComponent::PlaylistComponent(DJAudioPlayer* _player) : player(_player)
{    
    playlistTable.getHeader().addColumn("Track title", 1, 250);
    playlistTable.getHeader().addColumn("Length(hh:mm:ss)", 2, 100);
    playlistTable.getHeader().addColumn("Preview", 3, 100);
    playlistTable.getHeader().addColumn("Load Player 1", 4, 125);
    playlistTable.getHeader().addColumn("Load Player 2", 5, 125);
    playlistTable.getHeader().addColumn("Delete", 6, 100);

    addAndMakeVisible(playlistTable);
    playlistTable.setModel(this);

    addAndMakeVisible(loadPlaylistButton);
    loadPlaylistButton.addListener(this);

    //add search bar and listener
    addAndMakeVisible(searchPlaylist);
    searchPlaylist.addListener(this);
    //add label for search bar
    addAndMakeVisible(searchLabel);
    searchLabel.setText("Find Track: ", juce::dontSendNotification);
    searchLabel.setJustificationType(Justification::centred);

    
    String urlStr;
    
    if (playlistSession.exists())
    {
        FileInputStream readSession(playlistSession);
        if (readSession.openedOk())
        {
            while (!readSession.isExhausted())
            {
                urlStr = readSession.readNextLine();
                if (urlStr.isNotEmpty())
                {
                    URL url(urlStr);
                    tracks.push_back(url);
                }
            }
        }
    }
    else
    {
        std::cout << "PlaylistSession.txt does not exists to open the file to read data!" << std::endl;
    }

    //If there are existing files before
    if (tracks.size() != 0)
    {
        //adds existing files into my files
        for (URL& trackURL : tracks)
        {
            // Add the track to the playlist
            player->getAudioDetails(trackURL);
        }
        
        //updates the table component to display the file names and duration
        playlistTable.updateContent();
    }
}

PlaylistComponent::~PlaylistComponent()
{
}

void PlaylistComponent::paint (Graphics& g)
{
    g.fillAll (Colours::darkcyan); 


    playlistTable.setColour(TableListBox::backgroundColourId, Colours::darkcyan);

    playlistTable.getHeader().setColour(TableHeaderComponent::outlineColourId, Colours::white);
    playlistTable.getHeader().setColour(TableHeaderComponent::textColourId, Colours::black);
}

void PlaylistComponent::resized()
{
    double rowH = getHeight() / 8;
    double colW = getWidth() / 8;
    loadPlaylistButton.setBounds(0, 0, getWidth(), rowH);
    searchPlaylist.setBounds(colW, rowH, getWidth(), rowH);
    searchLabel.setBounds(0, rowH, colW, rowH);
    playlistTable.setBounds(0, rowH*2, getWidth(), rowH*6);

    playlistTable.getHeader().setColumnWidth(1, colW * 2.5);
    playlistTable.getHeader().setColumnWidth(2, colW);
    playlistTable.getHeader().setColumnWidth(3, colW);
    playlistTable.getHeader().setColumnWidth(4, colW * 1.25);
    playlistTable.getHeader().setColumnWidth(5, colW * 1.25);
    playlistTable.getHeader().setColumnWidth(6, colW);

    
    Font font(20.0f, Font::italic);
    searchPlaylist.setTextToShowWhenEmpty("Search for tracks in Playlist...", Colours::white);
    searchPlaylist.setFont(20.0f);
}

int PlaylistComponent::getNumRows(){
    return player->searchTrackTitles.size();
}
void PlaylistComponent::paintRowBackground(Graphics & g, int rowNumber, int width, int height, bool rowIsSelected){
    if (rowIsSelected) {
        g.fillAll(Colours::gold);
    }else{
        g.fillAll(Colours::darkcyan);
    }
    g.setColour(Colours::white);
    g.drawRect(getLocalBounds(), 1);

    g.setColour(Colours::black);

}
void PlaylistComponent::paintCell(Graphics & g, int rowNumber, int columnId, int width, int height, bool rowIsSelected){
    if (columnId == 1)
    {
        g.drawText(player->searchTrackTitles[rowNumber], 3, 0, width - 4, height, Justification::centredLeft, true);
        g.setColour(Colours::black);
    }
    if (columnId == 2)
    {
        g.drawText(player->searchTrackLength[rowNumber], 0, 0, width - 4, height, Justification::centred, true);
        g.setColour(Colours::black);
    }
    g.setColour(Colours::white);
    g.drawRect(getLocalBounds(), 1);
}

Component* PlaylistComponent::refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component *existingComponentToUpdate) {
    if(columnId == 3){
        if(existingComponentToUpdate==nullptr){
            TextButton* btn = new TextButton{"Play"};
            String id{std::to_string(rowNumber)};
            btn->setComponentID(id);
            btn->addListener(this);
            existingComponentToUpdate=btn;
            btn->onClick = [this] {previewTrack();};
        }
    }
    if (columnId == 4) {//column 3 is a button that loads the audio file into deck 1
        if (existingComponentToUpdate == nullptr) {
            TextButton* btn = new TextButton{ "Player 1" }; //creates button with deck 1 as the text
            String id{std::to_string(rowNumber) + "_" + std::to_string(columnId)};
            btn->setComponentID(id); //set component id to button for identifying which button was clicked
            btn->addListener(this); //adding listener to the button
            existingComponentToUpdate = btn;
        }
    }
    if (columnId == 5) {//column 4 is a button that loads the audio file into deck 2
        if (existingComponentToUpdate == nullptr) {
            TextButton* btn = new TextButton{ "Player 2" }; //creates button with deck 2 as the text
            String id{std::to_string(rowNumber) + "_" + std::to_string(columnId)};
            btn->setComponentID(id);
            btn->addListener(this);
            existingComponentToUpdate = btn;
        }
    }
    if (columnId == 6) {//column 5 is a button that deletes the audio file
        if (existingComponentToUpdate == nullptr) {
            TextButton* btn = new TextButton{ "Delete" }; //creates button with delete as the text
            String id{ std::to_string(rowNumber) };
            btn->setComponentID(id);
            btn->addListener(this);
            existingComponentToUpdate = btn;
            btn->onClick = [this] {deleteTrack();};//deleteFile() runs when button is clicked
        }
    }
    return existingComponentToUpdate;
}

bool PlaylistComponent::isInterestedInFileDrag(const StringArray& files)
{
    std::cout << "PlaylistComponent::isInterestedInFileDrag" << std::endl;
    return true;
}

void PlaylistComponent::filesDropped(const StringArray& files, int x, int y)
{
    std::cout << "PlaylistComponent::filesDropped" << std::endl;
    //perform if files have been dropped (mouse released with files) 
    for (String file : files)
    {
        File fileSelected(file);
        //for each file URL, get filepath and file name 
        if(fileSelected.existsAsFile())
        {
            URL trackURL(fileSelected);

            for (URL& existingTrack : tracks)
            {
                if (trackURL == existingTrack)
                {
                    // Handle duplicate track
                    AlertWindow::showMessageBoxAsync(MessageBoxIconType::WarningIcon, "Duplicate file detected", "Duplicated file " + trackURL.getFileName() + " will not be added!", "Ok");
                    duplicates = true;
                    break;
                }
            }
            if (!duplicates)
            {
                // Add the track to the playlist
                player->getAudioDetails(trackURL);
                tracks.push_back(trackURL);
            }
            duplicates = false;
        }
        else
        {
            std::cout << "File does not exist!" << file << std::endl;
        }
    }
    playlistTable.updateContent();

    createSession();

}

void PlaylistComponent::buttonClicked(Button * button) {
    
    if (button == &loadPlaylistButton)
    {
        auto fileChooserFlags = FileBrowserComponent::canSelectMultipleItems;
        fChooser.launchAsync(fileChooserFlags, [this](const FileChooser& chooser)
            {
                fileSelections = chooser.getResults();
                for (File& fileSelected : fileSelections)
                {
                    URL trackURL(fileSelected);
                    for (URL& existingTrack : tracks)
                    {
                        if (trackURL == existingTrack)
                        {
                            // Handle duplicate track
                            AlertWindow::showMessageBoxAsync(MessageBoxIconType::WarningIcon, "Duplicate File Detected!", "Duplicated file " + trackURL.getFileName() + " will not be added!", "Ok");
                            duplicates = true;
                            break;
                        }
                    }
                    if (!duplicates)
                    {
                        // Add the track to the playlist
                        player -> getAudioDetails(trackURL);
                        tracks.push_back(trackURL);
                    }
                    duplicates = false;
                }
                playlistTable.updateContent();

                createSession();
            });
    }

    StringArray splitTableID;
    splitTableID.addTokens(button->getComponentID(), "_", "\"");
    if (splitTableID.size() >= 2)
    {
        rowClicked = splitTableID[0].getIntValue();
        colClicked = splitTableID[1].getIntValue();
        if (buttonClickCallback)
        {
            buttonClickCallback(rowClicked, colClicked);
        }
    }
    else
    {
        rowClicked = button->getComponentID().getIntValue();
    } 
}

void PlaylistComponent::setButtonClickCallback(ButtonClickCallback callback)
{
    buttonClickCallback = std::move(callback);
}


void PlaylistComponent::textEditorTextChanged(TextEditor& textEditor)
{
    //whenever the search box is modified, clear the vectors that will be used for the table 
    player->searchTrackTitles.clear();
    player->searchTrackLength.clear();

    String searchingText = textEditor.getText();
    // start at position 0 of the original library list and increment until the last element of the list
    for (int i = 0; i < player->trackTitles.size(); ++i)
    {
        //check if the texts typed in the search box is a substring of the track title we are currently processing
        if (player->trackTitles[i].containsIgnoreCase(searchingText))
        {
            player->searchTrackTitles.push_back(player->trackTitles[i]);
            player->searchTrackLength.push_back(player->trackLength[i]);
        }
    }
    playlistTable.updateContent();
}

void PlaylistComponent::timerCallback()
{
    player->stop();
    stopTimer();
    std::cout << "Timer expired, audio stopped" << std::endl;
}

void PlaylistComponent::previewTrack()
{
    if (rowClicked >= 0 && rowClicked < tracks.size())
    {
        std::cout << std::endl;
        std::cout << "Preview button was clicked: " << (tracks[rowClicked].toString(true)) << std::endl;
        player->loadURL(tracks[rowClicked]);
        std::cout << std::endl;
        std::cout << "File loaded" << std::endl;
        player->start();
        std::cout << "song start" << std::endl;
        startTimer(5000);
    }
}

void PlaylistComponent::deleteTrack() 
{
    if (rowClicked >= 0 && rowClicked < tracks.size())
    {
        
        auto callback = ModalCallbackFunction::create([this](int result) {
            if (result == 0) 
            { 
                std::cout << "Delete was canceled" << std::endl; 
            }
            if (result == 1) 
            {
                playlistTable.deleteKeyPressed(rowClicked);

                tracks.erase(tracks.begin() + rowClicked);
                player->trackTitles.erase(player->trackTitles.begin() + rowClicked);
                player -> trackLength.erase(player -> trackLength.begin() + rowClicked);

                player->searchTrackTitles.erase(player->searchTrackTitles.begin() + rowClicked);
                player -> searchTrackLength.erase(player -> searchTrackLength.begin() + rowClicked);

                playlistTable.updateContent();

                createSession();
            }
            });
        AlertWindow::showOkCancelBox(MessageBoxIconType::QuestionIcon, "Deleting Track!", "Are you sure you want to delete the track " + (player->trackTitles[rowClicked]) + " from Playlist? ", "Delete", "Cancel", nullptr, callback);
    }
}

void PlaylistComponent::createSession()
{
    FileOutputStream createSession(playlistSession);
    if (playlistSession.exists())
    {
        if (createSession.openedOk())
        {
            // Clear the existing content by truncating the file
            createSession.setPosition(0); // Move to the beginning of the file
            createSession.truncate();
            for (URL& existingTrack : tracks)
            {
                createSession.writeString(existingTrack.toString(true) + "\n");
            }
        }
        else
        {
            std::cout << "Failed to open the file to create data!" << std::endl;
        }
    }
    else
    {
        std::cout << "PlaylistSession.txt does not exists to open the file to create data!" << std::endl;
    }
}



