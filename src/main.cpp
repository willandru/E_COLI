
#include "Window.h"
#include "Camera.h"
#include "InputMouse.h"
#include "InputKeyboard.h"
#include "Shader.h"
#include "Grid.h"
#include <iostream>

// ============================================================
// SHADERS
// ============================================================

const char* GRID_VERTEX_SHADER = R"(
#version 330 core

layout (location = 0) in vec3 aPosition;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position =
        projection *
        view *
        model *
        vec4(aPosition, 1.0);
}
)";


const char* GRID_FRAGMENT_SHADER = R"(
#version 330 core

out vec4 FragColor;

uniform vec3 color;

void main()
{
    FragColor =
        vec4(color, 1.0);
}
)";


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
        GRID_VERTEX_SHADER,
        GRID_FRAGMENT_SHADER
    );


    // ========================================================
    // 5. WORLD
    // ========================================================

    Grid grid(
        100.0f,
        100
    );


    // ========================================================
    // 6. TIME
    // ========================================================

    float lastFrame = 0.0f;


    // ========================================================
    // 7. MAIN LOOP
    // ========================================================

    while (!window.shouldClose())
    {
        // ====================================================
        // TIME
        // ====================================================

        float currentFrame =
            static_cast<float>(
                glfwGetTime()
            );


        float deltaTime =
            currentFrame -
            lastFrame;


        lastFrame =
            currentFrame;


        // ====================================================
        // INPUT
        // ====================================================

        mouse.update();

        keyboard.update();

        window.processEvents();


        // ====================================================
        // CAMERA MOVEMENT
        // ====================================================

        float speed =
            5.0f * deltaTime;


        if (keyboard.isPressed(GLFW_KEY_W))
        {
            camera.moveForward(speed);
        }


        if (keyboard.isPressed(GLFW_KEY_S))
        {
            camera.moveBackward(speed);
        }


        if (keyboard.isPressed(GLFW_KEY_A))
        {
            camera.moveLeft(speed);
        }


        if (keyboard.isPressed(GLFW_KEY_D))
        {
            camera.moveRight(speed);
        }


        if (keyboard.isPressed(GLFW_KEY_SPACE))
        {
            camera.moveUp(speed);
        }


        if (keyboard.isPressed(
            GLFW_KEY_LEFT_CONTROL))
        {
            camera.moveDown(speed);
        }


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
        // CAMERA ROTATION
        // ====================================================

        if (mouse.isRotating())
        {
            camera.rotate(
                mouse.getDeltaX(),
                mouse.getDeltaY()
            );
        }


        // ====================================================
        // CAMERA ZOOM
        // ====================================================

        if (mouse.getScrollDelta() != 0.0f)
        {
            camera.zoom(
                mouse.getScrollDelta() * 3.0f
            );
        }


        // ====================================================
        // RENDER
        // ====================================================

        window.clear();


        // ----------------------------------------------------
        // Camera matrices
        // ----------------------------------------------------

        int width =
            window.getWidth();

        int height =
            window.getHeight();


        float aspectRatio =
            static_cast<float>(width) /
            static_cast<float>(height);


        glm::mat4 model =
            glm::mat4(1.0f);


        glm::mat4 view =
            camera.getViewMatrix();


        glm::mat4 projection =
            camera.getProjectionMatrix(
                aspectRatio
            );


        // ----------------------------------------------------
        // Grid shader
        // ----------------------------------------------------

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


        // ----------------------------------------------------
        // Grid
        // ----------------------------------------------------

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