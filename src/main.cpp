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
#include "DNARenderer.h"

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
            8.0f
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
    // 4. TIME
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

    DNA dna(
        "../data/ncbi_dataset/ncbi_dataset/data/GCF_000005845.2/GCF_000005845.2_ASM584v2_genomic.fna"
    );


    if (!dna.isLoaded())
    {
        std::cerr
            << "Error: no se pudo cargar el ADN.\n";

        return -1;
    }


    // ========================================================
    // 7. RENDERERS
    // ========================================================

    GridRenderer gridRenderer;


    DNARenderer dnaRenderer(
        dna
    );


    // ========================================================
    // PERFORMANCE RENDERER
    // ========================================================

    PerformanceRenderer performanceRenderer(
        "../assets/Agdasima/Agdasima-Regular.ttf"
    );


    // ========================================================
    // 8. MAIN LOOP
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


        float deltaTime =
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
        // ESC - EXIT
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


        // ----------------------------------------------------
        // DNA
        // ----------------------------------------------------

        dnaRenderer.render(
            camera,
            window
        );


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