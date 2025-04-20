#include "../JuceLibraryCode/JuceHeader.h"
#include "DeckGUI.h"

DeckGUI::DeckGUI(int index, DJAudioPlayer* _player,
                    AudioFormatManager& formatManagerToUse,
                    AudioThumbnailCache& cacheToUse,
                    PlaylistComponent* _playlistComponent)
                    : deckIndex(index), player(_player),
                    waveformDisplay(formatManagerToUse, cacheToUse),
                    playbackWaveform(formatManagerToUse, cacheToUse),
                    playlistComponent(_playlistComponent)
{

    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(previousButton);
    addAndMakeVisible(nextButton);
    addAndMakeVisible(unloopButton);
    addAndMakeVisible(loopButton);

    addAndMakeVisible(cueButton);

    addAndMakeVisible(volSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(volLabel);
    addAndMakeVisible(speedLabel);

    addAndMakeVisible(waveformDisplay);
    addAndMakeVisible(playbackWaveform);
    addAndMakeVisible(posSlider);

    playButton.addListener(this);
    stopButton.addListener(this);
    previousButton.addListener(this);
    nextButton.addListener(this);
    unloopButton.addListener(this);
    loopButton.addListener(this);

    cueButton.addListener(this);

    volSlider.addListener(this);
    volSlider.setLookAndFeel(&myLookAndFeel);
    volSlider.setSliderStyle(Slider::Rotary);
    volSlider.setTextBoxStyle(Slider::TextBoxLeft, false, 100, 25);
    volSlider.setColour(Slider::textBoxOutlineColourId, Colours::black);

    volLabel.setText("Volume", NotificationType::dontSendNotification);
    volLabel.setFont(15.0f);
    volLabel.setJustificationType(Justification::centred);
    //volLabel.attachToComponent(&volSlider, false);

    speedSlider.addListener(this);
    speedSlider.setLookAndFeel(&myLookAndFeel);
    speedSlider.setSliderStyle(Slider::Rotary);
    speedSlider.setTextBoxStyle(Slider::TextBoxRight, false, 100, 25);
    speedSlider.setColour(Slider::textBoxOutlineColourId, Colours::black);
    
    speedLabel.setText("Speed", NotificationType::dontSendNotification);
    speedLabel.setFont(15.0f);
    speedLabel.setJustificationType(Justification::centred);
    //speedLabel.attachToComponent(&speedSlider, false);

    posSlider.addListener(this);
    posSlider.setSliderStyle(Slider::LinearBar);
    posSlider.setTextBoxStyle(Slider::NoTextBox, true, 100, 25);
    posSlider.setColour(Slider::trackColourId,Colours::transparentWhite);
    posSlider.setColour(Slider::textBoxOutlineColourId,Colours::transparentWhite);

    //posLabel.setText("Playback", NotificationType::dontSendNotification);
    //posLabel.setJustificationType(Justification::centred);

    volSlider.setRange(0.0, 1.0);
    speedSlider.setRange(0.0, 5.0);
    posSlider.setRange(0.0, 1.0);

    //EQSlidersComponent();
    
    startTimer(500);

    addAndMakeVisible(cueComponent);
    
}

DeckGUI::~DeckGUI()
{
    stopTimer();
}

void DeckGUI::paint (Graphics& g)
{
    g.fillAll (Colours::darkcyan);
    Font font(22.5f, Font::bold);

    g.setColour(Colours::black);
    g.drawRect(getLocalBounds(), 1);

    g.setColour(Colours::white);
    g.setFont(font);
    String player = "Player " + String(deckIndex);
    g.drawText(player, getLocalBounds(),
            Justification::centredTop, true);
    
}

void DeckGUI::resized()
{
    double rowH = getHeight() / 12;
    double colW = getWidth() / 4;

    waveformDisplay.setBounds(0, rowH * 9, getWidth(), rowH * 3);
    waveformDisplay.setAudioName(true);
    playbackWaveform.setBounds(0, rowH * 7, getWidth(), rowH*2);
    playbackWaveform.setAudioName(false);
    posSlider.setBounds(0, rowH * 6, getWidth(), rowH*2);
    

    volSlider.setBounds(0, rowH * 2, colW*2, rowH*2);
    speedSlider.setBounds(colW * 2, rowH * 2, colW*2, rowH*2);
    volLabel.setBounds(0, rowH * 1.8, colW, rowH);
    speedLabel.setBounds(colW * 3, rowH * 1.8, colW, rowH);

    //freqSlider.setBounds(colW * 2, rowH * 4.5, colW, rowH);
    //freqLabel.setBounds(colW * 2, rowH  * 3.5, 100, rowH);

    unloopButton.setBounds(0, rowH * 1, colW, rowH);
    previousButton.setBounds(colW, rowH * 1, colW/2, rowH);
    playButton.setBounds(colW * 1.5, rowH * 1, colW / 2, rowH);
    stopButton.setBounds(colW * 2, rowH * 1, colW / 2, rowH);
    nextButton.setBounds(colW * 2.5, rowH * 1, colW/2, rowH);
    loopButton.setBounds(colW * 3, rowH * 1, colW, rowH);

    //cueButton.setBounds(colW * 3, rowH * 5, colW, rowH);
    cueComponent.setBounds(2, rowH * 4, getWidth(), rowH * 3);
    
}

void DeckGUI::buttonClicked(Button* button)
{
    URL track;
    if (button == &playButton)
    {
        std::cout << "Play button was clicked " << std::endl;
        player->start();
    }
    if (button == &stopButton)
    {
        std::cout << "Stop button was clicked " << std::endl;
        player->stop();
    }
    if (button == &previousButton)
    {
        int previousButtonClicked = playlistRowClicked - 1;
        if (previousButtonClicked >= 0 && previousButtonClicked < playlistComponent->tracks.size())
        {
            track = playlistComponent->tracks[previousButtonClicked];
            player->loadURL(track);
            waveformDisplay.loadURL(track);
            playbackWaveform.loadURL(track);
            playlistRowClicked = playlistRowClicked - 1;
        }
        else 
        {
            AlertWindow::showMessageBoxAsync(MessageBoxIconType::WarningIcon, "No Track Detected!", "No track detected/no previous track from current track!", "Ok");
        }
    }
    if (button == &nextButton)
    {
        int nextButtonClicked = playlistRowClicked + 1;
        if (nextButtonClicked >= 0 && nextButtonClicked < playlistComponent->tracks.size())
        {
            track = playlistComponent->tracks[nextButtonClicked];
            player->loadURL(track);
            waveformDisplay.loadURL(track);
            playbackWaveform.loadURL(track);
            playlistRowClicked = playlistRowClicked + 1;
        }
        else
        {
            AlertWindow::showMessageBoxAsync(MessageBoxIconType::WarningIcon, "No Track Detected!", "No track detected/no next track from current track!", "Ok");
        }
    }
    if (button == &unloopButton)
    {
        loopAudio = false;
        unloopButton.setToggleState(true, false);
        loopButton.setToggleState(false, false);
    }
    if (button == &loopButton)
    {
        loopAudio = true;
        loopButton.setToggleState(true, false);
        unloopButton.setToggleState(false, false);
    }


    if (button == &cueButton)
    {
        double cuePos = posSlider.getValue();
        std::cout << "cuePos: " << cuePos << std::endl;
    }
}

void DeckGUI::sliderValueChanged (Slider *slider)
{
    if (slider == &volSlider)
    {
        player->setGain(slider->getValue());
    }

    if (slider == &speedSlider)
    {
        player->setSpeed(slider->getValue());
    }
    
    if (slider == &posSlider)
    {
        player->setPositionRelative(slider->getValue());
    }    
}

void DeckGUI::timerCallback()
{
    //std::cout << "DeckGUI::timerCallback" << std::endl;
    waveformDisplay.setPositionRelative(player->getPositionRelative());
    playbackWaveform.setPositionRelative(player->getPositionRelative());

    if (loopAudio == true && player->getPositionRelative() >= 1)  //and the position more than 1 meaning it is the end of the audio file
    {
        player->setPositionRelative(0); //Sets position back to the start of the audio file
        player->start(); // Stops the audio file
    } 
    else if(loopAudio == false && player->getPositionRelative() >= 1)//and the position more than 1 meaning it is the end of the audio file 
    {
        player->setPositionRelative(0); //Sets position back to the start of the audio file
        player->stop(); // Stops the audio file
        
    }
}

void DeckGUI::loadFromPlaylist(int deckNum, int rowNum)
{
    URL track;
    playlistRowClicked = rowNum;

    if (rowNum != -1)
    {
        if (deckNum == 1)
        {
            track = playlistComponent->tracks[playlistRowClicked];
            player->loadURL(track);
            cueComponent.getURL(track);
            waveformDisplay.loadURL(track);
            playbackWaveform.loadURL(track);
        }
        if (deckNum == 2)
        {
            track = playlistComponent->tracks[playlistRowClicked];
            player->loadURL(track);
            cueComponent.getURL(track);
            waveformDisplay.loadURL(track);
            playbackWaveform.loadURL(track);
        }
    }
    else
    {
        std::cout << "No files from playlist loaded into deck!" << std::endl;
    }
}


void DeckGUI::EQSlidersComponent()
{
    addAndMakeVisible(lowFreqSlider);
    addAndMakeVisible(midFreqSlider);
    addAndMakeVisible(highFreqSlider);

    lowFreqSlider.addListener(this);
    midFreqSlider.addListener(this);
    highFreqSlider.addListener(this);

    lowFreqSlider.setRange(0.0, 300.0);
    midFreqSlider.setRange(300.0, 2000.0);
    highFreqSlider.setRange(2000.0, 20000.0);



    addAndMakeVisible(lowGainSlider);
    addAndMakeVisible(lowQFactorSlider);
    
    addAndMakeVisible(midGainSlider);
    addAndMakeVisible(midQFactorSlider);
    
    addAndMakeVisible(highGainSlider);
    addAndMakeVisible(highQFactorSlider);

    addAndMakeVisible(freqLabel);
    addAndMakeVisible(gainLabel);
    addAndMakeVisible(qFactorLabel);
    addAndMakeVisible(lowLabel);
    addAndMakeVisible(midLabel);
    addAndMakeVisible(highLabel);

    //low EQ
    lowFreqSlider.setLookAndFeel(&myLookAndFeel);
    lowFreqSlider.setSliderStyle(Slider::Rotary);
    lowFreqSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 20);
    lowFreqSlider.setColour(Slider::textBoxOutlineColourId, Colours::black);
    
    lowGainSlider.setLookAndFeel(&myLookAndFeel);
    lowGainSlider.setSliderStyle(Slider::Rotary);
    lowGainSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 20);
    lowGainSlider.setColour(Slider::textBoxOutlineColourId, Colours::black);

    lowQFactorSlider.addListener(this);
    lowQFactorSlider.setLookAndFeel(&myLookAndFeel);
    lowQFactorSlider.setSliderStyle(Slider::Rotary);
    lowQFactorSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 20);
    lowQFactorSlider.setColour(Slider::textBoxOutlineColourId, Colours::black);

    //mid EQ
    midFreqSlider.setLookAndFeel(&myLookAndFeel);
    midFreqSlider.setSliderStyle(Slider::Rotary);
    midFreqSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 20);
    midFreqSlider.setColour(Slider::textBoxOutlineColourId, Colours::black);

    midGainSlider.addListener(this);
    midGainSlider.setLookAndFeel(&myLookAndFeel);
    midGainSlider.setSliderStyle(Slider::Rotary);
    midGainSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 20);
    midGainSlider.setColour(Slider::textBoxOutlineColourId, Colours::black);

    midQFactorSlider.addListener(this);
    midQFactorSlider.setLookAndFeel(&myLookAndFeel);
    midQFactorSlider.setSliderStyle(Slider::Rotary);
    midQFactorSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 20);
    midQFactorSlider.setColour(Slider::textBoxOutlineColourId, Colours::black);

    //high EQ
    highFreqSlider.setLookAndFeel(&myLookAndFeel);
    highFreqSlider.setSliderStyle(Slider::Rotary);
    highFreqSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 20);
    highFreqSlider.setColour(Slider::textBoxOutlineColourId, Colours::black);

    highGainSlider.addListener(this);
    highGainSlider.setLookAndFeel(&myLookAndFeel);
    highGainSlider.setSliderStyle(Slider::Rotary);
    highGainSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 20);
    highGainSlider.setColour(Slider::textBoxOutlineColourId, Colours::black);

    highQFactorSlider.addListener(this);
    highQFactorSlider.setLookAndFeel(&myLookAndFeel);
    highQFactorSlider.setSliderStyle(Slider::Rotary);
    highQFactorSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 20);
    highQFactorSlider.setColour(Slider::textBoxOutlineColourId, Colours::black);

    // EQ Labels
    freqLabel.setText("Freq", NotificationType::dontSendNotification);
    freqLabel.setFont(15.0f);
    freqLabel.setJustificationType(Justification::centred);
    freqLabel.attachToComponent(&lowFreqSlider, true);

    gainLabel.setText("Gain", NotificationType::dontSendNotification);
    gainLabel.setFont(15.0f);
    gainLabel.setJustificationType(Justification::centred);
    gainLabel.attachToComponent(&lowGainSlider, true);

    qFactorLabel.setText("QFactor", NotificationType::dontSendNotification);
    qFactorLabel.setFont(15.0f);
    qFactorLabel.setJustificationType(Justification::centred);
    qFactorLabel.attachToComponent(&lowQFactorSlider, true);

    lowLabel.setText("Low", NotificationType::dontSendNotification);
    lowLabel.setFont(15.0f);
    lowLabel.setJustificationType(Justification::centred);
    lowLabel.attachToComponent(&lowFreqSlider, false);

    midLabel.setText("Mid", NotificationType::dontSendNotification);
    midLabel.setFont(15.0f);
    midLabel.setJustificationType(Justification::centred);
    midLabel.attachToComponent(&midFreqSlider, false);

    highLabel.setText("High", NotificationType::dontSendNotification);
    highLabel.setFont(15.0f);
    highLabel.setJustificationType(Justification::centred);
    highLabel.attachToComponent(&highFreqSlider, false);

    //0hz to 300hz
    lowGainSlider.setRange(-12.0, 12.0);
    lowQFactorSlider.setRange(0.1, 10.0);

    //300hz to 2000hz
    midGainSlider.setRange(-12.0, 12.0);
    midQFactorSlider.setRange(0.1, 10.0);

    //2000hz to 20000hz
    highGainSlider.setRange(-12.0, 12.0);
    highQFactorSlider.setRange(0.1, 10.0);
}
