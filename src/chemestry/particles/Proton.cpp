#include "Proton.h"


namespace Chemistry
{

Proton::Proton()
    : position(0.0f)
{
}


Proton::Proton(
    const glm::vec3& position
)
    : position(position)
{
}


const glm::vec3& Proton::getPosition() const
{
    return position;
}


float Proton::getMass() const
{
    return MASS;
}


float Proton::getCharge() const
{
    return CHARGE;
}

}