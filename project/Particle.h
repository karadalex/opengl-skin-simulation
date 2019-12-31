#ifndef PARTICLE_H
#define PARTICLE_H

#include <iostream>
#include <vector>
#include <functional>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "RigidBody.h"

#define USE_QUATERNIONS

using namespace glm;
using namespace std;

class Particle : public RigidBody {
    public:
        float b, k, l0, l;

        glm::vec3 a;

        /**
         * list of indices of all adjacent neighbours
         * distance = 1 
         */
        vector<unsigned int> neighboursD1;

        /**
         * list of indices of all neighbours
         * with distance = 2 which are not adjacent
         */
        vector<unsigned int> neighboursD2;

        /**
         * Check if particle is in the boundary of mesh
         * A vertex is in the boundary if it has less than 6 neighbours.
         */
        float isInBoundary;

        Particle(vec3 pos, vec3 vel, float mass, float length, 
            vec3 anchor, float stiffness, float damping, float restLength);
        void addNeighbourD1(unsigned int neigbourIndex);
        void addNeighbourD2(unsigned int neigbourIndex);
        void update(float t, float dt);
        void draw();
        void applyForce(vec3 $force);
        ~Particle();
};

#endif
