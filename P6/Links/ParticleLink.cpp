#include "ParticleLink.h"

using namespace P6;

float P6::ParticleLink::CurrentLength()
{
    MyVector ret = this->particles[0]->Position - this->particles[1]->Position;
    return ret.Magnitude();
}
