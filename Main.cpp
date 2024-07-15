#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> 
#include <glm/gtc/quaternion.hpp> 
#include <glm/gtx/quaternion.hpp>



#include <iostream>
#include <string>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include "chrono"
using namespace std::chrono_literals;
constexpr std::chrono::nanoseconds timestep(30ms);

#include "Renderer/Shader.h"
#include "Renderer/Model3D.h"

#include "P6/Vector.h"
#include "P6/Particle.h"
#include "P6/PhysicsWorld.h"
#include "P6/RenderParticle.h"
#include "P6/ForceRegistry.h"
#include "P6/ForceGenerator.h"
#include "P6/DragForceGenerator.h"
#include "P6/ParticleContact.h"
#include "P6/ContactResolver.h"

#include "P6/Springs/AnchoredSpring.h"
#include "P6/Springs/ParticleSpring.h"

#include "P6/Links/ParticleLink.h"
#include "P6/Links/Rod.h"

//Camera
#include "Camera/Camera.h"
#include "Camera/OrthoCamera.h"
#include "Camera/PerspectiveCamera.h"

//activites
#include "Activities/FireworkHandler.h"

//helper functions / Utility functions
#include "Utility/NumberRandomizer.h"

float width = 800;
float height = 800;


//camera initialization
OrthoCamera orthoCam({ 0,0,-400 }, { 0,1,0 }, { 0.1,0.1,1 }, height, width, width, true);
PerspectiveCamera persCam({ 0,0,-400 }, { 0,1,0 }, { 0,0,1 }, 90.f, height, width, 800, false);//test view
glm::vec3 moveCam({ 0, 0, 0 });

float pitch = 0.0f;
float yaw = -90.0f;
int camState = 0;


//Fireworks
bool isPaused = false;

std::vector <P6::P6Particle*> res;

glm::vec3 camRotation(bool vertical, bool pos) {
    const float speed = 0.1f;

    //checks what button was pressed
    if (vertical) {
        if (pos) {
            pitch += speed;//y w was pressed
        }
        else {
            pitch -= speed;//y s was pressed
        }

    }
    else {
        if (pos) {
            yaw += speed; //x d was pressed
        }
        else {
            yaw -= speed; //x a was pressed
        }

    }



    //making sure that you can't 360 via neck breaking
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    //setting camera pos, the number multiplied is the radius

    glm::vec3 camPos;
    camPos.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch)) * 400;
    camPos.y = sin(glm::radians(pitch)) * 400;
    camPos.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch)) * 400;
    return  camPos;

}







void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);


    if (glfwGetKey(window, GLFW_KEY_1)) {
        if (camState != 0) {
            camState = 0;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_2)) {
        if (camState != 1) {
            camState = 1;
        }
    }


    if (glfwGetKey(window, GLFW_KEY_A)) {
        moveCam = camRotation(false, false);
    }
    if (glfwGetKey(window, GLFW_KEY_D)) {
        moveCam = camRotation(false, true);
    }
    if (glfwGetKey(window, GLFW_KEY_S)) {
        moveCam = camRotation(true, false);
    }
    if (glfwGetKey(window, GLFW_KEY_W)) {
        moveCam = camRotation(true, true);
    }


    //checks pausing
    if (glfwGetKey(window, GLFW_KEY_SPACE)) {
        if (isPaused) isPaused = false;
        else isPaused = true;
    }
}


