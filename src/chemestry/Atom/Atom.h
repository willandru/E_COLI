#pragma once

#include <glm/glm.hpp>

#include "AtomData.h"

namespace Chemistry
{

class Atom
{
public:

    Atom(
        const AtomData& data,
        const glm::vec3& position
    );


    // ========================================================
    // Element data
    // ========================================================

    const AtomData& getData() const;


    // ========================================================
    // Position
    // ========================================================

    const glm::vec3& getPosition() const;

    void setPosition(
        const glm::vec3& position
    );


private:

    const AtomData* data;

    glm::vec3 position;
};

}