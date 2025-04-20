#pragma once

#include <JuceHeader.h>

class MyLookAndFeel  : public Component,
                       public LookAndFeel_V4
{
public:
    MyLookAndFeel();
    ~MyLookAndFeel() override;

    void paint (Graphics&) override;
    void resized() override;

    void drawRotarySlider(Graphics& g,
                          int x,
                          int y,
                          int width,
                          int height,
                          float sliderPos,
                          const float rotaryStartAngle,
                          const float rotaryEndAngle,
                          Slider&) override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MyLookAndFeel)
};
