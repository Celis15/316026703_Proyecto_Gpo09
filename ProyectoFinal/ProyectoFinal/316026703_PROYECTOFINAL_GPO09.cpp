
// Std. Includes
#include <string>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

#include "stb_image.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include "SOIL2/SOIL2.h"


// GL includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();
void animacion();
void animacion2();

// Properties
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;




// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 0.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;
// Light attributes
glm::vec3 PosIni(0, 0, 0);
bool active;



// Light attributes
glm::vec3 lightPos(7.0f, 0.5f, 1.5f);
float movelightPosx = 0.0f;
float movelightPosy = 0.0f;
float movelightPosz = 0.0f;
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
float rot = 0.0f;
bool activanim = false;

//Variables para la animacion de la puerta
float rotPuerta = 0.0f;
int rotacionPuerta = 0;
bool PuertaAbierta = false;
bool PuertaCerrada = false;

//Variables para la animacion del molino
float rotMolino = 0.0f;
int rotacionMolino = 0;
bool GiroMolino = false;

//Variables para la animacion de la mecedora
float rotMecedora = 0.0f;
int rotacionMecedora = 0;
bool Activo = false;
bool Movimiento = false;

//Animación de la nave
float movKitXNave = 0.0;
float movKitZNave = 0.0;
float rotKitNave = 90.0;

//variables para controlar los estados de animacion.
bool circuito = false;
bool circuito2 = false;
bool recorrido1 = true;
bool recorrido2 = false;
bool recorrido3 = false;
bool recorrido4 = false;
bool recorrido5 = false;

