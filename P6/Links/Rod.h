#pragma once

#include "ParticleLink.h"
#include "../ParticleContact.h"

namespace P6 {
	class Rod : public ParticleLink{
	public:
		//particles should always be at this length
		float length = 1;
		float restitution = 0;

		ParticleContact* GetContact() override;
	};
}