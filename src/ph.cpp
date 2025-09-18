#include "ph.h"

#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// class ph::Window
ph::Window::Window(const int width, const int height) {
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
ph::Window::~Window() {
    glfwTerminate();
}
bool ph::Window::isOpen() const {
    return glfwWindowShouldClose(window) == false;
}
void ph::Window::setShouldClose(const bool val) const {
    glfwSetWindowShouldClose(window, val);
}
bool ph::Window::isKeyPressed(const input::Key key) const {
    int glfwKey;
    switch (key) {
    case input::Key::Escape: glfwKey = GLFW_KEY_ESCAPE;
        break;
    case input::Key::F1: glfwKey = GLFW_KEY_F1;
        break;
    case input::Key::F2: glfwKey = GLFW_KEY_F2;
        break;
    case input::Key::F3: glfwKey = GLFW_KEY_F3;
        break;
    case input::Key::F4: glfwKey = GLFW_KEY_F4;
        break;
    case input::Key::F5: glfwKey = GLFW_KEY_F5;
        break;
    case input::Key::F6: glfwKey = GLFW_KEY_F6;
        break;
    case input::Key::F7: glfwKey = GLFW_KEY_F7;
        break;
    case input::Key::F8: glfwKey = GLFW_KEY_F8;
        break;
    case input::Key::F9: glfwKey = GLFW_KEY_F9;
        break;
    case input::Key::F10: glfwKey = GLFW_KEY_F10;
        break;
    case input::Key::F11: glfwKey = GLFW_KEY_F11;
        break;
    case input::Key::F12: glfwKey = GLFW_KEY_F12;
        break;

    case input::Key::Grave: glfwKey = GLFW_KEY_GRAVE_ACCENT;
        break;
    case input::Key::One: glfwKey = GLFW_KEY_1;
        break;
    case input::Key::Two: glfwKey = GLFW_KEY_2;
        break;
    case input::Key::Three: glfwKey = GLFW_KEY_3;
        break;
    case input::Key::Four: glfwKey = GLFW_KEY_4;
        break;
    case input::Key::Five: glfwKey = GLFW_KEY_5;
        break;
    case input::Key::Six: glfwKey = GLFW_KEY_6;
        break;
    case input::Key::Seven: glfwKey = GLFW_KEY_7;
        break;
    case input::Key::Eight: glfwKey = GLFW_KEY_8;
        break;
    case input::Key::Nine: glfwKey = GLFW_KEY_9;
        break;
    case input::Key::Zero: glfwKey = GLFW_KEY_0;
        break;
    case input::Key::Minus: glfwKey = GLFW_KEY_MINUS;
        break;
    case input::Key::Equal: glfwKey = GLFW_KEY_EQUAL;
        break;
    case input::Key::Backspace: glfwKey = GLFW_KEY_BACKSPACE;
        break;

    case input::Key::Tab: glfwKey = GLFW_KEY_TAB;
        break;
    case input::Key::Q: glfwKey = GLFW_KEY_Q;
        break;
    case input::Key::W: glfwKey = GLFW_KEY_W;
        break;
    case input::Key::E: glfwKey = GLFW_KEY_E;
        break;
    case input::Key::R: glfwKey = GLFW_KEY_R;
        break;
    case input::Key::T: glfwKey = GLFW_KEY_T;
        break;
    case input::Key::Y: glfwKey = GLFW_KEY_Y;
        break;
    case input::Key::U: glfwKey = GLFW_KEY_U;
        break;
    case input::Key::I: glfwKey = GLFW_KEY_I;
        break;
    case input::Key::O: glfwKey = GLFW_KEY_O;
        break;
    case input::Key::P: glfwKey = GLFW_KEY_P;
        break;
    case input::Key::LeftBracket: glfwKey = GLFW_KEY_LEFT_BRACKET;
        break;
    case input::Key::RightBracket: glfwKey = GLFW_KEY_RIGHT_BRACKET;
        break;
    case input::Key::Backslash: glfwKey = GLFW_KEY_BACKSLASH;
        break;

    case input::Key::CapsLock: glfwKey = GLFW_KEY_CAPS_LOCK;
        break;
    case input::Key::A: glfwKey = GLFW_KEY_A;
        break;
    case input::Key::S: glfwKey = GLFW_KEY_S;
        break;
    case input::Key::D: glfwKey = GLFW_KEY_D;
        break;
    case input::Key::F: glfwKey = GLFW_KEY_F;
        break;
    case input::Key::G: glfwKey = GLFW_KEY_G;
        break;
    case input::Key::H: glfwKey = GLFW_KEY_H;
        break;
    case input::Key::J: glfwKey = GLFW_KEY_J;
        break;
    case input::Key::K: glfwKey = GLFW_KEY_K;
        break;
    case input::Key::L: glfwKey = GLFW_KEY_L;
        break;
    case input::Key::Semicolon: glfwKey = GLFW_KEY_SEMICOLON;
        break;
    case input::Key::Apostrophe: glfwKey = GLFW_KEY_APOSTROPHE;
        break;
    case input::Key::Enter: glfwKey = GLFW_KEY_ENTER;
        break;

    case input::Key::LeftShift: glfwKey = GLFW_KEY_LEFT_SHIFT;
        break;
    case input::Key::Z: glfwKey = GLFW_KEY_Z;
        break;
    case input::Key::X: glfwKey = GLFW_KEY_X;
        break;
    case input::Key::C: glfwKey = GLFW_KEY_C;
        break;
    case input::Key::V: glfwKey = GLFW_KEY_V;
        break;
    case input::Key::B: glfwKey = GLFW_KEY_B;
        break;
    case input::Key::N: glfwKey = GLFW_KEY_N;
        break;
    case input::Key::M: glfwKey = GLFW_KEY_M;
        break;
    case input::Key::Comma: glfwKey = GLFW_KEY_COMMA;
        break;
    case input::Key::Period: glfwKey = GLFW_KEY_PERIOD;
        break;
    case input::Key::Slash: glfwKey = GLFW_KEY_SLASH;
        break;
    case input::Key::RightShift: glfwKey = GLFW_KEY_RIGHT_SHIFT;
        break;

    case input::Key::LeftCtrl: glfwKey = GLFW_KEY_LEFT_CONTROL;
        break;
    case input::Key::LeftSuper: glfwKey = GLFW_KEY_LEFT_SUPER;
        break;
    case input::Key::LeftAlt: glfwKey = GLFW_KEY_LEFT_ALT;
        break;
    case input::Key::Space: glfwKey = GLFW_KEY_SPACE;
        break;
    case input::Key::RightAlt: glfwKey = GLFW_KEY_RIGHT_ALT;
        break;
    case input::Key::RightSuper: glfwKey = GLFW_KEY_RIGHT_SUPER;
        break;
    case input::Key::Menu: glfwKey = GLFW_KEY_MENU;
        break;
    case input::Key::RightCtrl: glfwKey = GLFW_KEY_RIGHT_CONTROL;
        break;

    case input::Key::Insert: glfwKey = GLFW_KEY_INSERT;
        break;
    case input::Key::Delete: glfwKey = GLFW_KEY_DELETE;
        break;
    case input::Key::Home: glfwKey = GLFW_KEY_HOME;
        break;
    case input::Key::End: glfwKey = GLFW_KEY_END;
        break;
    case input::Key::PageUp: glfwKey = GLFW_KEY_PAGE_UP;
        break;
    case input::Key::PageDown: glfwKey = GLFW_KEY_PAGE_DOWN;
        break;

    case input::Key::PrintScreen: glfwKey = GLFW_KEY_PRINT_SCREEN;
        break;
    case input::Key::ScrollLock: glfwKey = GLFW_KEY_SCROLL_LOCK;
        break;
    case input::Key::Pause: glfwKey = GLFW_KEY_PAUSE;
        break;

    case input::Key::Up: glfwKey = GLFW_KEY_UP;
        break;
    case input::Key::Down: glfwKey = GLFW_KEY_DOWN;
        break;
    case input::Key::Left: glfwKey = GLFW_KEY_LEFT;
        break;
    case input::Key::Right: glfwKey = GLFW_KEY_RIGHT;
        break;

    case input::Key::NumLock: glfwKey = GLFW_KEY_NUM_LOCK;
        break;
    case input::Key::NumpadDivide: glfwKey = GLFW_KEY_KP_DIVIDE;
        break;
    case input::Key::NumpadMultiply: glfwKey = GLFW_KEY_KP_MULTIPLY;
        break;
    case input::Key::NumpadSubtract: glfwKey = GLFW_KEY_KP_SUBTRACT;
        break;
    case input::Key::Numpad7: glfwKey = GLFW_KEY_KP_7;
        break;
    case input::Key::Numpad8: glfwKey = GLFW_KEY_KP_8;
        break;
    case input::Key::Numpad9: glfwKey = GLFW_KEY_KP_9;
        break;
    case input::Key::NumpadAdd: glfwKey = GLFW_KEY_KP_ADD;
        break;
    case input::Key::Numpad4: glfwKey = GLFW_KEY_KP_4;
        break;
    case input::Key::Numpad5: glfwKey = GLFW_KEY_KP_5;
        break;
    case input::Key::Numpad6: glfwKey = GLFW_KEY_KP_6;
        break;
    case input::Key::Numpad1: glfwKey = GLFW_KEY_KP_1;
        break;
    case input::Key::Numpad2: glfwKey = GLFW_KEY_KP_2;
        break;
    case input::Key::Numpad3: glfwKey = GLFW_KEY_KP_3;
        break;
    case input::Key::NumpadEnter: glfwKey = GLFW_KEY_KP_ENTER;
        break;
    case input::Key::Numpad0: glfwKey = GLFW_KEY_KP_0;
        break;
    case input::Key::NumpadDecimal: glfwKey = GLFW_KEY_KP_DECIMAL;
        break;

    default:
        return false; // Unknown key
    }

    return glfwGetKey(window, glfwKey) == GLFW_PRESS;
}
void ph::Window::pollEvents() {
    glfwPollEvents();
}
void ph::Window::swapBuffers() const {
    glfwSwapBuffers(window);
}

// class ph::Shader
ph::Shader::Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath) {
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
    const char *vs_cstr = vs_str.c_str(), *fs_cstr = fs_str.c_str();

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
ph::Shader::~Shader() {
    glDeleteProgram(id);
}
GLuint ph::Shader::getID() const {
    return id;
}

// class ph::Texture
ph::Texture::Texture(const std::string& imagePath) {
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
        GL_TEXTURE_2D, // texture target
        0, // mipmap level
        format, // texture data format
        width, // width of image (px)
        height, // height of image (px)
        0, // legacy stuff
        format, // image data interpretation
        GL_UNSIGNED_BYTE, // image data format
        data
    );
    glGenerateMipmap(GL_TEXTURE_2D);
}
ph::Texture::~Texture() {
    glDeleteTextures(1, &id);
}
GLuint ph::Texture::getID() const {
    return id;
}


