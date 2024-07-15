#pragma once

#include "iostream"
#include "list"
#include "vector"

#include "../P6/Vector.h"
#include "../P6/PhysicsWorld.h"
#include "../P6/RenderParticle.h"

#include "../Utility/NumberRandomizer.h"

namespace Activities{
	class FireworkHandler
	{
	private:
		int particleAmount = 0;
		int currentParticleCount = 1;
		P6::MyVector randForce = P6::MyVector(0,0,0);
		float randSize = 0;
		int coneRadius = 2500;

		float windowHeight = 800;
		float windowWidth = 800;

		Utility::NumberRandomizer numberRand;

		//firework renderer
		Shader* sphereShader;
		unsigned int* VAO;
		std::vector<GLfloat>* fullVertexData;

	public:
		FireworkHandler();
		FireworkHandler(float height, float width);

	public:
		void AssignShader(Shader* shader, unsigned int* VAO, std::vector<GLfloat>* fullVertexData);
		void Perform(std::list<P6::RenderParticle*>* renderPartices, P6::PhysicsWorld* pWorld);

	};
}

