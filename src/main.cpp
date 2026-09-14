#include "Window.h"
#include "Camera.h"
#include "InputMouse.h"
#include "InputKeyboard.h"
#include "Timer1.h"
#include "Performance.h"
#include "PerformanceRenderer.h"

#include "Grid.h"
#include "GridRenderer.h"

#include "DNA.h"

#include "Atom.h"

#include "NucleusRenderer.h"
#include "ElectronicRenderer.h"

#include "OrbitalBuilder.h"
#include "Orbital.h"

#include "EffectivePotential.h"

#include "WaveFunctionS.h"
#include "WaveFunctionP.h"

#include "ElectronDensity.h"
#include "OrbitalGeometry.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <cmath>
#include <iostream>
#include <memory>
#include <vector>
#include <utility>


// ============================================================
// SCENE CONFIGURATION
// ============================================================

constexpr float GRID_SIZE = 100.0f;

constexpr int GRID_DIVISIONS = 100;


// ============================================================
// ATOMIC VISUAL SCALE
// ============================================================

constexpr float NUCLEON_RADIUS = 0.08f;

constexpr double BOHR_RADIUS_NM =
    0.0529177210903;

constexpr float BOHR_RADIUS_VISUAL =
    NUCLEON_RADIUS * 10.0f;

constexpr float ATOMIC_SCALE =
    BOHR_RADIUS_VISUAL /
    static_cast<float>(BOHR_RADIUS_NM);


// ============================================================
// ORBITAL CONFIGURATION
// ============================================================

constexpr double ORBITAL_RADIUS_NM = 0.5;

constexpr int ORBITAL_GRID_SIZE = 64;


// Relative density threshold.
//
// 0.02 = render points with density >= 2%
// of the maximum density of the orbital.
//

constexpr double ORBITAL_DENSITY_THRESHOLD_FACTOR =
    0.02;


// ============================================================
// ATOM VISUAL REPRESENTATION
// ============================================================

struct AtomOrbitalGeometry
{
    std::vector<glm::vec3> positions;

    // Normalized electron density.
    std::vector<float> values;

    // Signed wave function psi.
    //
    // Used by the renderer to determine
    // the phase/color of the orbital.
    //
    std::vector<float> waveFunctionValues;

    Quantum::Orbital orbital;
};


// ============================================================
// ELECTRON COUNT BY PRINCIPAL LEVEL
// ============================================================

int countElectronsInPrincipalLevel(
    const std::vector<Quantum::Orbital>& orbitals,
    int principalQuantumNumber)
{
    int electronCount = 0;


    for (const Quantum::Orbital& orbital : orbitals)
    {
        if (
            orbital.getPrincipalQuantumNumber()
            ==
            principalQuantumNumber
        )
        {
            electronCount +=
                orbital.getElectronCount();
        }
    }


    return electronCount;
}


// ============================================================
// SLATER SCREENING
// ============================================================
//
// Current model:
//
// n = 1:
//     sigma = 0.30 * (N1 - 1)
//
// n = 2:
//     sigma = 0.85 * N1
//           + 0.35 * (N2 - 1)
//
// Phenomenological approximation.
// Not Hartree-Fock or DFT.
//

double calculateSlaterScreening(
    const std::vector<Quantum::Orbital>& orbitals,
    const Quantum::Orbital& orbital)
{
    const int n =
        orbital.getPrincipalQuantumNumber();


    const int electronsN1 =
        countElectronsInPrincipalLevel(
            orbitals,
            1
        );


    const int electronsN2 =
        countElectronsInPrincipalLevel(
            orbitals,
            2
        );


    // --------------------------------------------------------
    // 1s
    // --------------------------------------------------------

    if (n == 1)
    {
        return
            0.30 *
            static_cast<double>(
                electronsN1 - 1
            );
    }


    // --------------------------------------------------------
    // n = 2
    // --------------------------------------------------------

    if (n == 2)
    {
        return
            0.85 *
            static_cast<double>(
                electronsN1
            )
            +
            0.35 *
            static_cast<double>(
                electronsN2 - 1
            );
    }


    // --------------------------------------------------------
    // Higher levels
    // --------------------------------------------------------

    return 0.0;
}


// ============================================================
// MAIN
// ============================================================

