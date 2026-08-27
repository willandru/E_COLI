#pragma once

#include <GLFW/glfw3.h>


class InputKeyboard
{
public:

    explicit InputKeyboard(
        GLFWwindow* window
    );


    void update();


    bool isPressed(
        int key
    ) const;


    bool shouldClose() const;


private:

    GLFWwindow* m_window;
};
