#include "WaveFunctionS.h"

#include <cmath>
#include <stdexcept>


namespace Quantum
{

    // ============================================================
    // CONSTRUCTOR
    // ============================================================

    WaveFunctionS::WaveFunctionS(
        int principalQuantumNumber,
        double nuclearCharge
    )
        : principalQuantumNumber(
            principalQuantumNumber
        ),
          nuclearCharge(
              nuclearCharge
          )
    {
        validatePrincipalQuantumNumber(
            principalQuantumNumber
        );

        validateNuclearCharge(
            nuclearCharge
        );
    }


    // ============================================================
    // QUANTUM NUMBERS
    // ============================================================

    void WaveFunctionS::setPrincipalQuantumNumber(
        int n
    )
    {
        validatePrincipalQuantumNumber(n);

        principalQuantumNumber = n;
    }


    int WaveFunctionS::getPrincipalQuantumNumber() const
    {
        return principalQuantumNumber;
    }


    int WaveFunctionS::getAngularQuantumNumber() const
    {
        /*
            An s orbital always has:

                l = 0
        */

        return 0;
    }


    char WaveFunctionS::getType() const
    {
        return 's';
    }


    // ============================================================
    // EFFECTIVE NUCLEAR CHARGE
    // ============================================================

    void WaveFunctionS::setNuclearCharge(
        double charge
    )
    {
        validateNuclearCharge(charge);

        nuclearCharge = charge;
    }


    double WaveFunctionS::getNuclearCharge() const
    {
        return nuclearCharge;
    }


    // ============================================================
    // RADIAL WAVE FUNCTION
    // ============================================================

    double WaveFunctionS::evaluateRadial(
        double radiusNm
    ) const
    {
        validateRadius(radiusNm);


        /*
            Hydrogen-like s orbital.

            General radial wave function:

                R_n0(r)
                =
                sqrt(
                    (2Z / (n a0))^3
                    *
                    (n-1)! / (2n n!)
                )
                *
                exp(-rho/2)
                *
                L_(n-1)^1(rho)

            where:

                rho = 2 Z r / (n a0)

            For l = 0:

                n - l - 1 = n - 1

            and:

                alpha = 2l + 1 = 1.
        */


        const double n =
            static_cast<double>(
                principalQuantumNumber
            );


        const double Z =
            nuclearCharge;


        const double rho =
            (
                2.0
                * Z
                * radiusNm
            )
            /
            (
                n
                * BOHR_RADIUS_NM
            );


        /*
            Normalization factor.

            Starting from:

                sqrt(
                    (2Z/(n a0))^3
                    *
                    (n-1)!/(2n n!)
                )

            and using:

                n! = n (n-1)!

            gives:

                sqrt(
                    (2Z/(n a0))^3
                    /
                    (2n^2)
                )

            which is equivalent to:

                sqrt(
                    4 Z^3
                    /
                    (n^5 a0^3)
                )
        */

        const double factor =
            (
                2.0
                * Z
            )
            /
            (
                n
                * BOHR_RADIUS_NM
            );


        const double normalization =
            std::sqrt(
                std::pow(factor, 3.0)
                /
                (
                    2.0
                    * n
                    * n
                )
            );


        /*
            Associated Laguerre polynomial:

                L_(n-1)^1(rho)
        */

        const double laguerre =
            associatedLaguerre(
                principalQuantumNumber - 1,
                1,
                rho
            );


        /*
            Exponential radial decay:

                exp(-rho/2)
        */

        const double exponential =
            std::exp(
                -0.5 * rho
            );


        return
            normalization
            * exponential
            * laguerre;
    }


    // ============================================================
    // NODES
    // ============================================================

    int WaveFunctionS::getRadialNodeCount() const
    {
        /*
            Number of radial nodes:

                n - l - 1

            For an s orbital:

                l = 0

            therefore:

                n - 1
        */

        return principalQuantumNumber - 1;
    }


    int WaveFunctionS::getAngularNodeCount() const
    {
        /*
            Number of angular nodes:

                l

            For s:

                l = 0
        */

        return 0;
    }


    int WaveFunctionS::getTotalNodeCount() const
    {
        /*
            Total number of nodes:

                n - 1
        */

        return principalQuantumNumber - 1;
    }


    // ============================================================
    // ASSOCIATED LAGUERRE POLYNOMIAL
    // ============================================================

    double WaveFunctionS::associatedLaguerre(
        int polynomialOrder,
        int alpha,
        double x
    )
    {
        if (polynomialOrder < 0)
        {
            throw std::invalid_argument(
                "Laguerre polynomial order "
                "cannot be negative."
            );
        }


        if (alpha < 0)
        {
            throw std::invalid_argument(
                "Laguerre alpha cannot be negative."
            );
        }


        /*
            Generalized associated Laguerre polynomial:

                L_n^alpha(x)

            Base cases:

                L_0^alpha(x) = 1

                L_1^alpha(x)
                =
                alpha + 1 - x
        */

        if (polynomialOrder == 0)
        {
            return 1.0;
        }


        if (polynomialOrder == 1)
        {
            return
                static_cast<double>(
                    alpha + 1
                )
                - x;
        }


        double previousPrevious = 1.0;


        double previous =
            static_cast<double>(
                alpha + 1
            )
            - x;


        for (
            int k = 2;
            k <= polynomialOrder;
            ++k
        )
        {
            /*
                Recurrence relation:

                k L_k^alpha(x)
                =
                (2k - 1 + alpha - x)
                L_(k-1)^alpha(x)
                -
                (k - 1 + alpha)
                L_(k-2)^alpha(x)
            */

            const double kd =
                static_cast<double>(k);


            const double current =
                (
                    (
                        2.0 * kd
                        - 1.0
                        + static_cast<double>(alpha)
                        - x
                    )
                    * previous
                    -
                    (
                        kd
                        - 1.0
                        + static_cast<double>(alpha)
                    )
                    * previousPrevious
                )
                /
                kd;


            previousPrevious =
                previous;


            previous =
                current;
        }


        return previous;
    }


    // ============================================================
    // VALIDATION
    // ============================================================

    void WaveFunctionS::validatePrincipalQuantumNumber(
        int n
    )
    {
        if (n < 1)
        {
            throw std::invalid_argument(
                "Principal quantum number n "
                "must be at least 1."
            );
        }
    }


    void WaveFunctionS::validateNuclearCharge(
        double charge
    )
    {
        if (charge <= 0.0)
        {
            throw std::invalid_argument(
                "Effective nuclear charge must "
                "be greater than zero."
            );
        }
    }


    void WaveFunctionS::validateRadius(
        double radiusNm
    )
    {
        if (radiusNm < 0.0)
        {
            throw std::invalid_argument(
                "Radius cannot be negative."
            );
        }
    }

}