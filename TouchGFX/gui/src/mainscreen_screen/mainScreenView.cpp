#include <gui/mainscreen_screen/mainScreenView.hpp>
#include "cmsis_os.h"
#include <string.h>
#include "main.h"
extern osMessageQueueId_t transmitQHandle;

mainScreenView::mainScreenView()
{

}

void mainScreenView::setupScreen()
{
    mainScreenViewBase::setupScreen();
}

void mainScreenView::tearDownScreen()
{
    mainScreenViewBase::tearDownScreen();
}

void mainScreenView::updateVal(unsigned int newValue){
	Unicode::snprintf(kirimTextBuffer, KIRIMTEXT_SIZE, "%d", newValue);
	kirimText.resizeToCurrentText();
	kirimText.invalidate();
}

void mainScreenView::updateLatitude(float latitude){
	Unicode::snprintfFloat(latitudeTextBuffer, LATITUDETEXT_SIZE, "%f", latitude);
	latitudeText.resizeToCurrentText();
	latitudeText.invalidate();
}

void mainScreenView::updateLongitude(float longitude){
	Unicode::snprintfFloat(longitudeTextBuffer, LONGITUDETEXT_SIZE, "%f", longitude);
	longitudeText.resizeToCurrentText();
	longitudeText.invalidate();
}

void mainScreenView::updateCurrent(uint16_t current){
	Unicode::snprintf(currentTextBuffer, CURRENTTEXT_SIZE, "%d", current);
	currentText.resizeToCurrentText();
	currentText.invalidate();
}

void mainScreenView::updateVoltage(uint16_t voltage){
	Unicode::snprintf(voltageTextBuffer, VOLTAGETEXT_SIZE, "%d", voltage);
	voltageText.resizeToCurrentText();
	voltageText.invalidate();
}

void mainScreenView::updateTerimaText(uint8_t *buffer, uint16_t length){
	buffer[length] = 0;
	Unicode::UnicodeChar bufferUnicode[length];
	Unicode::fromUTF8(buffer, bufferUnicode, length);
	Unicode::snprintf(terimaTextBuffer, TERIMATEXT_SIZE, "%s", bufferUnicode);
	terimaText.resizeToCurrentText();
	terimaText.invalidate();
}

void mainScreenView::updateKirimText(uint8_t keyNumber){
	Unicode::UnicodeChar temp[64];
	switch(keyNumber){
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
		Unicode::snprintf(&kirimTextBuffer[Unicode::strlen(kirimTextBuffer)], KIRIMTEXT_SIZE - Unicode::strlen(kirimTextBuffer), "%d", keyNumber);
		break;
	case 'A':
		Unicode::snprintf(&kirimTextBuffer[Unicode::strlen(kirimTextBuffer)], KIRIMTEXT_SIZE - Unicode::strlen(kirimTextBuffer), " Salmon");
		break;
	case 'B':
		Unicode::snprintf(&kirimTextBuffer[Unicode::strlen(kirimTextBuffer)], KIRIMTEXT_SIZE - Unicode::strlen(kirimTextBuffer), " Lele");
		break;
	case 'C':
		Unicode::snprintf(&kirimTextBuffer[Unicode::strlen(kirimTextBuffer)], KIRIMTEXT_SIZE - Unicode::strlen(kirimTextBuffer), " Hiu");
		break;
	case 'D':
		Unicode::snprintf(&kirimTextBuffer[Unicode::strlen(kirimTextBuffer)], KIRIMTEXT_SIZE - Unicode::strlen(kirimTextBuffer), " Paus");
		break;
	case 'X':
		for(int i=0;i<63;i++) temp[i] = ' ';
		temp[63] = 0;
		Unicode::snprintf(kirimTextBuffer, KIRIMTEXT_SIZE, "%s", temp);
		kirimText.resizeToCurrentText();
		kirimText.invalidate();
		Unicode::snprintf(kirimTextBuffer, KIRIMTEXT_SIZE, "");
		break;
	case 'Y':
		static uint8_t messageSend[128];
		memset(messageSend, 0, 128);
		String_t messageStringSend;
		Unicode::toUTF8(kirimTextBuffer, messageSend, Unicode::strlen(kirimTextBuffer));
		messageStringSend.message = messageSend;
		messageStringSend.length = strlen((char*)messageSend) + 1;
		osMessageQueuePut(transmitQHandle, &messageStringSend, 0U, 0U);
		break;
	}
	//Unicode::snprintf(kirimTextBuffer, KIRIMTEXT_SIZE, "%d", keyNumber);
	kirimText.resizeToCurrentText();
	kirimText.invalidate();
}
