#include "WaveFunctionP.h"

#include <cmath>
#include <stdexcept>


namespace Quantum
{

    // ============================================================
    // CONSTRUCTOR
    // ============================================================

    WaveFunctionP::WaveFunctionP(
        int principalQuantumNumber,
        int magneticQuantumNumber,
        double nuclearCharge
    )
        : principalQuantumNumber(
            principalQuantumNumber
        ),
          magneticQuantumNumber(
              magneticQuantumNumber
          ),
          nuclearCharge(
              nuclearCharge
          )
    {
        validatePrincipalQuantumNumber(
            principalQuantumNumber
        );

        validateMagneticQuantumNumber(
            magneticQuantumNumber
        );

        validateNuclearCharge(
            nuclearCharge
        );
    }


    // ============================================================
    // QUANTUM NUMBERS
    // ============================================================

    void WaveFunctionP::setPrincipalQuantumNumber(
        int n
    )
    {
        validatePrincipalQuantumNumber(n);

        principalQuantumNumber = n;
    }


    int WaveFunctionP::getPrincipalQuantumNumber() const
    {
        return principalQuantumNumber;
    }


    int WaveFunctionP::getAngularQuantumNumber() const
    {
        /*
            A p orbital always has:

                l = 1
        */

        return 1;
    }


    void WaveFunctionP::setMagneticQuantumNumber(
        int m
    )
    {
        validateMagneticQuantumNumber(m);

        magneticQuantumNumber = m;
    }


    int WaveFunctionP::getMagneticQuantumNumber() const
    {
        return magneticQuantumNumber;
    }


    char WaveFunctionP::getType() const
    {
        return 'p';
    }


    // ============================================================
    // EFFECTIVE NUCLEAR CHARGE
    // ============================================================

    void WaveFunctionP::setNuclearCharge(
        double charge
    )
    {
        validateNuclearCharge(charge);

        nuclearCharge = charge;
    }


    double WaveFunctionP::getNuclearCharge() const
    {
        return nuclearCharge;
    }


    // ============================================================
    // RADIAL WAVE FUNCTION
    // ============================================================

    double WaveFunctionP::evaluateRadial(
        double radiusNm
    ) const
    {
        validateRadius(radiusNm);


        /*
            Hydrogen-like p orbital.

            General radial wave function:

                R_nl(r)
                =
                N
                *
                rho^l
                *
                exp(-rho/2)
                *
                L_(n-l-1)^(2l+1)(rho)

            where:

                rho = 2 Z r / (n a0)

            For p:

                l = 1

            therefore:

                radial polynomial order
                    = n - 2

                Laguerre alpha
                    = 3
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
            Normalization factor for l = 1:

                N =
                sqrt(
                    (2Z/(n a0))^3
                    *
                    (n-2)! /
                    (2n(n+1)!)
                )

            Since:

                (n+1)! =
                (n+1)n(n-1)(n-2)!

            this can be written as:

                N =
                sqrt(
                    (2Z/(n a0))^3 /
                    (
                        2 n^2 (n^2 - 1)
                    )
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
                    * (n * n - 1.0)
                )
            );


        /*
            Angular momentum contribution:

                rho^l

            For p:

                l = 1

            therefore:

                rho
        */

        const double radialPower =
            rho;


        /*
            Associated Laguerre polynomial:

                L_(n-2)^3(rho)
        */

        const double laguerre =
            associatedLaguerre(
                principalQuantumNumber - 2,
                3,
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
            * radialPower
            * exponential
            * laguerre;
    }


    // ============================================================
    // ANGULAR WAVE FUNCTION
    // ============================================================

    double WaveFunctionP::evaluateAngular(
        double theta,
        double phi
    ) const
    {
        /*
            Real spherical harmonics for p orbitals.

            m = 0:

                p_z

                Y_10 =
                sqrt(3 / 4pi) cos(theta)


            m = +1:

                p_x

                Y_11 =
                -sqrt(3 / 4pi)
                sin(theta) cos(phi)


            m = -1:

                p_y

                Y_1-1 =
                -sqrt(3 / 4pi)
                sin(theta) sin(phi)
        */


        constexpr double PI =
            3.14159265358979323846;


        // std::sqrt() is not constexpr with the current
        // MSVC configuration, so this must be const.
        const double normalization =
            std::sqrt(
                3.0
                /
                (4.0 * PI)
            );


        switch (magneticQuantumNumber)
        {
            // ----------------------------------------------------
            // m = 0  ->  p_z
            // ----------------------------------------------------

            case 0:

                return
                    normalization
                    * std::cos(theta);


            // ----------------------------------------------------
            // m = +1  ->  p_x
            // ----------------------------------------------------

            case 1:

                return
                    -normalization
                    * std::sin(theta)
                    * std::cos(phi);


            // ----------------------------------------------------
            // m = -1  ->  p_y
            // ----------------------------------------------------

            case -1:

                return
                    -normalization
                    * std::sin(theta)
                    * std::sin(phi);


            default:

                throw std::runtime_error(
                    "Invalid magnetic quantum number "
                    "for p orbital."
                );
        }
    }


    // ============================================================
    // COMPLETE WAVE FUNCTION
    // ============================================================

    double WaveFunctionP::evaluate(
        double radiusNm,
        double theta,
        double phi
    ) const
    {
        return
            evaluateRadial(radiusNm)
            *
            evaluateAngular(theta, phi);
    }


    // ============================================================
    // NODES
    // ============================================================

    int WaveFunctionP::getRadialNodeCount() const
    {
        /*
            Number of radial nodes:

                n - l - 1

            For p:

                l = 1

            therefore:

                n - 2
        */

        return principalQuantumNumber - 2;
    }


    int WaveFunctionP::getAngularNodeCount() const
    {
        /*
            Number of angular nodes:

                l

            For p:

                l = 1
        */

        return 1;
    }


    int WaveFunctionP::getTotalNodeCount() const
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

    double WaveFunctionP::associatedLaguerre(
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

    void WaveFunctionP::validatePrincipalQuantumNumber(
        int n
    )
    {
        /*
            A p orbital requires:

                n >= 2
        */

        if (n < 2)
        {
            throw std::invalid_argument(
                "A p orbital requires "
                "principal quantum number n >= 2."
            );
        }
    }


    void WaveFunctionP::validateMagneticQuantumNumber(
        int m
    )
    {
        /*
            For l = 1:

                m = -1, 0, +1
        */

        if (
            m < -1
            ||
            m > 1
        )
        {
            throw std::invalid_argument(
                "Magnetic quantum number m "
                "for a p orbital must be -1, 0, or +1."
            );
        }
    }


    void WaveFunctionP::validateNuclearCharge(
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


    void WaveFunctionP::validateRadius(
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
