#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> 
#include <glm/gtc/quaternion.hpp> 
#include <glm/gtx/quaternion.hpp>



#include <iostream>
#include <string>
#include <algorithm>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include "chrono"
using namespace std::chrono_literals;
constexpr std::chrono::nanoseconds timestep(30ms);

#include "P6/Vector.h"
#include "P6/Particle.h"
#include "P6/PhysicsWorld.h"

#include "Renderer/Shader.h"

/*========GLOBAL VARIABLES=========*/
//window dimensions
float width = 700;
float height = 700;

float xRot = 0.0f, yRot = 0.0f, zRot = 0.0f;

//structs
struct Racer{
    std::string name;
    int position;
    P6::MyVector finalVelocity;
    P6::MyVector averVelocity;
    float finishedTime;
}racer_red, racer_green, racer_blue, racer_yellow;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    //yrot
    if (glfwGetKey(window, GLFW_KEY_A)) {
        yRot -= 0.05;
    }
    if (glfwGetKey(window, GLFW_KEY_D)) {
        yRot += 0.05;
    }

    //xRot
    if (glfwGetKey(window, GLFW_KEY_S)) {
        xRot -= 0.05;
    }
    if (glfwGetKey(window, GLFW_KEY_W)) {
        xRot += 0.05;
    }

    //zRot
    if (glfwGetKey(window, GLFW_KEY_Q)) {
        zRot -= 0.05;
    }
    if (glfwGetKey(window, GLFW_KEY_E)) {
        zRot += 0.05;
    }

        
    
    
        
}


std::vector<GLfloat> getFullVertexData(std::string pathName) {

    std::string path = pathName;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> material;
    std::string warning, error;

    tinyobj::attrib_t attributes;

    bool success = tinyobj::LoadObj(
        &attributes,
        &shapes,
        &material,
        &warning,
        &error,
        path.c_str()
    );

    std::vector<GLfloat> fullVertexData;
    for (int i = 0; i < shapes[0].mesh.indices.size(); i++) {
        tinyobj::index_t vData = shapes[0].mesh.indices[i];


        fullVertexData.push_back(
            attributes.vertices[(vData.vertex_index * 3)]
        );

        fullVertexData.push_back(
            attributes.vertices[(vData.vertex_index * 3) + 1]
        );

        fullVertexData.push_back(
            attributes.vertices[(vData.vertex_index * 3) + 2]
        );

        fullVertexData.push_back(
            attributes.normals[(vData.normal_index * 3)]
        );

        fullVertexData.push_back(
            attributes.normals[(vData.normal_index * 3) + 1]
        );

        fullVertexData.push_back(
            attributes.normals[(vData.normal_index * 3) + 2]
        );

        fullVertexData.push_back(
            attributes.texcoords[(vData.texcoord_index * 2)]
        );

        fullVertexData.push_back(
            attributes.texcoords[(vData.texcoord_index * 2) + 1]
        );

    }

    return fullVertexData;
}

glm::mat4 getTransMat(glm::vec3 position) {

    glm::mat4 iden_Mat = glm::mat4(1.0f);

    glm::mat4 trans_Mat = glm::translate(iden_Mat, position);

    //X-rotation
    trans_Mat = glm::rotate(
        trans_Mat,
        glm::radians(xRot),
        glm::normalize(glm::vec3(1.0f, 0.0f, 0.0f))

    );//give quat

    //Y-rotation
    trans_Mat = glm::rotate(
        trans_Mat,
        glm::radians(yRot),
        glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f))
    );

    //Z-rotation
    trans_Mat = glm::rotate(
        trans_Mat,
        glm::radians(zRot),
        glm::normalize(glm::vec3(0.0f, 0.0f, 1.0f))
    );

    return trans_Mat;
}

float roundOff(float var)
{
    float value = (int)(var * 100 + .5);
    return (float)value / 100;
}

void printRaceResults(std::vector<Racer> racers) {
    for (int i = 0; i < (int)racers.size(); i++)
    {
        racers[i].position = i + 1;
        std::cout << "Ranking: " << racers[i].position << std::endl;
        std::cout << "Name: " << racers[i].name << std::endl;
        std::cout << "Mag. Velocity: " << roundOff(racers[i].finalVelocity.Magnitude()) << " m/s" << std::endl;

        std::cout << "Average Velocity: (" << roundOff(racers[i].averVelocity.x) << ", ";
        std::cout << roundOff(racers[i].averVelocity.y) << ", " << roundOff(racers[i].averVelocity.z);
        std::cout << ")m/s" << std::endl;

        std::cout << roundOff(racers[i].finishedTime / 1000) << " seconds" << std::endl << std::endl;
    }
}

bool compareByFloat(const Racer& a, const Racer& b) {
    return a.finishedTime < b.finishedTime; // Sort in descending order
}

