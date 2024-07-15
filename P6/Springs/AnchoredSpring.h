#pragma once

#include "../Vector.h"
#include "../ForceGenerator.h"

namespace P6 {
	class AnchoredSpring : public ForceGenerator
	{
	private:
		MyVector anchorPoint;
		float springConstant;
		float restLength;

	public:
		AnchoredSpring(MyVector pos, float _springConst, float _restLen) : 
			anchorPoint(pos), springConstant(_springConst), restLength(_restLen) {}
	
		void UpdateForce(P6Particle* p, float time) override;
	};
}
