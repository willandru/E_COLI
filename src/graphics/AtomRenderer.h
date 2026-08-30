#pragma once

#include <glm/glm.hpp>

#include "Atom.h"
#include "AtomData.h"
#include "Shader.h"
#include "Camera.h"
#include "Window.h"

class AtomRenderer
{
public:

// ========================================================
// Constructor
// ========================================================

AtomRenderer();


// ========================================================
// Destructor
// ========================================================

~AtomRenderer();


// ========================================================
// Render
// ========================================================

void render(
    const Chemistry::Atom& atom,
    const Camera& camera,
    const Window& window
);


private:


// ========================================================
// Sphere
// ========================================================

void createSphere();

void destroySphere();


// ========================================================
// Color
// ========================================================

glm::vec3 getAtomColor(
    const Chemistry::AtomData& data
) const;

private:


Shader m_shader;

unsigned int m_VAO;
unsigned int m_VBO;
unsigned int m_EBO;

int m_indexCount;


};
