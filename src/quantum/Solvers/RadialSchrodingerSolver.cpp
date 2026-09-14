#include "RadialSchrodingerSolver.h"

#include <algorithm>
#include <cmath>
#include <limits>
#include <stdexcept>


namespace Quantum
{

// ============================================================
// CONSTRUCTOR
// ============================================================

RadialSchrodingerSolver::RadialSchrodingerSolver()
{
    buildGrid();
}


// ============================================================
// GRID CONFIGURATION
// ============================================================

void RadialSchrodingerSolver::setMaximumRadius(
    double radiusNm
)
{
    if (radiusNm <= 0.0)
    {
        throw std::invalid_argument(
            "Maximum radius must be greater than zero."
        );
    }

    maximumRadiusNm = radiusNm;

    buildGrid();

    solutionAvailable = false;
}


double RadialSchrodingerSolver::getMaximumRadius() const
{
    return maximumRadiusNm;
}


void RadialSchrodingerSolver::setGridPoints(
    int points
)
{
    if (points < 100)
    {
        throw std::invalid_argument(
            "The radial grid requires at least 100 points."
        );
    }

    gridPoints = points;

    buildGrid();

    solutionAvailable = false;
}


int RadialSchrodingerSolver::getGridPoints() const
{
    return gridPoints;
}


// ============================================================
// QUANTUM NUMBER CONFIGURATION
// ============================================================

void RadialSchrodingerSolver::setAngularQuantumNumber(
    int angularQuantumNumber
)
{
    if (angularQuantumNumber < 0)
    {
        throw std::invalid_argument(
            "Angular quantum number cannot be negative."
        );
    }

    this->angularQuantumNumber =
        angularQuantumNumber;

    solutionAvailable = false;
}


int RadialSchrodingerSolver::getAngularQuantumNumber() const
{
    return angularQuantumNumber;
}


// ============================================================
// NUCLEAR CHARGE
// ============================================================

void RadialSchrodingerSolver::setNuclearCharge(
    int charge
)
{
    if (charge < 1)
    {
        throw std::invalid_argument(
            "Nuclear charge must be greater than zero."
        );
    }

    nuclearCharge = charge;

    solutionAvailable = false;
}


int RadialSchrodingerSolver::getNuclearCharge() const
{
    return nuclearCharge;
}


// ============================================================
// SOLVE
// ============================================================

void RadialSchrodingerSolver::solve()
{
    validateConfiguration();

    buildGrid();


    /*
        Hydrogenic energy:

            E_n = -13.605693122994 Z^2 / n^2

        This first version does not receive n directly.

        For a given angular quantum number l, the lowest
        possible principal quantum number is:

            n = l + 1

        Therefore:

            l = 0 -> n = 1 -> 1s
            l = 1 -> n = 2 -> 2p
            l = 2 -> n = 3 -> 3d
            l = 3 -> n = 4 -> 4f
    */

    energy = solveEnergy();


    /*
        Numerically solve the radial equation.
    */

    solveWaveFunction(
        energy
    );


    /*
        Normalize:

            integral |u(r)|^2 dr = 1
    */

    normalize();


    solutionAvailable = true;
}


// ============================================================
// RESULTS
// ============================================================

const std::vector<double>&
RadialSchrodingerSolver::getRadiusGrid() const
{
    return radiusGrid;
}


const std::vector<double>&
RadialSchrodingerSolver::getRadialSolution() const
{
    return radialSolution;
}


double RadialSchrodingerSolver::getEnergy() const
{
    return energy;
}


bool RadialSchrodingerSolver::hasSolution() const
{
    return solutionAvailable;
}


// ============================================================
// NORMALIZATION
// ============================================================

void RadialSchrodingerSolver::normalize()
{
    if (radialSolution.empty())
    {
        return;
    }


    double integral = 0.0;


    /*
        The reduced radial wave function u(r) is normalized as:

            integral |u(r)|^2 dr = 1
    */

    for (
        std::size_t i = 1;
        i < radialSolution.size();
        ++i
    )
    {
        const double dr =
            radiusGrid[i]
            - radiusGrid[i - 1];


        const double previous =
            radialSolution[i - 1]
            * radialSolution[i - 1];


        const double current =
            radialSolution[i]
            * radialSolution[i];


        integral +=
            0.5
            * (
                previous
                + current
            )
            * dr;
    }


    if (integral <= 0.0)
    {
        return;
    }


    const double normalizationFactor =
        1.0
        /
        std::sqrt(integral);


    for (double& value : radialSolution)
    {
        value *= normalizationFactor;
    }
}


// ============================================================
// NUCLEAR POTENTIAL
// ============================================================

double RadialSchrodingerSolver::evaluatePotential(
    double radiusNm
) const
{
    if (radiusNm <= 0.0)
    {
        return -std::numeric_limits<double>::infinity();
    }


    /*
        Coulomb potential:

            V(r)
            =
            - Z e^2
            ----------------
            4 pi epsilon_0 r

        In eV and nm:

            V(r)
            =
            -1.43996448 Z / r_nm
    */

    constexpr double COULOMB_EV_NM =
        1.43996448;


    const double Z =
        static_cast<double>(
            nuclearCharge
        );


    return
        -COULOMB_EV_NM
        * Z
        / radiusNm;
}


// ============================================================
// ENERGY
// ============================================================

double RadialSchrodingerSolver::solveEnergy() const
{
    /*
        Lowest hydrogenic state compatible with l:

            n = l + 1

        Energy:

            E_n
            =
            -13.605693122994 Z^2 / n^2
    */

    const double n =
        static_cast<double>(
            angularQuantumNumber + 1
        );


    const double Z =
        static_cast<double>(
            nuclearCharge
        );


    return
        -13.605693122994
        * Z
        * Z
        /
        (
            n
            * n
        );
}


// ============================================================
// WAVE FUNCTION
// ============================================================

void RadialSchrodingerSolver::solveWaveFunction(
    double energyEV
)
{
    radialSolution.clear();

    radialSolution.resize(
        radiusGrid.size(),
        0.0
    );


    if (radiusGrid.empty())
    {
        return;
    }


    /*
        Radial Schrödinger equation:

            -hbar²/(2m) u''(r)
            +
            V_eff(r) u(r)
            =
            E u(r)

        where:

            V_eff(r)
            =
            V(r)
            +
            hbar² l(l+1)/(2mr²)

        Therefore:

            u''(r)
            =
            f(r)u(r)

        with:

            f(r)
            =
            2m/hbar²
            [V_eff(r)-E]
    */


    constexpr double HBAR =
        1.054571817e-34;


    constexpr double MASS =
        ELECTRON_MASS_KG;


    constexpr double ELEMENTARY_CHARGE_C =
        ELEMENTARY_CHARGE;


    /*
        Convert energy:

            eV -> J
    */

    const double energyJ =
        energyEV
        * ELEMENTARY_CHARGE_C;


    /*
        Angular quantum number l.
    */

    const double l =
        static_cast<double>(
            angularQuantumNumber
        );


    /*
        Start away from r = 0 because the Coulomb potential
        is singular there.
    */

    const double firstRadiusNm =
        radiusGrid[0];


    const double firstRadiusM =
        firstRadiusNm
        * 1.0e-9;


    /*
        Regular solution near r = 0:

            u(r) ~ r^(l+1)
    */

    if (firstRadiusM > 0.0)
    {
        radialSolution[0] =
            std::pow(
                firstRadiusM,
                l + 1.0
            );
    }


    if (radiusGrid.size() < 2)
    {
        return;
    }


    const double secondRadiusNm =
        radiusGrid[1];


    const double secondRadiusM =
        secondRadiusNm
        * 1.0e-9;


    if (secondRadiusM > 0.0)
    {
        radialSolution[1] =
            std::pow(
                secondRadiusM,
                l + 1.0
            );
    }


    /*
        Effective differential-equation function.
    */

    auto effectiveFunction =
        [&](double radiusM) -> double
    {
        if (radiusM <= 0.0)
        {
            return 0.0;
        }


        const double radiusNm =
            radiusM
            * 1.0e9;


        /*
            Nuclear Coulomb potential in eV.
        */

        const double potentialEV =
            evaluatePotential(
                radiusNm
            );


        /*
            Convert to joules.
        */

        const double potentialJ =
            potentialEV
            * ELEMENTARY_CHARGE_C;


        /*
            Centrifugal potential:

                hbar² l(l+1)
                ----------------
                2 m r²
        */

        const double centrifugalJ =
            (
                HBAR
                * HBAR
                * l
                * (l + 1.0)
            )
            /
            (
                2.0
                * MASS
                * radiusM
                * radiusM
            );


        const double effectivePotentialJ =
            potentialJ
            + centrifugalJ;


        return
            (
                2.0
                * MASS
                /
                (
                    HBAR
                    * HBAR
                )
            )
            *
            (
                effectivePotentialJ
                - energyJ
            );
    };


    /*
        Numerov integration.

        For equally spaced points:

            u_(i+1)
            =
            [
                2u_i(1-h²f_i/12)
                -
                u_(i-1)(1+h²f_(i-1)/12)
            ]
            /
            [
                1+h²f_(i+1)/12
            ]
    */

    for (
        std::size_t i = 1;
        i + 1 < radiusGrid.size();
        ++i
    )
    {
        const double rMinus =
            radiusGrid[i - 1]
            * 1.0e-9;


        const double r =
            radiusGrid[i]
            * 1.0e-9;


        const double rPlus =
            radiusGrid[i + 1]
            * 1.0e-9;


        const double h =
            r - rMinus;


        if (h <= 0.0)
        {
            continue;
        }


        const double fMinus =
            effectiveFunction(
                rMinus
            );


        const double fCurrent =
            effectiveFunction(
                r
            );


        const double fPlus =
            effectiveFunction(
                rPlus
            );


        const double h2 =
            h * h;


        const double numerator =
            2.0
            * radialSolution[i]
            * (
                1.0
                - h2
                * fCurrent
                / 12.0
            )
            -
            radialSolution[i - 1]
            * (
                1.0
                + h2
                * fMinus
                / 12.0
            );


        const double denominator =
            1.0
            + h2
            * fPlus
            / 12.0;


        if (
            std::abs(denominator)
            < 1.0e-30
        )
        {
            radialSolution[i + 1] =
                0.0;
        }
        else
        {
            radialSolution[i + 1] =
                numerator
                / denominator;
        }


        /*
            Protect the integration against numerical overflow.
        */

        if (
            std::abs(
                radialSolution[i + 1]
            )
            > 1.0e100
        )
        {
            radialSolution[i + 1] =
                std::copysign(
                    1.0e100,
                    radialSolution[i + 1]
                );
        }
    }
}


// ============================================================
// BUILD RADIAL GRID
// ============================================================

void RadialSchrodingerSolver::buildGrid()
{
    radiusGrid.clear();


    radiusGrid.resize(
        static_cast<std::size_t>(
            gridPoints
        )
    );


    /*
        Do not start at r = 0 because:

            V(r) -> -infinity

        Start at a very small positive radius instead.
    */

    constexpr double MINIMUM_RADIUS_NM =
        1.0e-5;


    const double step =
        (
            maximumRadiusNm
            - MINIMUM_RADIUS_NM
        )
        /
        static_cast<double>(
            gridPoints - 1
        );


    for (
        int i = 0;
        i < gridPoints;
        ++i
    )
    {
        radiusGrid[
            static_cast<std::size_t>(i)
        ] =
            MINIMUM_RADIUS_NM
            +
            static_cast<double>(i)
            * step;
    }
}


// ============================================================
// VALIDATION
// ============================================================

void RadialSchrodingerSolver::validateConfiguration() const
{
    if (maximumRadiusNm <= 0.0)
    {
        throw std::invalid_argument(
            "Maximum radius must be greater than zero."
        );
    }


    if (gridPoints < 100)
    {
        throw std::invalid_argument(
            "Grid must contain at least 100 points."
        );
    }


    if (angularQuantumNumber < 0)
    {
        throw std::invalid_argument(
            "Angular quantum number cannot be negative."
        );
    }


    if (nuclearCharge < 1)
    {
        throw std::invalid_argument(
            "Nuclear charge must be greater than zero."
        );
    }
}

}