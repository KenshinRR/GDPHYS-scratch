#include "Rod.h"

using namespace P6;

ParticleContact* P6::Rod::GetContact()
{
    float currLen = CurrentLength();

    if (currLen == this->length)
    return nullptr;

    ParticleContact* ret = new ParticleContact();
    ret->particles[0] = this->particles[0];
    ret->particles[1] = this->particles[1];

    MyVector dir = this->particles[1]->Position - this->particles[0]->Position;
    dir = dir.Direction();

    if (currLen > this->length) {
        ret->contactNormal = dir;
        ret->depth = currLen - length;
    }
    else {
        ret->contactNormal = dir * -1;
        ret->depth = length - currLen;
    }

    ret->restitution = this->restitution;

    return ret;
}
