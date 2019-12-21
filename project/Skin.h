#ifndef SKIN_H
#define SKIN_H

#include <iostream>
#include <vector>
#include <functional>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

class Drawable;

#define USE_QUATERNIONS

class Skin {
public:
    Drawable* skin;
    glm::mat4 modelMatrix;

    #ifdef USE_QUATERNIONS
        glm::quat q;
    #else
        glm::mat3 R;
    #endif

    Skin(std::string path);
    ~Skin();

    void draw(unsigned int drawable = 0);
    void update(float t = 0, float dt = 0);
};

#endif