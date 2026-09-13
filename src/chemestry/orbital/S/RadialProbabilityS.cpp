#include "RadialProbabilityS.h"
#include "WaveFunctionS.h"

#include <cmath>
#include <vector>

namespace Chemistry
{

namespace
{

constexpr double PI =
    3.14159265358979323846;


// ============================================================
// RANGO MAXIMO
// ============================================================

double maximumRadius(
    int n,
    double Z
)
{
    return
        6.0 *
        static_cast<double>(n * n) /
        Z;
}

}


// ============================================================
// PROBABILIDAD RADIAL
// ============================================================

double RadialProbabilityS::probability(
    int n,
    double r,
    double Z
)
{
    if (r < 0.0)
        return 0.0;


    const double R =
        WaveFunctionS::radial(
            n,
            r,
            Z
        );


    // --------------------------------------------------------
    // P(r) = 4πr²|ψ|²
    //
    // ψ = R / sqrt(4π)
    //
    // Por tanto:
    //
    // P(r) = r²R²
    // --------------------------------------------------------

    return
        4.0 *
        PI *
        r *
        r *
        (
            R *
            R /
            (4.0 * PI)
        );
}


// ============================================================
// MAXIMOS RADIALES
// ============================================================

std::vector<double>
RadialProbabilityS::findMaxima(
    int n,
    double Z,
    int samples
)
{
    std::vector<double> maxima;


    if (n < 1)
        return maxima;


    if (Z <= 0.0)
        Z = 1.0;


    if (samples < 100)
        samples = 100;


    const double maxRadius =
        maximumRadius(
            n,
            Z
        );


    const double step =
        maxRadius /
        static_cast<double>(samples);


    std::vector<double> values;

    values.resize(
        samples + 1
    );


    // --------------------------------------------------------
    // MUESTREO
    // --------------------------------------------------------

    for (int i = 0;
         i <= samples;
         ++i)
    {
        const double r =
            static_cast<double>(i) *
            step;


        values[i] =
            probability(
                n,
                r,
                Z
            );
    }


    // --------------------------------------------------------
    // BUSCAR MAXIMOS LOCALES
    // --------------------------------------------------------

    for (int i = 1;
         i < samples;
         ++i)
    {
        const double previous =
            values[i - 1];

        const double current =
            values[i];

        const double next =
            values[i + 1];


        if (
            current > previous &&
            current >= next
        )
        {
            // ------------------------------------------------
            // INTERPOLACION PARABOLICA
            // ------------------------------------------------
            //
            // Mejora la estimación del máximo sin tener que
            // aumentar excesivamente la resolución.
            //

            const double denominator =
                previous -
                2.0 * current +
                next;


            double offset = 0.0;


            if (
                std::abs(
                    denominator
                ) > 1.0e-15
            )
            {
                offset =
                    0.5 *
                    (
                        previous -
                        next
                    )
                    /
                    denominator;
            }


            const double maximumIndex =
                static_cast<double>(i) +
                offset;


            const double radius =
                maximumIndex *
                step;


            maxima.push_back(
                radius
            );
        }
    }


    return maxima;
}

}