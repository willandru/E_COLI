#pragma once

#include <glm/vec3.hpp>


namespace Chemistry
{

class Neutron
{
public:

    Neutron();

    explicit Neutron(
        const glm::vec3& position
    );


    const glm::vec3& getPosition() const;

    float getMass() const;

    float getCharge() const;


private:

    glm::vec3 position;

    static constexpr float MASS =
        1.67492749804e-27f;

    static constexpr float CHARGE =
        0.0f;
};

}