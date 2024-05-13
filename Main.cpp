#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Model3D.h"
#include "Shader.h"
#include "Light.h"
#include "ColorLight.hpp"
#include "Camera.h"
#include "OrthoCamera.h"


#include <iostream>
#include <string>
#include <cmath>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"


//general movement
float x_scale = 2.0f, y_scale = 2.0f, z_scale = 2.0f;

float thetaX = 1.f, thetaY = 1.f;

float x_axisX = 0.f, y_axisX = 1.f, z_axisX = 0.f;

float x_axisY = 1.f, y_axisY = 0.f, z_axisY = 0.f;

//for all time related additions
float deltaTime = 0.0f;	
float lastFrame = 0.0f; 
float coolDown = 0.0f;

//initialize camera vars
glm::vec3 cameraPos = glm::vec3(0, 0, 2.f);
glm::vec3 WorldUp = glm::vec3(0, 1.0f, 0);
glm::vec3 Front = glm::vec3(0, 0.0f, -1);
//initialize for mouse movement
bool firstMouse = true;
float pitch = 0.0f;
float yaw = -90.0f;

//for initial mouse movement
float lastX = 400, lastY = 400;

float height = 800.0f;
float width = 800.0f;

//Initializing the object classes to be rendered
Model3D light_ball({ 0,1,0 });
OrthoCamera orca({ 0,2,0 });


//Point light variables
float brightness = 3.0f;
float dl_brightness = 1.0f;

glm::vec3 sphere_color = { 0.f,0.f,1.f };

