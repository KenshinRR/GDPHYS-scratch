#pragma once

#include "vector"
#include "algorithm"
#include "iostream"
#include "ParticleContact.h"

namespace P6 {
	class ContactResolver
	{
	public:
		unsigned max_iterations;

		ContactResolver(unsigned _maxIterations) : max_iterations(_maxIterations) {}

		void ResolveContacts(std::vector<ParticleContact*> contacts, float time);

	protected:
		unsigned current_iteration = 0;
	};
}

