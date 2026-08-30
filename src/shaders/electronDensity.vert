#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in float aDensity;


uniform mat4 view;
uniform mat4 projection;

uniform float pointSize;


out float density;


void main()
{
    gl_Position =
        projection *
        view *
        vec4(
            aPosition,
            1.0
        );


    gl_PointSize =
        pointSize;


    density =
        aDensity;
}