#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class WaveformDisplay    : public Component, 
                           public ChangeListener
{
public:
    WaveformDisplay(AudioFormatManager & 	formatManagerToUse,
                    AudioThumbnailCache & 	cacheToUse );
    ~WaveformDisplay();

    void paint (Graphics&) override;
    void resized() override;

    void changeListenerCallback (ChangeBroadcaster *source) override;

    void loadURL(URL audioURL);

    /** set the relative position of the playhead*/
    void setPositionRelative(double pos);

    void setAudioName(bool displayName);

private:
    AudioThumbnail audioThumb;
    bool fileLoaded; 
    double position;
    String audioName;
    bool displayAudioName;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformDisplay)
};
