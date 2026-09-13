#include "WaveFunctionS.h"

#include <vector>
#include <cmath>
#include <algorithm>
#include <limits>

namespace Chemistry
{

namespace
{

constexpr double EPSILON =
    1.0e-12;


// ============================================================
// FACTORIAL
// ============================================================

double factorial(
    int n
)
{
    if (n <= 1)
        return 1.0;

    double result = 1.0;

    for (int i = 2;
         i <= n;
         ++i)
    {
        result *=
            static_cast<double>(i);
    }

    return result;
}


// ============================================================
// ASSOCIATED LAGUERRE
// ============================================================
//
// L_k^alpha(x)
//
// Recurrencia:
//
// L_0^alpha(x) = 1
//
// L_1^alpha(x) = 1 + alpha - x
//
// (k+1)L_(k+1)^alpha(x)
// =
// (2k+1+alpha-x)L_k^alpha(x)
// -
// (k+alpha)L_(k-1)^alpha(x)
//

double associatedLaguerre(
    int k,
    int alpha,
    double x
)
{
    if (k == 0)
        return 1.0;


    if (k == 1)
    {
        return
            1.0 +
            static_cast<double>(alpha) -
            x;
    }


    double previous =
        1.0;

    double current =
        1.0 +
        static_cast<double>(alpha) -
        x;


    for (int i = 1;
         i < k;
         ++i)
    {
        const double next =
            (
                (
                    2.0 *
                    static_cast<double>(i) +
                    1.0 +
                    static_cast<double>(alpha) -
                    x
                ) *
                current
                -
                (
                    static_cast<double>(i) +
                    static_cast<double>(alpha)
                ) *
                previous
            )
            /
            static_cast<double>(i + 1);


        previous =
            current;

        current =
            next;
    }


    return current;
}

}


// ============================================================
// RADIAL WAVEFUNCTION
// ============================================================

double WaveFunctionS::radial(
    int n,
    double r,
    double Z
)
{
    if (n < 1)
        return 0.0;


    if (r < 0.0)
        return 0.0;


    if (Z <= 0.0)
        Z = 1.0;


    // --------------------------------------------------------
    // rho
    // --------------------------------------------------------

    const double rho =
        2.0 *
        Z *
        r /
        static_cast<double>(n);


    // --------------------------------------------------------
    // Normalización
    // --------------------------------------------------------
    //
    // Para l = 0:
    //
    // N =
    //
    // (2Z/n)^(3/2)
    //
    // ×
    //
    // sqrt[
    //     (n-1)!
    //     /
    //     (2n n!)
    // ]
    //
    // Como usamos r en unidades de a0,
    // no necesitamos introducir explícitamente a0.
    //

    const double numerator =
        factorial(n - 1);


    const double denominator =
        2.0 *
        static_cast<double>(n) *
        factorial(n);


    const double normalization =
        std::pow(
            2.0 *
            Z /
            static_cast<double>(n),
            1.5
        )
        *
        std::sqrt(
            numerator /
            denominator
        );


    // --------------------------------------------------------
    // Laguerre
    // --------------------------------------------------------
    //
    // Para s:
    //
    // k = n - 1
    //
    // alpha = 1
    //

    const double laguerre =
        associatedLaguerre(
            n - 1,
            1,
            rho
        );


    // --------------------------------------------------------
    // Exponencial
    // --------------------------------------------------------

    const double exponential =
        std::exp(
            -rho / 2.0
        );


    return
        normalization *
        exponential *
        laguerre;
}


// ============================================================
// NUMERO DE NODOS
// ============================================================

int WaveFunctionS::nodeCount(
    int n
)
{
    if (n < 1)
        return 0;

    return n - 1;
}


// ============================================================
// BUSCAR NODOS
// ============================================================

std::vector<double>
WaveFunctionS::findNodes(
    int n,
    double Z,
    int samples
)
{
    std::vector<double> nodes;


    if (n < 2)
        return nodes;


    if (Z <= 0.0)
        Z = 1.0;


    if (samples < 100)
        samples = 100;


    // --------------------------------------------------------
    // RANGO DE BÚSQUEDA
    // --------------------------------------------------------
    //
    // El tamaño característico de un orbital hidrogenoide
    // crece aproximadamente como n²/Z.
    //
    // Este rango es suficientemente grande para n <= 7.
    //

    const double maxRadius =
        4.0 *
        static_cast<double>(n * n) /
        Z;


    const double step =
        maxRadius /
        static_cast<double>(samples);


    double previousRadius =
        0.0;

    double previousValue =
        radial(
            n,
            previousRadius,
            Z
        );


    for (int i = 1;
         i <= samples;
         ++i)
    {
        const double radius =
            static_cast<double>(i) *
            step;


        const double value =
            radial(
                n,
                radius,
                Z
            );


        // ----------------------------------------------------
        // Cambio de signo
        // ----------------------------------------------------

        if (
            previousValue *
            value
            <
            0.0
        )
        {
            double left =
                previousRadius;

            double right =
                radius;


            // ------------------------------------------------
            // BISECCION
            // ------------------------------------------------

            for (int iteration = 0;
                 iteration < 60;
                 ++iteration)
            {
                const double middle =
                    0.5 *
                    (
                        left +
                        right
                    );


                const double middleValue =
                    radial(
                        n,
                        middle,
                        Z
                    );


                const double leftValue =
                    radial(
                        n,
                        left,
                        Z
                    );


                if (
                    leftValue *
                    middleValue
                    <=
                    0.0
                )
                {
                    right =
                        middle;
                }
                else
                {
                    left =
                        middle;
                }
            }


            const double node =
                0.5 *
                (
                    left +
                    right
                );


            // ------------------------------------------------
            // Evitar duplicados
            // ------------------------------------------------

            if (
                nodes.empty() ||
                std::abs(
                    node -
                    nodes.back()
                ) > EPSILON
            )
            {
                nodes.push_back(
                    node
                );
            }
        }


        previousRadius =
            radius;

        previousValue =
            value;
    }


    return nodes;
}

}