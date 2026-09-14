#version 330 core

in float density;
in float waveFunction;

uniform vec3 positiveColor;
uniform vec3 negativeColor;

out vec4 FragColor;


void main()
{
    /*
     * Convert the point into a circular particle.
     *
     * gl_PointCoord goes from (0,0) to (1,1)
     * across the point sprite.
     */

    vec2 centered =
        gl_PointCoord - vec2(0.5);

    float distanceFromCenter =
        length(centered);


    /*
     * Discard the corners of the square point.
     * This makes every point appear circular.
     */

    if (distanceFromCenter > 0.5)
    {
        discard;
    }


    /*
     * Density represents |psi|^2.
     *
     * It controls the opacity of the orbital cloud.
     */

    float alpha =
        smoothstep(
            0.0,
            0.35,
            density
        );


    /*
     * The wavefunction preserves its sign.
     *
     * psi > 0  -> positive phase
     * psi < 0  -> negative phase
     *
     * Therefore the two phases of the orbital
     * can now be displayed with different colors.
     */

    vec3 color;

    if (waveFunction >= 0.0)
    {
        color = positiveColor;
    }
    else
    {
        color = negativeColor;
    }


    /*
     * Soft circular falloff.
     */

    float edge =
        1.0 -
        smoothstep(
            0.35,
            0.5,
            distanceFromCenter
        );


    /*
     * Final color and opacity.
     */

    FragColor =
        vec4(
            color,
            alpha * edge
        );
}