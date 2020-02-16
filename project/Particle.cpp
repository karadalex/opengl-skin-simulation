#include "Particle.h"
#include <glm/gtc/matrix_transform.hpp>
#include <common/model.h>
#include <iostream>

using namespace glm;
using namespace std;


Particle::Particle(vec3 pos, vec3 vel, float mass, float length, 
    vec3 anchor, float stiffness, float damping, float restLength) {
    
    x = pos;
    v = vel;
    m = mass;
    l = length;
    a = anchor;
    k = stiffness;
    b = damping;
    l0 = restLength;
    
    P = m * v;

    mat3 I = mat3(
        1.0f / 6 * mass*l*l, 0, 0,
        0, 1.0f / 6 * mass*l*l, 0,
        0, 0, 1.0f / 6 * mass*l*l);
    I_inv = inverse(I);

	isInBoundary = true;
    // cout << "new particle created at position: " << x.x << " " << x.y << " " << x.z << endl;

	for (int i = 0; i < forces.size(); i++)
	{
		forces[i] = 0;
	}
}


void Particle::addNeighbourD1(Particle* neighbour) {
	bool neigbourAlreadyExists = false;
	for each (Particle* otherNeighbour in neighboursD1) {
		if (otherNeighbour == neighbour)
		{
			neigbourAlreadyExists = true;
		}
	}

	if (!neigbourAlreadyExists) {
		neighboursD1.push_back(neighbour);
        float restDistance = distance(x, neighbour->x);
        neighboursD1RestDistances.push_back(restDistance);
	}
    
    if (neighboursD1.size() > 5) {
        isInBoundary = false;
    }
}


void Particle::addNeighbourD2(Particle* neigbour) {
    neighboursD2.push_back(neigbour);
}


void Particle::update(float t, float dt) {
    // Calculate new distances with neighbours after applying the forces
    // and calculate resulting forces exerted to neighbours
    // for (int i = 0; i < neighboursD1.size(); i++) {
    //     Particle* neighbour = neighboursD1[i];
    //     float newDistance = distance(x, neighbour->x);
    //     vec3 unitVector = (x - neighbour->x)/(newDistance);
    //     vec3 newNeighbourForce = k * unitVector * (neighboursD1RestDistances[i] - newDistance) - b*v;
    //     vector<float> f(6, 0.0f);
    //     f[0] = -newNeighbourForce.x;
    //     f[1] = -newNeighbourForce.y;
    //     f[2] = -newNeighbourForce.z;
    //     neighbour->addForce(f);
    // }
    
    // apply forces, numerical integration
    advanceState(t, dt);
}


void Particle::draw() {
    // 
}


void Particle::addForce(vector<float> forceVector) {
    forces = forceVector;
	for (int i = 0; i < forces.size(); i++) {
		forces[i] += forceVector[i];
	}    
}

Particle::~Particle() {
    // 
}