int main(void)
{

    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;



    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, height, "JAROCO Engine", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }


    ;    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    gladLoadGL();


    Shader sphereShader("Shaders/vertShader.vert", "Shaders/fragShader.frag");




    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //=================models================//

    std::string path = "3D/sphere.obj";
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






    //Render Particles
    std::list<P6::RenderParticle*>* RenderParticles = new std::list<P6::RenderParticle*>;

    //P6 WORLD
    P6::PhysicsWorld* pWorld = new P6::PhysicsWorld();

    //PARTICLES
    //p1
    P6::P6Particle* particle1 = new P6::P6Particle(
        5.0f,
        P6::MyVector(0, 0, 0),
        P6::MyVector(0, 0, 0),
        P6::MyVector(0.f, 0.f, 0.f)
    );

    particle1->radius = 10.0f;
    float sc = particle1->radius;
    P6::MyVector particleScale = { sc,sc,sc };

    P6::MyVector color = P6::MyVector(0, 0, 1.f);
    Model3D* particleModel = new Model3D({ 0,0,0 });
    particleModel->setScale(particleScale.x, particleScale.y, particleScale.z);

    particle1->lifeSpan = 100.f;
    pWorld->AddParticle(particle1);
    P6::RenderParticle* newRP = new P6::RenderParticle(particle1, particleModel, color, &sphereShader, &VAO, &fullVertexData);
    RenderParticles->push_back(newRP);

    //p2
    color = P6::MyVector(1.f, 0, 0);
    P6::P6Particle* particle2 = new P6::P6Particle(
        5.0f,
        P6::MyVector(-40, 0, 0),
        P6::MyVector(0, 0, 0),
        P6::MyVector(0.f, 0.f, 0.f)
    );
    particle2->radius = 10.f;
    particle2->lifeSpan = 100.f;
    pWorld->AddParticle(particle2);
    P6::RenderParticle* newRP2 = new P6::RenderParticle(particle2, particleModel, color, &sphereShader, &VAO, &fullVertexData);
    RenderParticles->push_back(newRP2);

    //adding force
    particle1->AddForce({-1000.0f,0,0});

    //SPRING
    P6::AnchoredSpring aSpring = P6::AnchoredSpring(P6::MyVector(0, 0,0), 5, 0.5f);
    //pWorld->forceRegistry.Add(particle2, &aSpring);

    P6::ParticleSpring pSpring = P6::ParticleSpring(particle1, 5, 1);
    //pWorld->forceRegistry.Add(particle2, &pSpring);

    //ROD
    P6::Rod* r = new P6::Rod();
    r->particles[0] = particle1;
    r->particles[1] = particle2;
    r->length = 200;
    //pWorld->Links.push_back(r);

    ////FIREWORK PARTICLES
    //Activities::FireworkHandler fireworkHandler(height, width);
    //fireworkHandler.AssignShader(&sphereShader, &VAO, &fullVertexData);


    //drag
    /*
    P6::DragForceGenerator drag = P6::DragForceGenerator(0.14, 0.1);
    pWorld.forceRegistry.Add(&p1, &drag);*/



    //clock initialiaze
    using clock = std::chrono::high_resolution_clock;
    auto curr_time = clock::now();
    auto prev_time = curr_time;
    std::chrono::nanoseconds curr_ns(0);
    std::chrono::nanoseconds ns_tracker(0);
    int buffer;

    srand((unsigned)time(0));



    //1 m = 1 unit
    //1m = 1 px
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
            auto ms_tracker = std::chrono::duration_cast<std::chrono::milliseconds>(ns_tracker).count();

            curr_ns -= curr_ns;


            //fireworks


            //updates here
            if (!isPaused) {
                //fireworkHandler.Perform(RenderParticles, pWorld);
                pWorld->Update((float)ms.count() / 1000);
                //contact.Resolve((float)ms.count() / 1000);
            }
            else {
                std::cout << "Current particle count: " << RenderParticles->size() << std::endl;
            }



        }
        //std::cout << "normal upd" << std::endl;

        processInput(window);
        //glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        /* Render here */


        //cameras
        P6::MyVector converter{ 0,0,0 };

        switch (camState) {
        case 0:
            if (moveCam != glm::vec3{ 0, 0, 0 }) {//this is for on camCreation making sure it doesnt set it to 0,0,0 on start
                orthoCam.setCameraPos(moveCam);//set camera pos
            }


            converter = P6::MyVector{ orthoCam.getCameraPos().x,orthoCam.getCameraPos().y ,orthoCam.getCameraPos().z };//this makes a myVector for...
            orthoCam.setFront(glm::vec3{ -converter.Direction().x,-converter.Direction().y,-converter.Direction().z });
            //THIS cause im insane, takes the the position, turns that to NDC and THEN we point AWAY so that it points hopefully to center.
            orthoCam.setViewMatrix();

            orthoCam.perfromSpecifics(&sphereShader);//put the data into shader


            break;
        case 1:
            if (moveCam != glm::vec3{ 0, 0, 0 }) {//this is for on camCreation making sure it doesnt set it to 0,0,0 on start
                persCam.setCameraPos(moveCam);//set camera pos
            }

            converter = P6::MyVector{ persCam.getCameraPos().x,persCam.getCameraPos().y ,persCam.getCameraPos().z };//this makes a myVector for...
            persCam.setFront(glm::vec3{ -converter.Direction().x,-converter.Direction().y,-converter.Direction().z });
            //THIS cause im insane, takes the the position, turns that to NDC and THEN we point AWAY so that it points hopefully to center.
            persCam.setViewMatrix();

            persCam.perfromSpecifics(&sphereShader);//put the data into shader
            break;
        }

        for (std::list<P6::RenderParticle*>::iterator i = RenderParticles->begin(); i != RenderParticles->end(); i++) {
            (*i)->Draw();
        }




        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();

    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);








    glfwTerminate();
    return 0;
}