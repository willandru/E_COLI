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

// ============================================================
// ATOMS
// ============================================================

#include "Atom.h"
#include "AtomData.h"
#include "AtomRenderer.h"

// ============================================================
// BONDS
// ============================================================

#include "Bond.h"
#include "BondRenderer.h"

// ============================================================
// CHEMISTRY
// ============================================================

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
            65.0f
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
        150.0f,
        150
    );


    GridRenderer gridRenderer;


    // ========================================================
    // 6. DNA
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
    // 7. ATOMS
    // ========================================================
    //
    // Atom:
    //
    //     Atom(
    //         const AtomData& data,
    //         const glm::vec3& position
    //     );
    //
    // Por lo tanto primero pasamos Carbon y luego posición.
    //
    // ========================================================

    Chemistry::Atom atomA(
        Chemistry::Carbon,
        glm::vec3(
            -5.0f,
            0.0f,
            0.0f
        )
    );


    Chemistry::Atom atomB(
        Chemistry::Carbon,
        glm::vec3(
            5.0f,
            0.0f,
            0.0f
        )
    );


    // ========================================================
    // 8. ATOM RENDERER
    // ========================================================
    //
    // AtomRenderer está en el namespace global.
    //
    // NO:
    //
    //     Chemistry::AtomRenderer
    //
    // ========================================================

    AtomRenderer atomRenderer;


    // ========================================================
    // 9. BOND
    // ========================================================
    //
    // Bond utiliza los objetos Atom.
    //
    // El BondRenderer posteriormente hace:
    //
    //     bond.getAtomA()
    //     bond.getAtomB()
    //
    // ========================================================

    Chemistry::Bond bond(
        atomA,
        atomB,
        Chemistry::BondType::Single
    );


    // ========================================================
    // 10. BOND RENDERER
    // ========================================================
    //
    // El constructor ya crea la geometría del cilindro.
    //
    // NO existe:
    //
    //     bondRenderer.build();
    //
    // ========================================================

    Chemistry::BondRenderer bondRenderer;


    // ========================================================
    // 11. CONFIGURACIÓN ESPACIAL
    // ========================================================

    const glm::vec3 densitySize(
        8.0f,
        8.0f,
        8.0f
    );


    const glm::ivec3 densityResolution(
        50,
        50,
        50
    );


    // ========================================================
    // 12. CENTROS DE LOS ORBITALES
    // ========================================================

    const glm::vec3 position1s(
        -45.0f,
        0.0f,
        0.0f
    );


    const glm::vec3 position2s(
        -25.0f,
        0.0f,
        0.0f
    );


    const glm::vec3 position2pCenter(
        0.0f,
        0.0f,
        0.0f
    );


    const glm::vec3 position3s(
        25.0f,
        0.0f,
        0.0f
    );


    const glm::vec3 position3pCenter(
        50.0f,
        0.0f,
        0.0f
    );


    // ========================================================
    // 13. SEPARACIÓN ORBITALES p
    // ========================================================

    const float pSpacing = 10.0f;


    // ========================================================
    // 2p
    // ========================================================

    const glm::vec3 position2px(
        position2pCenter.x - pSpacing,
        0.0f,
        0.0f
    );


    const glm::vec3 position2py(
        position2pCenter.x,
        0.0f,
        0.0f
    );


    const glm::vec3 position2pz(
        position2pCenter.x + pSpacing,
        0.0f,
        0.0f
    );


    // ========================================================
    // 3p
    // ========================================================

    const glm::vec3 position3px(
        position3pCenter.x - pSpacing,
        0.0f,
        0.0f
    );


    const glm::vec3 position3py(
        position3pCenter.x,
        0.0f,
        0.0f
    );


    const glm::vec3 position3pz(
        position3pCenter.x + pSpacing,
        0.0f,
        0.0f
    );


    // ========================================================
    // 14. ATOMIC ORBITALS
    // ========================================================

    Chemistry::AtomicOrbital orbital1s(
        1,
        Chemistry::OrbitalType::S,
        Chemistry::OrbitalOrientation::None,
        position1s
    );


    Chemistry::AtomicOrbital orbital2s(
        2,
        Chemistry::OrbitalType::S,
        Chemistry::OrbitalOrientation::None,
        position2s
    );


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


    Chemistry::AtomicOrbital orbital3s(
        3,
        Chemistry::OrbitalType::S,
        Chemistry::OrbitalOrientation::None,
        position3s
    );


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
    // 15. ELECTRON DENSITIES
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


    // ========================================================
    // 16. GENERAR DENSIDADES
    // ========================================================

    std::cout
        << "Generando densidades electronicas...\n";


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


    std::cout
        << "Densidades generadas.\n";


    // ========================================================
    // 17. RENDERERS DE DENSIDAD
    // ========================================================

    Chemistry::ElectronDensityRenderer renderer1s;

    Chemistry::ElectronDensityRenderer renderer2s;

    Chemistry::ElectronDensityRenderer renderer2px;

    Chemistry::ElectronDensityRenderer renderer2py;

    Chemistry::ElectronDensityRenderer renderer2pz;

    Chemistry::ElectronDensityRenderer renderer3s;

    Chemistry::ElectronDensityRenderer renderer3px;

    Chemistry::ElectronDensityRenderer renderer3py;

    Chemistry::ElectronDensityRenderer renderer3pz;


    // ========================================================
    // 18. BUILD GPU
    // ========================================================

    std::cout
        << "Subiendo orbitales a GPU...\n";


    renderer1s.build(
        density1s
    );


    renderer2s.build(
        density2s
    );


    renderer2px.build(
        density2px
    );


    renderer2py.build(
        density2py
    );


    renderer2pz.build(
        density2pz
    );


    renderer3s.build(
        density3s
    );


    renderer3px.build(
        density3px
    );


    renderer3py.build(
        density3py
    );


    renderer3pz.build(
        density3pz
    );


    std::cout
        << "Orbitales listos.\n";


    // ========================================================
    // 19. PERFORMANCE RENDERER
    // ========================================================

    PerformanceRenderer performanceRenderer(
        "../assets/Agdasima/Agdasima-Regular.ttf",
        dna
    );


    // ========================================================
    // 20. MAIN LOOP
    // ========================================================

    while (!window.shouldClose())
    {
        // ====================================================
        // BEGIN FRAME
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
        // BOND
        // ====================================================

        bondRenderer.render(
            bond,
            camera,
            window
        );


        // ====================================================
        // ATOM A
        // ====================================================

        atomRenderer.render(
            atomA,
            camera,
            window
        );


        // ====================================================
        // ATOM B
        // ====================================================

        atomRenderer.render(
            atomB,
            camera,
            window
        );


        // ====================================================
        // 1s
        // ====================================================

        renderer1s.render(
            density1s,
            camera,
            window
        );


        // ====================================================
        // 2s
        // ====================================================

        renderer2s.render(
            density2s,
            camera,
            window
        );


        // ====================================================
        // 2px
        // ====================================================

        renderer2px.render(
            density2px,
            camera,
            window
        );


        // ====================================================
        // 2py
        // ====================================================

        renderer2py.render(
            density2py,
            camera,
            window
        );


        // ====================================================
        // 2pz
        // ====================================================

        renderer2pz.render(
            density2pz,
            camera,
            window
        );


        // ====================================================
        // 3s
        // ====================================================

        renderer3s.render(
            density3s,
            camera,
            window
        );


        // ====================================================
        // 3px
        // ====================================================

        renderer3px.render(
            density3px,
            camera,
            window
        );


        // ====================================================
        // 3py
        // ====================================================

        renderer3py.render(
            density3py,
            camera,
            window
        );


        // ====================================================
        // 3pz
        // ====================================================

        renderer3pz.render(
            density3pz,
            camera,
            window
        );


        // ====================================================
        // DNA
        // ====================================================
        //
        // Todavía no se renderiza.
        //
        // ====================================================


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
        // END FRAME
        // ====================================================

        performance.endFrame();
    }


    // ========================================================
    // EXIT
    // ========================================================

    return 0;
}