int main(void)
{
    GLFWwindow* window;


    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, height, "Reblando, Kenshin", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    gladLoadGL();


    /*      TEXTURE     */
    int img_width, img_height, colorChannels;

    stbi_set_flip_vertically_on_load(true);

    unsigned char* tex_bytes = stbi_load(
        "3D/ayaya.png",
        &img_width,
        &img_height,
        &colorChannels,
        0
    );

    GLuint texture;

    glGenTextures(1, &texture);

    glActiveTexture(GL_TEXTURE0);

    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D,
        0,
        GL_RGBA,
        img_width,
        img_height,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        tex_bytes);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(tex_bytes);

    //faces of the skybox
    //std::string facesSkybox[]{
    //    "Skybox/rainbow_rt.png", //RIGHT
    //    "Skybox/rainbow_lf.png", //left
    //    "Skybox/rainbow_up.png", //up
    //    "Skybox/rainbow_dn.png", //down
    //    "Skybox/rainbow_ft.png", //front
    //    "Skybox/rainbow_bk.png", //back
    //};

    //faces of the skybox
    std::string facesSkybox[]{
        "Skybox/image3x2.png", //RIGHT
        "Skybox/image1x2.png", //left
        "Skybox/image2x1.png", //up
        "Skybox/image2x3.png", //down
        "Skybox/image2x2.png", //front
        "Skybox/image4x2.png", //back
    };

    unsigned int skyboxTex;
    glGenTextures(1, &skyboxTex);

    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTex);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    for (unsigned int i = 0; i < 6; i++)
    {
        int w, h, skyCChannel;

        stbi_set_flip_vertically_on_load(false);

        unsigned char* data = stbi_load(facesSkybox[i].c_str(), &w, &h, &skyCChannel, 0);

        if (data) {
            glTexImage2D(
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0,
                GL_RGBA,
                w,
                h,
                0,
                GL_RGBA,
                GL_UNSIGNED_BYTE,
                data
            );

            stbi_image_free(data);
        }
    }

    stbi_set_flip_vertically_on_load(true);

    glEnable(GL_DEPTH_TEST);

    glViewport(0, 0, 800, 800);


    //      SHADERS
    Shader mainObjShader("Shaders/mainObj.vert", "Shaders/mainObj.frag");
    Shader sphereShader("Shaders/sphere.vert", "Shaders/sphere.frag");

    //compile shader vertex
    std::fstream vertSrc("Shaders/directionLight.vert");
    std::stringstream vertBuff;

    vertBuff << vertSrc.rdbuf();

    std::string vertS = vertBuff.str();
    const char* v = vertS.c_str();

    //compile shader fragment
    std::fstream fragSrc("Shaders/directionLight.frag");
    std::stringstream fragBuff;

    fragBuff << fragSrc.rdbuf();

    std::string fragS = fragBuff.str();
    const char* f = fragS.c_str();

    //SPHERE OBJECT SHADER ///////////////////////////////////////////////////////////////////////////
    //compile shader vertex
    std::fstream vertSrcSphere("Shaders/pointLight.vert");
    std::stringstream vertBuffSphere;

    vertBuffSphere << vertSrcSphere.rdbuf();

    std::string vertSSphere = vertBuffSphere.str();
    const char* vSphere = vertSSphere.c_str();

    //compile shader fragment
    std::fstream fragSrcSphere("Shaders/pointLight.frag");
    std::stringstream fragBuffSphere;

    fragBuffSphere << fragSrcSphere.rdbuf();

    std::string fragSSphere = fragBuffSphere.str();
    const char* fSphere = fragSSphere.c_str();

    //SKYBOX ////////////////////////////////////////////////////////////////////////////
    //compile skybox vertex
    std::fstream vertSkyboxSrc("Shaders/skybox.vert");
    std::stringstream vertSkyboxBuff;

    vertSkyboxBuff << vertSkyboxSrc.rdbuf();

    std::string vertSB = vertSkyboxBuff.str();
    const char* vsb = vertSB.c_str();

    //compile skybox fragment
    std::fstream fragSkyboxSrc("Shaders/skybox.frag");
    std::stringstream fragSkyboxBuff;

    fragSkyboxBuff << fragSkyboxSrc.rdbuf();

    std::string fragSB = fragSkyboxBuff.str();
    const char* fsb = fragSB.c_str();

    //MAIN OBJECT //////////////////////////////////////////////////////////////////////
    //create vertex shader(used for movements)
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertexShader, 1, &v, NULL);

    glCompileShader(vertexShader);

    //create frag shader (our objects are turned into pixels/fragments which we will use to color in an object)
    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(fragShader, 1, &f, NULL);

    glCompileShader(fragShader);

    //create shader program that'll just run both frag and vert together as one.
    GLuint shaderProg = glCreateProgram();
    glAttachShader(shaderProg, vertexShader);
    glAttachShader(shaderProg, fragShader);

    glLinkProgram(shaderProg);//compile to make sure computer remembers

    //SPHERE OBJECT //////////////////////////////////////////////////////////////////////
    //create vertex shader(used for movements)
    GLuint vertexShaderSphere = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertexShaderSphere, 1, &vSphere, NULL);

    glCompileShader(vertexShaderSphere);

    //create frag shader (our objects are turned into pixels/fragments which we will use to color in an object)
    GLuint fragShaderSphere = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(fragShaderSphere, 1, &fSphere, NULL);

    glCompileShader(fragShaderSphere);

    //create shader program that'll just run both frag and vert together as one.
    GLuint shaderProgSphere = glCreateProgram();
    glAttachShader(shaderProgSphere, vertexShaderSphere);
    glAttachShader(shaderProgSphere, fragShaderSphere);

    glLinkProgram(shaderProgSphere);//compile to make sure computer remembers

    //SKYBOX //////////////////////////////////////////////////////////////////////////
    //create vertex shader
    GLuint vertexSkyboxShader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertexSkyboxShader, 1, &vsb, NULL);

    glCompileShader(vertexSkyboxShader);

    //create frag shader (our objects are turned into pixels/fragments which we will use to color in an object)
    GLuint fragSkyboxShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(fragSkyboxShader, 1, &fsb, NULL);

    glCompileShader(fragSkyboxShader);

    //create shader program that'll just run both frag and vert together as one.
    GLuint skyboxShaderProg = glCreateProgram();
    glAttachShader(skyboxShaderProg, vertexSkyboxShader);
    glAttachShader(skyboxShaderProg, fragSkyboxShader);

    glLinkProgram(skyboxShaderProg);//compile to make sure computer remembers

    /////////////////////////////////////////////////////////////////////////////////////

    //credits to original artist Kenshin Reblando for art.
    std::string path = "3D/Cross.obj";
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

    GLfloat UV[]{
        0.f, 1.f,
        0.f, 0.f,
        1.f, 1.f,
        1.f, 0.f,
        1.f, 1.f,
        1.f, 0.f,
        0.f, 1.f,
        0.f, 0.f
    };
   
    //array of Mesh for the main object
    std::vector<GLfloat> fullVertexData;
    for (int i = 0; i < shapes[0].mesh.indices.size(); i++) {
        tinyobj::index_t vData = shapes[0].mesh.indices[i];

        //vertex
        fullVertexData.push_back(
            attributes.vertices[(vData.vertex_index * 3)]
        );

        fullVertexData.push_back(
            attributes.vertices[(vData.vertex_index * 3) + 1]
        );

        fullVertexData.push_back(
            attributes.vertices[(vData.vertex_index * 3) + 2]
        );

        //normal
        fullVertexData.push_back(
            attributes.normals[(vData.normal_index * 3)]
        );

        fullVertexData.push_back(
            attributes.normals[(vData.normal_index * 3) + 1]
        );

        fullVertexData.push_back(
            attributes.normals[(vData.normal_index * 3) + 2]
        );

        //texcoord
        fullVertexData.push_back(
            attributes.texcoords[(vData.texcoord_index * 2)]
        );

        fullVertexData.push_back(
            attributes.texcoords[(vData.texcoord_index * 2) + 1]
        );

    }

    //credits to original artist Kenshin Reblando for art.
    std::string sphere_path = "3D/sphere.obj";
    std::vector<tinyobj::shape_t> sphere_shapes;
    std::vector<tinyobj::material_t> sphere_material;

    tinyobj::attrib_t sphere_attributes;

    bool sphere_success = tinyobj::LoadObj(
        &sphere_attributes,
        &sphere_shapes,
        &sphere_material,
        &warning,
        &error,
        sphere_path.c_str()
    );

    //array of mesh for the light ball
    std::vector<GLfloat> ball_fullVertexData;
    for (int i = 0; i < sphere_shapes[0].mesh.indices.size(); i++) {
        tinyobj::index_t vData = sphere_shapes[0].mesh.indices[i];

        //vertex
        ball_fullVertexData.push_back(
            sphere_attributes.vertices[(vData.vertex_index * 3)]
        );

        ball_fullVertexData.push_back(
            sphere_attributes.vertices[(vData.vertex_index * 3) + 1]
        );

        ball_fullVertexData.push_back(
            sphere_attributes.vertices[(vData.vertex_index * 3) + 2]
        );

        //normal
        ball_fullVertexData.push_back(
            sphere_attributes.normals[(vData.normal_index * 3)]
        );

        ball_fullVertexData.push_back(
            sphere_attributes.normals[(vData.normal_index * 3) + 1]
        );

        ball_fullVertexData.push_back(
            sphere_attributes.normals[(vData.normal_index * 3) + 2]
        );

        //texcoord
        ball_fullVertexData.push_back(
            sphere_attributes.texcoords[(vData.texcoord_index * 2)]
        );

        ball_fullVertexData.push_back(
            sphere_attributes.texcoords[(vData.texcoord_index * 2) + 1]
        );

    }
    
    //SKYBOX
    //Vertices for the cube
    float skyboxVertices[]{
        -1.f, -1.f, 1.f, //0
        1.f, -1.f, 1.f,  //1
        1.f, -1.f, -1.f, //2
        -1.f, -1.f, -1.f,//3
        -1.f, 1.f, 1.f,  //4
        1.f, 1.f, 1.f,   //5
        1.f, 1.f, -1.f,  //6
        -1.f, 1.f, -1.f  //7
    };

    //Skybox Indices
    unsigned int skyboxIndices[]{
        1,2,6,
        6,5,1,

        0,4,7,
        7,3,0,

        4,5,6,
        6,7,4,

        0,3,2,
        2,1,0,

        0,1,5,
        5,4,0,

        3,7,6,
        6,2,3
    };

    //main object
    GLuint VAO, VBO;// EBO;// VBO_UV;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(GLfloat) * fullVertexData.size(),
        fullVertexData.data(),
        GL_DYNAMIC_DRAW
    );

    
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,

        8 * sizeof(float),
        (void*)0

    );

    glEnableVertexAttribArray(0);

    GLintptr litPtr = 3 * sizeof(float);
    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,

        8 * sizeof(float),
        (void*)litPtr

    );

    glEnableVertexAttribArray(1);

    GLintptr uvPtr = 6 * sizeof(float);
    glVertexAttribPointer(
        2,
        2,
        GL_FLOAT,
        GL_FALSE,

        8 * sizeof(float),
        (void*)uvPtr

    );

    glEnableVertexAttribArray(2);

    //sphere object
    GLuint sphere_VAO, sphere_VBO;

    glGenVertexArrays(1, &sphere_VAO);
    glGenBuffers(1, &sphere_VBO);

    glBindVertexArray(sphere_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, sphere_VBO);

    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(GLfloat)* ball_fullVertexData.size(),
        ball_fullVertexData.data(),
        GL_DYNAMIC_DRAW
    );


    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,

        8 * sizeof(float),
        (void*)0

    );

    glEnableVertexAttribArray(0);

    GLintptr sphere_litPtr = 3 * sizeof(float);
    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,

        8 * sizeof(float),
        (void*)sphere_litPtr

    );

    glEnableVertexAttribArray(1);

    GLintptr sphere_uvPtr = 6 * sizeof(float);
    glVertexAttribPointer(
        2,
        2,
        GL_FLOAT,
        GL_FALSE,

        8 * sizeof(float),
        (void*)sphere_uvPtr

    );

    glEnableVertexAttribArray(2);

    //skybox
    unsigned int skyboxVAO, skyboxVBO, skyboxEBO;

    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glGenBuffers(1, &skyboxEBO);

    glBindVertexArray(skyboxVAO);

    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxEBO);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GL_INT) * 36, &skyboxIndices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);

    //SET BINDINGS TO NULL
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glm::mat4 identity_matrix = glm::mat4(1.0f);

    /*glm::mat4 projection = glm::ortho(
        -2.f, //left
        2.f, //right
        - 2.f, //bot
        2.f, //top
        -1.f, //z near
        1.f);  //z far*/

    /*      CAMERA VARIABLES*/
    glm::mat4 projection = glm::perspective(
        glm::radians(60.f),//FOV
        height / width, //aspect ratio
        0.1f, //znear > 0
        1000.f //zfar
    );

    /*      LIGHT VARIABLES     */
    glm::vec3 lightPos = glm::vec3(-10, 10, 0);

    glm::vec3 lightColor = glm::vec3(1,1,1);

    float ambientStr = 0.2f;

    glm::vec3 ambientColor = lightColor;

    float specStr = 0.5f;

    float specPhong = 8;

    /*      Light Classes       */
    glm::vec3 lightDirection = { 4,-5,0 };
    ColorLight colorLight;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {

        //set time so that movement is uniform for all devices if needed
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        coolDown -= 1 * deltaTime;
        

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glm::mat4 viewMatrix;
        viewMatrix = glm::lookAt(orca.getCameraPos(), orca.getCameraPos() + orca.getFront(), orca.getWorldUp());

        //skybox
        glDepthMask(GL_FALSE);
        glDepthFunc(GL_LEQUAL);
        glUseProgram(skyboxShaderProg);

        glm::mat4 sky_view = glm::mat4(1.f);
        sky_view = glm::mat4(
            //cast the same view matrix of the camera turn it into a mat3 to remove translations
            glm::mat3(viewMatrix)
            //then reconvert it to a mat4
        );

        unsigned int skyboxViewLoc = glGetUniformLocation(skyboxShaderProg, "view");
        glUniformMatrix4fv(skyboxViewLoc, 1, GL_FALSE, glm::value_ptr(sky_view));

        unsigned int skyboxProjLoc = glGetUniformLocation(skyboxShaderProg, "projection");
        glUniformMatrix4fv(skyboxProjLoc,
            1,
            GL_FALSE,
            glm::value_ptr(projection));

        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTex);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);
        
        /*      SPHERE OBJECT       */
        
        sphereShader.use();

        //
        float ball_scale = 0.5f;
        light_ball.setScale({ ball_scale ,ball_scale ,ball_scale });

        //set the camera to ortho
        light_ball.setCamera(orca.getProjection(), orca.getCameraPos(), orca.getFront(), orca.getViewMat());
        
        colorLight.setColor(sphere_color); //change the color
        colorLight.perform(&sphereShader); //attaches the values of light into the shader program of sphere
        light_ball.sphereDraw(&sphereShader, &sphere_VAO, &ball_fullVertexData);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
        

    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
   /* glDeleteBuffers(1, &EBO);*/
    glfwTerminate();
    return 0;
}