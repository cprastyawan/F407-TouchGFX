#ifndef MODELLISTENER_HPP
#define MODELLISTENER_HPP

#include <gui/model/Model.hpp>
#include <stdint.h>

class ModelListener
{
public:
    ModelListener() : model(0) {}
    
    virtual ~ModelListener() {}

    void bind(Model* m)
    {
        model = m;
    }

    virtual void setNewValue(unsigned int value) {}
    virtual void setLatitude(float latitude) {}
    virtual void setLongitude(float longitude) {}
    virtual void setVoltage(uint16_t voltage) {}
    virtual void setCurrent(uint16_t current) {}
    virtual void setTerimaText(uint8_t *buffer, uint16_t length) {}
    virtual void setKirimText(uint8_t keyNumber) {}
protected:
    Model* model;
};

#endif // MODELLISTENER_HPP