// class ph::VertexArray
ph::VertexArray::VertexArray(const float* vertices, const size_t count, const std::vector<int>& attributeSizes) : count(
    count) {
    glGenVertexArrays(1, &id);
    glGenBuffers(1, &vbo_id);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
    glBufferData(GL_ARRAY_BUFFER, count * sizeof(float), vertices, GL_STATIC_DRAW);

    glBindVertexArray(id);

    int stride = 0;
    for (const auto s : attributeSizes) {
        stride += s * sizeof(float);
    }
    size_t offset = 0;
    for (size_t i = 0; i < attributeSizes.size(); i++) {
        // parameters: which attribute, size of vertex attribute, data type, normalize?, stride, offset (void*).
        glVertexAttribPointer(i, attributeSizes[i], GL_FLOAT, GL_FALSE, stride,
                              reinterpret_cast<void*>(offset));
        glEnableVertexAttribArray(i);
        offset += attributeSizes[i] * sizeof(float);
    }
}
ph::VertexArray::~VertexArray() {
    glDeleteVertexArrays(1, &id);
    glDeleteBuffers(1, &vbo_id);
}
GLuint ph::VertexArray::getID() const {
    return id;
}
size_t ph::VertexArray::getCount() const {
    return count;
}

// namespace ph::gl
void ph::gl::clear(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void ph::gl::bind(const Texture& texture) {
    glBindTexture(GL_TEXTURE_2D, texture.getID());
}
void ph::gl::bind(const Shader& shader) {
    glUseProgram(shader.getID());
}
void ph::gl::bind(const VertexArray& vertexArray) {
    glBindVertexArray(vertexArray.getID());
}
void ph::gl::setUniform(const Shader& shader, const std::string& name, const int value) {
    glUniform1i(glGetUniformLocation(shader.getID(), name.c_str()), value);
}
void ph::gl::setUniform(const Shader& shader, const std::string& name, const glm::mat4& value) {
    const GLint location = glGetUniformLocation(shader.getID(), name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}
void ph::gl::setUniform(const Shader& shader, const std::string& name, const glm::vec3& value) {
    glUniform3f(glGetUniformLocation(shader.getID(), name.c_str()), value.x, value.y, value.z);
}
void ph::gl::draw(const VertexArray& vertexArray) {
    glDrawArrays(GL_TRIANGLES, 0, vertexArray.getCount());
}

// class ph::Camera
ph::Camera::Camera(const glm::vec3& position, const glm::vec3& target) : position(position), target(target) {}
glm::mat4 ph::Camera::viewMatrix() const {
    return glm::lookAt(position, target, up);
};
