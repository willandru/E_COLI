#pragma once

#include <vector>


namespace Quantum
{

class RadialSchrodingerSolver
{
public:

    // ============================================================
    // CONSTRUCTOR
    // ============================================================

    RadialSchrodingerSolver();


    // ============================================================
    // CONFIGURATION
    // ============================================================

    void setMaximumRadius(
        double radiusNm
    );

    double getMaximumRadius() const;


    void setGridPoints(
        int points
    );

    int getGridPoints() const;


    void setAngularQuantumNumber(
        int angularQuantumNumber
    );

    int getAngularQuantumNumber() const;


    void setNuclearCharge(
        int charge
    );

    int getNuclearCharge() const;


    // ============================================================
    // SOLVE
    // ============================================================

    void solve();


    // ============================================================
    // RESULTS
    // ============================================================

    const std::vector<double>&
    getRadiusGrid() const;


    const std::vector<double>&
    getRadialSolution() const;


    double getEnergy() const;


    bool hasSolution() const;


    // ============================================================
    // NORMALIZATION
    // ============================================================

    void normalize();


    // ============================================================
    // PHYSICAL CONSTANTS
    // ============================================================

    static constexpr double BOHR_RADIUS_NM =
        0.0529177210903;


    static constexpr double HBAR_EV_S =
        6.582119569e-16;


    static constexpr double ELECTRON_MASS_KG =
        9.1093837139e-31;


    static constexpr double ELEMENTARY_CHARGE =
        1.602176634e-19;


private:

    // ============================================================
    // SOLVER STATE
    // ============================================================

    double maximumRadiusNm = 5.0;

    int gridPoints = 2000;

    int angularQuantumNumber = 0;

    int nuclearCharge = 1;


    std::vector<double> radiusGrid;

    std::vector<double> radialSolution;


    double energy = 0.0;


    bool solutionAvailable = false;


    // ============================================================
    // INTERNAL CALCULATIONS
    // ============================================================

    double evaluatePotential(
        double radiusNm
    ) const;


    double solveEnergy() const;


    void solveWaveFunction(
        double energyEV
    );


    // ============================================================
    // GRID
    // ============================================================

    void buildGrid();


    // ============================================================
    // VALIDATION
    // ============================================================

    void validateConfiguration() const;
};

}