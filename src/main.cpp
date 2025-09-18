#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ph.h"

enum class Tile {
    DIRT, GRAY_BRICK, WALL_BRICK, WOOD, GOAL
};

// returns the posCoords for the specified tile
std::vector<float> getTilePosCoords(const glm::vec2& tilePos) {
    return {
        tilePos.x-0.5f,  tilePos.y-0.5f,  0.5f,   // top left
        tilePos.x+0.5f,  tilePos.y-0.5f,  0.5f,   // top right
        tilePos.x+0.5f,  tilePos.y+0.5f,  0.5f,   // bottom right
        tilePos.x+0.5f,  tilePos.y+0.5f,  0.5f,   // bottom right
        tilePos.x-0.5f,  tilePos.y+0.5f,  0.5f,   // bottom left
        tilePos.x-0.5f,  tilePos.y-0.5f,  0.5f,   // top left
    };
}

// returns the normalCoords for a tile
std::vector<float> getTileNormalCoords() {
    return {
        0.0f,  0.0f,  1.0f,
        0.0f,  0.0f,  1.0f,
        0.0f,  0.0f,  1.0f,
        0.0f,  0.0f,  1.0f,
        0.0f,  0.0f,  1.0f,
        0.0f,  0.0f,  1.0f,
    };
}

// returns the texCoords for the specified tile
std::vector<float> getTileTexCoords(const Tile t) {
    const glm::vec2 numTiles{16, 16};                   // number of tiles in the tilemap
    const glm::vec2 tSize{16.0f/256.0f, 16.0f/256.0f};  // tile texture width as a percent of tilemap width
    glm::vec2 tPos;                                         // position of top left

    // recall: texCoords are (0,0) at bottom left and (1,1) at top right of texture
    switch (t) {
    default:
    case Tile::DIRT:        tPos = glm::vec2{0, numTiles.y-1} * tSize;     break;
    case Tile::GRAY_BRICK:  tPos = glm::vec2{1, numTiles.y-1} * tSize;     break;
    case Tile::WALL_BRICK:  tPos = glm::vec2{2, numTiles.y-1} * tSize;     break;
    case Tile::WOOD:        tPos = glm::vec2{3, numTiles.y-1} * tSize;     break;
    case Tile::GOAL:        tPos = glm::vec2{4, numTiles.y-1} * tSize;     break;
    };

    return {
        tPos.x+0.0f,      tPos.y+0.0f,
        tPos.x+tSize.x,   tPos.y+0.0f,
        tPos.x+tSize.x,   tPos.y+tSize.y,
        tPos.x+tSize.x,   tPos.y+tSize.y,
        tPos.x+0.0f,      tPos.y+tSize.y,
        tPos.x+0.0f,      tPos.y+0.0f,
    };
}

ph::VertexArray createVA(const std::vector<float>& positions, const std::vector<float>& normals, const std::vector<float>& texCoords) {
    constexpr int dimPosition = 3;
    constexpr int dimNormal = 3;
    constexpr int dimTexCoords = 2;
    constexpr int dimVertex = dimPosition + dimNormal + dimTexCoords;

    const auto count = positions.size() / dimPosition;

    std::vector<float> vertices;
    vertices.reserve(dimVertex * count);
    for (size_t i=0; i < count; ++i) {
        // position (posCoords)
        vertices.push_back(positions[i*dimPosition + 0]);
        vertices.push_back(positions[i*dimPosition + 1]);
        vertices.push_back(positions[i*dimPosition + 2]);

        // normal (normalCoords)
        vertices.push_back(normals[i*dimNormal + 0]);
        vertices.push_back(normals[i*dimNormal + 1]);
        vertices.push_back(normals[i*dimNormal + 2]);

        // texCoords
        vertices.push_back(texCoords[i*dimTexCoords + 0]);
        vertices.push_back(texCoords[i*dimTexCoords + 1]);
    }

    return ph::VertexArray(vertices.data(), count*dimVertex, {dimPosition, dimNormal, dimTexCoords});
}

