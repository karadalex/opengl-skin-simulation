#include "Particle.h"
#include "Skin.h"
#include <glm/gtc/matrix_transform.hpp>
#include <common/model.h>
#include <iostream>

Particle* vertexParticle;

using namespace glm;
using namespace std;


Skin::Skin(string path) {
    skin = new Drawable(path);

    #ifdef USE_QUATERNIONS
        q = quat(0, vec3(1, 0, 0));
    #else
        R = mat3(1, 0, 0, 0, 1, 0, 0, 0, 1);
    #endif

    numOfParticles = skin->indexedVertices.size();
	for each (auto vertex in skin->indexedVertices)
	{
        vertexParticle = new Particle(vertex, vec3(0), vec3(0), 1.0f);
        particles.push_back(vertexParticle);
	}
}

Skin::~Skin() {
    delete skin;
}

void Skin::draw(unsigned int drawable) {
    skin->bind();
    skin->draw();
}

void Skin::update(float t, float dt) {
    // numerical integration
    // advanceState(t, dt);

    // compute model matrix
    mat4 scale = glm::scale(mat4(), vec3(1, 1, 1));
    mat4 tranlation = translate(mat4(), vec3(0, 0, 0));
    #ifdef USE_QUATERNIONS
        mat4 rotation = mat4_cast(q);
    #else
        mat4 rotation = mat4(R);
    #endif
    modelMatrix = tranlation * rotation * scale;
}