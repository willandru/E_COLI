#include "Nucleus.h"

#include <cmath>


namespace Chemistry
{

// ============================================================
// CONSTRUCTOR
// ============================================================

Nucleus::Nucleus()
{
}


// ============================================================
// CONSTRUCTOR
// ============================================================

Nucleus::Nucleus(
    int protonCount,
    int neutronCount
)
{
    if (protonCount < 0)
        protonCount = 0;

    if (neutronCount < 0)
        neutronCount = 0;


    protons.reserve(
        protonCount
    );

    neutrons.reserve(
        neutronCount
    );


    // --------------------------------------------------------
    // Protons
    // --------------------------------------------------------

    for (int i = 0;
         i < protonCount;
         ++i)
    {
        protons.emplace_back();
    }


    // --------------------------------------------------------
    // Neutrons
    // --------------------------------------------------------

    for (int i = 0;
         i < neutronCount;
         ++i)
    {
        neutrons.emplace_back();
    }


    // --------------------------------------------------------
    // Posiciones
    // --------------------------------------------------------

    generatePositions();
}


// ============================================================
// GENERATE POSITIONS
// ============================================================

void Nucleus::generatePositions()
{
    const int protonCount =
        static_cast<int>(
            protons.size()
        );


    const int neutronCount =
        static_cast<int>(
            neutrons.size()
        );


    const int total =
        protonCount
        +
        neutronCount;


    if (total == 0)
        return;


    // --------------------------------------------------------
    // Protones
    // --------------------------------------------------------

    for (int i = 0;
         i < protonCount;
         ++i)
    {
        glm::vec3 position =
            generatePosition(
                i,
                total
            );


        protons[i] =
            Proton(position);
    }


    // --------------------------------------------------------
    // Neutrones
    // --------------------------------------------------------

    for (int i = 0;
         i < neutronCount;
         ++i)
    {
        glm::vec3 position =
            generatePosition(
                protonCount + i,
                total
            );


        neutrons[i] =
            Neutron(position);
    }
}


// ============================================================
// GENERATE POSITION
// ============================================================

glm::vec3 Nucleus::generatePosition(
    int index,
    int total
) const
{
    if (total <= 1)
    {
        return glm::vec3(0.0f);
    }


    // --------------------------------------------------------
    // Golden angle
    // --------------------------------------------------------

    constexpr float goldenAngle =
        2.39996323f;


    // --------------------------------------------------------
    // Separación visual entre nucleones
    // --------------------------------------------------------

    constexpr float spacing =
        0.22f;


    const float i =
        static_cast<float>(
            index
        );


    const float n =
        static_cast<float>(
            total
        );


    // --------------------------------------------------------
    // Radio
    // --------------------------------------------------------

    const float radius =
        spacing *
        std::cbrt(i);


    // --------------------------------------------------------
    // Ángulo azimutal
    // --------------------------------------------------------

    const float theta =
        goldenAngle * i;


    // --------------------------------------------------------
    // Ángulo polar
    // --------------------------------------------------------

    const float phi =
        std::acos(
            1.0f
            -
            2.0f *
            (
                (i + 0.5f)
                /
                n
            )
        );


    const float sinPhi =
        std::sin(phi);


    // --------------------------------------------------------
    // Posición
    // --------------------------------------------------------

    return glm::vec3(
        radius
        * sinPhi
        * std::cos(theta),

        radius
        * std::cos(phi),

        radius
        * sinPhi
        * std::sin(theta)
    );
}


// ============================================================
// GET PROTON COUNT
// ============================================================

int Nucleus::getProtonCount() const
{
    return static_cast<int>(
        protons.size()
    );
}


// ============================================================
// GET NEUTRON COUNT
// ============================================================

int Nucleus::getNeutronCount() const
{
    return static_cast<int>(
        neutrons.size()
    );
}


// ============================================================
// GET MASS NUMBER
// ============================================================

int Nucleus::getMassNumber() const
{
    return
        getProtonCount()
        +
        getNeutronCount();
}


// ============================================================
// GET CHARGE
// ============================================================

float Nucleus::getCharge() const
{
    if (protons.empty())
        return 0.0f;


    return
        getProtonCount()
        *
        protons[0].getCharge();
}


// ============================================================
// GET MASS
// ============================================================

float Nucleus::getMass() const
{
    float mass = 0.0f;


    for (const Proton& proton :
         protons)
    {
        mass += proton.getMass();
    }


    for (const Neutron& neutron :
         neutrons)
    {
        mass += neutron.getMass();
    }


    return mass;
}


// ============================================================
// GET PROTONS
// ============================================================

const std::vector<Proton>&
Nucleus::getProtons() const
{
    return protons;
}


// ============================================================
// GET NEUTRONS
// ============================================================

const std::vector<Neutron>&
Nucleus::getNeutrons() const
{
    return neutrons;
}

}