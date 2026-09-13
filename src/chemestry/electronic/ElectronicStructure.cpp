#include "ElectronicStructure.h"

namespace Chemistry
{

ElectronicStructure::ElectronicStructure()
    : electronCount(0)
{
}

ElectronicStructure::ElectronicStructure(const Nucleus& nucleus)
    : electronCount(nucleus.getProtonCount())
{
    build();
}

void ElectronicStructure::build()
{
    levels.clear();

    int remainingElectrons = electronCount;

    struct SublevelDefinition
    {
        int n;
        int l;
        int capacity;
    };

    const SublevelDefinition fillingOrder[] =
    {
        {1, 0, 2},   // 1s
        {2, 0, 2},   // 2s
        {2, 1, 6},   // 2p
        {3, 0, 2},   // 3s
        {3, 1, 6},   // 3p
        {4, 0, 2},   // 4s
        {3, 2, 10},  // 3d
        {4, 1, 6},   // 4p
        {5, 0, 2},   // 5s
        {4, 2, 10},  // 4d
        {5, 1, 6},   // 5p
        {6, 0, 2},   // 6s
        {4, 3, 14},  // 4f
        {5, 2, 10},  // 5d
        {6, 1, 6},   // 6p
        {7, 0, 2},   // 7s
        {5, 3, 14},  // 5f
        {6, 2, 10},  // 6d
        {7, 1, 6}    // 7p
    };

    for (const SublevelDefinition& definition : fillingOrder)
    {
        if (remainingElectrons <= 0)
            break;

        const int electrons =
            remainingElectrons < definition.capacity
                ? remainingElectrons
                : definition.capacity;

        bool levelExists = false;

        for (EnergyLevel& level : levels)
        {
            if (level.n == definition.n)
            {
                level.sublevels.push_back(
                    {
                        definition.n,
                        definition.l,
                        electrons
                    }
                );

                levelExists = true;
                break;
            }
        }

        if (!levelExists)
        {
            EnergyLevel level;
            level.n = definition.n;

            level.sublevels.push_back(
                {
                    definition.n,
                    definition.l,
                    electrons
                }
            );

            levels.push_back(level);
        }

        remainingElectrons -= electrons;
    }
}

int ElectronicStructure::getElectronCount() const
{
    return electronCount;
}

const std::vector<EnergyLevel>& ElectronicStructure::getLevels() const
{
    return levels;
}

std::string ElectronicStructure::getConfiguration() const
{
    std::string configuration;

    for (const EnergyLevel& level : levels)
    {
        for (const Sublevel& sublevel : level.sublevels)
        {
            char orbital = '?';

            switch (sublevel.l)
            {
                case 0:
                    orbital = 's';
                    break;

                case 1:
                    orbital = 'p';
                    break;

                case 2:
                    orbital = 'd';
                    break;

                case 3:
                    orbital = 'f';
                    break;
            }

            configuration +=
                std::to_string(sublevel.n) +
                orbital +
                std::to_string(sublevel.electronCount) +
                " ";
        }
    }

    if (!configuration.empty())
        configuration.pop_back();

    return configuration;
}

}