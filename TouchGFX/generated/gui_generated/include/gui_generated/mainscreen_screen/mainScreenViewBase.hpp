/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#ifndef MAINSCREENVIEWBASE_HPP
#define MAINSCREENVIEWBASE_HPP

#include <gui/common/FrontendApplication.hpp>
#include <mvp/View.hpp>
#include <gui/mainscreen_screen/mainScreenPresenter.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <touchgfx/containers/Container.hpp>
#include <touchgfx/containers/progress_indicators/BoxProgress.hpp>
#include <touchgfx/widgets/TextArea.hpp>
#include <touchgfx/containers/clock/DigitalClock.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>
#include <touchgfx/widgets/canvas/Line.hpp>
#include <touchgfx/widgets/canvas/PainterRGB565.hpp>

class mainScreenViewBase : public touchgfx::View<mainScreenPresenter>
{
public:
    mainScreenViewBase();
    virtual ~mainScreenViewBase() {}
    virtual void setupScreen();

protected:
    FrontendApplication& application() {
        return *static_cast<FrontendApplication*>(touchgfx::Application::getInstance());
    }

    /*
     * Member Declarations
     */
    touchgfx::Box __background;
    touchgfx::Container topContainer;
    touchgfx::BoxProgress batteryProgress;
    touchgfx::TextArea dateText;
    touchgfx::DigitalClock digitalClock1;
    touchgfx::Container mid1Container;
    touchgfx::TextArea idnodeText;
    touchgfx::TextAreaWithOneWildcard voltageText;
    touchgfx::TextAreaWithOneWildcard currentText;
    touchgfx::Line line1;
    touchgfx::PainterRGB565 line1Painter;
    touchgfx::Container mid2Container;
    touchgfx::TextArea labelkirimText;
    touchgfx::TextAreaWithOneWildcard kirimText;
    touchgfx::Line line2;
    touchgfx::PainterRGB565 line2Painter;
    touchgfx::Line line3;
    touchgfx::PainterRGB565 line3Painter;
    touchgfx::Container mid3Container;
    touchgfx::TextArea labelterimaText;
    touchgfx::TextAreaWithOneWildcard terimaText;
    touchgfx::Line line4;
    touchgfx::PainterRGB565 line4Painter;
    touchgfx::Container bottomContainer;
    touchgfx::TextAreaWithOneWildcard latitudeText;
    touchgfx::TextAreaWithOneWildcard longitudeText;
    touchgfx::TextArea lokasilabelText;

    /*
     * Wildcard Buffers
     */
    static const uint16_t VOLTAGETEXT_SIZE = 10;
    touchgfx::Unicode::UnicodeChar voltageTextBuffer[VOLTAGETEXT_SIZE];
    static const uint16_t CURRENTTEXT_SIZE = 10;
    touchgfx::Unicode::UnicodeChar currentTextBuffer[CURRENTTEXT_SIZE];
    static const uint16_t KIRIMTEXT_SIZE = 65;
    touchgfx::Unicode::UnicodeChar kirimTextBuffer[KIRIMTEXT_SIZE];
    static const uint16_t TERIMATEXT_SIZE = 65;
    touchgfx::Unicode::UnicodeChar terimaTextBuffer[TERIMATEXT_SIZE];
    static const uint16_t LATITUDETEXT_SIZE = 33;
    touchgfx::Unicode::UnicodeChar latitudeTextBuffer[LATITUDETEXT_SIZE];
    static const uint16_t LONGITUDETEXT_SIZE = 33;
    touchgfx::Unicode::UnicodeChar longitudeTextBuffer[LONGITUDETEXT_SIZE];

private:

    /*
     * Canvas Buffer Size
     */
    static const uint16_t CANVAS_BUFFER_SIZE = 3600;
    uint8_t canvasBuffer[CANVAS_BUFFER_SIZE];
};

#endif // MAINSCREENVIEWBASE_HPP
