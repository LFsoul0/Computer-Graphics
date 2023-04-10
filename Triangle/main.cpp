#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "Shader.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

void calc_color_gradient(float* red, float* green, float* blue);
void calc_transform(glm::mat4* trans);

// settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 800;
const unsigned int RENDER_SIZE = 800;
const unsigned int MSAA_SAMPLES = 9;

const float TRI_SCALE = 0.6;
const float TRI_INNER_R_SCALE = 0.7;
const float CIRCLE_SCALE = 0.12;
const int CIRCLE_PRECISION = 120;
const double COLOR_GRADIENT_PERIOD = 3; // actually 1/6 period
const float COLOR_UPPER_BOUND = 1.0f;
const float COLOR_LOWER_BOUND = 0.8f;
const double ROTATION_PERIOD = 18;
const unsigned int BLUR_TIMES = 10;
const float BLUR_ALPHA = 0.6f;

// unsigned int CURR_SCR_WIDTH = SCR_WIDTH;
// unsigned int CURR_SCR_HEIGHT = SCR_HEIGHT;
const float COLOR_DIFF = COLOR_UPPER_BOUND - COLOR_LOWER_BOUND;


int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Triangle", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // depth & stencil test
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_ALWAYS, 0, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    // blend for transparent object
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Shader triangleShader("triangle.vs", "triangle.fs");
    Shader blurShader("gauss_blur.vs", "gauss_blur.fs");
    Shader blendShader("blend.vs", "blend.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    // 1. equilateral triangle
    float vertices[] = {
        // Position         
        -glm::root_three<float>() / 2 * TRI_SCALE, -0.5 * TRI_SCALE, 0.0f, // left  
         glm::root_three<float>() / 2 * TRI_SCALE, -0.5 * TRI_SCALE, 0.0f, // right 
         0.0f,  TRI_SCALE, 0.0f  // top   
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    // 2. circle
    float* cVertices = new float[3 * (CIRCLE_PRECISION + 2)]{ 0.0f, 0.0f, 0.0f };
    for (int i = 0; i < CIRCLE_PRECISION + 1; ++i) {
        cVertices[3 * (i + 1)] = cos(glm::two_pi<float>() * i / CIRCLE_PRECISION) * CIRCLE_SCALE;
        cVertices[3 * (i + 1) + 1] = sin(glm::two_pi<float>() * i / CIRCLE_PRECISION) * CIRCLE_SCALE;
        cVertices[3 * (i + 1) + 2] = 0.0f;
    }

    unsigned int cVBO, cVAO;
    glGenVertexArrays(1, &cVAO);
    glGenBuffers(1, &cVBO);

    glBindVertexArray(cVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cVBO);
    glBufferData(GL_ARRAY_BUFFER, 3 * (CIRCLE_PRECISION + 2) * sizeof(float), cVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    // 3. screen quad
    float quadVertices[] = {
        // positions        // texture Coords
        -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
         1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
         1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    };

    unsigned int quadVAO, quadVBO;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);

    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);

    // configure (floating point) framebuffers
    // ---------------------------------------
    // 1. multisample FBO for MSAA
    unsigned int msFBO;
    glGenFramebuffers(1, &msFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, msFBO);
    
    unsigned int msColorBuffer;
    glGenTextures(1, &msColorBuffer);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, msColorBuffer);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, MSAA_SAMPLES, GL_RGBA16F, RENDER_SIZE, RENDER_SIZE, GL_TRUE);
    glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
    glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // attach texture to framebuffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, msColorBuffer, 0);

    unsigned int msRBO;
    glGenRenderbuffers(1, &msRBO);
    glBindRenderbuffer(GL_RENDERBUFFER, msRBO);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, MSAA_SAMPLES, GL_DEPTH24_STENCIL8, RENDER_SIZE, RENDER_SIZE);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, msRBO);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // 2. buffer before create bloom effect
    unsigned int FBO;
    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    // create 2 floating point color buffers (1 for normal rendering, other for brightness threshold values)
    unsigned int colorBuffer;
    glGenTextures(1, &colorBuffer);
    glBindTexture(GL_TEXTURE_2D, colorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, RENDER_SIZE, RENDER_SIZE, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  // we clamp to the edge as the blur filter would otherwise sample repeated texture values!
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // attach texture to framebuffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBuffer, 0);

    // create and attach depth buffer (renderbuffer)
    unsigned int RBO;
    glGenRenderbuffers(1, &RBO);
    glBindRenderbuffer(GL_RENDERBUFFER, RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, RENDER_SIZE, RENDER_SIZE);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

    // finally check if framebuffer is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // 3. ping-pong-framebuffer for blurring
    unsigned int pingpongFBO[2];
    unsigned int pingpongColorbuffers[2];
    glGenFramebuffers(2, pingpongFBO);
    glGenTextures(2, pingpongColorbuffers);
    for (unsigned int i = 0; i < 2; i++)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
        glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, RENDER_SIZE, RENDER_SIZE, 0, GL_RGBA, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // we clamp to the edge as the blur filter would otherwise sample repeated texture values!
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongColorbuffers[i], 0);

        // also check if framebuffers are complete (no need for depth buffer)
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "Framebuffer not complete!" << std::endl;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    // shader texture slots
    blurShader.use();
    blurShader.setInt("image", 0);
    blendShader.use();
    blendShader.setInt("scene", 0);
    blendShader.setInt("bloomBlur", 1);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // color gradient
        float redVal = 1.0f, greenVal = 1.0f, blueVal = 1.0f;
        calc_color_gradient(&redVal, &greenVal, &blueVal);

        // transform
        glm::mat4 trans = glm::mat4(1.0f);
        calc_transform(&trans);

        // render
        // ------
        // 1. bind to framebuffer and draw scene as we normally would to color texture 
        glBindFramebuffer(GL_FRAMEBUFFER, msFBO);
        glClearColor((1 - redVal) / 2, (1 - greenVal) / 2, (1 - blueVal) / 2, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        glViewport(0, 0, RENDER_SIZE, RENDER_SIZE);

        triangleShader.use();

        // 1) draw circle
        glStencilMask(0x00);
        triangleShader.setMat4("transform", trans);
        triangleShader.setVec4("uColor", redVal, greenVal, blueVal, 1.0f);
        glBindVertexArray(cVAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0, CIRCLE_PRECISION + 2);
        glBindVertexArray(0);

        // 2) block inner triangle
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glStencilMask(0xFF);
        triangleShader.setMat4("transform", glm::scale(trans, glm::vec3(TRI_INNER_R_SCALE, TRI_INNER_R_SCALE, 1.0f)));
        triangleShader.setVec4("uColor", 0.0f, 0.0f, 0.0f, 0.0f);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        // 3) draw triangle with stencil mask
        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        glStencilMask(0x00);
        glDisable(GL_DEPTH_TEST);
        triangleShader.setMat4("transform", trans);
        triangleShader.setVec4("uColor", redVal, greenVal, blueVal, 1.0f);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        glStencilMask(0xFF);
        glStencilFunc(GL_ALWAYS, 0, 0xFF);
        glEnable(GL_DEPTH_TEST);

        // 2. buffer blit
        glBindFramebuffer(GL_READ_FRAMEBUFFER, msFBO);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO);
        glBlitFramebuffer(0, 0, RENDER_SIZE, RENDER_SIZE, 0, 0, RENDER_SIZE, RENDER_SIZE, GL_COLOR_BUFFER_BIT, GL_NEAREST);

        // 3. blur bright fragments with two-pass Gaussian Blur 
        bool horizontal = true, first_iteration = true;
        unsigned int amount = BLUR_TIMES;
        blurShader.use();
        glDisable(GL_DEPTH_TEST); 
        for (unsigned int i = 0; i < amount; i++)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[horizontal]);
            // glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            blurShader.setInt("horizontal", horizontal);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, first_iteration ? colorBuffer : pingpongColorbuffers[!horizontal]);  // bind texture of other framebuffer (or scene if first iteration)
            
            glBindVertexArray(quadVAO);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            glBindVertexArray(0);

            horizontal = !horizontal;
            if (first_iteration)
                first_iteration = false;
        }
        glEnable(GL_DEPTH_TEST);

        // 4. draw to screen
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
        // clear all relevant buffers
        // glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // set clear color to white (not really necessary actually, since we won't be able to see behind the quad anyways)
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        int width = SCR_WIDTH, height = SCR_HEIGHT;
        glfwGetWindowSize(window, &width, &height);
        float widthScale = (float)width / RENDER_SIZE;
        float heightScale = (float)height / RENDER_SIZE;
        if (widthScale >= heightScale)
        {
            glViewport((width - heightScale * RENDER_SIZE) / 2, 0, heightScale * RENDER_SIZE, height);
        }
        else {
            glViewport(0, (height - widthScale * RENDER_SIZE) / 2, width, widthScale * RENDER_SIZE);
        }

        blendShader.use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, colorBuffer);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[!horizontal]);
        blendShader.setInt("bloom", true);
        blendShader.setFloat("alpha", BLUR_ALPHA);

        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindVertexArray(0);

        glEnable(GL_DEPTH_TEST);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    delete[] cVertices;
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    /*float widthScale = (float)width / SCR_WIDTH;
    float heightScale = (float)height / SCR_HEIGHT;
    if (widthScale >= heightScale)
    {
        glViewport((width - heightScale * SCR_WIDTH) / 2, 0, heightScale * SCR_WIDTH, height);
    }
    else {
        glViewport(0, (height - widthScale * SCR_HEIGHT) / 2, width, widthScale * SCR_HEIGHT);
    }*/
}

