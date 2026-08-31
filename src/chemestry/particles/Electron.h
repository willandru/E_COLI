#pragma once

#include <glm/vec3.hpp>


namespace Chemistry
{

class Electron
{
public:

    Electron();

    explicit Electron(
        const glm::vec3& position
    );


    const glm::vec3& getPosition() const;

    float getMass() const;

    float getCharge() const;


private:

    glm::vec3 position;

    static constexpr float MASS =
        9.1093837139e-31f;

    static constexpr float CHARGE =
        -1.602176634e-19f;
};
}
