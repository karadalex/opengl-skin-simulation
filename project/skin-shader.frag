#version 330 core

// interpolated values from the vertex shaders (vertex_position_modelspace, 
// vertex_normal_modelspace and vertex_UV coordinates)
in vec3 vertex_position_modelspace;
in vec3 vertex_normal_modelspace;
in vec2 vertex_UV;

// uniform variables (lightPosition_worldspace, V, M)
uniform vec3 light_position_worldspace;
uniform mat4 V;
uniform mat4 M;
uniform int objectVAO;

// Task 5.3: define uniform variables for the texture coordinates
// (diffuseColorSampler, specularColorSampler)
uniform sampler2D diffuseColorSampler;
uniform sampler2D specularColorSampler;

// output data
out vec4 fragment_color;

void main()
{
    // default color
    fragment_color = vec4(1, 1, 1, 1);

    // model light; specular (Ls), diffuse (Ld) and ambient (La) color
	vec3 Ls = vec3(1, 1, 1);
	vec3 Ld = vec3(1, 1, 1);
	vec3 La = vec3(1, 1, 1);

    // model material properties; specular (Ks), diffuse (Kd)
    // assign material properties from the texture maps
	vec3 Ks = vec3(0.1, 0.1, 0.1);
	vec3 Kd = vec3(0.1, 0.1, 0.1);
	if (objectVAO == 1) {
		Kd = vec3(texture(diffuseColorSampler, vertex_UV).rgb);
	}
    vec3 Ka = vec3(0.1, 0.1, 0.1);
    float Ns = 10;

    // model ambient intensity (Ia)
	vec3 Ia = Ka * La;
	fragment_color = vec4(Ia, 1.0);

    // model diffuse intensity
    // transform light position, vertex position and vertex normal 
    // in camera space (N). Attention: vertex normal is a directional vector 
    // (Homogeneous Transformations).
    // calculate distance to light and normalized direction (L) in camera space 

	vec3 vertex_position_cameraspace = vec3(V * M * vec4(vertex_position_modelspace, 1.0));
	vec3 vertex_normal_cameraspace = vec3(V * M * vec4(vertex_normal_modelspace, 1.0));
	vec3 light_position_cameraspace = vec3(V * vec4(light_position_worldspace, 1.0));

	float dist = length(light_position_cameraspace - vertex_position_cameraspace);
	vec3 L = normalize(light_position_cameraspace - vertex_position_cameraspace);
	vec3 N = normalize(vertex_normal_cameraspace);
	float cos_theta = -dot(L, N);

	// vec3 Id = Kd * Ld * cos_theta;
	vec3 Id = Kd * Ld;
	fragment_color = vec4(Ia + Id, 1.0);

}