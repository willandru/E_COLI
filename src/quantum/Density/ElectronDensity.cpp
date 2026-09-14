#include "ElectronDensity.h"

#include <algorithm>
#include <cmath>
#include <stdexcept>


namespace Quantum
{

    // ============================================================
    // CONSTRUCTOR
    // ============================================================

    ElectronDensity::ElectronDensity()
    {
    }


    // ============================================================
    // CONFIGURATION
    // ============================================================

    void ElectronDensity::setMaximumRadius(
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

        resultAvailable = false;
    }


    double ElectronDensity::getMaximumRadius() const
    {
        return maximumRadiusNm;
    }


    void ElectronDensity::setGridSize(
        int size
    )
    {
        if (size < 2)
        {
            throw std::invalid_argument(
                "Grid size must be at least 2."
            );
        }

        gridSize = size;

        resultAvailable = false;
    }


    int ElectronDensity::getGridSize() const
    {
        return gridSize;
    }


    void ElectronDensity::setWaveFunction(
        const WaveFunctionEvaluator& evaluator
    )
    {
        if (!evaluator)
        {
            throw std::invalid_argument(
                "Wave function evaluator cannot be empty."
            );
        }

        waveFunction = evaluator;

        resultAvailable = false;
    }


    // ============================================================
    // CALCULATION
    // ============================================================

    void ElectronDensity::calculate()
    {
        validateConfiguration();

        buildGrid();


        const std::size_t voxelCount =
            static_cast<std::size_t>(gridSize)
            *
            static_cast<std::size_t>(gridSize)
            *
            static_cast<std::size_t>(gridSize);


        density.clear();

        density.resize(
            voxelCount,
            0.0
        );


        /*
            Store the original wave function:

                psi(x,y,z)

            This is different from the probability density:

                rho(x,y,z) = |psi(x,y,z)|^2

            The sign of psi is preserved here.

            This will later allow the renderer to distinguish
            the positive and negative phases of an orbital.
        */

        waveFunctionValues.clear();

        waveFunctionValues.resize(
            voxelCount,
            0.0
        );


        /*
            Evaluate:

                rho(x,y,z) = |psi(x,y,z)|^2

            The wave function evaluator receives spherical
            coordinates:

                r     -> radius in nm
                theta -> polar angle
                phi   -> azimuthal angle
        */

        for (int z = 0; z < gridSize; ++z)
        {
            for (int y = 0; y < gridSize; ++y)
            {
                for (int x = 0; x < gridSize; ++x)
                {
                    const int index =
                        getIndex(
                            x,
                            y,
                            z
                        );


                    const std::size_t coordinateIndex =
                        static_cast<std::size_t>(index)
                        *
                        3;


                    const double px =
                        coordinates[
                            coordinateIndex + 0
                        ];


                    const double py =
                        coordinates[
                            coordinateIndex + 1
                        ];


                    const double pz =
                        coordinates[
                            coordinateIndex + 2
                        ];


                    const double radius =
                        cartesianToRadius(
                            px,
                            py,
                            pz
                        );


                    const double theta =
                        cartesianToTheta(
                            px,
                            py,
                            pz
                        );


                    const double phi =
                        cartesianToPhi(
                            px,
                            py
                        );


                    const double psi =
                        waveFunction(
                            radius,
                            theta,
                            phi
                        );


                    /*
                        Preserve the original wave function.

                            psi

                        This retains its sign.
                    */

                    waveFunctionValues[index] =
                        psi;


                    /*
                        Probability density:

                            rho = |psi|^2

                        Since psi is real in our current
                        orbital implementation:

                            |psi|^2 = psi * psi
                    */

                    density[index] =
                        psi * psi;
                }
            }
        }


        /*
            Normalize the three-dimensional probability density:

                integral rho(x,y,z) dV = 1
        */

        normalize();


        resultAvailable = true;
    }


    // ============================================================
    // RESULTS
    // ============================================================

    const std::vector<double>&
    ElectronDensity::getDensity() const
    {
        return density;
    }


    const std::vector<double>&
    ElectronDensity::getWaveFunctionValues() const
    {
        return waveFunctionValues;
    }


    const std::vector<double>&
    ElectronDensity::getCoordinates() const
    {
        return coordinates;
    }


    double ElectronDensity::getVoxelSize() const
    {
        return voxelSizeNm;
    }


    double ElectronDensity::getMaximumDensity() const
    {
        if (density.empty())
        {
            return 0.0;
        }


        return *std::max_element(
            density.begin(),
            density.end()
        );
    }


    double ElectronDensity::getTotalProbability() const
    {
        if (!resultAvailable)
        {
            return 0.0;
        }


        if (density.empty())
        {
            return 0.0;
        }


        const double voxelVolume =
            voxelSizeNm
            *
            voxelSizeNm
            *
            voxelSizeNm;


        double totalProbability = 0.0;


        for (const double value : density)
        {
            totalProbability +=
                value
                *
                voxelVolume;
        }


        return totalProbability;
    }


