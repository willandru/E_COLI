#pragma once

#include <glm/vec3.hpp>


namespace Chemistry
{

class Proton
{
public:

    Proton();

    explicit Proton(
        const glm::vec3& position
    );


    const glm::vec3& getPosition() const;

    float getMass() const;

    float getCharge() const;


private:

    glm::vec3 position;

    static constexpr float MASS =
        1.67262192369e-27f;

    static constexpr float CHARGE =
        1.602176634e-19f;
};

}