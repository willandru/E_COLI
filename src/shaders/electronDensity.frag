#version 330 core

in float density;


out vec4 FragColor;


void main()
{
    // ========================================================
    // Punto circular
    // ========================================================

    vec2 coordinate =
        gl_PointCoord -
        vec2(0.5);


    float distanceFromCenter =
        length(coordinate);


    if (distanceFromCenter > 0.5)
    {
        discard;
    }


    // ========================================================
    // Densidad -> transparencia
    // ========================================================

    float alpha =
        density;


    // ========================================================
    // Color
    // ========================================================

    FragColor =
        vec4(
            0.15,
            0.45,
            1.0,
            alpha
        );
}