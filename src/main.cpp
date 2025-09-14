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
    enum class Key {
        Escape, F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,

        Grave, One, Two, Three, Four, Five, Six, Seven, Eight, Nine, Zero, Minus, Equal, Backspace,
        Tab, Q, W, E, R, T, Y, U, I, O, P, LeftBracket, RightBracket, Backslash,
        CapsLock, A, S, D, F, G, H, J, K, L, Semicolon, Apostrophe, Enter,
        LeftShift, Z, X, C, V, B, N, M, Comma, Period, Slash, RightShift,
        LeftCtrl, LeftSuper, LeftAlt, Space, RightAlt, RightSuper, Menu, RightCtrl,

        Insert, Delete, Home, End, PageUp, PageDown,
        PrintScreen, ScrollLock, Pause,

        Up, Down, Left, Right,

        NumLock, NumpadDivide, NumpadMultiply, NumpadSubtract,
        Numpad7, Numpad8, Numpad9, NumpadAdd,
        Numpad4, Numpad5, Numpad6,
        Numpad1, Numpad2, Numpad3, NumpadEnter,
        Numpad0, NumpadDecimal
    };
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

        bool isKeyPressed(const Key key) const {
            int glfwKey;
            switch (key) {
                case Key::Escape:       glfwKey = GLFW_KEY_ESCAPE; break;
                case Key::F1:           glfwKey = GLFW_KEY_F1; break;
                case Key::F2:           glfwKey = GLFW_KEY_F2; break;
                case Key::F3:           glfwKey = GLFW_KEY_F3; break;
                case Key::F4:           glfwKey = GLFW_KEY_F4; break;
                case Key::F5:           glfwKey = GLFW_KEY_F5; break;
                case Key::F6:           glfwKey = GLFW_KEY_F6; break;
                case Key::F7:           glfwKey = GLFW_KEY_F7; break;
                case Key::F8:           glfwKey = GLFW_KEY_F8; break;
                case Key::F9:           glfwKey = GLFW_KEY_F9; break;
                case Key::F10:          glfwKey = GLFW_KEY_F10; break;
                case Key::F11:          glfwKey = GLFW_KEY_F11; break;
                case Key::F12:          glfwKey = GLFW_KEY_F12; break;

                case Key::Grave:        glfwKey = GLFW_KEY_GRAVE_ACCENT; break;
                case Key::One:          glfwKey = GLFW_KEY_1; break;
                case Key::Two:          glfwKey = GLFW_KEY_2; break;
                case Key::Three:        glfwKey = GLFW_KEY_3; break;
                case Key::Four:         glfwKey = GLFW_KEY_4; break;
                case Key::Five:         glfwKey = GLFW_KEY_5; break;
                case Key::Six:          glfwKey = GLFW_KEY_6; break;
                case Key::Seven:        glfwKey = GLFW_KEY_7; break;
                case Key::Eight:        glfwKey = GLFW_KEY_8; break;
                case Key::Nine:         glfwKey = GLFW_KEY_9; break;
                case Key::Zero:         glfwKey = GLFW_KEY_0; break;
                case Key::Minus:        glfwKey = GLFW_KEY_MINUS; break;
                case Key::Equal:        glfwKey = GLFW_KEY_EQUAL; break;
                case Key::Backspace:    glfwKey = GLFW_KEY_BACKSPACE; break;

                case Key::Tab:          glfwKey = GLFW_KEY_TAB; break;
                case Key::Q:            glfwKey = GLFW_KEY_Q; break;
                case Key::W:            glfwKey = GLFW_KEY_W; break;
                case Key::E:            glfwKey = GLFW_KEY_E; break;
                case Key::R:            glfwKey = GLFW_KEY_R; break;
                case Key::T:            glfwKey = GLFW_KEY_T; break;
                case Key::Y:            glfwKey = GLFW_KEY_Y; break;
                case Key::U:            glfwKey = GLFW_KEY_U; break;
                case Key::I:            glfwKey = GLFW_KEY_I; break;
                case Key::O:            glfwKey = GLFW_KEY_O; break;
                case Key::P:            glfwKey = GLFW_KEY_P; break;
                case Key::LeftBracket:  glfwKey = GLFW_KEY_LEFT_BRACKET; break;
                case Key::RightBracket: glfwKey = GLFW_KEY_RIGHT_BRACKET; break;
                case Key::Backslash:    glfwKey = GLFW_KEY_BACKSLASH; break;

                case Key::CapsLock:     glfwKey = GLFW_KEY_CAPS_LOCK; break;
                case Key::A:            glfwKey = GLFW_KEY_A; break;
                case Key::S:            glfwKey = GLFW_KEY_S; break;
                case Key::D:            glfwKey = GLFW_KEY_D; break;
                case Key::F:            glfwKey = GLFW_KEY_F; break;
                case Key::G:            glfwKey = GLFW_KEY_G; break;
                case Key::H:            glfwKey = GLFW_KEY_H; break;
                case Key::J:            glfwKey = GLFW_KEY_J; break;
                case Key::K:            glfwKey = GLFW_KEY_K; break;
                case Key::L:            glfwKey = GLFW_KEY_L; break;
                case Key::Semicolon:    glfwKey = GLFW_KEY_SEMICOLON; break;
                case Key::Apostrophe:   glfwKey = GLFW_KEY_APOSTROPHE; break;
                case Key::Enter:        glfwKey = GLFW_KEY_ENTER; break;

                case Key::LeftShift:    glfwKey = GLFW_KEY_LEFT_SHIFT; break;
                case Key::Z:            glfwKey = GLFW_KEY_Z; break;
                case Key::X:            glfwKey = GLFW_KEY_X; break;
                case Key::C:            glfwKey = GLFW_KEY_C; break;
                case Key::V:            glfwKey = GLFW_KEY_V; break;
                case Key::B:            glfwKey = GLFW_KEY_B; break;
                case Key::N:            glfwKey = GLFW_KEY_N; break;
                case Key::M:            glfwKey = GLFW_KEY_M; break;
                case Key::Comma:        glfwKey = GLFW_KEY_COMMA; break;
                case Key::Period:       glfwKey = GLFW_KEY_PERIOD; break;
                case Key::Slash:        glfwKey = GLFW_KEY_SLASH; break;
                case Key::RightShift:   glfwKey = GLFW_KEY_RIGHT_SHIFT; break;

                case Key::LeftCtrl:     glfwKey = GLFW_KEY_LEFT_CONTROL; break;
                case Key::LeftSuper:    glfwKey = GLFW_KEY_LEFT_SUPER; break;
                case Key::LeftAlt:      glfwKey = GLFW_KEY_LEFT_ALT; break;
                case Key::Space:        glfwKey = GLFW_KEY_SPACE; break;
                case Key::RightAlt:     glfwKey = GLFW_KEY_RIGHT_ALT; break;
                case Key::RightSuper:   glfwKey = GLFW_KEY_RIGHT_SUPER; break;
                case Key::Menu:         glfwKey = GLFW_KEY_MENU; break;
                case Key::RightCtrl:    glfwKey = GLFW_KEY_RIGHT_CONTROL; break;

                case Key::Insert:       glfwKey = GLFW_KEY_INSERT; break;
                case Key::Delete:       glfwKey = GLFW_KEY_DELETE; break;
                case Key::Home:         glfwKey = GLFW_KEY_HOME; break;
                case Key::End:          glfwKey = GLFW_KEY_END; break;
                case Key::PageUp:       glfwKey = GLFW_KEY_PAGE_UP; break;
                case Key::PageDown:     glfwKey = GLFW_KEY_PAGE_DOWN; break;

                case Key::PrintScreen:  glfwKey = GLFW_KEY_PRINT_SCREEN; break;
                case Key::ScrollLock:   glfwKey = GLFW_KEY_SCROLL_LOCK; break;
                case Key::Pause:        glfwKey = GLFW_KEY_PAUSE; break;

                case Key::Up:           glfwKey = GLFW_KEY_UP; break;
                case Key::Down:         glfwKey = GLFW_KEY_DOWN; break;
                case Key::Left:         glfwKey = GLFW_KEY_LEFT; break;
                case Key::Right:        glfwKey = GLFW_KEY_RIGHT; break;

                case Key::NumLock:      glfwKey = GLFW_KEY_NUM_LOCK; break;
                case Key::NumpadDivide: glfwKey = GLFW_KEY_KP_DIVIDE; break;
                case Key::NumpadMultiply: glfwKey = GLFW_KEY_KP_MULTIPLY; break;
                case Key::NumpadSubtract: glfwKey = GLFW_KEY_KP_SUBTRACT; break;
                case Key::Numpad7:      glfwKey = GLFW_KEY_KP_7; break;
                case Key::Numpad8:      glfwKey = GLFW_KEY_KP_8; break;
                case Key::Numpad9:      glfwKey = GLFW_KEY_KP_9; break;
                case Key::NumpadAdd:    glfwKey = GLFW_KEY_KP_ADD; break;
                case Key::Numpad4:      glfwKey = GLFW_KEY_KP_4; break;
                case Key::Numpad5:      glfwKey = GLFW_KEY_KP_5; break;
                case Key::Numpad6:      glfwKey = GLFW_KEY_KP_6; break;
                case Key::Numpad1:      glfwKey = GLFW_KEY_KP_1; break;
                case Key::Numpad2:      glfwKey = GLFW_KEY_KP_2; break;
                case Key::Numpad3:      glfwKey = GLFW_KEY_KP_3; break;
                case Key::NumpadEnter:  glfwKey = GLFW_KEY_KP_ENTER; break;
                case Key::Numpad0:      glfwKey = GLFW_KEY_KP_0; break;
                case Key::NumpadDecimal: glfwKey = GLFW_KEY_KP_DECIMAL; break;

                default:
                    return false; // Unknown key
            }

            return glfwGetKey(window, glfwKey) == GLFW_PRESS;
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
                std::cout << "Error: Failed to compile vertex shader! (" << infoLog << ")\n";
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
                std::cout << "Error: Failed to compile fragment shader! (" << infoLog << ")\n";
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
                std::cout << "Error: Failed to link shader program! (" << infoLog << ")\n";
            }
            glValidateProgram(id);
            // check validate status
            glGetProgramiv(id, GL_VALIDATE_STATUS, &success);
            if (!success) {
                char infoLog[1024];
                glGetProgramInfoLog(id, 1024, nullptr, infoLog);
                std::cout << "Error: Failed to validate shader program! (" << infoLog << ")\n";
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
    namespace gl {
        void clear(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 0.0f) {
            glClearColor(r, g, b, a);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }

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
    using namespace ph;
    
    constexpr int WIDTH = 1280;
    constexpr int HEIGHT = 720;
    const Window window{WIDTH, HEIGHT};


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
    const Texture brickTexture{"resources/textures/test.jpg"};
    const Texture lampTexture{"resources/textures/lamp.png"};

    // SHADERS
    // brick shader
    const Shader brickShader{"resources/shaders/basic.vert", "resources/shaders/basic.frag"};

    gl::bind(brickShader);
    // set which texture unit to use in the shader. You must use the shader program before
    // setting any uniforms in the shader.
    gl::bind(brickTexture);
    gl::setUniform(brickShader, "uTexture", 0);
    gl::setUniform(brickShader, "uLamp.color", glm::vec3(1.0f, 1.0f, 1.0f));
    gl::setUniform(brickShader, "uLamp.attenuation", glm::vec3(1.0f, 0.0f, 0.0075f));

    // lighting object (lamp) shader
    const Shader lampShader{"resources/shaders/basic.vert", "resources/shaders/lamp.frag"};
    gl::bind(lampShader);
    gl::bind(lampTexture);
    gl::setUniform(lampShader, "uTexture", 0);
    const auto M = glm::scale(glm::mat4(1.0f), {0.5f, 0.5f, 0.5f});
    gl::setUniform(lampShader, "uModel", M);


    // TRANSFORMATION DATA
    // camera
    constexpr float h = 8.0f;
    const glm::vec3 cameraPos{0.0f, 0.0f, h};
    const glm::vec3 cameraTarget{0.0f, 0.0f, 0.0f};
    Camera camera{cameraPos, cameraTarget};

    const auto projection = glm::perspective(glm::radians(45.0f), WIDTH/static_cast<float>(HEIGHT), 0.1f, 100.0f);
    gl::bind(brickShader);
    gl::setUniform(brickShader, "uProjection", projection);
    gl::bind(lampShader);
    gl::setUniform(lampShader, "uProjection", projection);

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
        if (window.isKeyPressed(Key::Escape))
            window.setShouldClose(true);

        // UPDATE
        const auto currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        if (window.isKeyPressed(Key::W))
            camera.position += cameraSpeed * deltaTime * xHat;
        if (window.isKeyPressed(Key::S))
            camera.position -= cameraSpeed * deltaTime * xHat;
        if (window.isKeyPressed(Key::A))
            camera.position += cameraSpeed * deltaTime * yHat;
        if (window.isKeyPressed(Key::D))
            camera.position -= cameraSpeed * deltaTime * yHat;

        constexpr float lampR = 10.0f;
        const auto t = currentFrame;
        const glm::vec3 lampPosition{lampR * cos(t), lampR * sin(t), 2.0*cos(5.0*t)};

        // RENDER
        gl::clear(0.0f, 0.05f, 0.1f, 1.0f);

        // The data flow for rendering is as follows:
        //  1)  Bind textures
        //  2)  Specify the shader program to use
        //  3)  Bind the vertex array object (i.e. pass the vertex data
        //      to the GPU)
        //  4)  Draw the object to the screen.

        // bricks
        gl::bind(brickTexture);
        gl::bind(brickShader);
        glBindVertexArray(VAO);

        const auto theta = static_cast<float>(glfwGetTime());
        const auto view = camera.viewMatrix();
        gl::setUniform(brickShader, "uView", view);
        gl::setUniform(brickShader, "uLamp.position", lampPosition);

        for (auto v : positions) {
            auto model = glm::mat4(1.0f);
            // model = glm::rotate(model, theta, glm::normalize(v));
            model = glm::translate(model, v);

            gl::setUniform(brickShader, "uModel", model);
            // parameters: primitive type, number of vertices, type of indices, ...
            glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices)/sizeof(float));
        }

        // lamp
        gl::bind(lampTexture);
        gl::bind(lampShader);
        gl::setUniform(lampShader, "uView", view);

        auto lampModel = glm::mat4(1.0f);
        lampModel = glm::scale(lampModel, {0.5f, 0.5f, 0.5f});
        lampModel = glm::translate(lampModel, lampPosition);
        gl::setUniform(lampShader, "uModel", lampModel);

        // use same VAO as before (cube)
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices)/sizeof(float));

        window.swapBuffers();
        Window::pollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    return EXIT_SUCCESS;
}