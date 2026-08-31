#pragma once

#include "Proton.h"
#include "Neutron.h"

#include <vector>


namespace Chemistry
{

class Nucleus
{
public:

    Nucleus();

    Nucleus(
        int protonCount,
        int neutronCount
    );


    int getProtonCount() const;

    int getNeutronCount() const;

    int getMassNumber() const;

    float getCharge() const;

    float getMass() const;


    const std::vector<Proton>&
    getProtons() const;


    const std::vector<Neutron>&
    getNeutrons() const;


private:

    std::vector<Proton> protons;

    std::vector<Neutron> neutrons;


    void generatePositions();

    glm::vec3 generatePosition(
        int index,
        int total
    ) const;
};

}