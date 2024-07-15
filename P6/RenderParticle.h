#pragma once

#include "Particle.h"
#include "../Renderer/Shader.h"
#include "../Renderer/Model3D.h"

namespace P6 {
	class RenderParticle {
	public:
		P6::P6Particle* PhysicsParticle;

		Model3D* RenderObject;

		P6::MyVector Color;

		Shader* shaderProg;

		GLuint* VAO;
		std::vector<GLfloat>* fullVertexData;

		RenderParticle(P6::P6Particle* p, Model3D* obj, Shader* ShaderProg, GLuint* VAO, std::vector<GLfloat>* fullVertexData)
			:PhysicsParticle(p), RenderObject(obj), shaderProg(ShaderProg) {
			this->VAO = VAO;
			this->fullVertexData = fullVertexData;
			Color = P6::MyVector(1, 0, 0);
		} // creates renderparticle with red color

		RenderParticle(P6::P6Particle* p, Model3D* obj, P6::MyVector c, Shader* ShaderProg, GLuint* VAO, std::vector<GLfloat>* fullVertexData)
			: PhysicsParticle(p), RenderObject(obj), Color(c), shaderProg(ShaderProg) {
			this->VAO = VAO;
			this->fullVertexData = fullVertexData;
		}; // creates renderparticle with SPECIFIED color

		void Draw();

	};
}