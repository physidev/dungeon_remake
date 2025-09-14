#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <memory>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace ph {
    class Window {
        GLFWwindow* window;

    public:
        Window(const int width, const int height) {
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
        GLuint id{0};

    public:
        explicit Texture(const std::string &imagePath) {
            // parameters: # of textures, id
            glGenTextures(1, &id);
            glBindTexture(GL_TEXTURE_2D, id);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

            int width = 0, height = 0, numChannels;
            stbi_set_flip_vertically_on_load(true);

            const stbi_uc* data = stbi_load(imagePath.c_str(), &width, &height, &numChannels, 0);
            if (!data)
                std::cout << "Error: Failed to load image at " << imagePath << "!\n";
            const auto format = (numChannels == 4) ? GL_RGBA : GL_RGB;

            glTexImage2D(
                GL_TEXTURE_2D,              // texture target
                0,                          // mipmap level
                format,               // texture data format
                width,                // width of image (px)
                height,               // height of image (px)
                0,                          // legacy stuff
                format,               // image data interpretation
                GL_UNSIGNED_BYTE,           // image data format
                data
            );
            glGenerateMipmap(GL_TEXTURE_2D);
        }

        ~Texture() {
            glDeleteTextures(1, &id);
        }

        GLuint getID() const {
            return id;
        }
    };
    class Shader {
        const GLuint id = glCreateProgram();

    public:
        Shader(const std::string &vertexShaderPath, const std::string &fragmentShaderPath) {
            // LOAD SHADER SOURCES
            std::stringstream vBuffer, fBuffer;
            std::ifstream file;
            file.open(vertexShaderPath);
            vBuffer << file.rdbuf();
            file.close();
            file.open(fragmentShaderPath);
            fBuffer << file.rdbuf();
            file.close();
            const std::string vs_str = vBuffer.str(), fs_str = fBuffer.str();
            const char* vs_cstr = vs_str.c_str(), *fs_cstr = fs_str.c_str();

            // CREATE VERTEX SHADER
            const GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vertexShader, 1, &vs_cstr, nullptr);
            glCompileShader(vertexShader);
            // check compilation success
            int success;
            glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
            if (!success) {
                char infoLog[1024];
                glGetShaderInfoLog(vertexShader, 1024, nullptr, infoLog);
                std::cout << "ph::Error: Failed to compile vertex shader! (" << infoLog << ")\n";
            }

            // CREATE FRAGMENT SHADER
            const GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fragmentShader, 1, &fs_cstr, nullptr);
            glCompileShader(fragmentShader);
            // check compilation status
            glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
            if (!success) {
                char infoLog[1024];
                glGetShaderInfoLog(fragmentShader, 1024, nullptr, infoLog);
                std::cout << "ph::Error: Failed to compile fragment shader! (" << infoLog << ")\n";
            }

            // CREATE PROGRAM
            glAttachShader(id, vertexShader);
            glAttachShader(id, fragmentShader);
            glLinkProgram(id);
            // check link success
            glGetProgramiv(id, GL_LINK_STATUS, &success);
            if (!success) {
                char infoLog[1024];
                glGetProgramInfoLog(id, 1024, nullptr, infoLog);
                std::cout << "ph::Error: Failed to link shader program! (" << infoLog << ")\n";
            }
            glValidateProgram(id);
            // check validate status
            glGetProgramiv(id, GL_VALIDATE_STATUS, &success);
            if (!success) {
                char infoLog[1024];
                glGetProgramInfoLog(id, 1024, nullptr, infoLog);
                std::cout << "ph::Error: Failed to validate shader program! (" << infoLog << ")\n";
            }
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);
        }

        ~Shader() {
            glDeleteProgram(id);
        }

        GLuint getID() const {
            return id;
        }
    };
    // OpenGL state
    namespace gl {
        void bind(const Texture &texture) {
            glBindTexture(GL_TEXTURE_2D, texture.getID());
        }

        void bind(const Shader &shader) {
            glUseProgram(shader.getID());
        }

        void setUniform(const Shader& shader, const std::string &name, const int value) {
            glUniform1i(glGetUniformLocation(shader.getID(), name.c_str()), value);
        }

        void setUniform(const Shader& shader, const std::string &name, const glm::mat4 &value) {
            const GLint location = glGetUniformLocation(shader.getID(), name.c_str());
            glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
        }

        void setUniform(const Shader& shader, const std::string &name, const glm::vec3 &value) {
            glUniform3f(glGetUniformLocation(shader.getID(), name.c_str()), value.x, value.y, value.z);
        }
    }

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
    // Thirty-six vertices forming twelve triangles modeling the
    // six faces of one cube. The normals are not normalized here.
    constexpr float vertices[] = {
        // positions            // normals              // texCoords
        -0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  0.0f,    // -z
         0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,     0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,    // +z
         0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,     0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f,   -1.0f,  0.0f,  0.0f,     0.0f,  0.0f,    // -x
        -0.5f,  0.5f, -0.5f,   -1.0f,  0.0f,  0.0f,     1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,   -1.0f,  0.0f,  0.0f,     1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,   -1.0f,  0.0f,  0.0f,     1.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,   -1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,   -1.0f,  0.0f,  0.0f,     0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,    1.0f,  0.0f,  0.0f,     0.0f,  0.0f,    // +x
         0.5f,  0.5f, -0.5f,    1.0f,  0.0f,  0.0f,     1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,    1.0f,  0.0f,  0.0f,     1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,    1.0f,  0.0f,  0.0f,     1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,    1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,    1.0f,  0.0f,  0.0f,     0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  0.0f,    // -y
         0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,     1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,     1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,     1.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,     0.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  0.0f,    // +y
         0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,     1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,     1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,     0.0f,  0.0f,
    };

    // Vertex array objects remember the precise binding and unbinding order
    // of the vertex buffer and element buffer objects. It then suffices to
    // bind the vertex array object when making render calls, instead of binding
    // each buffer and specifying the attribute pointers.
    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(VAO);
    // position attribute
    // parameters: which attribute, size of vertex attribute, data type, normalize?, stride, offset (void*).
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // TEXTURE DATA
    ph::Texture brickTexture{"resources/textures/test.jpg"};
    ph::Texture lampTexture{"resources/textures/lamp.png"};

    // SHADERS
    // brick shader
    const ph::Shader brickShader{"resources/shaders/basic.vert", "resources/shaders/basic.frag"};

    ph::gl::bind(brickShader);
    // set which texture unit to use in the shader. You must use the shader program before
    // setting any uniforms in the shader.
    ph::gl::bind(brickTexture);
    ph::gl::setUniform(brickShader, "uTexture", 0);
    ph::gl::setUniform(brickShader, "uLamp.color", glm::vec3(1.0f, 1.0f, 1.0f));
    ph::gl::setUniform(brickShader, "uLamp.attenuation", glm::vec3(1.0f, 0.0f, 0.0075f));

    // lighting object (lamp) shader
    const ph::Shader lampShader{"resources/shaders/basic.vert", "resources/shaders/lamp.frag"};
    ph::gl::bind(lampShader);
    ph::gl::bind(lampTexture);
    ph::gl::setUniform(lampShader, "uTexture", 0);
    const auto M = glm::scale(glm::mat4(1.0f), {0.5f, 0.5f, 0.5f});
    ph::gl::setUniform(lampShader, "uModel", M);


    // TRANSFORMATION DATA
    // camera
    constexpr float h = 8.0f;
    const glm::vec3 cameraPos{0.0f, 0.0f, h};
    const glm::vec3 cameraTarget{0.0f, 0.0f, 0.0f};
    ph::Camera camera{cameraPos, cameraTarget};

    const auto projection = glm::perspective(glm::radians(45.0f), WIDTH/static_cast<float>(HEIGHT), 0.1f, 100.0f);
    ph::gl::bind(brickShader);
    ph::gl::setUniform(brickShader, "uProjection", projection);
    ph::gl::bind(lampShader);
    ph::gl::setUniform(lampShader, "uProjection", projection);

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
    constexpr float cameraSpeed = 8.0f;
    const glm::vec3 xHat{1.0f, 0.0f, 0.0f};
    const glm::vec3 yHat{0.0f, 1.0f, 0.0f};


    // GAME LOOP
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    while (window.isOpen()) {
        if (window.isKeyPressed(GLFW_KEY_ESCAPE))
            window.setShouldClose(true);

        // UPDATE
        const auto currentFrame = static_cast<float>(glfwGetTime());
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

        constexpr float lampR = 10.0f;
        const auto t = currentFrame;
        const glm::vec3 lampPosition{lampR * cos(t), lampR * sin(t), 2.0*cos(5.0*t)};

        // RENDER
        glClearColor(0.0f, 0.05f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // The data flow for rendering is as follows:
        //  1)  Bind textures
        //  2)  Specify the shader program to use
        //  3)  Bind the vertex array object (i.e. pass the vertex data
        //      to the GPU)
        //  4)  Draw the object to the screen.

        // bricks
        ph::gl::bind(brickTexture);
        ph::gl::bind(brickShader);
        glBindVertexArray(VAO);

        const auto theta = static_cast<float>(glfwGetTime());
        const auto view = camera.viewMatrix();
        ph::gl::setUniform(brickShader, "uView", view);
        ph::gl::setUniform(brickShader, "uLamp.position", lampPosition);

        for (auto v : positions) {
            auto model = glm::mat4(1.0f);
            // model = glm::rotate(model, theta, glm::normalize(v));
            model = glm::translate(model, v);

            ph::gl::setUniform(brickShader, "uModel", model);
            // parameters: primitive type, number of vertices, type of indices, ...
            glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices)/sizeof(float));
        }

        // lamp
        ph::gl::bind(lampTexture);
        ph::gl::bind(lampShader);
        ph::gl::setUniform(lampShader, "uView", view);

        auto lampModel = glm::mat4(1.0f);
        lampModel = glm::scale(lampModel, {0.5f, 0.5f, 0.5f});
        lampModel = glm::translate(lampModel, lampPosition);
        ph::gl::setUniform(lampShader, "uModel", lampModel);

        // use same VAO as before (cube)
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices)/sizeof(float));

        window.swapBuffers();
        ph::Window::pollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    return EXIT_SUCCESS;
}