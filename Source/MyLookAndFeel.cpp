#include <JuceHeader.h>
#include "MyLookAndFeel.h"

MyLookAndFeel::MyLookAndFeel()
{
}

MyLookAndFeel::~MyLookAndFeel()
{
}

void MyLookAndFeel::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background
}

void MyLookAndFeel::resized()
{
}


void MyLookAndFeel::drawRotarySlider(Graphics& g,
                                     int x,
                                     int y,
                                     int width,
                                     int height,
                                     float sliderPos,
                                     const float rotaryStartAngle,
                                     const float rotaryEndAngle,
                                     Slider&)
{
    float radius = (float)jmin(width / 2, height / 2) - 4.0f;
    float centreX = (float)x + (float)width * 0.5f;
    float centreY = (float)y + (float)height * 0.5f;
    float rx = centreX - radius;
    float ry = centreY - radius;
    float rw = radius * 2.0f;
    float angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

    // fill
    g.setColour(Colours::darkgrey);
    g.fillEllipse(rx, ry, rw, rw);

    // outline
    g.setColour(Colours::black);
    g.drawEllipse(rx, ry, rw, rw, 1.0f);

    Path p;
    float pointerLength = radius * 0.8f;
    float pointerThickness = 2.0f;
    p.addRectangle(-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
    p.applyTransform(AffineTransform::rotation(angle).translated(centreX, centreY));

    // pointer
    g.setColour(Colours::white);
    g.fillPath(p);
}