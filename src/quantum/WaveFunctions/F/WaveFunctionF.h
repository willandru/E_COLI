#pragma once

namespace Quantum
{
    class WaveFunctionF
    {
    public:

        // ============================================================
        // CONSTRUCTOR
        // ============================================================

        explicit WaveFunctionF(
            int principalQuantumNumber = 4,
            int magneticQuantumNumber = 0,
            int nuclearCharge = 1
        );


        // ============================================================
        // QUANTUM NUMBERS
        // ============================================================

        void setPrincipalQuantumNumber(int n);
        int getPrincipalQuantumNumber() const;

        int getAngularQuantumNumber() const;

        void setMagneticQuantumNumber(int m);
        int getMagneticQuantumNumber() const;

        char getType() const;


        // ============================================================
        // NUCLEAR CHARGE
        // ============================================================

        void setNuclearCharge(int nuclearCharge);
        int getNuclearCharge() const;


        // ============================================================
        // RADIAL WAVE FUNCTION
        // ============================================================

        double evaluateRadial(double radiusNm) const;


        // ============================================================
        // ANGULAR PART
        // ============================================================

        double evaluateAngular(
            double theta,
            double phi
        ) const;


        // ============================================================
        // COMPLETE WAVE FUNCTION
        // ============================================================

        double evaluate(
            double radiusNm,
            double theta,
            double phi
        ) const;


        // ============================================================
        // NODES
        // ============================================================

        int getRadialNodeCount() const;
        int getAngularNodeCount() const;
        int getTotalNodeCount() const;


        // ============================================================
        // CONSTANTS
        // ============================================================

        static constexpr double BOHR_RADIUS_NM =
            0.0529177210903;


    private:

        int principalQuantumNumber = 4;

        int magneticQuantumNumber = 0;

        int nuclearCharge = 1;


        // ============================================================
        // ASSOCIATED LAGUERRE POLYNOMIAL
        // ============================================================

        static double associatedLaguerre(
            int polynomialOrder,
            int alpha,
            double x
        );


        // ============================================================
        // VALIDATION
        // ============================================================

        static void validatePrincipalQuantumNumber(int n);

        static void validateMagneticQuantumNumber(int m);

        static void validateNuclearCharge(int nuclearCharge);

        static void validateRadius(double radiusNm);
    };
}