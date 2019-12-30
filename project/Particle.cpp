#include "Particle.h"
#include <glm/gtc/matrix_transform.hpp>
#include <common/model.h>
#include <iostream>

using namespace glm;
using namespace std;


Particle::Particle(vec3 pos, vec3 vel, vec3 _force, float mass) {
    x = pos;
    v = vel;
    m = mass;
    P = m * v;
	isInBoundary = true;
    cout << "new particle created at position: " << x.x << " " << x.y << " " << x.z << endl;
}


void Particle::addNeighbourD1(unsigned int neigbourIndex) {
	bool neigbourIndexAlreadyExists = false;
	for each (unsigned int otherNeighbour in neighboursD1) {
		if (otherNeighbour == neigbourIndex)
		{
			neigbourIndexAlreadyExists = true;
		}
	}

	if (!neigbourIndexAlreadyExists) {
		neighboursD1.push_back(neigbourIndex);
	}
    
    if (neighboursD1.size() > 5) {
        isInBoundary = false;
    }
}


void Particle::addNeighbourD2(unsigned int neigbourIndex) {
    neighboursD2.push_back(neigbourIndex);
}


void Particle::update(float t, float dt) {
    //integration
    advanceState(t, dt);
}


void Particle::draw() {
    // 
}


void Particle::applyForce(vec3 $force) {
    // 
}


Particle::~Particle() {
    // 
}