#include "Atom.h"


namespace Chemistry
{

// ============================================================
// CONSTRUCTOR
// ============================================================

Atom::Atom()

    : nucleus(),
      electronicStructure(
          nucleus
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
          nucleus
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