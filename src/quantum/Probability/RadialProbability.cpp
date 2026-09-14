#include "RadialProbability.h"

#include <algorithm>
#include <cmath>
#include <stdexcept>


namespace Quantum
{

    // ============================================================
    // CONSTRUCTOR
    // ============================================================

    RadialProbability::RadialProbability()
    {
    }


    // ============================================================
    // INPUT
    // ============================================================

    void RadialProbability::setRadialGrid(
        const std::vector<double>& grid
    )
    {
        radiusGrid = grid;

        resultAvailable = false;
    }


    void RadialProbability::setRadialSolution(
        const std::vector<double>& solution
    )
    {
        radialSolution = solution;

        resultAvailable = false;
    }


    // ============================================================
    // CALCULATION
    // ============================================================

    void RadialProbability::calculate()
    {
        validateInput();


        probability.clear();

        cumulativeProbability.clear();


        probability.resize(
            radialSolution.size(),
            0.0
        );


        cumulativeProbability.resize(
            radialSolution.size(),
            0.0
        );


        /*
            The radial Schrödinger solver gives us the reduced
            radial wave function:

                u(r)

            which is related to the usual radial wave function by:

                u(r) = r R(r)

            The normalization condition is:

                integral |u(r)|^2 dr = 1

            Therefore the radial probability density is:

                P(r) = |u(r)|^2

            and:

                P(r) dr

            represents the probability of finding the electron
            between r and r + dr.
        */


        for (std::size_t i = 0;
             i < radialSolution.size();
             ++i)
        {
            probability[i] =
                radialSolution[i]
                * radialSolution[i];
        }


        /*
            Normalize the radial probability.
        */

        normalize();


        /*
            Calculate cumulative probability:

                C(r)
                =
                integral_0^r P(r') dr'
        */

        cumulativeProbability[0] = 0.0;


        for (std::size_t i = 1;
             i < probability.size();
             ++i)
        {
            const double dr =
                radiusGrid[i]
                - radiusGrid[i - 1];


            const double previous =
                probability[i - 1];


            const double current =
                probability[i];


            cumulativeProbability[i] =
                cumulativeProbability[i - 1]
                +
                0.5
                * (
                    previous
                    + current
                )
                * dr;
        }


        /*
            Numerical errors can make the final value slightly
            different from exactly 1.

            Normalize once more so the cumulative probability
            ends exactly at approximately 1.
        */

        const double total =
            cumulativeProbability.back();


        if (total > 0.0)
        {
            for (double& value :
                 cumulativeProbability)
            {
                value /= total;
            }
        }


        resultAvailable = true;
    }


    // ============================================================
    // RESULTS
    // ============================================================

    const std::vector<double>&
    RadialProbability::getProbability() const
    {
        return probability;
    }


    const std::vector<double>&
    RadialProbability::getCumulativeProbability() const
    {
        return cumulativeProbability;
    }


    // ============================================================
    // PROBABILITY AT A RADIUS
    // ============================================================

    double RadialProbability::evaluate(
        double radiusNm
    ) const
    {
        if (!resultAvailable)
        {
            throw std::runtime_error(
                "Radial probability has not been calculated."
            );
        }


        if (radiusNm < radiusGrid.front()
            || radiusNm > radiusGrid.back())
        {
            return 0.0;
        }


        /*
            Find the interval containing radiusNm.
        */

        auto upper =
            std::lower_bound(
                radiusGrid.begin(),
                radiusGrid.end(),
                radiusNm
            );


        if (upper == radiusGrid.begin())
        {
            return probability.front();
        }


        if (upper == radiusGrid.end())
        {
            return probability.back();
        }


        const std::size_t upperIndex =
            static_cast<std::size_t>(
                upper - radiusGrid.begin()
            );


        const std::size_t lowerIndex =
            upperIndex - 1;


        const double r1 =
            radiusGrid[lowerIndex];


        const double r2 =
            radiusGrid[upperIndex];


        const double p1 =
            probability[lowerIndex];


        const double p2 =
            probability[upperIndex];


        /*
            Linear interpolation.
        */

        const double fraction =
            (radiusNm - r1)
            /
            (r2 - r1);


        return
            p1
            +
            fraction
            * (p2 - p1);
    }


    // ============================================================
    // MOST PROBABLE RADIUS
    // ============================================================

    double
    RadialProbability::getMostProbableRadius() const
    {
        if (!resultAvailable)
        {
            throw std::runtime_error(
                "Radial probability has not been calculated."
            );
        }


        if (probability.empty())
        {
            return 0.0;
        }


        const auto maximum =
            std::max_element(
                probability.begin(),
                probability.end()
            );


        const std::size_t index =
            static_cast<std::size_t>(
                maximum - probability.begin()
            );


        return radiusGrid[index];
    }


    // ============================================================
    // TOTAL PROBABILITY
    // ============================================================

    double
    RadialProbability::getTotalProbability() const
    {
        if (!resultAvailable)
        {
            return 0.0;
        }


        return integrateProbability();
    }


    // ============================================================
    // NORMALIZATION
    // ============================================================

    void RadialProbability::normalize()
    {
        if (probability.empty())
        {
            return;
        }


        const double total =
            integrateProbability();


        if (total <= 0.0)
        {
            return;
        }


        const double factor =
            1.0 / total;


        for (double& value : probability)
        {
            value *= factor;
        }
    }


    // ============================================================
    // STATUS
    // ============================================================

    bool RadialProbability::hasResult() const
    {
        return resultAvailable;
    }


    // ============================================================
    // VALIDATION
    // ============================================================

    void RadialProbability::validateInput() const
    {
        if (radiusGrid.empty())
        {
            throw std::invalid_argument(
                "Radius grid cannot be empty."
            );
        }


        if (radialSolution.empty())
        {
            throw std::invalid_argument(
                "Radial solution cannot be empty."
            );
        }


        if (radiusGrid.size()
            != radialSolution.size())
        {
            throw std::invalid_argument(
                "Radius grid and radial solution "
                "must have the same size."
            );
        }


        for (std::size_t i = 1;
             i < radiusGrid.size();
             ++i)
        {
            if (radiusGrid[i]
                <= radiusGrid[i - 1])
            {
                throw std::invalid_argument(
                    "Radius grid must be strictly increasing."
                );
            }
        }
    }


    // ============================================================
    // INTEGRATION
    // ============================================================

    double
    RadialProbability::integrateProbability() const
    {
        if (probability.size() < 2)
        {
            return 0.0;
        }


        double integral = 0.0;


        /*
            Trapezoidal integration:

                integral P(r) dr
        */

        for (std::size_t i = 1;
             i < probability.size();
             ++i)
        {
            const double dr =
                radiusGrid[i]
                - radiusGrid[i - 1];


            integral +=
                0.5
                * (
                    probability[i - 1]
                    + probability[i]
                )
                * dr;
        }


        return integral;
    }

}