int main()
{
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Proyecto Final", nullptr, nullptr);

    if (nullptr == window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();

        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);

    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

    // Set the required callback functions
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MouseCallback);

    // GLFW Options
    glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    // Define the viewport dimensions
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // OpenGL options
    glEnable(GL_DEPTH_TEST);

    // Setup and compile our shaders
    Shader shader("Shaders/modelLoading.vs", "Shaders/modelLoading.frag");
    Shader lampshader("Shaders/lamp.vs", "Shaders/lamp.frag");
    Shader lightingShader("Shaders/lighting.vs", "Shaders/lighting.frag");



    // Load models

    Model armchair((char*)"Models/Armchair/armchair.obj");
    Model Lampara((char*)"Models/Lampara/Lampara.obj");
    Model SueloExt((char*)"Models/SueloExterior/SueloTierra.obj");
    Model TV((char*)"Models/TV/TV/TV.obj");
    Model Fachada((char*)"Models/Fachada/Fachada.obj");
    Model PuertaD((char*)"Models/Puerta/Puerta.obj");
    Model MarcoPuertaD((char*)"Models/Puerta/MarcoPuerta.obj");
    Model PuertaT((char*)"Models/Fachada/PuertaTrasera.obj");
    Model Tubo((char*)"Models/Fachada/TuboFachada.obj");
    Model Molino((char*)"Models/Molino/Molino.obj");
    Model MolinoAnim((char*)"Models/Molino/MolinoCirculo.obj");
    Model Escalera((char*)"Models/Escalera/Escalera.obj");
    Model Alfombra((char*)"Models/Alfombra/Alfombra.obj");
    Model Banco((char*)"Models/Banco/BancoMadera.obj");
    Model Mecedora((char*)"Models/Mecedora/Mecedora.obj");
    Model Reloj((char*)"Models/Reloj/Reloj.obj");
    Model Telefono((char*)"Models/Telefono/Telefono.obj");
    Model UFO((char*)"Models/UFO/UFO.obj");
    Model Ventanas((char*)"Models/Ventanas/Ventanas.obj");

    glm::mat4 projection = glm::perspective(camera.GetZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 400.0f);

    float vertices[] = {
      -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    // First, set the container's VAO (and VBO)
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Load textures

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    int textureWidth, textureHeight, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* image;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);

    image = stbi_load("images/goku.jpg", &textureWidth, &textureHeight, &nrChannels, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    if (image)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(image);


    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Set frame time
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Check and call events
        glfwPollEvents();
        DoMovement();
        animacion();
        animacion2();

        // Clear the colorbuffer
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        
        //Empezamos a usar el lightinShader
        lightingShader.Use();
        GLint lightPosLoc = glGetUniformLocation(lightingShader.Program, "light.position");
        GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
        glUniform3f(lightPosLoc, lightPos.x + movelightPosx, lightPos.y + movelightPosy, lightPos.z + movelightPosz);
        glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);


        // Set lights properties
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.ambient"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.diffuse"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.specular"), 1.0f, 1.0f, 1.0f);




        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        // Set material properties
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.ambient"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 5.0f);


        //glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

        view = camera.GetViewMatrix();
        // Get the uniform locations
        GLint modelLoc = glGetUniformLocation(lampshader.Program, "model");
        GLint viewLoc = glGetUniformLocation(lampshader.Program, "view");
        GLint projLoc = glGetUniformLocation(lampshader.Program, "projection");

        // Pass the matrices to the shader
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


        glm::mat4 model(1);

       
        //Dibujamos todos los elementos de la fachada
        view = camera.GetViewMatrix();
  
        //Empezamos a usar el lampshader para la carga de los objetos
        lampshader.Use();
        modelLoc = glGetUniformLocation(lampshader.Program, "model");
        viewLoc = glGetUniformLocation(lampshader.Program, "view");
        projLoc = glGetUniformLocation(lampshader.Program, "projection");
        glBindVertexArray(0);
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        
            //Para el Suelo 
        model = glm::mat4(1.0f);
        //model = glm::translate(model, glm::vec3(lightPos.x + movelightPosx, lightPos.y + movelightPosy, lightPos.z + movelightPosz));
        model = glm::scale(model, glm::vec3(20.0,0.0f,20.0f));
        glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        SueloExt.Draw(lampshader);
        glBindVertexArray(0);

            //Para la fachada
             model = glm::mat4(1.0f);
            //model = glm::translate(model, glm::vec3(4.0f,0.0f,0.0f));
            //model = glm::scale(model, glm::vec3(0.02f));
            glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
            glBindVertexArray(VAO);
            Fachada.Draw(lampshader);
            glBindVertexArray(0);

            //Para la puerta delantera 

            //Para el marco de la puerta 
                model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(-4.373f, 9.25f, 30.083));
                 //model = glm::scale(model, glm::vec3(0.02f));
                glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
                glBindVertexArray(VAO);
                MarcoPuertaD.Draw(lampshader);
                glBindVertexArray(0);

            //Para la animacion de la puerta9

            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(-4.32f,9.415f, 30.083));
            model = glm::rotate(model, glm::radians(rotPuerta), glm::vec3(0.0f, 1.0f, 0.0f));
            //model = glm::scale(model, glm::vec3(0.02f));
            glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
            glBindVertexArray(VAO);
            PuertaD.Draw(lampshader);
            glBindVertexArray(0);

            //Para la puerta trasera 
            model = glm::mat4(1.0f);
            //model = glm::translate(model, glm::vec3(4.0f, 0.0f, 0.0f));
            //model = glm::scale(model, glm::vec3(0.02f));
            glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
            glBindVertexArray(VAO);
            PuertaT.Draw(lampshader);
            glBindVertexArray(0);

            //Para el tubo de la fachada 
            
             model = glm::mat4(1.0f);
            //model = glm::translate(model, glm::vec3(4.0f, 0.0f, 0.0f));
            //model = glm::scale(model, glm::vec3(0.02f));
            glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
            glBindVertexArray(VAO);
            Tubo.Draw(lampshader);
            glBindVertexArray(0);

            //Para el molino 
                //Para la base del molino
                model = glm::mat4(1.0f);
                //model = glm::translate(model, glm::vec3(4.0f, 0.0f, 0.0f));
                //model = glm::scale(model, glm::vec3(0.02f));
                glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
                glBindVertexArray(VAO);
                Molino.Draw(lampshader);
                glBindVertexArray(0);

                //Para la animacion del molino
                model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(74.645f, 85.051f, -82.005f));
                model = glm::rotate(model, glm::radians(rotMolino), glm::vec3(0.0f, 0.0f, 1.0f));
                //model = glm::scale(model, glm::vec3(0.02f));
                glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
                glBindVertexArray(VAO);
                MolinoAnim.Draw(lampshader);
                glBindVertexArray(0);
        //Dibujamos los elementos de la habitacion 

            //Para el sillon
            model = glm::mat4(1.0f);
            //model = glm::translate(model, glm::vec3(4.0f, 0.0f, 0.0f));
            //model = glm::scale(model, glm::vec3(0.02f));
            glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
            glBindVertexArray(VAO);
            armchair.Draw(lampshader);
            glBindVertexArray(0);

            //Para la lampara 
            model = glm::mat4(1.0f);
            //model = glm::translate(model, glm::vec3(4.0f, 0.0f, 0.0f));
            //model = glm::scale(model, glm::vec3(0.02f));
            glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
            glBindVertexArray(VAO);
            Lampara.Draw(lampshader);
            glBindVertexArray(0);

            //Para la Television 

            model = glm::mat4(1.0f);
            //model = glm::translate(model, glm::vec3(4.0f, 0.0f, 0.0f));
            //model = glm::scale(model, glm::vec3(0.02f));
            glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
            glBindVertexArray(VAO);
            TV.Draw(lampshader);
            glBindVertexArray(0);

            //Para la escalera 
            model = glm::mat4(1.0f);
            //model = glm::translate(model, glm::vec3(4.0f, 0.0f, 0.0f));
            //model = glm::scale(model, glm::vec3(0.02f));
            glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
            glBindVertexArray(VAO);
            Escalera.Draw(lampshader);
            

            //Para la alfombra 

            model = glm::mat4(1.0f);
            //model = glm::translate(model, glm::vec3(4.0f, 0.0f, 0.0f));
            //model = glm::scale(model, glm::vec3(0.02f));
            glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
            glBindVertexArray(VAO);
            Alfombra.Draw(lampshader);
            

            //Para el banco 
            model = glm::mat4(1.0f);
            //model = glm::translate(model, glm::vec3(4.0f, 0.0f, 0.0f));
            //model = glm::scale(model, glm::vec3(0.02f));
            glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
            glBindVertexArray(VAO);
            Banco.Draw(lampshader);
            

            //Para la animacion de la mecedora 
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0.0f, 2.0f, 18.0f));
            model = glm::rotate(model, glm::radians(-5.0f), glm::vec3(0.0f, 0.0f, 1.0f));
            model = glm::rotate(model, glm::radians(rotMecedora), glm::vec3(0.0f, 0.0f, 1.0f));
            //model = glm::scale(model, glm::vec3(0.02f));
            glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
            glBindVertexArray(VAO);
            Mecedora.Draw(lampshader);
            

            //Para el reloj 
            model = glm::mat4(1.0f);
            //model = glm::translate(model, glm::vec3(4.0f, 0.0f, 0.0f));
            //model = glm::scale(model, glm::vec3(0.02f));
            glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
            glBindVertexArray(VAO);
            Reloj.Draw(lampshader);
           

            //Para la Nave 
            model = glm::mat4(1);
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            model = glm::translate(model, glm::vec3(movKitXNave, 0, movKitZNave));
            //model = glm::rotate(model, glm::radians(rotKitNave), glm::vec3(0.0f, 1.0f, 0.0));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            UFO.Draw(lampshader);


            //Para el telefono 

            model = glm::mat4(1);
            //model = glm::translate(model, glm::vec3(4.0f, 0.0f, 0.0f));
            //model = glm::scale(model, glm::vec3(0.02f));
            glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
            glBindVertexArray(VAO);
            Telefono.Draw(lampshader);
            glBindVertexArray(0);

            model = glm::mat4(1);
            //model = glm::translate(model, glm::vec3(4.0f, 0.0f, 0.0f));
            //model = glm::scale(model, glm::vec3(0.02f));
            glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
            glBindVertexArray(VAO);
            Ventanas.Draw(lampshader);
            glBindVertexArray(0);


           



        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}



