#include "Neutron.h"


namespace Chemistry
{

Neutron::Neutron()
    : position(0.0f)
{
}


Neutron::Neutron(
    const glm::vec3& position
)
    : position(position)
{
}


const glm::vec3& Neutron::getPosition() const
{
    return position;
}


float Neutron::getMass() const
{
    return MASS;
}


float Neutron::getCharge() const
{
    return CHARGE;
}

}