int main()
{
    // ========================================================
    // WINDOW
    // ========================================================

    Window window(
        1280,
        720,
        "E. coli Simulation"
    );


    GLFWwindow* nativeWindow =
        window.getNativeWindow();


    if (!nativeWindow)
    {
        std::cerr
            << "Error: no se pudo crear la ventana.\n";

        return -1;
    }


    // ========================================================
    // CAMERA
    // ========================================================

    Camera camera(
        glm::vec3(
            0.0f,
            2.0f,
            85.0f
        )
    );


    // ========================================================
    // INPUT
    // ========================================================

    InputMouse mouse(nativeWindow);

    InputKeyboard keyboard(nativeWindow);


    // ========================================================
    // CORE
    // ========================================================

    Timer1 timer;

    Performance performance;


    // ========================================================
    // GRID
    // ========================================================

    Grid grid(
        GRID_SIZE,
        GRID_DIVISIONS
    );

    GridRenderer gridRenderer;


    // ========================================================
    // DNA
    // ========================================================

    DNA dna(
        "../data/ncbi_dataset/"
        "ncbi_dataset/data/"
        "GCF_000005845.2/"
        "GCF_000005845.2_ASM584v2_genomic.fna"
    );


    if (!dna.isLoaded())
    {
        std::cerr
            << "Error: no se pudo cargar el ADN.\n";

        return -1;
    }


    // ========================================================
    // ATOMS
    // ========================================================

    /*
     * First six elements of the periodic table:
     *
     *     H   Z = 1
     *     He  Z = 2
     *     Li  Z = 3
     *     Be  Z = 4
     *     B   Z = 5
     *     C   Z = 6
     */

    std::vector<Chemistry::Atom> atoms;

    atoms.emplace_back(1, 0);
    atoms.emplace_back(2, 2);
    atoms.emplace_back(3, 4);
    atoms.emplace_back(4, 5);
    atoms.emplace_back(5, 6);
    atoms.emplace_back(6, 6);


    // ========================================================
    // ATOM POSITIONS
    // ========================================================

    std::vector<glm::vec3> atomPositions =
    {
        glm::vec3(-45.0f, 0.0f, 0.0f),
        glm::vec3(-27.0f, 0.0f, 0.0f),
        glm::vec3( -9.0f, 0.0f, 0.0f),
        glm::vec3(  9.0f, 0.0f, 0.0f),
        glm::vec3( 27.0f, 0.0f, 0.0f),
        glm::vec3( 45.0f, 0.0f, 0.0f)
    };


    // ========================================================
    // NUCLEUS RENDERER
    // ========================================================

    NucleusRenderer nucleusRenderer;


    // ========================================================
    // ELECTRONIC RENDERER
    // ========================================================

    ElectronicRenderer electronicRenderer;

    electronicRenderer.initialize();

    electronicRenderer.setPointSize(
        4.0f
    );


    // ========================================================
    // BUILD ALL ORBITAL GEOMETRY
    // ========================================================

    std::vector<
        std::vector<AtomOrbitalGeometry>
    >
        atomOrbitalGeometry;


    atomOrbitalGeometry.resize(
        atoms.size()
    );


    for (
        std::size_t atomIndex = 0;
        atomIndex < atoms.size();
        ++atomIndex
    )
    {
        const Chemistry::Atom& atom =
            atoms[atomIndex];


        // ====================================================
        // ELECTRONIC STRUCTURE
        // ====================================================

        const auto orbitals =
            Quantum::OrbitalBuilder::build(
                atom.getElectronicStructure()
            );


        const int atomicNumber =
            atom.getNucleus().getProtonCount();


        std::cout
            << "\n========================================\n";


        std::cout
            << "ATOM Z = "
            << atomicNumber
            << "\n";


        std::cout
            << "Orbitales ocupados: "
            << orbitals.size()
            << "\n";


        // ====================================================
        // EACH OCCUPIED ORBITAL
        // ====================================================

        for (
            const Quantum::Orbital& orbital :
            orbitals
        )
        {
            // =================================================
            // SLATER SCREENING
            // =================================================

            const double screeningConstant =
                calculateSlaterScreening(
                    orbitals,
                    orbital
                );


            // =================================================
            // EFFECTIVE NUCLEAR POTENTIAL
            // =================================================

            Quantum::EffectivePotential
                effectivePotential(
                    atomicNumber
                );


            effectivePotential.setScreeningConstant(
                screeningConstant
            );


            const double effectiveNuclearCharge =
                effectivePotential
                    .getEffectiveNuclearCharge();


            // =================================================
            // CONSOLE INFORMATION
            // =================================================

            std::cout
                << "  "
                << orbital.getPrincipalQuantumNumber()
                << orbital.getType()
                << "  m="
                << orbital.getMagneticQuantumNumber()
                << "  e="
                << orbital.getElectronCount()
                << "  sigma="
                << screeningConstant
                << "  Zeff="
                << effectiveNuclearCharge
                << "\n";


            // =================================================
            // ELECTRON DENSITY
            // =================================================

            Quantum::ElectronDensity
                electronDensity;


            electronDensity.setMaximumRadius(
                ORBITAL_RADIUS_NM
            );


            electronDensity.setGridSize(
                ORBITAL_GRID_SIZE
            );


            // =================================================
            // S ORBITAL
            // =================================================

            if (
                orbital.getAngularQuantumNumber() == 0
            )
            {
                auto waveFunction =
                    std::make_shared<
                        Quantum::WaveFunctionS
                    >(
                        orbital.getPrincipalQuantumNumber(),
                        effectiveNuclearCharge
                    );


                electronDensity.setWaveFunction(
                    [waveFunction](
                        double radiusNm,
                        double theta,
                        double phi)
                    {
                        constexpr double PI =
                            3.14159265358979323846;


                        (void)theta;
                        (void)phi;


                        const double radial =
                            waveFunction->evaluateRadial(
                                radiusNm
                            );


                        return
                            radial
                            /
                            std::sqrt(
                                4.0 * PI
                            );
                    }
                );
            }


            // =================================================
            // P ORBITAL
            // =================================================

            else if (
                orbital.getAngularQuantumNumber() == 1
            )
            {
                auto waveFunction =
                    std::make_shared<
                        Quantum::WaveFunctionP
                    >(
                        orbital.getPrincipalQuantumNumber(),
                        orbital.getMagneticQuantumNumber(),
                        effectiveNuclearCharge
                    );


                electronDensity.setWaveFunction(
                    [waveFunction](
                        double radiusNm,
                        double theta,
                        double phi)
                    {
                        return
                            waveFunction->evaluate(
                                radiusNm,
                                theta,
                                phi
                            );
                    }
                );
            }


            // =================================================
            // OTHER ORBITALS
            // =================================================

            else
            {
                std::cerr
                    << "Orbital "
                    << orbital.getType()
                    << " no implementado todavía.\n";

                continue;
            }


            // =================================================
            // CALCULATE DENSITY
            // =================================================

            electronDensity.calculate();


            if (!electronDensity.hasResult())
            {
                std::cerr
                    << "Error calculando densidad.\n";

                return -1;
            }


            // =================================================
            // ORBITAL GEOMETRY
            // =================================================

            Geometry::OrbitalGeometry
                geometry;


            geometry.setDensity(
                electronDensity.getDensity()
            );


            // ------------------------------------------------
            // IMPORTANT:
            //
            // Preserve the original signed wave function.
            // ------------------------------------------------

            geometry.setWaveFunctionValues(
                electronDensity
                    .getWaveFunctionValues()
            );


            geometry.setCoordinates(
                electronDensity.getCoordinates()
            );


            geometry.setGridSize(
                electronDensity.getGridSize()
            );


            geometry.setVoxelSize(
                electronDensity.getVoxelSize()
            );


            // =================================================
            // RELATIVE DENSITY THRESHOLD
            // =================================================

            const double densityThreshold =
                electronDensity.getMaximumDensity()
                *
                ORBITAL_DENSITY_THRESHOLD_FACTOR;


            geometry.setDensityThreshold(
                densityThreshold
            );


            // =================================================
            // GENERATE GEOMETRY
            // =================================================

            geometry.generate();


            if (!geometry.hasGeometry())
            {
                std::cerr
                    << "Error generando geometría orbital.\n";

                return -1;
            }


            // =================================================
            // STORE RESULT
            // =================================================

            AtomOrbitalGeometry result;


            result.positions =
                geometry.getPositions();


            result.values.clear();

            result.values.reserve(
                geometry.getValues().size()
            );


            // ------------------------------------------------
            // Normalize density
            //
            // Density = |psi|²
            // ------------------------------------------------

            const double maximumDensity =
                electronDensity.getMaximumDensity();


            if (maximumDensity > 0.0)
            {
                for (
                    float value :
                    geometry.getValues()
                )
                {
                    result.values.push_back(
                        static_cast<float>(
                            static_cast<double>(value)
                            /
                            maximumDensity
                        )
                    );
                }
            }


            // ------------------------------------------------
            // Signed wave function
            //
            // psi > 0 -> positive phase
            // psi < 0 -> negative phase
            // ------------------------------------------------

            result.waveFunctionValues =
                geometry.getWaveFunctionValues();


            result.orbital =
                orbital;


            atomOrbitalGeometry[atomIndex]
                .push_back(
                    std::move(result)
                );
        }
    }


    std::cout
        << "\n========================================\n\n";


    // ========================================================
    // PERFORMANCE
    // ========================================================

    PerformanceRenderer performanceRenderer(
        "../assets/Agdasima/Agdasima-Regular.ttf",
        dna
    );


    // ========================================================
    // MAIN LOOP
    // ========================================================

    while (!window.shouldClose())
    {
        // ====================================================
        // FRAME
        // ====================================================

        performance.beginFrame();


        // ====================================================
        // TIMER
        // ====================================================

        timer.update();


        const float deltaTime =
            timer.getDeltaTime();


        // ====================================================
        // UPDATE
        // ====================================================

        performance.beginUpdate();


        mouse.update();

        keyboard.update();

        window.processEvents();


        camera.update(
            keyboard,
            mouse,
            deltaTime
        );


        if (keyboard.shouldClose())
        {
            performance.endUpdate();

            performance.endFrame();

            break;
        }


        performance.endUpdate();


        // ====================================================
        // RENDER
        // ====================================================

        performance.beginRender();

        window.clear();


        // ====================================================
        // GRID
        // ====================================================

        gridRenderer.render(
            grid,
            camera,
            window
        );


        // ====================================================
        // CAMERA MATRICES
        // ====================================================

        const glm::mat4 view =
            camera.getViewMatrix();


        const glm::mat4 projection =
            camera.getProjectionMatrix(
                window.getAspectRatio()
            );


        // ====================================================
        // RENDER ALL ATOMS
        // ====================================================

        for (
            std::size_t atomIndex = 0;
            atomIndex < atoms.size();
            ++atomIndex
        )
        {
            const Chemistry::Atom& atom =
                atoms[atomIndex];


            const glm::vec3& atomPosition =
                atomPositions[atomIndex];


            // =================================================
            // NUCLEUS
            // =================================================

            nucleusRenderer.render(
                atom.getNucleus(),
                atomPosition,
                camera,
                window
            );


            // =================================================
            // ORBITALS
            // =================================================

            for (
                const AtomOrbitalGeometry&
                    orbitalGeometry :
                atomOrbitalGeometry[atomIndex]
            )
            {
                if (
                    orbitalGeometry.positions.empty()
                )
                {
                    continue;
                }


                // =============================================
                // MODEL MATRIX
                // =============================================

                const glm::mat4 model =
                    glm::translate(
                        glm::mat4(1.0f),
                        atomPosition
                    )
                    *
                    glm::scale(
                        glm::mat4(1.0f),
                        glm::vec3(
                            ATOMIC_SCALE,
                            ATOMIC_SCALE,
                            ATOMIC_SCALE
                        )
                    );


                // =============================================
                // MATRICES
                // =============================================

                electronicRenderer.setModelMatrix(
                    model
                );


                electronicRenderer.setViewMatrix(
                    view
                );


                electronicRenderer.setProjectionMatrix(
                    projection
                );


                // =============================================
                // POSITIONS
                // =============================================

                electronicRenderer.setPositions(
                    orbitalGeometry.positions
                );


                // =============================================
                // DENSITY
                // =============================================

                electronicRenderer.setValues(
                    orbitalGeometry.values
                );


                // =============================================
                // WAVE FUNCTION
                // =============================================
                //
                // This is what allows the shader to distinguish
                // positive and negative phase.
                //
                // Positive psi -> positiveColor
                // Negative psi -> negativeColor
                //

                electronicRenderer.setWaveFunctionValues(
                    orbitalGeometry.waveFunctionValues
                );


                // =============================================
                // RENDER
                // =============================================

                electronicRenderer.render();
            }
        }


        // ====================================================
        // END RENDER
        // ====================================================

        performance.endRender();


        // ====================================================
        // PERFORMANCE UI
        // ====================================================

        performanceRenderer.render(
            performance,
            window
        );


        // ====================================================
        // PRESENT
        // ====================================================

        window.present();


        // ====================================================
        // END FRAME
        // ====================================================

        performance.endFrame();
    }


    return 0;
}