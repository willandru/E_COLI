#pragma once

#include "Atom.h"

namespace Chemistry
{

enum class BondType
{
    Single,
    Double,
    Triple
};


class Bond
{
public:

    // ========================================================
    // Constructor
    // ========================================================

    Bond(
        Atom& atomA,
        Atom& atomB,
        BondType type = BondType::Single
    );


    // ========================================================
    // Átomos
    // ========================================================

    const Atom& getAtomA() const;

    const Atom& getAtomB() const;


    // ========================================================
    // Tipo de enlace
    // ========================================================

    BondType getType() const;


private:

    Atom* m_atomA;

    Atom* m_atomB;

    BondType m_type;
};

}