std::vector<Racer> getRacerRankings(const std::vector<Racer> racers) {
    std::vector<Racer> sortedVector = racers;
    std::sort(sortedVector.begin(), sortedVector.end(), compareByFloat);
    return sortedVector;
}



int main(void)
{
    
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, height , "PC01 Reblando", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

;    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    gladLoadGL();

    

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    
    //=================models================//

    std::vector<GLfloat> fullVertexData = getFullVertexData("3D/sphere.obj");

    //=================VBO/VAO==========//

    unsigned int VBO, VAO;
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
   

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * fullVertexData.size(), fullVertexData.data(), GL_DYNAMIC_DRAW);

   

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    GLintptr normalptr = (3 * sizeof(float));

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)normalptr);
    glEnableVertexAttribArray(1);

    GLintptr UVptr = 6 * sizeof(float);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)UVptr);
    glEnableVertexAttribArray(2);


    
    //Camera inits
    //float screen_scale = 40.0f;
    //glm::mat4 projection = glm::ortho(-screen_scale, screen_scale, -screen_scale, screen_scale, -screen_scale, screen_scale);
    glm::mat4 projection = glm::ortho(-width/2, width/2, -height/2, height/2, -350.f, 350.f);
    

    //Initialize particles

    std::vector<P6::P6Particle*> particles;
    for (int i = 0; i < 4; i++)
    {
        particles.push_back(
            new P6::P6Particle);
    }

    //Red particle
    particles[0]->Position = {width/-2, height/2, 201.0f};
    P6::MyVector direction = P6::MyVector(particles[0]->Position.Direction());
    particles[0]->Velocity = P6::MyVector(direction.ScalarMult(-80.0f));
    P6::MyVector redInitVel = particles[0]->Velocity;
    particles[0]->Acceleration = P6::MyVector(direction.ScalarMult(-14.5f));

    //Green particle
    particles[1]->Position = { width / 2, height / 2, 173.0f };
    direction = P6::MyVector(particles[1]->Position.Direction());
    particles[1]->Velocity = P6::MyVector(direction.ScalarMult(-90.0f));
    P6::MyVector greenInitVel = particles[1]->Velocity;
    particles[1]->Acceleration = P6::MyVector(direction.ScalarMult(-8.0f));

    //blue particle
    particles[2]->Position = { width / 2, height / -2, -300.0f };
    direction = P6::MyVector(particles[2]->Position.Direction());
    particles[2]->Velocity = P6::MyVector(direction.ScalarMult(-130.0f));
    P6::MyVector blueInitVel = particles[2]->Velocity;
    particles[2]->Acceleration = P6::MyVector(direction.ScalarMult(-1.0f));

    //yellow particle
    particles[3]->Position = { width / -2, height / -2, -150.0f };
    direction = P6::MyVector(particles[3]->Position.Direction());
    particles[3]->Velocity = P6::MyVector(direction.ScalarMult(-110.0f));
    P6::MyVector yellowInitVel = particles[3]->Velocity;
    particles[3]->Acceleration = P6::MyVector(direction.ScalarMult(-3.0f));

    std::vector<glm::vec3> rgb_values;
    rgb_values.push_back(glm::vec3(1.0f, 0.0f, 0.0f)); //red
    rgb_values.push_back(glm::vec3(0.0f, 1.0f, 0.0f)); //green
    rgb_values.push_back(glm::vec3(0.0f, 0.0f, 1.0f)); //blue
    rgb_values.push_back(glm::vec3(1.0f, 1.0f, 0.0f)); //yellow
    
    //physics world
    P6::PhysicsWorld physicsWorld;
    for (P6::P6Particle* particle : particles)
    {
        physicsWorld.AddParticle(particle);
    }

    //Getting Shader
    std::vector<Renderer::Shader> particleShaders;
    for (int i = 0; i < particles.size(); i++)
    {
        Renderer::Shader particleShader("Shaders/vertShader.vert", "Shaders/fragShader.frag");
        particleShaders.push_back(particleShader);
    }

    //clock initialiaze
    using clock = std::chrono::high_resolution_clock;
    auto curr_time = clock::now();
    auto prev_time = curr_time;
    std::chrono::nanoseconds curr_ns(0);
    std::chrono::nanoseconds ns_tracker(0);
    int buffer;
   
    /*==============RACING VALUES=============*/
    bool isRedFinished = false;
    bool isBlueFinished = false;
    bool isGreenFinished = false;
    bool isYellowFinished = false;

    std::vector<Racer> vecRacers;
    vecRacers.push_back(racer_red);
    vecRacers.push_back(racer_green);
    vecRacers.push_back(racer_blue);
    vecRacers.push_back(racer_yellow);

    vecRacers[0].name = "Red";
    vecRacers[1].name = "Green";
    vecRacers[2].name = "Blue";
    vecRacers[3].name = "Yellow";

    //adding force
    particles[0]->AddForce(P6::MyVector(1000.0f, -1000.0f, 0.0f));

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        curr_time = clock::now();

        auto dur = std::chrono::duration_cast<std::chrono::nanoseconds> (curr_time - prev_time);

        prev_time = curr_time;

        curr_ns += dur;
        ns_tracker += dur;
        if (curr_ns >= timestep) {
            auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(curr_ns);
            auto ms_tracker = std::chrono::duration_cast<std::chrono::milliseconds>(ns_tracker);
            curr_ns -= curr_ns;

            //updates here
            /*for (P6::P6Particle* par : particles)
            {
                par->Update((float)ms.count() / 1000);
            }*/
            physicsWorld.Update((float)ms.count() / 1000);

            //check if red finished
            if (particles[0]->Position.x >= 0 && particles[0]->Position.y <= 0
                && !isRedFinished) {
                //putting the final values
                vecRacers[0].finishedTime = std::chrono::duration_cast<std::chrono::milliseconds>(ns_tracker).count();
                vecRacers[0].finalVelocity = particles[1]->Velocity;
                vecRacers[0].averVelocity = (redInitVel + vecRacers[0].finalVelocity) / 2;
                //Stopping the particle from moving
                particles[0]->Velocity = { 0,0,0 };
                particles[0]->Acceleration = { 0,0,0 };
                isRedFinished = true;
            }

            if (particles[1]->Position.x <= 0 && particles[1]->Position.y <= 0 
                && !isGreenFinished) {
                //putting the final values
                vecRacers[1].finishedTime = std::chrono::duration_cast<std::chrono::milliseconds>(ns_tracker).count();
                vecRacers[1].finalVelocity = particles[1]->Velocity;
                vecRacers[1].averVelocity = (greenInitVel + vecRacers[1].finalVelocity) / 2;
                //Stopping the particle from moving
                particles[1]->Velocity = { 0,0,0 };
                particles[1]->Acceleration = { 0,0,0 };
                
                isGreenFinished = true;
            }
            if (particles[2]->Position.x <= 0 && particles[2]->Position.y >= 0
                && !isBlueFinished) {
                //putting the final values
                vecRacers[2].finishedTime = std::chrono::duration_cast<std::chrono::milliseconds>(ns_tracker).count();
                vecRacers[2].finalVelocity = particles[1]->Velocity;
                vecRacers[2].averVelocity = (blueInitVel + vecRacers[2].finalVelocity) / 2;
                //Stopping the particle from moving
                particles[2]->Velocity = { 0,0,0 };
                particles[2]->Acceleration = { 0,0,0 };
                isBlueFinished = true;
            }
            if (particles[3]->Position.x >= 0 && particles[3]->Position.y >= 0
                && !isYellowFinished) {
                //putting the final values
                vecRacers[3].finishedTime = std::chrono::duration_cast<std::chrono::milliseconds>(ns_tracker).count();
                vecRacers[3].finalVelocity = particles[1]->Velocity;
                vecRacers[3].averVelocity = (yellowInitVel + vecRacers[3].finalVelocity) / 2;
                //Stopping the particle from moving
                particles[3]->Velocity = { 0,0,0 };
                particles[3]->Acceleration = { 0,0,0 };
                isYellowFinished = true;
            }

            if (isRedFinished && isYellowFinished &&
                isBlueFinished && isGreenFinished) {

                printRaceResults(getRacerRankings(vecRacers));


                //buffer so it won't close immeditietly
                std::cout << "Input any letter/number to close..." << std::endl;
                std::cin >> buffer;
                glfwSetWindowShouldClose(window, 1);
            }
        }
        //std::cout << "normal upd" << std::endl;

        processInput(window);
        //glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        /* Render here */

        for (int i = 0; i < particles.size(); i++)
        {
            glm::mat4 trans_mat = getTransMat((glm::vec3)particles[i]->Position);
            particleShaders[i].use();
            particleShaders[i].setInt("ourTexture", 0);
            particleShaders[i].setInt("ourText", 1);
            particleShaders[i].setMat4("transform", trans_mat);
            particleShaders[i].setMat4("projection", projection);
            particleShaders[i].setFloat("red", rgb_values[i].x);
            particleShaders[i].setFloat("green", rgb_values[i].y);
            particleShaders[i].setFloat("blue", rgb_values[i].z);

            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, i, fullVertexData.size() / 8);
        }

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    for (auto particleShader : particleShaders) {
        glDeleteProgram(particleShader.getID());
    }
    //glDeleteProgram(shaderProg);
    glfwTerminate();
    return 0;
}