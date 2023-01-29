#include <gui/mainscreen_screen/mainScreenView.hpp>
#include <gui/mainscreen_screen/mainScreenPresenter.hpp>

mainScreenPresenter::mainScreenPresenter(mainScreenView& v)
    : view(v)
{

}

void mainScreenPresenter::activate()
{

}

void mainScreenPresenter::deactivate()
{

}

void mainScreenPresenter::setNewValue(unsigned int value){
	view.updateVal(value);
}

void mainScreenPresenter::setLatitude(float latitude){
	view.updateLatitude(latitude);
}

void mainScreenPresenter::setLongitude(float longitude){
	view.updateLongitude(longitude);
}

void mainScreenPresenter::setVoltage(uint16_t voltage){
	view.updateVoltage(voltage);
}

void mainScreenPresenter::setCurrent(uint16_t current){
	view.updateCurrent(current);
}

void mainScreenPresenter::setTerimaText(uint8_t* buffer, uint16_t length){
	view.updateTerimaText(buffer, length);
}

void mainScreenPresenter::setKirimText(uint8_t keyNumber){
	view.updateKirimText(keyNumber);
}
