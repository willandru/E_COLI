#include "Electron.h"


namespace Chemistry
{

Electron::Electron()
    : position(0.0f)
{
}


Electron::Electron(
    const glm::vec3& position
)
    : position(position)
{
}


const glm::vec3& Electron::getPosition() const
{
    return position;
}


float Electron::getMass() const
{
    return MASS;
}


float Electron::getCharge() const
{
    return CHARGE;
}

}