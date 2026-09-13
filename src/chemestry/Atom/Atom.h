#pragma once

#include "Nucleus.h"
#include "ElectronicStructure.h"

namespace Chemistry
{

class Atom
{
private:
    Nucleus nucleus;
    ElectronicStructure electronicStructure;

public:
    Atom();

    Atom(
        int protonCount,
        int neutronCount
    );

    const Nucleus& getNucleus() const;

    const ElectronicStructure& getElectronicStructure() const;
};

}