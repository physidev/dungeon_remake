#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace ph {
    class Window {
    public:
        GLFWwindow* window;
        int width;
        int height;

        Window(const int width, const int height) : width(width), height(height) {
            // load GLFW and create window
            if (!glfwInit()) {
                std::cerr << "Error: Failed to initialize GLFW!" << std::endl;
                exit(EXIT_FAILURE);
            }
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
            glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

            window = glfwCreateWindow(width, height, "Dungeon remake!!", nullptr, nullptr);

            // check for valid context
            if (window == nullptr) {
                std::cerr << "Error: Failed to create GLFW context!" << std::endl;
                glfwTerminate();
                exit(EXIT_FAILURE);
            }

            // create context and load OpenGL functions
            glfwMakeContextCurrent(window);
            gladLoadGL();
            std::cout << "OpenGL " << glGetString(GL_VERSION) << "\n";

            glViewport(0, 0, width, height);
            glEnable(GL_DEPTH_TEST);

            glfwSetWindowSizeCallback(window, [](GLFWwindow* _, const int w, const int h) {
                glViewport(0, 0, w, h);
            });
        }

        ~Window() {
            glfwTerminate();
        }

        bool isOpen() const {
            return glfwWindowShouldClose(window) == false;
        }

        void setShouldClose(const bool val) const {
            glfwSetWindowShouldClose(window, val);
        }

        bool isKeyPressed(const int key) const {
            return glfwGetKey(window, key) == GLFW_PRESS;
        }

        static void pollEvents() {
            glfwPollEvents();
        }

        void swapBuffers() const {
            glfwSwapBuffers(window);
        }
    };
    class Texture {
    public:
        GLuint id{0};

        explicit Texture(const std::string &imagePath) {
            // parameters: # of textures, id
            glGenTextures(1, &id);
            glBindTexture(GL_TEXTURE_2D, id);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

            int width, height, numChannels;
            stbi_set_flip_vertically_on_load(true);
            stbi_uc* data = stbi_load(imagePath.c_str(), &width, &height, &numChannels, 0);
            if (!data)
                std::cout << "Error: Failed to load image at " << imagePath << "!\n";
            const auto format = (numChannels == 4) ? GL_RGBA : GL_RGB;
            glTexImage2D(
                GL_TEXTURE_2D,      // texture target
                0,                  // mipmap level
                format,             // texture data format
                width,              // width of image (px)
                height,             // height of image (px)
                0,                  // legacy stuff
                format,             // image data interpretation
                GL_UNSIGNED_BYTE,   // image data format
                data
            );
            glGenerateMipmap(GL_TEXTURE_2D);
            stbi_image_free(data);
        }

        void bind() const {
            glBindTexture(GL_TEXTURE_2D, id);
        }
    };
    class Shader {
    public:
        const GLuint program = glCreateProgram();
        Shader(const std::string &vertexShaderSource, const std::string &fragmentShaderSource) {

            // CREATE VERTEX SHADER
            const GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
            const char* vs_cstr = vertexShaderSource.c_str();
            glShaderSource(vertexShader, 1, &vs_cstr, nullptr);
            glCompileShader(vertexShader);
            // check compilation success
            int success;
            glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
            if (!success) {
                char infoLog[1024];
                glGetShaderInfoLog(vertexShader, 1024, nullptr, infoLog);
                std::cout << "Error: Vertex shader compilation failed! " << infoLog << std::endl;
            }

            // CREATE FRAGMENT SHADER
            const GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
            const char* fs_cstr = fragmentShaderSource.c_str();
            glShaderSource(fragmentShader, 1, &fs_cstr, nullptr);
            glCompileShader(fragmentShader);
            // check compilation status
            glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
            if (!success) {
                char infoLog[1024];
                glGetShaderInfoLog(fragmentShader, 1024, nullptr, infoLog);
                std::cout << "Error: Fragment shader compilation failed! " << infoLog << std::endl;
            }

            // CREATE PROGRAM
            glAttachShader(program, vertexShader);
            glAttachShader(program, fragmentShader);
            glLinkProgram(program);
            // check link success
            glGetProgramiv(program, GL_LINK_STATUS, &success);
            if (!success) {
                char infoLog[1024];
                glGetProgramInfoLog(program, 1024, nullptr, infoLog);
                std::cout << "Error: Shader program link failed! " << infoLog << std::endl;
            }
            glValidateProgram(program);
            // check validate status
            glGetProgramiv(program, GL_VALIDATE_STATUS, &success);
            if (!success) {
                char infoLog[1024];
                glGetProgramInfoLog(program, 1024, nullptr, infoLog);
                std::cout << "Error: Shader program validation failed! " << infoLog << std::endl;
            }
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);
        }

        ~Shader() {
            glDeleteProgram(program);
        }

        void use() const {
            glUseProgram(program);
        }

        void setUniform(const std::string &name, const int value) const {
            glUniform1i(glGetUniformLocation(program, name.c_str()), value);
        }

        void setUniform(const std::string &name, const glm::mat4 &value) const {
            const GLint location = glGetUniformLocation(program, name.c_str());
            glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
        }

        void setUniform(const std::string &name, const glm::vec3 &value) const {
            glUniform3f(glGetUniformLocation(program, name.c_str()), value.x, value.y, value.z);
        }

        static Shader loadFromFile(const std::string &vertexShaderPath, const std::string &fragmentShaderPath) {
            std::stringstream vBuffer, fBuffer;
            std::ifstream file;
            file.open(vertexShaderPath);
            vBuffer << file.rdbuf();
            file.close();
            file.open(fragmentShaderPath);
            fBuffer << file.rdbuf();
            file.close();
            return {vBuffer.str(), fBuffer.str()};
        }
    };
    class Camera {
    public:
        glm::vec3 position;
        glm::vec3 target;
        glm::vec3 up{0.0f, 1.0f, 0.0f};

        Camera(const glm::vec3 &position, const glm::vec3 &target) : position(position) , target(target) {}

        glm::mat4 viewMatrix() const {
            const glm::vec3 viewDirection = glm::normalize(position - target);
            // To obtain a vector pointing in the +x direction, we use the Gram-Schmidt procedure.
            // In particular, note that -z camera = -z world, so +x = cross(+y, +z)), where +z is
            // the camera direction.
            const glm::vec3 right = glm::normalize(glm::cross(up, viewDirection));
            // glm::lookAt computes the view matrix for a camera, given its (1) position,
            // (2) target, (3) up vector.
            return glm::lookAt(position, target, right);
        };
    };
}