void calc_color_gradient(float* red, float* green, float* blue)
{
    double timeVal = glfwGetTimerValue() % (uint64_t)(COLOR_GRADIENT_PERIOD * 6 * glfwGetTimerFrequency()) 
        / (double)glfwGetTimerFrequency();

    if (timeVal >= 0 && timeVal < COLOR_GRADIENT_PERIOD) {
        *red = COLOR_LOWER_BOUND;
        *green = COLOR_UPPER_BOUND - COLOR_DIFF * (timeVal / COLOR_GRADIENT_PERIOD);
        *blue = COLOR_UPPER_BOUND;
    }
    else if (timeVal >= COLOR_GRADIENT_PERIOD && timeVal < 2.0 * COLOR_GRADIENT_PERIOD) {
        timeVal -= COLOR_GRADIENT_PERIOD;
        *red = COLOR_LOWER_BOUND + COLOR_DIFF * (timeVal / COLOR_GRADIENT_PERIOD);
        *green = COLOR_LOWER_BOUND;
        *blue = COLOR_UPPER_BOUND;
    }
    else if (timeVal >= 2.0 * COLOR_GRADIENT_PERIOD && timeVal < 3.0 * COLOR_GRADIENT_PERIOD) {
        timeVal -= 2.0 * COLOR_GRADIENT_PERIOD;
        *red = COLOR_UPPER_BOUND;
        *green = COLOR_LOWER_BOUND;
        *blue = COLOR_UPPER_BOUND - COLOR_DIFF * (timeVal / COLOR_GRADIENT_PERIOD);
    }
    else if (timeVal >= 3.0 * COLOR_GRADIENT_PERIOD && timeVal < 4.0 * COLOR_GRADIENT_PERIOD) {
        timeVal -= 3.0 * COLOR_GRADIENT_PERIOD;
        *red = COLOR_UPPER_BOUND;
        *green = COLOR_LOWER_BOUND + COLOR_DIFF * (timeVal / COLOR_GRADIENT_PERIOD);
        *blue = COLOR_LOWER_BOUND;
    }
    else if (timeVal >= 4.0 * COLOR_GRADIENT_PERIOD && timeVal < 5.0 * COLOR_GRADIENT_PERIOD) {
        timeVal -= 4.0 * COLOR_GRADIENT_PERIOD;
        *red = COLOR_UPPER_BOUND - COLOR_DIFF * (timeVal / COLOR_GRADIENT_PERIOD);
        *green = COLOR_UPPER_BOUND;
        *blue = COLOR_LOWER_BOUND;
    }
    else if (timeVal >= 5.0 * COLOR_GRADIENT_PERIOD && timeVal < 6.0 * COLOR_GRADIENT_PERIOD) {
        timeVal -= 5.0 * COLOR_GRADIENT_PERIOD;
        *red = COLOR_LOWER_BOUND;
        *green = COLOR_UPPER_BOUND;
        *blue = COLOR_LOWER_BOUND + COLOR_DIFF * (timeVal / COLOR_GRADIENT_PERIOD);
    }
}

void calc_transform(glm::mat4* trans)
{
    float rotVal = glfwGetTimerValue() % (uint64_t)(ROTATION_PERIOD * glfwGetTimerFrequency())
        / (double)glfwGetTimerFrequency() / ROTATION_PERIOD * glm::two_pi<float>();

    *trans = glm::mat4(1.0f);
    *trans = glm::rotate(*trans, rotVal, glm::vec3(0.0f, 0.0f, 1.0f));
}