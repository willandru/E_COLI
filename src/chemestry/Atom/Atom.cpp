#include "Atom.h"

namespace Chemistry
{

Atom::Atom(
    const AtomData& data,
    const glm::vec3& position
)
    : data(&data),
      position(position)
{
}


// ============================================================
// Element data
// ============================================================

const AtomData& Atom::getData() const
{
    return *data;
}


// ============================================================
// Position
// ============================================================

const glm::vec3& Atom::getPosition() const
{
    return position;
}


void Atom::setPosition(
    const glm::vec3& newPosition
)
{
    position = newPosition;
}

}