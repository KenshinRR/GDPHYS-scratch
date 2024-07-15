#include "FireworkHandler.h"

using namespace Activities;

Activities::FireworkHandler::FireworkHandler()
{
    P6::MyVector color = P6::MyVector(1.0, 0.0, 0.0);

    std::cout << "How many particles? ";
    std::cin >> particleAmount;
}

Activities::FireworkHandler::FireworkHandler(float height, float width)
{
    this->windowWidth = width;
    this->windowWidth = height;

    P6::MyVector color = P6::MyVector(1.0, 0.0, 0.0);

    std::cout << "How many particles? ";
    std::cin >> particleAmount;
}

void Activities::FireworkHandler::Perform(std::list<P6::RenderParticle*>* renderPartices, P6::PhysicsWorld* pWorld)
{
    P6::MyVector color;

    if (this->currentParticleCount < this->particleAmount) {
        Model3D* newFWPM = new Model3D({ 0,0,0 }); //FWPM short for firework particle model
        P6::P6Particle* newFWP = new P6::P6Particle(
            1.0f,
            P6::MyVector(0, -this->windowHeight / 2.0f + 10.0f, 0),
            P6::MyVector(0, 0, 0),
            P6::MyVector(0.f, 0.f, 0.f)
        );

        //FORCE
        this->randForce.x = this->numberRand.GetRandomInt(0, coneRadius);
        if (this->numberRand.GetRandomInt(1, 2) <= 1) this->randForce.x *= -1;

        this->randForce.z = this->numberRand.GetRandomInt(0, coneRadius);
        if (this->numberRand.GetRandomInt(1, 2) <= 1) this->randForce.z *= -1;

        this->randForce.y = this->numberRand.GetRandomInt(1000, 6000);
        //if (this->numberRand.GetRandomInt(1, 2) <= 1) this->randForce.y *= -1;

        //std::cout << "Xforce: " << this->randForce.x << std::endl;
        //std::cout << "Yforce: " << this->randForce.y << std::endl << std::endl;

        newFWP->AddForce(P6::MyVector(this->randForce.x, this->randForce.y, this->randForce.z));

        //CHANGING COLOR
        color = P6::MyVector(
            this->numberRand.GetRandomFloat(0.0f, 1.0f), //r
            this->numberRand.GetRandomFloat(0.0f, 1.0f), //g
            this->numberRand.GetRandomFloat(0.0f, 1.0f) //b
        );

        //CHANGING SIZE
        this->randSize = this->numberRand.GetRandomFloat(2.0f, 10.0f);
        //std::cout << "Rand size: " << this->randSize << std::endl;
        newFWPM->setScale(this->randSize, this->randSize, this->randSize);

        //GIVING LIFE
        newFWP->lifeSpan = this->numberRand.GetRandomFloat(1.0f, 10.f);

        //ADDING IT TO THE LISTS
        pWorld->AddParticle(newFWP);
        P6::RenderParticle* newRP = new P6::RenderParticle(newFWP, newFWPM, color, this->sphereShader, this->VAO, this->fullVertexData);
        renderPartices->push_back(newRP);

        this->currentParticleCount++;
    }
}

void Activities::FireworkHandler::AssignShader(Shader* shader, unsigned int* VAO, std::vector<GLfloat>* fullVertexData)
{
    this->sphereShader = shader;
    this->VAO = VAO;
    this->fullVertexData = fullVertexData;
}
