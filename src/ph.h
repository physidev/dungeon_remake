#pragma once

#include <string>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace ph {
    namespace input {
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
    }
    class Window {
        GLFWwindow* window;

    public:
        Window(int width, int height);
        ~Window();

        bool isOpen() const;
        void setShouldClose(bool val) const;
        bool isKeyPressed(input::Key key) const;
        static void pollEvents();
        void swapBuffers() const;
    };

    class Texture {
        GLuint id{0};

    public:
        explicit Texture(const std::string& imagePath);
        ~Texture();

        GLuint getID() const;
    };
    class Shader {
        const GLuint id = glCreateProgram();

    public:
        Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
        ~Shader();

        GLuint getID() const;
    };
    // Vertex array objects remember the precise binding and unbinding order
    // of the vertex buffer and element buffer objects. It then suffices to
    // bind the vertex array object when making render calls, instead of binding
    // each buffer and specifying the attribute pointers.
    //
    // For now, we only store floats in our vertex data.
    class VertexArray {
        GLuint id{0};
        GLuint vbo_id{0};
        size_t count{0};

    public:
        VertexArray(const float* vertices, size_t count, const std::vector<int>& attributeSizes);
        ~VertexArray();

        GLuint getID() const;
        size_t getCount() const;
    };
    namespace gl {
        void clear(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 0.0f);

        void bind(const Texture& texture);
        void bind(const Shader& shader);
        void bind(const VertexArray& vertexArray);

        void setUniform(const Shader& shader, const std::string& name, int value);
        void setUniform(const Shader& shader, const std::string& name, const glm::mat4& value);
        void setUniform(const Shader& shader, const std::string& name, const glm::vec3& value);

        void draw(const VertexArray& vertexArray);
    }

    class Camera {
    public:
        glm::vec3 position;
        glm::vec3 target;
        glm::vec3 up{0.0f, 1.0f, 0.0f};

        Camera(const glm::vec3& position, const glm::vec3& target);
        glm::mat4 viewMatrix() const;
    };
}