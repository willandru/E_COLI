#pragma once

#include <glm/vec3.hpp>

#include <vector>


namespace Geometry
{

    class OrbitalGeometry
    {
    public:

        OrbitalGeometry();


        // ============================================================
        // CONFIGURATION
        // ============================================================

        void setDensity(
            const std::vector<double>& density);


        void setWaveFunctionValues(
            const std::vector<double>& waveFunctionValues);


        void setCoordinates(
            const std::vector<double>& coordinates);


        void setGridSize(
            int gridSize);


        void setVoxelSize(
            double voxelSizeNm);


        void setDensityThreshold(
            double threshold);


        // ============================================================
        // CALCULATION
        // ============================================================

        void generate();


        // ============================================================
        // RESULTS
        // ============================================================

        const std::vector<glm::vec3>&
        getPositions() const;


        const std::vector<float>&
        getValues() const;


        const std::vector<float>&
        getWaveFunctionValues() const;


        double getDensityThreshold() const;

        int getGridSize() const;

        double getVoxelSize() const;

        std::size_t getPointCount() const;

        bool hasGeometry() const;


        // ============================================================
        // CLEAR
        // ============================================================

        void clear();


    private:

        /*
         * Probability density:
         *
         *      rho = |psi|^2
         *
         * Used to determine which points belong
         * to the visible orbital.
         */

        std::vector<double> density;


        /*
         * Original wave function:
         *
         *      psi
         *
         * This preserves the sign of the wave function.
         *
         * It will later be used by the renderer
         * to distinguish positive and negative phases.
         */

        std::vector<double> waveFunctionValues;


        /*
         * Coordinates are stored as:
         *
             x0, y0, z0,
             x1, y1, z1,
             x2, y2, z2,
             ...
         *
         * Therefore the vector contains
         * 3 values per grid point.
         */

        std::vector<double> coordinates;


        /*
         * Positions of points that passed
         * the density threshold.
         */

        std::vector<glm::vec3> positions;


        /*
         * Density values corresponding
         * to each generated position.
         */

        std::vector<float> values;


        /*
         * Wave function values corresponding
         * to each generated position.
         */

        std::vector<float> generatedWaveFunctionValues;


        int gridSize = 0;

        double voxelSizeNm = 0.0;

        double densityThreshold = 0.01;

        bool geometryAvailable = false;


        void validateInput() const;


        int getIndex(
            int x,
            int y,
            int z) const;
    };

}
