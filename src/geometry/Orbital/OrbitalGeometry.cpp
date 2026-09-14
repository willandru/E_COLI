#include "OrbitalGeometry.h"

#include <stdexcept>


namespace Geometry
{

    // ============================================================
    // CONSTRUCTOR
    // ============================================================

    OrbitalGeometry::OrbitalGeometry()
    {
    }


    // ============================================================
    // CONFIGURATION
    // ============================================================

    void OrbitalGeometry::setDensity(
        const std::vector<double>& newDensity)
    {
        density = newDensity;

        geometryAvailable = false;
    }


    void OrbitalGeometry::setWaveFunctionValues(
        const std::vector<double>& newWaveFunctionValues)
    {
        waveFunctionValues =
            newWaveFunctionValues;

        geometryAvailable = false;
    }


    void OrbitalGeometry::setCoordinates(
        const std::vector<double>& newCoordinates)
    {
        coordinates = newCoordinates;

        geometryAvailable = false;
    }


    void OrbitalGeometry::setGridSize(
        int newGridSize)
    {
        if (newGridSize < 2)
        {
            throw std::invalid_argument(
                "OrbitalGeometry: grid size must be at least 2."
            );
        }

        gridSize = newGridSize;

        geometryAvailable = false;
    }


    void OrbitalGeometry::setVoxelSize(
        double newVoxelSizeNm)
    {
        if (newVoxelSizeNm <= 0.0)
        {
            throw std::invalid_argument(
                "OrbitalGeometry: voxel size must be greater than zero."
            );
        }

        voxelSizeNm =
            newVoxelSizeNm;

        geometryAvailable = false;
    }


    void OrbitalGeometry::setDensityThreshold(
        double threshold)
    {
        if (threshold < 0.0)
        {
            throw std::invalid_argument(
                "OrbitalGeometry: density threshold cannot be negative."
            );
        }

        densityThreshold =
            threshold;

        geometryAvailable = false;
    }


    // ============================================================
    // CALCULATION
    // ============================================================

    void OrbitalGeometry::generate()
    {
        validateInput();

        clear();


        /*
         * We inspect every point of the 3D density grid.
         *
         * The probability density determines whether
         * the point belongs to the visible orbital:
         *
         *      rho = |psi|^2
         *
         * The wave function itself is preserved separately:
         *
         *      psi
         *
         * This allows the renderer to later determine
         * the phase of the orbital.
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


                    const double densityValue =
                        density[
                            static_cast<std::size_t>(index)
                        ];


                    /*
                     * Density controls visibility.
                     *
                     * Points below the threshold
                     * are not included in the geometry.
                     */

                    if (densityValue < densityThreshold)
                    {
                        continue;
                    }


                    /*
                     * ElectronDensity stores coordinates as:
                     *
                         [x, y, z, x, y, z, ...]
                     *
                     * Therefore each grid point occupies
                     * three consecutive positions.
                     */

                    const std::size_t coordinateIndex =
                        static_cast<std::size_t>(index)
                        *
                        3;


                    const glm::vec3 position(
                        static_cast<float>(
                            coordinates[
                                coordinateIndex
                            ]
                        ),

                        static_cast<float>(
                            coordinates[
                                coordinateIndex + 1
                            ]
                        ),

                        static_cast<float>(
                            coordinates[
                                coordinateIndex + 2
                            ]
                        )
                    );


                    positions.push_back(
                        position
                    );


                    /*
                     * Store the probability density
                     * associated with this point.
                     */

                    values.push_back(
                        static_cast<float>(
                            densityValue
                        )
                    );


                    /*
                     * Store the original wave function
                     * associated with this point.
                     *
                     * This preserves its sign:
                     *
                     *      psi > 0  -> positive phase
                     *
                     *      psi < 0  -> negative phase
                     */

                    generatedWaveFunctionValues.push_back(
                        static_cast<float>(
                            waveFunctionValues[
                                static_cast<std::size_t>(index)
                            ]
                        )
                    );
                }
            }
        }


        geometryAvailable = true;
    }


    // ============================================================
    // RESULTS
    // ============================================================

    const std::vector<glm::vec3>&
    OrbitalGeometry::getPositions() const
    {
        return positions;
    }


    const std::vector<float>&
    OrbitalGeometry::getValues() const
    {
        return values;
    }


    const std::vector<float>&
    OrbitalGeometry::getWaveFunctionValues() const
    {
        return generatedWaveFunctionValues;
    }


    double OrbitalGeometry::getDensityThreshold() const
    {
        return densityThreshold;
    }


    int OrbitalGeometry::getGridSize() const
    {
        return gridSize;
    }


    double OrbitalGeometry::getVoxelSize() const
    {
        return voxelSizeNm;
    }


    std::size_t OrbitalGeometry::getPointCount() const
    {
        return positions.size();
    }


    bool OrbitalGeometry::hasGeometry() const
    {
        return geometryAvailable;
    }


    // ============================================================
    // CLEAR
    // ============================================================

    void OrbitalGeometry::clear()
    {
        positions.clear();

        values.clear();

        generatedWaveFunctionValues.clear();

        geometryAvailable = false;
    }


    // ============================================================
    // VALIDATION
    // ============================================================

    void OrbitalGeometry::validateInput() const
    {
        if (gridSize < 2)
        {
            throw std::invalid_argument(
                "OrbitalGeometry: invalid grid size."
            );
        }


        /*
         * There is one density value per voxel:
         *
             gridSize^3
         */

        const std::size_t expectedDensitySize =
            static_cast<std::size_t>(gridSize)
            *
            static_cast<std::size_t>(gridSize)
            *
            static_cast<std::size_t>(gridSize);


        if (density.size() != expectedDensitySize)
        {
            throw std::invalid_argument(
                "OrbitalGeometry: density size does not match "
                "the 3D grid."
            );
        }


        /*
         * There must also be one wave function value
         * per voxel.
         */

        if (waveFunctionValues.size() != expectedDensitySize)
        {
            throw std::invalid_argument(
                "OrbitalGeometry: wave function value size does "
                "not match the 3D grid."
            );
        }


        /*
         * There are three coordinate values per voxel:
         *
             x, y, z
         *
         * Therefore:
         *
             coordinates.size()
             =
             gridSize^3 * 3
         */

        const std::size_t expectedCoordinateSize =
            expectedDensitySize
            *
            3;


        if (coordinates.size() != expectedCoordinateSize)
        {
            throw std::invalid_argument(
                "OrbitalGeometry: coordinate size does not "
                "match the 3D grid."
            );
        }


        if (voxelSizeNm <= 0.0)
        {
            throw std::invalid_argument(
                "OrbitalGeometry: invalid voxel size."
            );
        }
    }


    // ============================================================
    // INDEXING
    // ============================================================

    int OrbitalGeometry::getIndex(
        int x,
        int y,
        int z) const
    {
        return
            x
            +
            gridSize *
            (
                y
                +
                gridSize * z
            );
    }

}