#pragma once

#include "iostream";

namespace Utility {
	class NumberRandomizer
	{
	public:
		int GetRandomInt(int start, int end);
		int GetRandomFloat(float start, float end);
	};
}

