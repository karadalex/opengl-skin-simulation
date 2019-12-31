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

    // Build particles
    numOfParticles = skin->indexedVertices.size();
	for each (auto vertex in skin->indexedVertices)
	{
        vertexParticle = new Particle(
            vertex, vec3(0), 1.0f,
            1.0f, vertex, 50, 1.5, 2
        );
        particles.push_back(vertexParticle);
	}

    // Get neighbors of each vertex/particle
	// loop over triangle indices, get 3 vertices each time
	for (int i = 0; i < skin->indices.size(); i+=3)
	{
		unsigned int indexOfVertexA = skin->indices.at(i);
		unsigned int indexOfVertexB = skin->indices.at(i+1);
		unsigned int indexOfVertexC = skin->indices.at(i+2);

        Particle* vertexA = particles.at(indexOfVertexA);
        Particle* vertexB = particles.at(indexOfVertexB);
        Particle* vertexC = particles.at(indexOfVertexC);

        vertexA->addNeighbourD1(indexOfVertexB);
        vertexA->addNeighbourD1(indexOfVertexC);

        vertexB->addNeighbourD1(indexOfVertexA);
        vertexB->addNeighbourD1(indexOfVertexC);

        vertexC->addNeighbourD1(indexOfVertexA);
        vertexC->addNeighbourD1(indexOfVertexB);
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
	for each (auto skinParticle in particles)
	{
		skinParticle->update(t, dt);
	}
}