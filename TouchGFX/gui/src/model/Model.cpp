#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>

#include "cmsis_os.h"
#include "main.h"
//Test
extern osMessageQueueId_t testQHandle;
unsigned int counter = 0;

//GPS
extern osMessageQueueId_t latitudeQHandle;
extern osMessageQueueId_t longitudeQHandle;
extern osMessageQueueId_t currentQHandle;
extern osMessageQueueId_t voltageQHandle;
extern osMessageQueueId_t receiveQHandle;
extern osMessageQueueId_t keypadQHandle;

float lat = 0.0;
float lon = 0.0;
uint16_t current = 0;
uint16_t voltage = 0;
uint8_t terimaBuffer[128];
uint8_t keypadNumber = 0;

String_t rxQueue;

Model::Model() : modelListener(0)
{

}

void Model::tick()
{
//	if(osMessageQueueGet(testQHandle, &counter, NULL, 0) == osOK){
//		modelListener->setNewValue(counter);
//	}

	if(osMessageQueueGet(keypadQHandle, &keypadNumber, NULL, 0) == osOK){
		modelListener->setKirimText(keypadNumber);
	}

	if(osMessageQueueGet(latitudeQHandle, &lat, NULL, 0) == osOK){
		modelListener->setLatitude(lat);
	}

	if(osMessageQueueGet(longitudeQHandle, &lon, NULL, 0) == osOK){
		modelListener->setLongitude(lon);
	}

	if(osMessageQueueGet(voltageQHandle, &voltage, 0U, 0U) == osOK){
		modelListener->setVoltage(voltage);
	}

	if(osMessageQueueGet(currentQHandle, &current, 0U, 0U) == osOK){
		modelListener->setCurrent(current);
	}

	if(osMessageQueueGet(receiveQHandle, &rxQueue, 0U, 0U) == osOK){
		modelListener->setTerimaText(rxQueue.message, rxQueue.length);
	}

//	do{
//		receiveStatusQ = osMessageQueueGet(receiveQHandle, &terimaBuffer[i], 0U, 0U);
//		i++;
//	} while(receiveStatusQ == osOK);

//	if(i != 0){
//		terimaBuffer[i + 1] = 0;
//		modelListener->setTerimaText(terimaBuffer, i);
//	}
}
