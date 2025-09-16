#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ph.h"

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
    const VertexArray cubeArray{vertices, sizeof(vertices)/sizeof(float), {3, 3, 2}};

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
        if (window.isKeyPressed(input::Key::Escape))
            window.setShouldClose(true);

        // UPDATE
        const auto currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        if (window.isKeyPressed(input::Key::W))
            camera.position += cameraSpeed * deltaTime * xHat;
        if (window.isKeyPressed(input::Key::S))
            camera.position -= cameraSpeed * deltaTime * xHat;
        if (window.isKeyPressed(input::Key::A))
            camera.position += cameraSpeed * deltaTime * yHat;
        if (window.isKeyPressed(input::Key::D))
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
        gl::bind(cubeArray);

        const auto theta = static_cast<float>(glfwGetTime());
        const auto view = camera.viewMatrix();
        gl::setUniform(brickShader, "uView", view);
        gl::setUniform(brickShader, "uLamp.position", lampPosition);

        for (auto v : positions) {
            auto model = glm::mat4(1.0f);
            // model = glm::rotate(model, theta, glm::normalize(v));
            model = glm::translate(model, v);

            gl::setUniform(brickShader, "uModel", model);
            gl::draw(cubeArray);
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
        gl::draw(cubeArray);

        window.swapBuffers();
        Window::pollEvents();
    }
    return EXIT_SUCCESS;
}