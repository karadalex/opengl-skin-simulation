#include "Particle.h"
#include <glm/gtc/matrix_transform.hpp>
#include <common/model.h>
#include <iostream>

using namespace glm;
using namespace std;


Particle::Particle(vec3 pos, vec3 vel, vec3 _force, float m) {
    position = pos;
    velocity = vel;
    force = _force;
    mass = m;
    cout << "new particle created at position: " << position.x << endl;
}


void update(float dt) {
    // 
}


void draw() {
    // 
}


void applyForce(vec3 $force) {
    // 
}


Particle::~Particle() {
    // 
}