// Moves/alters the camera positions based on user input
void DoMovement()
{
    // Camera controls
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
    {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }

    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
    {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }

    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
    {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }

    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
    {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }

    if (activanim)
    {
        if (rot > -90.0f)
            rot -= 0.1f;
    }

    if (PuertaAbierta)
    {
        if (rotPuerta <= 90.0f)
        {
            rotPuerta += 1.0f;
        }

        if (rotPuerta == 90.0f)
        {
            rotacionPuerta = 1;

        }
    }
    if (PuertaCerrada)
    {
        if (rotPuerta >= 0.0f)
        {
            rotPuerta -= 1.0f;
        }

        if (rotPuerta == 0)
        {
            rotacionPuerta = 0;
        }
    }

    if (GiroMolino)

    {

        if (rotMolino <= 360)
        {
            rotMolino += 1;
        }

        if (rotMolino == 360)
        {
            rotacionMolino = 1;
            rotMolino = 0;
        }

    }

    if (Movimiento) {
        rotMecedora += (Activo) ? -0.1f : 0.1f;
        if (rotMecedora > 21.0f) //Movimiento hacia atras
            Activo = true;
        if (rotMecedora < 0.0f) //Movimiento hacia adelante
            Activo = false;
    }

    if (keys[GLFW_KEY_B]) {
        Movimiento = true;
    }

    if (keys[GLFW_KEY_V]) {
        Movimiento = false;
    }

    if (keys[GLFW_KEY_I])
    {
        circuito = true;
    }

    if (keys[GLFW_KEY_O])
    {
        circuito = false;
        circuito2 = false;
    }
    if (keys[GLFW_KEY_X])
    {
        circuito2 = true;
    }
    

}

