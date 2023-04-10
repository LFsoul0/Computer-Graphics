#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <fstream>
#include <vector>

#include "Shader.h"
#include "Font.h"
#include "Point.h"
#include "Edge.h"
#include "TextureData.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void processInput(GLFWwindow* window);
void scanline(Edge** edges, TextureData& data, size_t max_poly = 10);

// settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 800;
const unsigned int SSAA_MULTIPLE = 2;
const unsigned int REN_WIDTH = SCR_WIDTH * SSAA_MULTIPLE;
const unsigned int REN_HEIGHT = SCR_HEIGHT * SSAA_MULTIPLE;
const glm::vec3 Background = glm::vec3(0, 0, 0);
unsigned int Index = 0;


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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Scanline", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // shaders
    Shader screenShader("scanline.vs", "scanline.fs");

    // fonts
    Font comicFont("comici.ttf", 72, SCR_WIDTH, SCR_HEIGHT);

    // Set OpenGL options
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    std::ifstream ifs;
    std::cout << "Preparing...";

    if (!glfwWindowShouldClose(window))
    {
        // Clear the colorbuffer
        glClearColor(Background.r, Background.g, Background.b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        comicFont.RenderText("Loading...", SCR_WIDTH / 2 - 160, SCR_HEIGHT / 2 - 20, 1.0f, glm::vec3(1, 1, 1) - Background);

        // Swap the buffers
        glfwSwapBuffers(window);
    }
    
    // 1. overlapping
    float overlapVertices[72]{ 0 };
    ifs.open("overlapping.tri");
    for (float &f : overlapVertices) {
        ifs >> f;
    }
    ifs.close();

    Edge** overlapEdges = new Edge*[REN_HEIGHT]{ nullptr };
    for (int i = 0; i < 4; ++i) {
        Point p[3];
        for (int j = 0; j < 3; ++j) {
            p[j].x = overlapVertices[18 * i + 3 * j];
            p[j].y = overlapVertices[18 * i + 3 * j + 1];
            p[j].z = overlapVertices[18 * i + 3 * j + 2];
            p[j].r = overlapVertices[18 * i + 3 * j + 9];
            p[j].g = overlapVertices[18 * i + 3 * j + 10];
            p[j].b = overlapVertices[18 * i + 3 * j + 11];

            p[j].scale(3 * SSAA_MULTIPLE, 3 * SSAA_MULTIPLE);
            p[j].x += REN_WIDTH / 2;
            p[j].y += REN_HEIGHT / 2;
        }

        if (p[0].y < p[1].y) {
            overlapEdges[int(p[0].y)] = new Edge(p[0], p[1], i, overlapEdges[int(p[0].y)]);
        }
        else if (p[0].y > p[1].y) {
            overlapEdges[int(p[1].y)] = new Edge(p[1], p[0], i, overlapEdges[int(p[1].y)]);
        }
        if (p[0].y < p[2].y) {
            overlapEdges[int(p[0].y)] = new Edge(p[0], p[2], i, overlapEdges[int(p[0].y)]);
        }
        else if (p[0].y > p[2].y) {
            overlapEdges[int(p[2].y)] = new Edge(p[2], p[0], i, overlapEdges[int(p[2].y)]);
        }
        if (p[1].y < p[2].y) {
            overlapEdges[int(p[1].y)] = new Edge(p[1], p[2], i, overlapEdges[int(p[1].y)]);
        }
        else if (p[1].y > p[2].y) {
            overlapEdges[int(p[2].y)] = new Edge(p[2], p[1], i, overlapEdges[int(p[2].y)]);
        }
    }

    TextureData overlapData(REN_WIDTH, REN_HEIGHT, Background);
    scanline(overlapEdges, overlapData);
    unsigned int overlapTexture;
    glGenTextures(1, &overlapTexture);
    glBindTexture(GL_TEXTURE_2D, overlapTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, REN_WIDTH, REN_HEIGHT, 0, GL_RGB, GL_FLOAT, overlapData.data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    // 2. intersecting
    float intersectVertices[36]{ 0 };
    ifs.open("intersecting.tri");
    for (float& f : intersectVertices) {
        ifs >> f;
    }
    ifs.close();

    Edge** intersectEdges = new Edge*[REN_HEIGHT]{ nullptr };
    for (int i = 0; i < 2; ++i) {
        Point p[3];
        for (int j = 0; j < 3; ++j) {
            p[j].x = intersectVertices[18 * i + 3 * j];
            p[j].y = intersectVertices[18 * i + 3 * j + 1];
            p[j].z = intersectVertices[18 * i + 3 * j + 2];
            p[j].r = intersectVertices[18 * i + 3 * j + 9];
            p[j].g = intersectVertices[18 * i + 3 * j + 10];
            p[j].b = intersectVertices[18 * i + 3 * j + 11];

            p[j].y -= 50;
            p[j].x -= 50;
            p[j].projection(-0.6f, 0.1f);
            p[j].scale(4 * SSAA_MULTIPLE, 4 * SSAA_MULTIPLE);
            p[j].x += REN_WIDTH / 2;
            p[j].y += REN_HEIGHT / 2;
        }

        if (p[0].y < p[1].y) {
            intersectEdges[int(p[0].y)] = new Edge(p[0], p[1], i, intersectEdges[int(p[0].y)]);
        }
        else if (p[0].y > p[1].y) {
            intersectEdges[int(p[1].y)] = new Edge(p[1], p[0], i, intersectEdges[int(p[1].y)]);
        }
        if (p[0].y < p[2].y) {
            intersectEdges[int(p[0].y)] = new Edge(p[0], p[2], i, intersectEdges[int(p[0].y)]);
        }
        else if (p[0].y > p[2].y) {
            intersectEdges[int(p[2].y)] = new Edge(p[2], p[0], i, intersectEdges[int(p[2].y)]);
        }
        if (p[1].y < p[2].y) {
            intersectEdges[int(p[1].y)] = new Edge(p[1], p[2], i, intersectEdges[int(p[1].y)]);
        }
        else if (p[1].y > p[2].y) {
            intersectEdges[int(p[2].y)] = new Edge(p[2], p[1], i, intersectEdges[int(p[2].y)]);
        }
    }

    TextureData intersectData(REN_WIDTH, REN_HEIGHT, Background);
    scanline(intersectEdges, intersectData);
    unsigned int intersectTexture;
    glGenTextures(1, &intersectTexture);
    glBindTexture(GL_TEXTURE_2D, intersectTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, REN_WIDTH, REN_HEIGHT, 0, GL_RGB, GL_FLOAT, intersectData.data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    // 3. weaving
    Edge** weaveEdges = new Edge*[REN_HEIGHT] { nullptr };
    for (int i = 0; i < 6; ++i) {
        Point pleft[8], pright[8];

        // bottom-left --> top-right
        for (int j = 0; j < 8; ++j) {
            if (j == 0) {
                pleft[0].setPos(-118 + 20 * i, -60, 11 - 2 * (i % 2));
                pright[0].setPos(-102 + 20 * i, -60, 19 - 18 * (i % 2));
            }
            else if (j == 7) {
                pleft[7].setPos(2 + 20 * i, 60, 11 - 2 * (i % 2));
                pright[7].setPos(18 + 20 * i, 60, 19 - 18 * (i % 2));
            }
            else {
                pleft[j].setPos(-129 + 20 * (i + j), -71 + 20 * j, 20 * ((i + j) % 2));
                pright[j].setPos(-121 + 20 * (i + j), -79 + 20 * j, 20 * ((i + j) % 2));
            }

            if (i != 5 && j <= (6 - i) / 2 || i != 0 && j >= 7 - i / 2) {
                pleft[j].setColor(glm::vec3(0.3f, 0.5f, 0.5f) + glm::vec3(0.5f, 0.5f, 0.5f));
                pright[j].setColor(glm::vec3(0.3f, 0.4f, 0.5f) + glm::vec3(0.5f, 0.5f, 0.5f));
            }
            else {
                pleft[j].setColor(glm::vec3(0.3f, 0.5f, 0.5f) + pleft[j].z / 20 * glm::vec3(0.5f, 0.5f, 0.5f));
                pright[j].setColor(glm::vec3(0.3f, 0.4f, 0.5f) + pright[j].z / 20 * glm::vec3(0.5f, 0.5f, 0.5f));
            }

            pleft[j].scale(4 * SSAA_MULTIPLE, 4 * SSAA_MULTIPLE);
            pleft[j].x += REN_WIDTH / 2;
            pleft[j].y += REN_HEIGHT / 2;
            pright[j].scale(4 * SSAA_MULTIPLE, 4 * SSAA_MULTIPLE);
            pright[j].x += REN_WIDTH / 2;
            pright[j].y += REN_HEIGHT / 2;
        }

        for (int j = 0; j < 7; ++j) {
            weaveEdges[int(pleft[j].y)] = new Edge(pleft[j], pleft[j + 1], 7 * i + j, weaveEdges[int(pleft[j].y)]);
            weaveEdges[int(pright[j].y)] = new Edge(pright[j], pright[j + 1], 7 * i + j, weaveEdges[int(pright[j].y)]);
            if (j != 0) {
                weaveEdges[int(pright[j].y)] = new Edge(pright[j], pleft[j], 7 * i + j, weaveEdges[int(pright[j].y)]);
            }
            if (j != 6) {
                weaveEdges[int(pright[j + 1].y)] = new Edge(pright[j + 1], pleft[j + 1], 7 * i + j, weaveEdges[int(pright[j + 1].y)]);
            }
        }
        
        // top-left --> bottom-right
        for (int j = 0; j < 8; ++j) {
            if (j == 0) {
                pleft[0].setPos(-118 + 20 * i, 60, 9 + 2 * (i % 2));
                pright[0].setPos(-102 + 20 * i, 60, 1 + 18 * (i % 2));
            }
            else if (j == 7) {
                pleft[7].setPos(2 + 20 * i, -60, 9 + 2 * (i % 2));
                pright[7].setPos(18 + 20 * i, -60, 1 + 18 * (i % 2));
            }
            else {
                pleft[j].setPos(-129 + 20 * (i + j), 71 - 20 * j, 20 - 20 * ((i + j) % 2));
                pright[j].setPos(-121 + 20 * (i + j), 79 - 20 * j, 20 - 20 * ((i + j) % 2));
            }

            if (i != 5 && j <= (6 - i) / 2 || i != 0 && j >= 7 - i / 2) {
                pleft[j].setColor(glm::vec3(0.3f, 0.3f, 0.5f) + glm::vec3(0.5f, 0.5f, 0.5f));
                pright[j].setColor(glm::vec3(0.3f, 0.4f, 0.5f) + glm::vec3(0.5f, 0.5f, 0.5f));
            }
            else {
                pleft[j].setColor(glm::vec3(0.3f, 0.3f, 0.5f) + pleft[j].z / 20 * glm::vec3(0.5f, 0.5f, 0.5f));
                pright[j].setColor(glm::vec3(0.3f, 0.4f, 0.5f) + pright[j].z / 20 * glm::vec3(0.5f, 0.5f, 0.5f));
            }

            pleft[j].scale(4 * SSAA_MULTIPLE, 4 * SSAA_MULTIPLE);
            pleft[j].x += REN_WIDTH / 2;
            pleft[j].y += REN_HEIGHT / 2;
            pright[j].scale(4 * SSAA_MULTIPLE, 4 * SSAA_MULTIPLE);
            pright[j].x += REN_WIDTH / 2;
            pright[j].y += REN_HEIGHT / 2;
        }

        for (int j = 0; j < 7; ++j) {
            weaveEdges[int(pleft[j + 1].y)] = new Edge(pleft[j + 1], pleft[j], 42 + 7 * i + j, weaveEdges[int(pleft[j + 1].y)]);
            weaveEdges[int(pright[j + 1].y)] = new Edge(pright[j + 1], pright[j], 42 + 7 * i + j, weaveEdges[int(pright[j + 1].y)]);
            if (j != 0) {
                weaveEdges[int(pleft[j].y)] = new Edge(pleft[j], pright[j], 42 + 7 * i + j, weaveEdges[int(pleft[j].y)]);
            }
            if (j != 6) {
                weaveEdges[int(pleft[j + 1].y)] = new Edge(pleft[j + 1], pright[j + 1], 42 + 7 * i + j, weaveEdges[int(pleft[j + 1].y)]);
            }
        }
    }
    TextureData weaveData(REN_WIDTH, REN_HEIGHT, Background);
    scanline(weaveEdges, weaveData, 100);
    unsigned int weaveTexture;
    glGenTextures(1, &weaveTexture);
    glBindTexture(GL_TEXTURE_2D, weaveTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, REN_WIDTH, REN_HEIGHT, 0, GL_RGB, GL_FLOAT, weaveData.data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    std::cout << std::endl << "Done." << std::endl;

    // screen quad
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

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearColor(Background.r, Background.g, Background.b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        screenShader.use();
        glActiveTexture(GL_TEXTURE0);
        switch (Index)
        {
        case 0:
            glBindTexture(GL_TEXTURE_2D, overlapTexture);
            break;
        case 1:
            glBindTexture(GL_TEXTURE_2D, intersectTexture);
            break;
        case 2:
            glBindTexture(GL_TEXTURE_2D, weaveTexture);
            break;
        default:
            break;
        }
        
        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindVertexArray(0);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    delete[] overlapEdges;
    delete[] intersectEdges;
    delete[] weaveEdges;
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
    float widthScale = (float)width / SCR_WIDTH;
    float heightScale = (float)height / SCR_HEIGHT;
    if (widthScale >= heightScale)
    {
        glViewport((width - heightScale * SCR_WIDTH) / 2, 0, heightScale * SCR_WIDTH, height);
    }
    else {
        glViewport(0, (height - widthScale * SCR_HEIGHT) / 2, width, widthScale * SCR_HEIGHT);
    }

}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    const static unsigned int frame_num = 3;

    if (action == GLFW_PRESS) switch (button) {
    case GLFW_MOUSE_BUTTON_LEFT:
        Index = (Index + 1) % frame_num;
        break;
    case GLFW_MOUSE_BUTTON_RIGHT:
        Index = (Index + frame_num - 1) % frame_num;
        break;
    default:
        break;
    }
    return;
}

void scanline(Edge** edges, TextureData& data, size_t max_poly)
{
    std::vector<Edge> aet;
    std::vector<SubEdge> aset;
    SubEdge** subEdges = new SubEdge * [data.width]{ nullptr };
    Edge** polySlot = new Edge * [max_poly] { nullptr };

    for (size_t y = 0; y < data.height; ++y) {
        // 1. remove
        for (auto it = aet.begin(); it < aet.end();) {
            if (it->max_y <= y) {
                it = aet.erase(it);
            }
            else {
                ++it;
            }
        }

        // 2. insert
        while (edges[y]) {
            auto it = aet.begin();
            while (it < aet.end() && it->curr_x < edges[y]->curr_x) ++it;

            Edge* e = edges[y];
            edges[y] = e->next;
            e->next = nullptr;

            aet.insert(it, *e);
            delete e;
        }

        // 3. paint line
        for (Edge& e : aet) {
            if (polySlot[e.poly_id]) {
                subEdges[int(polySlot[e.poly_id]->curr_x)] = new SubEdge(*polySlot[e.poly_id], e,
                    subEdges[int(polySlot[e.poly_id]->curr_x)]);
                polySlot[e.poly_id] = nullptr;
            }
            else {
                polySlot[e.poly_id] = &e;
            }
        }

        aset.clear();
        for (size_t x = 0; x < data.width; ++x) {
            // 3.1 remove
            for (auto it = aset.begin(); it < aset.end();) {
                if (it->max_x < x) {
                    it = aset.erase(it);
                }
                else {
                    ++it;
                }
            }

            // 3.2 insert
            while (subEdges[x]) {
                auto it = aset.begin();
                while (it < aset.end() && it->curr_z > subEdges[x]->curr_z) ++it;

                SubEdge* se = subEdges[x];
                subEdges[x] = se->next;
                se->next = nullptr;

                aset.insert(it, *se);
                delete se;
            }

            // 3.3 paint point
            if (!aset.empty()) {
                data.setPixel(x, y, aset[0].curr_color);
            }
            
            // 3.4 update
            for (SubEdge& se : aset) {
                se.curr_z += se.dz;
                se.curr_color += se.dcolor;
            }

            // 3.5 bubble sort
            while (!aset.empty()) {
                bool flag = false;
                size_t size = aset.size();
                for (size_t i = 0; i < size - 1; ++i) {
                    if (aset[i].curr_z < aset[i + 1].curr_z) {
                        SubEdge se = aset[i];
                        aset[i] = aset[i + 1];
                        aset[i + 1] = se;
                        flag = true;
                    }
                }

                if (!flag) break;
            }
        }

        // 4. update
        for (Edge& e : aet) {
            e.curr_x += e.dx;
            e.curr_z += e.dz;
            e.curr_color += e.dcolor;
        }

        // 5. bubble sort
        while (!aet.empty()) {
            bool flag = false;
            size_t size = aet.size();
            for (size_t i = 0; i < size - 1; ++i) {
                if (aet[i].curr_x > aet[i + 1].curr_x) {
                    Edge e = aet[i];
                    aet[i] = aet[i + 1];
                    aet[i + 1] = e;
                    flag = true;
                }
            }

            if (!flag) break;
        }
    }

    delete[] subEdges;
    delete[] polySlot;

}
