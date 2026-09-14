#include "WaveFunctionD.h"

#include <cmath>
#include <stdexcept>


namespace Quantum
{

    // ============================================================
    // CONSTRUCTOR
    // ============================================================

    WaveFunctionD::WaveFunctionD(
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

    void WaveFunctionD::setPrincipalQuantumNumber(
        int n
    )
    {
        validatePrincipalQuantumNumber(n);

        principalQuantumNumber = n;
    }


    int WaveFunctionD::getPrincipalQuantumNumber() const
    {
        return principalQuantumNumber;
    }


    int WaveFunctionD::getAngularQuantumNumber() const
    {
        /*
            A d orbital always has:

                l = 2
        */

        return 2;
    }


    void WaveFunctionD::setMagneticQuantumNumber(
        int m
    )
    {
        validateMagneticQuantumNumber(m);

        magneticQuantumNumber = m;
    }


    int WaveFunctionD::getMagneticQuantumNumber() const
    {
        return magneticQuantumNumber;
    }


    char WaveFunctionD::getType() const
    {
        return 'd';
    }


    // ============================================================
    // EFFECTIVE NUCLEAR CHARGE
    // ============================================================

    void WaveFunctionD::setNuclearCharge(
        double charge
    )
    {
        validateNuclearCharge(charge);

        nuclearCharge = charge;
    }


    double WaveFunctionD::getNuclearCharge() const
    {
        return nuclearCharge;
    }


    // ============================================================
    // RADIAL WAVE FUNCTION
    // ============================================================

    double WaveFunctionD::evaluateRadial(
        double radiusNm
    ) const
    {
        validateRadius(radiusNm);


        /*
            Hydrogen-like d orbital.

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

            For d:

                l = 2

            therefore:

                rho power = 2

                Laguerre order = n - 3

                Laguerre alpha = 5
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
            Normalization:

                N =
                sqrt(
                    (2Z/(n a0))^3
                    *
                    (n-3)! /
                    (2n(n+2)!)
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


        const double denominator =
            2.0
            * n
            * n
            * (
                n - 1.0
            )
            * (
                n + 1.0
            )
            * (
                n + 2.0
            );


        const double normalization =
            std::sqrt(
                std::pow(factor, 3.0)
                /
                denominator
            );


        /*
            Angular momentum contribution:

                rho^l

            For d:

                l = 2

            therefore:

                rho^2
        */

        const double radialPower =
            rho * rho;


        /*
            Associated Laguerre polynomial:

                L_(n-3)^5(rho)
        */

        const double laguerre =
            associatedLaguerre(
                principalQuantumNumber - 3,
                5,
                rho
            );


        /*
            Exponential decay:

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

    double WaveFunctionD::evaluateAngular(
        double theta,
        double phi
    ) const
    {
        /*
            Real d-orbital spherical harmonics.

            l = 2

            Five possible real orbitals:

                m =  0  -> d_z2

                m = +1  -> d_xz

                m = -1  -> d_yz

                m = +2  -> d_x2-y2

                m = -2  -> d_xy
        */


        constexpr double PI =
            3.14159265358979323846;


        const double cosTheta =
            std::cos(theta);

        const double sinTheta =
            std::sin(theta);


        const double cosPhi =
            std::cos(phi);

        const double sinPhi =
            std::sin(phi);


        switch (magneticQuantumNumber)
        {
            // ----------------------------------------------------
            // m = 0
            // d_z2
            // ----------------------------------------------------

            case 0:
            {
                const double normalization =
                    std::sqrt(
                        5.0
                        /
                        (16.0 * PI)
                    );

                return
                    normalization
                    *
                    (
                        3.0
                        * cosTheta
                        * cosTheta
                        - 1.0
                    );
            }


            // ----------------------------------------------------
            // m = +1
            // d_xz
            // ----------------------------------------------------

            case 1:
            {
                const double normalization =
                    std::sqrt(
                        15.0
                        /
                        (4.0 * PI)
                    );

                return
                    -normalization
                    *
                    sinTheta
                    * cosTheta
                    * cosPhi;
            }


            // ----------------------------------------------------
            // m = -1
            // d_yz
            // ----------------------------------------------------

            case -1:
            {
                const double normalization =
                    std::sqrt(
                        15.0
                        /
                        (4.0 * PI)
                    );

                return
                    -normalization
                    *
                    sinTheta
                    * cosTheta
                    * sinPhi;
            }


            // ----------------------------------------------------
            // m = +2
            // d_x2-y2
            // ----------------------------------------------------

            case 2:
            {
                const double normalization =
                    std::sqrt(
                        15.0
                        /
                        (16.0 * PI)
                    );

                return
                    normalization
                    *
                    sinTheta
                    * sinTheta
                    *
                    (
                        cosPhi * cosPhi
                        -
                        sinPhi * sinPhi
                    );
            }


            // ----------------------------------------------------
            // m = -2
            // d_xy
            // ----------------------------------------------------

            case -2:
            {
                const double normalization =
                    std::sqrt(
                        15.0
                        /
                        (4.0 * PI)
                    );

                return
                    normalization
                    *
                    sinTheta
                    * sinTheta
                    *
                    sinPhi
                    * cosPhi;
            }


            default:

                throw std::runtime_error(
                    "Invalid magnetic quantum number "
                    "for d orbital."
                );
        }
    }


    // ============================================================
    // COMPLETE WAVE FUNCTION
    // ============================================================

    double WaveFunctionD::evaluate(
        double radiusNm,
        double theta,
        double phi
    ) const
    {
        return
            evaluateRadial(radiusNm)
            *
            evaluateAngular(
                theta,
                phi
            );
    }


    // ============================================================
    // NODES
    // ============================================================

    int WaveFunctionD::getRadialNodeCount() const
    {
        /*
            Number of radial nodes:

                n - l - 1

            For d:

                l = 2

            therefore:

                n - 3
        */

        return principalQuantumNumber - 3;
    }


    int WaveFunctionD::getAngularNodeCount() const
    {
        /*
            Number of angular nodes:

                l = 2
        */

        return 2;
    }


    int WaveFunctionD::getTotalNodeCount() const
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

    double WaveFunctionD::associatedLaguerre(
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
            Base case:

                L_0^alpha(x) = 1
        */

        if (polynomialOrder == 0)
        {
            return 1.0;
        }


        /*
            First-order polynomial:

                L_1^alpha(x)
                =
                alpha + 1 - x
        */

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

    void WaveFunctionD::validatePrincipalQuantumNumber(
        int n
    )
    {
        /*
            A d orbital requires:

                n >= 3
        */

        if (n < 3)
        {
            throw std::invalid_argument(
                "A d orbital requires "
                "principal quantum number n >= 3."
            );
        }
    }


    void WaveFunctionD::validateMagneticQuantumNumber(
        int m
    )
    {
        /*
            For l = 2:

                m = -2, -1, 0, +1, +2
        */

        if (
            m < -2
            ||
            m > 2
        )
        {
            throw std::invalid_argument(
                "Magnetic quantum number m "
                "for a d orbital must be "
                "-2, -1, 0, +1, or +2."
            );
        }
    }


    void WaveFunctionD::validateNuclearCharge(
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


    void WaveFunctionD::validateRadius(
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