void animacion()
{
if (circuito)
{
    if (recorrido1)
    {
        movKitXNave += 1.0f;
        if (movKitXNave > 438)
        {
            recorrido1 = false;
            recorrido2 = true;
        }
    }
    if (recorrido2)
    {
        rotKitNave = -45;
        movKitZNave += 1.0f;
        movKitXNave -= 1.0f;
        if (movKitZNave > 90 && movKitXNave < 0)
        {
            recorrido2 = false;
            recorrido3 = true;

        }
    }


    if (recorrido3)
    {
        rotKitNave = -180;
        movKitZNave -= 1.0f;
        if (movKitZNave < 0)
        {
            recorrido3 = false;
            recorrido4 = true;
        }
    }
    if (recorrido4)
    {
        rotKitNave = 90;
        movKitXNave += 1.0f;
        if (movKitXNave > 0)
        {
            recorrido4 = false;
            recorrido1 = true;
        }
    }


}
}
void animacion2()
{

    //Movimiento del coche
    if (circuito2)
    {
        if (recorrido1)
        {
            movKitXNave += 1.0f;
            if (movKitXNave > 90)
            {
                recorrido1 = false;
                recorrido2 = true;
            }
        }
        if (recorrido2)
        {
            rotKitNave = 0;
            movKitZNave += 1.0f;
            if (movKitZNave > 90)
            {
                recorrido2 = false;
                recorrido3 = true;

            }
        }

        if (recorrido3)
        {
            rotKitNave = -90;
            movKitXNave -= 1.0f;
            if (movKitXNave < 0)
            {
                recorrido3 = false;
                recorrido4 = true;
            }
        }

        if (recorrido4)
        {
            rotKitNave = -180;
            movKitZNave -= 1.0f;
            if (movKitZNave < 0)
            {
                recorrido4 = false;
                recorrido5 = true;
            }
        }
        if (recorrido5)
        {
            rotKitNave = 90;
            movKitXNave += 1.0f;
            if (movKitXNave > 0)
            {
                recorrido5 = false;
                recorrido1 = true;
            }
        }


    }
}
// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
        {
            keys[key] = true;
        }
        else if (action == GLFW_RELEASE)
        {
            keys[key] = false;
        }

        if (keys[GLFW_KEY_P])
        {
            if (rotacionPuerta == 0)
            {
                PuertaAbierta = true;
                PuertaCerrada = false;


            }

            if (rotacionPuerta == 1)
            {
                PuertaAbierta = false;
                PuertaCerrada = true;
                


            }

        }

        if (keys[GLFW_KEY_M])
        {
            if (rotacionPuerta == 0)
            {
                GiroMolino = true;


            }

            if (rotacionPuerta == 1)
            {
                GiroMolino = true;



            }

        }
        
        if (keys[GLFW_KEY_L])
        {
            GiroMolino = false;
        }


        if (keys[GLFW_KEY_B])
        {

                Movimiento = true;
                //MecedoraAtras = true;
                //MecedoraAdelante = false;


         }

            if (keys[GLFW_KEY_V])
            {
                Movimiento = false;
                //MecedoraAtras = false;
                //MecedoraAdelante = true;
                



            }

        
    }



    if (keys[GLFW_KEY_U])
    {
        //activanim = true;
        movelightPosz += 0.1f;
    }

    if (keys[GLFW_KEY_J])
    {
        //activanim = true;
        movelightPosz -= 0.1f;
    }
    if (keys[GLFW_KEY_K])
    {
        //activanim = true;
        movelightPosx += 0.1f;
    }

    if (keys[GLFW_KEY_H])
    {
        //activanim = true;
        movelightPosx -= 0.1f;
    }
    if (keys[GLFW_KEY_M])
    {
        //activanim = true;
        movelightPosy += 0.1f;
    }

    if (keys[GLFW_KEY_N])
    {
        //activanim = true;
        movelightPosy -= 0.1f;
    }
    
    


}


void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    if (firstMouse)
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

    lastX = xPos;
    lastY = yPos;

    camera.ProcessMouseMovement(xOffset, yOffset);
}





