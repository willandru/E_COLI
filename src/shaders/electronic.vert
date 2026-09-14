#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in float aDensity;
layout (location = 2) in float aWaveFunction;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float pointSize;

out float density;
out float waveFunction;


void main()
{
    density = aDensity;
    waveFunction = aWaveFunction;

    gl_Position =
        projection *
        view *
        model *
        vec4(aPosition, 1.0);

    /*
     * The point size is kept constant for now.
     *
     * Later this can be made dependent on:
     * - camera distance
     * - density
     * - orbital scale
     * - level of detail
     */

    gl_PointSize = pointSize;
}