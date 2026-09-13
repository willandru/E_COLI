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

#include <glm/glm.hpp>

#include <iostream>


// ============================================================
// SCENE CONFIGURATION
// ============================================================

// Distancia entre los centros de los átomos.
constexpr float ATOM_SPACING = 10.0f;

// Tamaño de la cuadrícula.
constexpr float GRID_SIZE = 100.0f;

constexpr int GRID_DIVISIONS = 100;


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
            3.0f,
            40.0f
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

    Chemistry::Atom hydrogen(
        1,
        0
    );

    Chemistry::Atom helium(
        2,
        2
    );

    Chemistry::Atom lithium(
        3,
        4
    );

    Chemistry::Atom beryllium(
        4,
        5
    );

    Chemistry::Atom boron(
        5,
        6
    );

    Chemistry::Atom carbon(
        6,
        6
    );

    Chemistry::Atom nitrogen(
        7,
        7
    );

    Chemistry::Atom oxygen(
        8,
        8
    );

    Chemistry::Atom fluorine(
        9,
        10
    );

    Chemistry::Atom neon(
        10,
        10
    );


    // ========================================================
    // ATOM POSITIONS
    // ========================================================

    const glm::vec3 hydrogenPosition(
        -4.5f * ATOM_SPACING,
        0.0f,
        0.0f
    );

    const glm::vec3 heliumPosition(
        -3.5f * ATOM_SPACING,
        0.0f,
        0.0f
    );

    const glm::vec3 lithiumPosition(
        -2.5f * ATOM_SPACING,
        0.0f,
        0.0f
    );

    const glm::vec3 berylliumPosition(
        -1.5f * ATOM_SPACING,
        0.0f,
        0.0f
    );

    const glm::vec3 boronPosition(
        -0.5f * ATOM_SPACING,
        0.0f,
        0.0f
    );

    const glm::vec3 carbonPosition(
        0.5f * ATOM_SPACING,
        0.0f,
        0.0f
    );

    const glm::vec3 nitrogenPosition(
        1.5f * ATOM_SPACING,
        0.0f,
        0.0f
    );

    const glm::vec3 oxygenPosition(
        2.5f * ATOM_SPACING,
        0.0f,
        0.0f
    );

    const glm::vec3 fluorinePosition(
        3.5f * ATOM_SPACING,
        0.0f,
        0.0f
    );

    const glm::vec3 neonPosition(
        4.5f * ATOM_SPACING,
        0.0f,
        0.0f
    );


    // ========================================================
    // NUCLEUS RENDERER
    // ========================================================

    NucleusRenderer nucleusRenderer;


    // ========================================================
    // ELECTRONIC RENDERERS
    // ========================================================

    ElectronicRenderer hydrogenElectronicRenderer;

    ElectronicRenderer heliumElectronicRenderer;

    ElectronicRenderer lithiumElectronicRenderer;

    ElectronicRenderer berylliumElectronicRenderer;

    ElectronicRenderer boronElectronicRenderer;

    ElectronicRenderer carbonElectronicRenderer;

    ElectronicRenderer nitrogenElectronicRenderer;

    ElectronicRenderer oxygenElectronicRenderer;

    ElectronicRenderer fluorineElectronicRenderer;

    ElectronicRenderer neonElectronicRenderer;


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
        // HYDROGEN
        // ====================================================

        nucleusRenderer.render(
            hydrogen.getNucleus(),
            hydrogenPosition,
            camera,
            window
        );

        hydrogenElectronicRenderer.render(
            hydrogen.getElectronicStructure(),
            hydrogenPosition,
            camera,
            window
        );


        // ====================================================
        // HELIUM
        // ====================================================

        nucleusRenderer.render(
            helium.getNucleus(),
            heliumPosition,
            camera,
            window
        );

        heliumElectronicRenderer.render(
            helium.getElectronicStructure(),
            heliumPosition,
            camera,
            window
        );


        // ====================================================
        // LITHIUM
        // ====================================================

        nucleusRenderer.render(
            lithium.getNucleus(),
            lithiumPosition,
            camera,
            window
        );

        lithiumElectronicRenderer.render(
            lithium.getElectronicStructure(),
            lithiumPosition,
            camera,
            window
        );


        // ====================================================
        // BERYLLIUM
        // ====================================================

        nucleusRenderer.render(
            beryllium.getNucleus(),
            berylliumPosition,
            camera,
            window
        );

        berylliumElectronicRenderer.render(
            beryllium.getElectronicStructure(),
            berylliumPosition,
            camera,
            window
        );


        // ====================================================
        // BORON
        // ====================================================

        nucleusRenderer.render(
            boron.getNucleus(),
            boronPosition,
            camera,
            window
        );

        boronElectronicRenderer.render(
            boron.getElectronicStructure(),
            boronPosition,
            camera,
            window
        );


        // ====================================================
        // CARBON
        // ====================================================

        nucleusRenderer.render(
            carbon.getNucleus(),
            carbonPosition,
            camera,
            window
        );

        carbonElectronicRenderer.render(
            carbon.getElectronicStructure(),
            carbonPosition,
            camera,
            window
        );


        // ====================================================
        // NITROGEN
        // ====================================================

        nucleusRenderer.render(
            nitrogen.getNucleus(),
            nitrogenPosition,
            camera,
            window
        );

        nitrogenElectronicRenderer.render(
            nitrogen.getElectronicStructure(),
            nitrogenPosition,
            camera,
            window
        );


        // ====================================================
        // OXYGEN
        // ====================================================

        nucleusRenderer.render(
            oxygen.getNucleus(),
            oxygenPosition,
            camera,
            window
        );

        oxygenElectronicRenderer.render(
            oxygen.getElectronicStructure(),
            oxygenPosition,
            camera,
            window
        );


        // ====================================================
        // FLUORINE
        // ====================================================

        nucleusRenderer.render(
            fluorine.getNucleus(),
            fluorinePosition,
            camera,
            window
        );

        fluorineElectronicRenderer.render(
            fluorine.getElectronicStructure(),
            fluorinePosition,
            camera,
            window
        );


        // ====================================================
        // NEON
        // ====================================================

        nucleusRenderer.render(
            neon.getNucleus(),
            neonPosition,
            camera,
            window
        );

        neonElectronicRenderer.render(
            neon.getElectronicStructure(),
            neonPosition,
            camera,
            window
        );


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