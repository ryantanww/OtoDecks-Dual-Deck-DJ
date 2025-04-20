#include "../JuceLibraryCode/JuceHeader.h"
#include "WaveformDisplay.h"

WaveformDisplay::WaveformDisplay(AudioFormatManager & 	formatManagerToUse,
                                 AudioThumbnailCache & 	cacheToUse) :
                                 audioThumb(1000, formatManagerToUse, cacheToUse), 
                                 fileLoaded(false), 
                                 position(0),
                                 displayAudioName(false)
{
  audioThumb.addChangeListener(this);
}

WaveformDisplay::~WaveformDisplay()
{
}

void WaveformDisplay::paint (Graphics& g)
{
    g.fillAll (Colours::darkcyan); 

    g.setColour (Colours::black);
    g.drawRect(getLocalBounds(), 1); 

    if(fileLoaded)
    {
        if (displayAudioName == true)
        {
            g.setColour(Colours::mediumpurple);
            audioThumb.drawChannel(g,
                getLocalBounds(),
                0,
                audioThumb.getTotalLength(),
                0,
                1.0f);

            g.setColour(Colours::gold);
            g.drawRect(position * getWidth(), 0, 8, getHeight());

            g.setColour(Colours::white);
            g.setFont(20.0f);
            g.drawText(audioName, getLocalBounds(),
                Justification::centred, true);
        }
        else
        {
            g.setColour(Colours::white);
            audioThumb.drawChannel(g,
                getLocalBounds(),
                0,
                audioThumb.getTotalLength(),
                0,
                1.0f);

            g.setColour(Colours::black);
            g.drawRect(position * getWidth(), 0, 8, getHeight());
        }
    }
    else 
    {
        g.setColour(Colours::white);
        g.setFont (20.0f);
        g.drawText ("File not loaded...", getLocalBounds(),
                    Justification::centred, true);  

    }
}

void WaveformDisplay::resized()
{
}

void WaveformDisplay::loadURL(URL audioURL)
{
  audioThumb.clear();
  fileLoaded  = audioThumb.setSource(new URLInputSource(audioURL));
  if (fileLoaded)
  {
    std::cout << "wfd: loaded! " << std::endl;
    audioName = audioURL.getFileName();
    repaint();
  }
  else {
    std::cout << "wfd: not loaded! " << std::endl;
  }

}

void WaveformDisplay::changeListenerCallback (ChangeBroadcaster *source)
{
    std::cout << "wfd: change received! " << std::endl;

    repaint();

}

void WaveformDisplay::setPositionRelative(double pos)
{
  if (pos != position)
  {
    position = pos;
    repaint();
  }
}

void WaveformDisplay::setAudioName(bool displayName)
{
    displayAudioName = displayName;
    repaint(); 
}