    bool ElectronDensity::hasResult() const
    {
        return resultAvailable;
    }


    // ============================================================
    // NORMALIZATION
    // ============================================================

    void ElectronDensity::normalize()
    {
        if (density.empty())
        {
            return;
        }


        const double voxelVolume =
            voxelSizeNm
            *
            voxelSizeNm
            *
            voxelSizeNm;


        double totalProbability = 0.0;


        for (const double value : density)
        {
            totalProbability +=
                value
                *
                voxelVolume;
        }


        if (totalProbability <= 0.0)
        {
            return;
        }


        const double normalizationFactor =
            1.0
            /
            totalProbability;


        for (double& value : density)
        {
            value *=
                normalizationFactor;
        }
    }


    // ============================================================
    // INDEXING
    // ============================================================

    int ElectronDensity::getIndex(
        int x,
        int y,
        int z
    ) const
    {
        return
            x
            +
            gridSize
            *
            (
                y
                +
                gridSize * z
            );
    }


    // ============================================================
    // VALIDATION
    // ============================================================

    void ElectronDensity::validateConfiguration() const
    {
        if (maximumRadiusNm <= 0.0)
        {
            throw std::invalid_argument(
                "Maximum radius must be greater than zero."
            );
        }


        if (gridSize < 2)
        {
            throw std::invalid_argument(
                "Grid size must be at least 2."
            );
        }


        if (!waveFunction)
        {
            throw std::invalid_argument(
                "Wave function has not been assigned."
            );
        }
    }


    // ============================================================
    // BUILD CARTESIAN GRID
    // ============================================================

    void ElectronDensity::buildGrid()
    {
        /*
            The orbital is represented inside a cube:

                [-R, R]

            along X, Y and Z.

            Therefore the total width is:

                2R
        */

        voxelSizeNm =
            (2.0 * maximumRadiusNm)
            /
            static_cast<double>(
                gridSize - 1
            );


        const std::size_t voxelCount =
            static_cast<std::size_t>(gridSize)
            *
            static_cast<std::size_t>(gridSize)
            *
            static_cast<std::size_t>(gridSize);


        coordinates.clear();

        coordinates.resize(
            voxelCount * 3,
            0.0
        );


        const double minimum =
            -maximumRadiusNm;


        for (int z = 0; z < gridSize; ++z)
        {
            for (int y = 0; y < gridSize; ++y)
            {
                for (int x = 0; x < gridSize; ++x)
                {
                    const double px =
                        minimum
                        +
                        static_cast<double>(x)
                        *
                        voxelSizeNm;


                    const double py =
                        minimum
                        +
                        static_cast<double>(y)
                        *
                        voxelSizeNm;


                    const double pz =
                        minimum
                        +
                        static_cast<double>(z)
                        *
                        voxelSizeNm;


                    const int index =
                        getIndex(
                            x,
                            y,
                            z
                        );


                    const std::size_t coordinateIndex =
                        static_cast<std::size_t>(index)
                        *
                        3;


                    coordinates[
                        coordinateIndex + 0
                    ] =
                        px;


                    coordinates[
                        coordinateIndex + 1
                    ] =
                        py;


                    coordinates[
                        coordinateIndex + 2
                    ] =
                        pz;
                }
            }
        }
    }


    // ============================================================
    // CARTESIAN -> RADIUS
    // ============================================================

    double ElectronDensity::cartesianToRadius(
        double x,
        double y,
        double z
    ) const
    {
        return std::sqrt(
            x * x
            +
            y * y
            +
            z * z
        );
    }


    // ============================================================
    // CARTESIAN -> THETA
    // ============================================================

    double ElectronDensity::cartesianToTheta(
        double x,
        double y,
        double z
    ) const
    {
        const double radius =
            cartesianToRadius(
                x,
                y,
                z
            );


        /*
            At the origin theta has no physical meaning.

            We simply choose theta = 0.
        */

        if (radius <= 0.0)
        {
            return 0.0;
        }


        double value =
            z / radius;


        /*
            Protect acos from small floating-point
            errors outside [-1,1].
        */

        value =
            std::clamp(
                value,
                -1.0,
                1.0
            );


        return std::acos(value);
    }


    // ============================================================
    // CARTESIAN -> PHI
    // ============================================================

    double ElectronDensity::cartesianToPhi(
        double x,
        double y
    ) const
    {
        /*
            atan2 automatically handles all four quadrants.

                phi = atan2(y,x)

            Result:

                [-pi, pi]
        */

        return std::atan2(
            y,
            x
        );
    }

}