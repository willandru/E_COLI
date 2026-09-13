#pragma once

#include "Nucleus.h"

#include <string>
#include <vector>

namespace Chemistry
{

struct Sublevel
{
    int n;
    int l;
    int electronCount;
};

struct EnergyLevel
{
    int n;
    std::vector<Sublevel> sublevels;
};

class ElectronicStructure
{
private:
    int electronCount;
    std::vector<EnergyLevel> levels;

    void build();

public:
    ElectronicStructure();
    explicit ElectronicStructure(const Nucleus& nucleus);

    int getElectronCount() const;

    const std::vector<EnergyLevel>& getLevels() const;

    std::string getConfiguration() const;
};

}