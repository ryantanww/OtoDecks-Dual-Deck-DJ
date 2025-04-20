#include <JuceHeader.h>
#include "CueComponent.h"

//==============================================================================
CueComponent::CueComponent() 

{
    cueTable.getHeader().addColumn("Track Title", 1, 125);
    cueTable.getHeader().addColumn("Cue No.", 2, 50);
    cueTable.getHeader().addColumn("Cued Time", 3, 75);
    cueTable.getHeader().addColumn("Jump To Cue", 4, 75);
    cueTable.getHeader().addColumn("Delete", 5, 75);

    addAndMakeVisible(cueTable);
    cueTable.setModel(this);

    addAndMakeVisible(addCueButton);
    addCueButton.addListener(this);
    addCueButton.setColour(TextButton::buttonColourId, Colours::skyblue);
    addCueButton.setColour(TextButton::textColourOffId, Colours::black);
}

CueComponent::~CueComponent()
{
}

void CueComponent::paint (juce::Graphics& g)
{

    g.fillAll(Colours::darkcyan);

    cueTable.setColour(TableListBox::backgroundColourId, Colours::darkcyan);

    cueTable.getHeader().setColour(TableHeaderComponent::outlineColourId, Colours::white);
    cueTable.getHeader().setColour(TableHeaderComponent::textColourId, Colours::black);
}

void CueComponent::resized()
{
    double rowH = getHeight() / 3;
    double colW = getWidth() / 8;

    cueTable.setBounds(0, 0, getWidth(), getHeight());
    cueTable.setHeaderHeight(rowH / 1.5);
    cueTable.getHeader().setColumnWidth(1, colW * 2.5);
    cueTable.getHeader().setColumnWidth(2, colW);
    cueTable.getHeader().setColumnWidth(3, colW * 1.5);
    cueTable.getHeader().setColumnWidth(4, colW * 1.5);
    cueTable.getHeader().setColumnWidth(5, colW * 1.5);

    addCueButton.setBounds(colW*1.45, 0, colW, rowH / 1.5);
}


int CueComponent::getNumRows()
{
    return cues.size();
}

void CueComponent::paintRowBackground(Graphics& g, int rowNumber, int width, int height, bool rowIsSelected)
{
    if (rowIsSelected) {
        g.fillAll(Colours::gold);
    }
    else {
        g.fillAll(Colours::darkcyan);
    }
    g.setColour(Colours::white);
    g.drawRect(getLocalBounds(), 1);

    g.setColour(Colours::black);
}

void CueComponent::paintCell(Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected)
{

    if (columnId == 1)
    {
        g.drawText(cueTitles[rowNumber], 3, 0, width - 4, height, Justification::centredLeft, true);
        g.setColour(Colours::black);
    }
    if (columnId == 2)
    {
        g.drawText(String(rowNumber + 1), 0, 0, width - 4, height, Justification::centred, true);
        g.setColour(Colours::black);
    }
    if (columnId == 3)
    {
        g.drawText(cuedTimeText[rowNumber], 0, 0, width - 4, height, Justification::centred, true);
        g.setColour(Colours::black);
    }

    g.setColour(Colours::white);
    g.drawRect(getLocalBounds(), 1);
}

Component* CueComponent::refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component* existingComponentToUpdate)
{
    if (columnId == 4) {//column 3 is a button that loads the audio file into deck 1
        if (existingComponentToUpdate == nullptr) {
            TextButton* btn = new TextButton{ "Jump" }; //creates button with deck 1 as the text
            String id{ std::to_string(rowNumber) };
            btn->setComponentID(id);
            btn->addListener(this);
            existingComponentToUpdate = btn;
            btn->onClick = [this] {jumpCue(); };//deleteFile() runs when button is clicked
        }
    }
    if (columnId == 5) {//column 4 is a button that loads the audio file into deck 2
        if (existingComponentToUpdate == nullptr) {
            TextButton* btn = new TextButton{ "Delete" }; //creates button with deck 2 as the text
            String id{ std::to_string(rowNumber) };
            btn->setComponentID(id);
            btn->addListener(this);
            existingComponentToUpdate = btn;
            btn->onClick = [this] {deleteCue(); };//deleteFile() runs when button is clicked
        }
    }

    return existingComponentToUpdate;
}

void CueComponent::buttonClicked(Button* button)
{
    double relLength = 0.0;
    int length = 0;
    int hrs = 0;
    int mins = 0;
    int secs = 0;

    if (button == &addCueButton)
    {
        URL cueURL = getCueURL;
        relLength = player.getPositionRelative();
        std::cout << "DJAudioPlayer:: getCueLength: " << relLength << std::endl;
        length = player.getCuePosition();
        hrs = length / 3600;
        mins = ((length - hrs * 3600) / 60);
        secs = (length - hrs * 3600 - mins * 60);
        String secondsToTime = String::formatted("%02d:%02d:%02d", hrs, mins, secs);
        std::cout << "DJAudioPlayer:: secondsToTime: " << secondsToTime << std::endl;

        cues.push_back(cueURL);
        cueTitles.push_back(cueURL.getFileName().toStdString());
        cuedTimeText.push_back(secondsToTime);
        cuedTime.push_back(relLength);
        cueTable.updateContent();
    }
    rowClicked = button->getComponentID().getIntValue();
}
/*void CueComponent::buttonClicked(Button* button)
{
    double relLength = 0.0;
    int length = 0;
    int hrs = 0;
    int mins = 0;
    int secs = 0;

    if (button == &addCueButton)
    {
        URL cueURL = getCueURL;
        player.getCueDetails(cueURL);
        cues.push_back(cueURL);
        cueTable.updateContent();
    }
    rowClicked = button->getComponentID().getIntValue();
}*/
void CueComponent::deleteCue()
{
    if (rowClicked >= 0 && rowClicked < cues.size())
    {

        auto callback = ModalCallbackFunction::create([this](int result) {
            if (result == 0)
            {
                std::cout << "Delete was canceled" << std::endl;
            }
            if (result == 1)
            {
                cueTable.deleteKeyPressed(rowClicked);

                cues.erase(cues.begin() + rowClicked);
                player.cueTitles.erase(player.cueTitles.begin() + rowClicked);
                player.cuedTime.erase(player.cuedTime.begin() + rowClicked);

                cueTable.updateContent();

                createSession();
            }
            });
        AlertWindow::showOkCancelBox(MessageBoxIconType::QuestionIcon, "Deleting Track!", "Are you sure you want to delete the track " + (player.cueTitles[rowClicked]) + " from Playlist? ", "Delete", "Cancel", nullptr, callback);
    }
}

void CueComponent::jumpCue()
{
    if (rowClicked >= 0 && rowClicked < cues.size())
    {
        double cuePos = player.cuedTime[rowClicked];
        std::cout << "cuePos: " << cuePos<< std::endl;
        player.setPositionRelative(cuePos);
    }
    
}

void CueComponent::createSession()
{

}

void CueComponent::getURL(URL audioURL)
{
    getCueURL = audioURL;
}