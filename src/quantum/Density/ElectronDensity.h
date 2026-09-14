#pragma once

#include <functional>
#include <vector>


namespace Quantum
{
    class ElectronDensity
    {
    public:

        using WaveFunctionEvaluator =
            std::function<double(
                double radiusNm,
                double theta,
                double phi
            )>;


        ElectronDensity();


        // ============================================================
        // CONFIGURATION
        // ============================================================

        void setMaximumRadius(double radiusNm);

        double getMaximumRadius() const;


        void setGridSize(int gridSize);

        int getGridSize() const;


        void setWaveFunction(
            const WaveFunctionEvaluator& waveFunction
        );


        // ============================================================
        // CALCULATION
        // ============================================================

        void calculate();


        // ============================================================
        // RESULTS
        // ============================================================

        const std::vector<double>& getDensity() const;

        const std::vector<double>& getWaveFunctionValues() const;

        const std::vector<double>& getCoordinates() const;


        double getVoxelSize() const;

        double getMaximumDensity() const;

        double getTotalProbability() const;


        bool hasResult() const;


        // ============================================================
        // NORMALIZATION
        // ============================================================

        void normalize();


        // ============================================================
        // INDEXING
        // ============================================================

        int getIndex(
            int x,
            int y,
            int z
        ) const;


    private:

        double maximumRadiusNm = 1.0;

        int gridSize = 64;

        double voxelSizeNm = 0.0;


        WaveFunctionEvaluator waveFunction;


        /*
         * Probability density:
         *
         *      rho = |psi|^2
         *
         * Used to determine where the electron
         * probability is concentrated.
         */

        std::vector<double> density;


        /*
         * Original wave function:
         *
         *      psi
         *
         * Unlike |psi|^2, psi preserves its sign.
         *
         * Therefore this value allows the renderer
         * to distinguish the different phases/lobes
         * of an orbital.
         */

        std::vector<double> waveFunctionValues;


        /*
         * Cartesian coordinates:
         *
         *      x, y, z
         */

        std::vector<double> coordinates;


        bool resultAvailable = false;


        void validateConfiguration() const;

        void buildGrid();


        double cartesianToRadius(
            double x,
            double y,
            double z
        ) const;


        double cartesianToTheta(
            double x,
            double y,
            double z
        ) const;


        double cartesianToPhi(
            double x,
            double y
        ) const;
    };
}