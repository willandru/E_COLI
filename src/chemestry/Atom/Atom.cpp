#include "Atom.h"


namespace Chemistry
{

// ============================================================
// CONSTRUCTOR
// ============================================================

Atom::Atom()

    : nucleus(),
      electronicStructure(
          1,
          0
      )

{
}


// ============================================================
// CONSTRUCTOR
// ============================================================

Atom::Atom(

    int protonCount,

    int neutronCount

)

    : nucleus(
          protonCount,
          neutronCount
      ),

      electronicStructure(
          protonCount,
          0
      )

{
}


// ============================================================
// GET NUCLEUS
// ============================================================

const Nucleus& Atom::getNucleus() const
{
    return nucleus;
}


// ============================================================
// GET ELECTRONIC STRUCTURE
// ============================================================

const ElectronicStructure&
Atom::getElectronicStructure() const
{
    return electronicStructure;
}

}