int main() {
    constexpr int WIDTH = 1280;
    constexpr int HEIGHT = 720;
    const ph::Window window{WIDTH, HEIGHT};

    // GEOMETRY/VERTEX DATA
    // send vertex data to GPU
    constexpr float vertices[] = {
        // positions            // colors           // texCoords
        0.5f,   0.5f,  0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,     // 0 top      right   front
        0.5f,  -0.5f,  0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,     // 1 bottom   right   front
       -0.5f,  -0.5f,  0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,     // 2 bottom   left    front
       -0.5f,   0.5f,  0.5f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f,     // 3 top      left    front
        0.5f,   0.5f,  -0.5f,  1.0f, 0.0f, 0.0f,   1.0f, 1.0f,     // 4 top      right   back
        0.5f,  -0.5f,  -0.5f,  0.0f, 1.0f, 0.0f,   1.0f, 0.0f,     // 5 bottom   right   back
       -0.5f,  -0.5f,  -0.5f,  0.0f, 0.0f, 1.0f,   0.0f, 0.0f,     // 6 bottom   left    back
       -0.5f,   0.5f,  -0.5f,  1.0f, 1.0f, 1.0f,   0.0f, 1.0f,     // 7 top      left    back
   };
    constexpr GLuint indices[] = {
        0, 1, 3, 1, 2, 3,   // front
        7, 4, 6, 4, 6, 5,   // back
        0, 3, 4, 3, 4, 7,   // top
        1, 2, 5, 2, 5, 6,   // bottom
        3, 2, 7, 2, 7, 6,   // left
        0, 1, 4, 1, 4, 5,   // right
    };

    // Vertex array objects remember the precise binding and unbinding order
    // of the vertex buffer and element buffer objects. It then suffices to
    // bind the vertex array object when making render calls, instead of binding
    // each buffer and specifying the attribute pointers.
    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    // parameters: which attribute, size of vertex attribute, data type, normalize?, stride, offset (void*).
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);


    // TEXTURE DATA (brick cube)
    ph::Texture brick{"resources/textures/test.jpg"};
    ph::Texture lamp{"resources/textures/lamp.png"};

    // SHADERS
    // brick shader
    const ph::Shader shader = ph::Shader::loadFromFile(
    "resources/shaders/basic.vert", "resources/shaders/basic.frag");
    shader.use();
    // set which texture unit to use in the shader
    shader.setUniform("uTexture", 0);
    shader.setUniform("uLightColor", glm::vec3(1.0f, 1.0f, 1.0f));

    // lighting object (lamp) shader
    const ph::Shader lampShader = ph::Shader::loadFromFile(
        "resources/shaders/basic.vert", "resources/shaders/light.frag");

    // glBindTexture(GL_TEXTURE_2D, brickTexture);
    brick.bind();
    lampShader.use();
    shader.setUniform("uTexture", 0);

    // TRANSFORMATION DATA
    // camera
    constexpr float h = 5.0f;
    constexpr float r = 5.0f;
    const glm::vec3 cameraPos{r, 0.0f, h};
    const glm::vec3 cameraTarget{0.0f, 0.0f, 0.0f};
    ph::Camera camera{cameraPos, cameraTarget};

    const auto projection = glm::perspective(glm::radians(45.0f), WIDTH/static_cast<float>(HEIGHT), 0.1f, 100.0f);

    glm::vec3 positions[] = {
        {0.0f, 3.0f, 2.0f},
        {2.4f, -0.6f, 2.8f},
        {-3.0f, -1.4f, -1.4f},
        {3.6f, 1.6f, -0.4f},
        {-1.8f, 2.4f, -2.2f},
        {0.8f, -3.2f, 1.8f},
        {-2.6f, -0.2f, 2.8f},
        {2.2f, -2.0f, -2.4f},
        {-1.2f, 1.0f, 3.4f}
    };
    const float cameraSpeed = 8.0f;
    const glm::vec3 xHat{1.0f, 0.0f, 0.0f};
    const glm::vec3 yHat{0.0f, 1.0f, 0.0f};

    // GAME LOOP
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    while (window.isOpen()) {
        if (window.isKeyPressed(GLFW_KEY_ESCAPE))
            window.setShouldClose(true);

        // UPDATE
        const float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        if (window.isKeyPressed(GLFW_KEY_W))
            camera.position += cameraSpeed * deltaTime * xHat;
        if (window.isKeyPressed(GLFW_KEY_S))
            camera.position -= cameraSpeed * deltaTime * xHat;
        if (window.isKeyPressed(GLFW_KEY_A))
            camera.position += cameraSpeed * deltaTime * yHat;
        if (window.isKeyPressed(GLFW_KEY_D))
            camera.position -= cameraSpeed * deltaTime * yHat;

        // RENDER
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // The data flow for rendering is as follows:
        //  1)  Bind textures
        //  2)  Specify the shader program to use
        //  3)  Bind the vertex array object (i.e. pass the vertex data
        //      to the GPU)
        //  4)  Draw the object to the screen.
        brick.bind();
        shader.use();

        glBindVertexArray(VAO);

        const auto theta = static_cast<float>(glfwGetTime());

        //camera.position = {h*sin(theta), h*cos(theta), 0.0};
        const auto view = camera.viewMatrix();

        shader.setUniform("uView", view);
        shader.setUniform("uProjection", projection);

        for (auto v : positions) {
            auto model = glm::mat4(1.0f);
            //model = glm::rotate(model, theta, glm::normalize(v));
            model = glm::translate(model, v);

            shader.setUniform("uModel", model);
            glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(GLuint), GL_UNSIGNED_INT, 0);
        }

        window.swapBuffers();
        ph::Window::pollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    return EXIT_SUCCESS;
}