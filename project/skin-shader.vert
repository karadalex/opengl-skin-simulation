#version 330 core

// construct input layout for the corresponding attributes
// (vertexPosition_modelspace, vertexNormal_modelspace, vertexUV)
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexNormal_modelspace;
layout(location = 2) in vec2 vertexUV;

// Output variables (position_modelspace, normal_modelspace and UV coordinates), 
// that will be interpolated for each fragment
out vec3 vertex_position_modelspace;
out vec3 vertex_normal_modelspace;
out vec2 vertex_UV;

// uniforms (P, V, M)
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;
uniform mat4 OscillationMatrix;

void main()
{
	vec4 vertex = vec4(vertexPosition_modelspace, 1);

	if (vertex.x > 0) {
		vertex = OscillationMatrix * vertex;
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
