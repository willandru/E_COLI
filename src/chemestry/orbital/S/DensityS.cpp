#include "DensityS.h"
#include "WaveFunctionS.h"

#include <cmath>
#include <vector>

namespace Chemistry
{

namespace
{

constexpr double PI =
    3.14159265358979323846;


double maximumRadius(
    int n,
    double Z
)
{
    return
        4.0 *
        static_cast<double>(n * n) /
        Z;
}

}


// ============================================================
// DENSIDAD
// ============================================================

double DensityS::density(
    int n,
    double r,
    double Z
)
{
    const double R =
        WaveFunctionS::radial(
            n,
            r,
            Z
        );


    return
        (
            R *
            R
        )
        /
        (
            4.0 *
            PI
        );
}


// ============================================================
// MAXIMOS DE DENSIDAD
// ============================================================

std::vector<double>
DensityS::findMaxima(
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


    std::vector<double> values(
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
            density(
                n,
                r,
                Z
            );
    }


    // --------------------------------------------------------
    // MAXIMOS LOCALES
    // --------------------------------------------------------

    for (int i = 1;
         i < samples;
         ++i)
    {
        if (
            values[i] >
            values[i - 1] &&
            values[i] >=
            values[i + 1]
        )
        {
            const double previous =
                values[i - 1];

            const double current =
                values[i];

            const double next =
                values[i + 1];


            const double denominator =
                previous -
                2.0 * current +
                next;


            double offset =
                0.0;


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


            maxima.push_back(
                maximumIndex *
                step
            );
        }
    }


    return maxima;
}

}