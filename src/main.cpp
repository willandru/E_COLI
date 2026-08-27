#include "Window.h"
#include "Camera.h"
#include "InputMouse.h"
#include "InputKeyboard.h"
#include "Shader.h"
#include "Grid.h"
#include "Timer1.h"

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
    // 4. SHADER
    // ========================================================

    Shader gridShader(
        "shaders/grid.vert",
        "shaders/grid.frag"
    );


    // ========================================================
    // 5. WORLD
    // ========================================================

    Grid grid(
        100.0f,
        100
    );


    // ========================================================
    // 6. TIMER
    // ========================================================

    Timer1 timer;


    // ========================================================
    // 7. MAIN LOOP
    // ========================================================

    while (!window.shouldClose())
    {
        // ====================================================
        // TIME
        // ====================================================

        float deltaTime =
            timer.getDeltaTime();


        // ====================================================
        // INPUT
        // ====================================================

        mouse.update();

        keyboard.update();

        window.processEvents();


        // ====================================================
        // CAMERA
        // ====================================================

        camera.update(
            keyboard,
            mouse,
            deltaTime
        );


        // ====================================================
        // ESC
        // ====================================================

        if (keyboard.isPressed(GLFW_KEY_ESCAPE))
        {
            glfwSetWindowShouldClose(
                nativeWindow,
                true
            );
        }


        // ====================================================
        // RENDER
        // ====================================================

        window.clear();


        float aspectRatio =
            window.getAspectRatio();


        glm::mat4 model =
            glm::mat4(1.0f);


        glm::mat4 view =
            camera.getViewMatrix();


        glm::mat4 projection =
            camera.getProjectionMatrix(
                aspectRatio
            );


        // ====================================================
        // GRID
        // ====================================================

        gridShader.bind();


        gridShader.setMat4(
            "model",
            model
        );


        gridShader.setMat4(
            "view",
            view
        );


        gridShader.setMat4(
            "projection",
            projection
        );


        gridShader.setVec3(
            "color",
            glm::vec3(
                0.25f,
                0.30f,
                0.35f
            )
        );


        grid.render(
            gridShader
        );


        // ====================================================
        // PRESENT
        // ====================================================

        window.present();
    }


    return 0;
}