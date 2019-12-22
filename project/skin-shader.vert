#version 330 core

// construct input layout for the corresponding attributes
// (vertexPosition_modelspace, vertexNormal_modelspace, vertexUV)
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexNormal_modelspace;
layout(location = 2) in vec2 vertexUV;
in int gl_VertexID;

// Output variables (position_modelspace, normal_modelspace and UV coordinates), 
// that will be interpolated for each fragment
out vec3 vertex_position_modelspace;
out vec3 vertex_normal_modelspace;
out vec2 vertex_UV;

// uniforms (P, V, M)
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;
uniform float time;
uniform int objectVAO;
uniform mat4 skinTransformations[250];

mat4 translate(float x, float y, float z);
mat4 scale(float x, float y, float z);


void main()
{
	vec4 vertex = vec4(vertexPosition_modelspace, 1);

	if (objectVAO == 1) {
		vertex = skinTransformations[gl_VertexID] * vertex;
	} if (objectVAO == 2) {
        vertex = translate(0.0, 10-time, 0.0) * scale(0.5, 0.5, 0.5) * vertex;
    }

    // Output position of the vertex, in clip space : MVP * position
    gl_Position =  P * V * M * vertex;

    // propagate the position of the vertex to fragment shader
    vertex_position_modelspace = vertexPosition_modelspace;

    // propagate the normal of the vertex to fragment shader
    vertex_normal_modelspace = vertexNormal_modelspace; 
    
    // propagate the UV coordinates   
    vertex_UV = vertexUV;
}

mat4 scale(float x, float y, float z){
    return mat4(
        vec4(x,   0.0, 0.0, 0.0),
        vec4(0.0, y,   0.0, 0.0),
        vec4(0.0, 0.0, z,   0.0),
        vec4(0.0, 0.0, 0.0, 1.0)
    );
}

mat4 translate(float x, float y, float z){
    return mat4(
        vec4(1.0, 0.0, 0.0, 0.0),
        vec4(0.0, 1.0, 0.0, 0.0),
        vec4(0.0, 0.0, 1.0, 0.0),
        vec4(x,   y,   z,   1.0)
    );
}