int main() {
    using namespace ph;

    constexpr int WIDTH = 1280;
    constexpr int HEIGHT = 720;
    const Window window{WIDTH, HEIGHT};

    //  LEVEL MODEL INITIALIZATION
    //-------------------------------
    // LEVEL DATA
    constexpr int MAP_SIZE_X = 100;
    constexpr int MAP_SIZE_Y = 100;
    constexpr int NUM_TILES = MAP_SIZE_X * MAP_SIZE_Y;

    Tile map[NUM_TILES];
    for (auto& t : map) {
        t = Tile::DIRT;
    }
    map[0 * MAP_SIZE_X + 1] = Tile::GRAY_BRICK; // (1, 0)
    map[1 * MAP_SIZE_X + 0] = Tile::WALL_BRICK; // (0, 1)

    // LEVEL GEOMETRY DATA
    // level normals
    std::vector<float> levelNormals;
    levelNormals.reserve(NUM_TILES * 6 * 3);    // 6 vertices per tile, 3 normalCoords per vertex

    const auto tileNormals = getTileNormalCoords();
    for (int i=0; i<NUM_TILES; ++i) {
        levelNormals.insert(levelNormals.end(), tileNormals.begin(), tileNormals.end());
    }

    // level positions and texCoords
    std::vector<float> levelPositions, levelTexCoords;
    levelPositions.reserve(NUM_TILES * 6 * 3);  // 6 vertices per tile, 3 posCoords per vertex
    levelTexCoords.reserve(NUM_TILES * 6 * 2);  // 6 vertices per tile, 2 texCoords per vertex
    for (int y=0; y<MAP_SIZE_Y; ++y) {
        for (int x=0; x<MAP_SIZE_X; ++x) {
            auto tilePositions = getTilePosCoords({x, y});
            levelPositions.insert(levelPositions.end(), tilePositions.begin(), tilePositions.end());

            auto tileTexCoords = getTileTexCoords(map[y * MAP_SIZE_X + x]);
            levelTexCoords.insert(levelTexCoords.end(), tileTexCoords.begin(), tileTexCoords.end());
        }
    }

    const VertexArray levelVA = createVA(levelPositions, levelNormals, levelTexCoords);
    const Texture levelTexture{"resources/textures/tilemap.png"};
    const Shader levelShader{"resources/shaders/basic.vert", "resources/shaders/basic.frag"};

    //  LAMP MODEL INITIALIZATION
    //-------------------------------
    // LAMP GEOMETRY DATA
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

    const VertexArray lampVA{vertices, sizeof(vertices)/sizeof(float), {3, 3, 2}};
    const Texture lampTexture{"resources/textures/lamp.png"};
    const Shader lampShader{"resources/shaders/basic.vert", "resources/shaders/lamp.frag"};
    //-------------------------------

    // SHADER DATA
    gl::bind(levelShader);
    // set which texture unit to use in the shader. You must bind the shader program before
    // setting any uniforms in the shader.
    gl::bind(levelTexture);
    gl::setUniform(levelShader, "uTexture", 0);
    gl::setUniform(levelShader, "uLamp.color", glm::vec3(1.0f, 1.0f, 1.0f));
    gl::setUniform(levelShader, "uLamp.attenuation", glm::vec3(1.0f, 0.0f, 0.0075f));

    // lighting object (lamp) shader
    gl::bind(lampShader);
    gl::bind(lampTexture);
    gl::setUniform(lampShader, "uTexture", 0);
    const auto M = glm::scale(glm::mat4(1.0f), {0.5f, 0.5f, 0.5f});
    gl::setUniform(lampShader, "uModel", M);


    // TRANSFORMATION DATA
    // camera
    constexpr auto h = 8.0f;
    const glm::vec3 cameraPos{0.0f, 0.0f, h};
    const glm::vec3 cameraTarget{0.0f, 0.0f, 0.0f};
    Camera camera{cameraPos, cameraTarget};

    const auto projection = glm::perspective(glm::radians(45.0f), WIDTH/static_cast<float>(HEIGHT), 0.1f, 100.0f);
    gl::bind(levelShader);
    gl::setUniform(levelShader, "uProjection", projection);
    gl::bind(lampShader);
    gl::setUniform(lampShader, "uProjection", projection);

    const glm::vec3 xHat{1.0f, 0.0f, 0.0f};
    const glm::vec3 yHat{0.0f, 1.0f, 0.0f};
    const glm::vec3 zHat{0.0f, 0.0f, 1.0f};

    // PLAYER DATA
    struct Player {
        glm::vec3 acceleration{0.0f, 0.0f, 0.0f};
        glm::vec3 velocity{0.0f, 0.0f, 0.0f};
        glm::vec3 position{0.0f, 0.0f, 0.0f};
    } player;

    //  GAME LOOP
    //-------------------------------
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    while (window.isOpen()) {
        if (window.isKeyPressed(input::Key::Escape))
            window.setShouldClose(true);

        //  UPDATE
        //-------------------------------
        const auto currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // UPDATE PLAYER POSITION
        // handle input
        constexpr float maxSpeed{8.0f};
        constexpr float maxAccel{32.0f};
        constexpr float friction{16.0f};
        if (window.isKeyPressed(input::Key::W)) {
            player.acceleration.y = +maxAccel;
        } else if (window.isKeyPressed(input::Key::S)) {
            player.acceleration.y = -maxAccel;
        } else {
            player.acceleration.y = -friction * player.velocity.y;
        }
        if (window.isKeyPressed(input::Key::D)) {
            player.acceleration.x = +maxAccel;
        } else if (window.isKeyPressed(input::Key::A)) {
            player.acceleration.x = -maxAccel;
        } else {
            player.acceleration.x = -friction * player.velocity.x;
        }

        // integrate acceleration
        player.velocity += deltaTime * player.acceleration;
        if (glm::length(player.velocity) > maxSpeed) {
            player.velocity = maxSpeed * glm::normalize(player.velocity);
        }
        // integrate velocity
        player.position += player.velocity * deltaTime;

        // update camera motion
        constexpr float cameraSpeed = 8.0f;
        if (window.isKeyPressed(input::Key::Space))
            camera.position.z += cameraSpeed * deltaTime;
        if (window.isKeyPressed(input::Key::LeftShift))
            camera.position.z -= cameraSpeed * deltaTime;

        camera.position = {player.position.x, player.position.y, camera.position.z};
        camera.target = player.position + 0.1f * player.velocity;
        const auto lampPosition = glm::vec3{1.0f, 1.0f, 0.25f} * camera.position;

        //  RENDER
        //-------------------------------
        gl::clear(0.0f, 0.05f, 0.1f, 1.0f);

        // The data flow for rendering is as follows:
        //  1)  Bind textures
        //  2)  Bind shader program
        //  5)  Bind the vertex array  (i.e. pass the vertex data to the GPU)
        //  4)  Set shader uniforms (any that changed between last render and now)
        //  6)  Draw the object to the screen.

        // DRAW LEVEL MODEL
        gl::bind(levelTexture);
        gl::bind(levelShader);
        gl::bind(levelVA);

        const auto view = camera.viewMatrix();
        gl::setUniform(levelShader, "uView", view);
        gl::setUniform(levelShader, "uModel", glm::mat4{1.0f});
        gl::setUniform(levelShader, "uLamp.position", lampPosition);

        gl::draw(levelVA);

        // DRAW LAMP
        gl::bind(lampTexture);
        gl::bind(lampShader);
        gl::bind(lampVA);

        gl::setUniform(lampShader, "uView", view);
        auto lampModel = glm::mat4(1.0f);
        lampModel = glm::translate(lampModel, lampPosition);
        gl::setUniform(lampShader, "uModel", lampModel);

        gl::draw(lampVA);
        //-------------------------------

        window.swapBuffers();
        Window::pollEvents();
    }
    return EXIT_SUCCESS;
}