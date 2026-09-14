#pragma once

#include <vector>


namespace Quantum
{
    class RadialProbability
    {
    public:

        // ============================================================
        // CONSTRUCTOR
        // ============================================================

        RadialProbability();


        // ============================================================
        // INPUT
        // ============================================================

        void setRadialGrid(
            const std::vector<double>& radiusGrid
        );

        void setRadialSolution(
            const std::vector<double>& radialSolution
        );


        // ============================================================
        // CALCULATION
        // ============================================================

        void calculate();


        // ============================================================
        // RESULTS
        // ============================================================

        const std::vector<double>&
        getProbability() const;


        const std::vector<double>&
        getCumulativeProbability() const;


        // ============================================================
        // PROBABILITY AT A RADIUS
        // ============================================================

        double evaluate(double radiusNm) const;


        // ============================================================
        // MOST PROBABLE RADIUS
        // ============================================================

        double getMostProbableRadius() const;


        // ============================================================
        // TOTAL PROBABILITY
        // ============================================================

        double getTotalProbability() const;


        // ============================================================
        // NORMALIZATION
        // ============================================================

        void normalize();


        // ============================================================
        // STATUS
        // ============================================================

        bool hasResult() const;


    private:

        // ============================================================
        // INPUT DATA
        // ============================================================

        std::vector<double> radiusGrid;

        std::vector<double> radialSolution;


        // ============================================================
        // RESULTS
        // ============================================================

        std::vector<double> probability;

        std::vector<double> cumulativeProbability;


        // ============================================================
        // STATUS
        // ============================================================

        bool resultAvailable = false;


        // ============================================================
        // INTERNAL METHODS
        // ============================================================

        void validateInput() const;

        double integrateProbability() const;
    };
}