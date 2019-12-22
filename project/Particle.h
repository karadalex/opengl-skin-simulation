#ifndef PARTICLE_H
#define PARTICLE_H

#include <iostream>
#include <vector>
#include <functional>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#define USE_QUATERNIONS

using namespace glm;

class Particle {
    public:
        float mass;
        vec3 position;
        vec3 velocity;
        vec3 force;

        Particle(vec3 pos, vec3 vel, vec3 force, float m);
        void update(float dt);
        void draw();
        void applyForce(vec3 $force);
        ~Particle();
};

#endif
