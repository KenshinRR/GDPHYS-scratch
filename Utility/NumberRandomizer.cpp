#include "NumberRandomizer.h"

using namespace Utility;

int Utility::NumberRandomizer::GetRandomInt(int start, int end)
{
    return (rand() % end) + start;
}

int Utility::NumberRandomizer::GetRandomFloat(float start, float end)
{
    return start + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (end - start)));
}
