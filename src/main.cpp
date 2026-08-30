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

#include "ElectronConfiguration.h"
#include "AtomicOrbital.h"
#include "ElectronDensity.h"
#include "ElectronDensityRenderer.h"

#include <iostream>


// ============================================================
// MAIN
// ============================================================

int main()
{
    // ========================================================
    // 1. WINDOW
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
    // 2. CAMERA
    // ========================================================

    Camera camera(
        glm::vec3(
            0.0f,
            3.0f,
            35.0f
        )
    );


    // ========================================================
    // 3. INPUT
    // ========================================================

    InputMouse mouse(
        nativeWindow
    );


    InputKeyboard keyboard(
        nativeWindow
    );


    // ========================================================
    // 4. TIME / PERFORMANCE
    // ========================================================

    Timer1 timer;

    Performance performance;


    // ========================================================
    // 5. WORLD
    // ========================================================

    Grid grid(
        100.0f,
        100
    );


    // ========================================================
    // 6. DNA
    // ========================================================
    //
    // El ADN se mantiene cargado porque otros módulos pueden
    // utilizarlo, pero NO se renderiza.
    //
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
    // 7. GRID RENDERER
    // ========================================================

    GridRenderer gridRenderer;


    // ========================================================
    // 8. CONFIGURACIÓN DEL CAMPO ELECTRÓNICO
    // ========================================================

    const glm::vec3 densitySize(
        8.0f,
        8.0f,
        8.0f
    );


    const glm::ivec3 densityResolution(
        70,
        70,
        70
    );


    // ========================================================
    // 9. ESPACIAMIENTO DE LOS GRUPOS
    // ========================================================
    //
    // Cada grupo se encuentra separado sobre X.
    //
    //     1s
    //
    //          2s + 2p
    //
    //                    3s + 3p
    //
    //                              4s + 4p
    //
    // ========================================================

    constexpr float GROUP_SPACING = 18.0f;


    const float group1X = -27.0f;
    const float group2X =  -9.0f;
    const float group3X =   9.0f;
    const float group4X =  27.0f;


    // ========================================================
    // 10. POSICIONES DE LOS ORBITALES
    // ========================================================

    // --------------------------------------------------------
    // Grupo 1
    // --------------------------------------------------------

    const glm::vec3 position1s(
        group1X,
        0.0f,
        0.0f
    );


    // --------------------------------------------------------
    // Grupo 2
    // --------------------------------------------------------

    const glm::vec3 position2s(
        group2X,
        0.0f,
        0.0f
    );


    const glm::vec3 position2px(
        group2X,
        0.0f,
        0.0f
    );


    const glm::vec3 position2py(
        group2X,
        0.0f,
        0.0f
    );


    const glm::vec3 position2pz(
        group2X,
        0.0f,
        0.0f
    );


    // --------------------------------------------------------
    // Grupo 3
    // --------------------------------------------------------

    const glm::vec3 position3s(
        group3X,
        0.0f,
        0.0f
    );


    const glm::vec3 position3px(
        group3X,
        0.0f,
        0.0f
    );


    const glm::vec3 position3py(
        group3X,
        0.0f,
        0.0f
    );


    const glm::vec3 position3pz(
        group3X,
        0.0f,
        0.0f
    );


    // --------------------------------------------------------
    // Grupo 4
    // --------------------------------------------------------

    const glm::vec3 position4s(
        group4X,
        0.0f,
        0.0f
    );


    const glm::vec3 position4px(
        group4X,
        0.0f,
        0.0f
    );


    const glm::vec3 position4py(
        group4X,
        0.0f,
        0.0f
    );


    const glm::vec3 position4pz(
        group4X,
        0.0f,
        0.0f
    );


    // ========================================================
    // 11. ATOMIC ORBITALS
    // ========================================================

    // ========================================================
    // 1s
    // ========================================================

    Chemistry::AtomicOrbital orbital1s(
        1,
        Chemistry::OrbitalType::S,
        Chemistry::OrbitalOrientation::None,
        position1s
    );


    // ========================================================
    // 2s
    // ========================================================

    Chemistry::AtomicOrbital orbital2s(
        2,
        Chemistry::OrbitalType::S,
        Chemistry::OrbitalOrientation::None,
        position2s
    );


    // ========================================================
    // 2p
    // ========================================================

    Chemistry::AtomicOrbital orbital2px(
        2,
        Chemistry::OrbitalType::P,
        Chemistry::OrbitalOrientation::X,
        position2px
    );


    Chemistry::AtomicOrbital orbital2py(
        2,
        Chemistry::OrbitalType::P,
        Chemistry::OrbitalOrientation::Y,
        position2py
    );


    Chemistry::AtomicOrbital orbital2pz(
        2,
        Chemistry::OrbitalType::P,
        Chemistry::OrbitalOrientation::Z,
        position2pz
    );


    // ========================================================
    // 3s
    // ========================================================

    Chemistry::AtomicOrbital orbital3s(
        3,
        Chemistry::OrbitalType::S,
        Chemistry::OrbitalOrientation::None,
        position3s
    );


    // ========================================================
    // 3p
    // ========================================================

    Chemistry::AtomicOrbital orbital3px(
        3,
        Chemistry::OrbitalType::P,
        Chemistry::OrbitalOrientation::X,
        position3px
    );


    Chemistry::AtomicOrbital orbital3py(
        3,
        Chemistry::OrbitalType::P,
        Chemistry::OrbitalOrientation::Y,
        position3py
    );


    Chemistry::AtomicOrbital orbital3pz(
        3,
        Chemistry::OrbitalType::P,
        Chemistry::OrbitalOrientation::Z,
        position3pz
    );


    // ========================================================
    // 4s
    // ========================================================

    Chemistry::AtomicOrbital orbital4s(
        4,
        Chemistry::OrbitalType::S,
        Chemistry::OrbitalOrientation::None,
        position4s
    );


    // ========================================================
    // 4p
    // ========================================================

    Chemistry::AtomicOrbital orbital4px(
        4,
        Chemistry::OrbitalType::P,
        Chemistry::OrbitalOrientation::X,
        position4px
    );


    Chemistry::AtomicOrbital orbital4py(
        4,
        Chemistry::OrbitalType::P,
        Chemistry::OrbitalOrientation::Y,
        position4py
    );


    Chemistry::AtomicOrbital orbital4pz(
        4,
        Chemistry::OrbitalType::P,
        Chemistry::OrbitalOrientation::Z,
        position4pz
    );


    // ========================================================
    // 12. ELECTRON DENSITIES
    // ========================================================

    Chemistry::ElectronDensity density1s(
        position1s - densitySize * 0.5f,
        densitySize,
        densityResolution
    );


    Chemistry::ElectronDensity density2s(
        position2s - densitySize * 0.5f,
        densitySize,
        densityResolution
    );


    Chemistry::ElectronDensity density2px(
        position2px - densitySize * 0.5f,
        densitySize,
        densityResolution
    );


    Chemistry::ElectronDensity density2py(
        position2py - densitySize * 0.5f,
        densitySize,
        densityResolution
    );


    Chemistry::ElectronDensity density2pz(
        position2pz - densitySize * 0.5f,
        densitySize,
        densityResolution
    );


    Chemistry::ElectronDensity density3s(
        position3s - densitySize * 0.5f,
        densitySize,
        densityResolution
    );


    Chemistry::ElectronDensity density3px(
        position3px - densitySize * 0.5f,
        densitySize,
        densityResolution
    );


    Chemistry::ElectronDensity density3py(
        position3py - densitySize * 0.5f,
        densitySize,
        densityResolution
    );


    Chemistry::ElectronDensity density3pz(
        position3pz - densitySize * 0.5f,
        densitySize,
        densityResolution
    );


    Chemistry::ElectronDensity density4s(
        position4s - densitySize * 0.5f,
        densitySize,
        densityResolution
    );


    Chemistry::ElectronDensity density4px(
        position4px - densitySize * 0.5f,
        densitySize,
        densityResolution
    );


    Chemistry::ElectronDensity density4py(
        position4py - densitySize * 0.5f,
        densitySize,
        densityResolution
    );


    Chemistry::ElectronDensity density4pz(
        position4pz - densitySize * 0.5f,
        densitySize,
        densityResolution
    );


    // ========================================================
    // 13. GENERAR DENSIDADES
    // ========================================================

    density1s.generate(
        orbital1s
    );


    density2s.generate(
        orbital2s
    );


    density2px.generate(
        orbital2px
    );


    density2py.generate(
        orbital2py
    );


    density2pz.generate(
        orbital2pz
    );


    density3s.generate(
        orbital3s
    );


    density3px.generate(
        orbital3px
    );


    density3py.generate(
        orbital3py
    );


    density3pz.generate(
        orbital3pz
    );


    density4s.generate(
        orbital4s
    );


    density4px.generate(
        orbital4px
    );


    density4py.generate(
        orbital4py
    );


    density4pz.generate(
        orbital4pz
    );


    // ========================================================
    // 14. ELECTRON DENSITY RENDERER
    // ========================================================

    Chemistry::ElectronDensityRenderer
        electronDensityRenderer;


    // ========================================================
    // 15. PERFORMANCE RENDERER
    // ========================================================

    PerformanceRenderer performanceRenderer(
        "../assets/Agdasima/Agdasima-Regular.ttf",
        dna
    );


    // ========================================================
    // 16. MAIN LOOP
    // ========================================================

    while (!window.shouldClose())
    {
        // ====================================================
        // PERFORMANCE - BEGIN FRAME
        // ====================================================

        performance.beginFrame();


        // ====================================================
        // TIME
        // ====================================================

        timer.update();


        const float deltaTime =
            timer.getDeltaTime();


        // ====================================================
        // UPDATE
        // ====================================================

        performance.beginUpdate();


        // ----------------------------------------------------
        // INPUT
        // ----------------------------------------------------

        mouse.update();

        keyboard.update();

        window.processEvents();


        // ----------------------------------------------------
        // CAMERA
        // ----------------------------------------------------

        camera.update(
            keyboard,
            mouse,
            deltaTime
        );


        // ----------------------------------------------------
        // ESC
        // ----------------------------------------------------

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


        // ----------------------------------------------------
        // CLEAR
        // ----------------------------------------------------

        window.clear();


        // ----------------------------------------------------
        // GRID
        // ----------------------------------------------------

        gridRenderer.render(
            grid,
            camera,
            window
        );


        // ====================================================
        // 1s
        // ====================================================

        electronDensityRenderer.render(
            density1s,
            camera,
            window
        );


        // ====================================================
        // 2s
        // ====================================================

        electronDensityRenderer.render(
            density2s,
            camera,
            window
        );


        // ====================================================
        // 2px
        // ====================================================

        electronDensityRenderer.render(
            density2px,
            camera,
            window
        );


        // ====================================================
        // 2py
        // ====================================================

        electronDensityRenderer.render(
            density2py,
            camera,
            window
        );


        // ====================================================
        // 2pz
        // ====================================================

        electronDensityRenderer.render(
            density2pz,
            camera,
            window
        );


        // ====================================================
        // 3s
        // ====================================================

        electronDensityRenderer.render(
            density3s,
            camera,
            window
        );


        // ====================================================
        // 3px
        // ====================================================

        electronDensityRenderer.render(
            density3px,
            camera,
            window
        );


        // ====================================================
        // 3py
        // ====================================================

        electronDensityRenderer.render(
            density3py,
            camera,
            window
        );


        // ====================================================
        // 3pz
        // ====================================================

        electronDensityRenderer.render(
            density3pz,
            camera,
            window
        );


        // ====================================================
        // 4s
        // ====================================================

        electronDensityRenderer.render(
            density4s,
            camera,
            window
        );


        // ====================================================
        // 4px
        // ====================================================

        electronDensityRenderer.render(
            density4px,
            camera,
            window
        );


        // ====================================================
        // 4py
        // ====================================================

        electronDensityRenderer.render(
            density4py,
            camera,
            window
        );


        // ====================================================
        // 4pz
        // ====================================================

        electronDensityRenderer.render(
            density4pz,
            camera,
            window
        );


        // ----------------------------------------------------
        // NO SE RENDERIZA DNA
        // ----------------------------------------------------


        performance.endRender();


        // ====================================================
        // PERFORMANCE OVERLAY
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
        // PERFORMANCE - END FRAME
        // ====================================================

        performance.endFrame();
    }


    // ========================================================
    // EXIT
    // ========================================================

    return 0;
}