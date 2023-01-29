#ifndef MAINSCREENVIEW_HPP
#define MAINSCREENVIEW_HPP

#include <gui_generated/mainscreen_screen/mainScreenViewBase.hpp>
#include <gui/mainscreen_screen/mainScreenPresenter.hpp>

class mainScreenView : public mainScreenViewBase
{
public:
    mainScreenView();
    virtual ~mainScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    void updateVal(unsigned int newValue);
    void updateLatitude(float latitude);
    void updateLongitude(float longitude);
    void updateVoltage(uint16_t voltage);
    void updateCurrent(uint16_t current);
    void updateTerimaText(uint8_t *buffer, uint16_t length);
    void updateKirimText(uint8_t keyNumber);
protected:
};

#endif // MAINSCREENVIEW_HPP
