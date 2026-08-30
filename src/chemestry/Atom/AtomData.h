#pragma once

namespace Chemistry
{

struct AtomData
{
    // ========================================================
    // Identificación
    // ========================================================

    int atomicNumber;

    const char* symbol;
    const char* name;


    // ========================================================
    // Propiedades físicas
    // ========================================================

    float atomicMass;


    // ========================================================
    // Propiedades geométricas
    // ========================================================

    float covalentRadius;
    float vanDerWaalsRadius;


    // ========================================================
    // Propiedades electrónicas
    // ========================================================

    int valenceElectrons;
};


// ============================================================
// ELEMENTOS QUÍMICOS
// ============================================================

inline constexpr AtomData Hydrogen
{
    1,
    "H",
    "Hydrogen",

    1.008f,

    0.31f,
    1.20f,

    1
};


inline constexpr AtomData Carbon
{
    6,
    "C",
    "Carbon",

    12.011f,

    0.76f,
    1.70f,

    4
};


inline constexpr AtomData Nitrogen
{
    7,
    "N",
    "Nitrogen",

    14.007f,

    0.71f,
    1.55f,

    5
};


inline constexpr AtomData Oxygen
{
    8,
    "O",
    "Oxygen",

    15.999f,

    0.66f,
    1.52f,

    6
};


inline constexpr AtomData Phosphorus
{
    15,
    "P",
    "Phosphorus",

    30.974f,

    1.07f,
    1.80f,

    5
};


inline constexpr AtomData Sulfur
{
    16,
    "S",
    "Sulfur",

    32.06f,

    1.05f,
    1.80f,

    6
};

}