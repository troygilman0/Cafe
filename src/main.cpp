#include "Cafe.h"

void initEngine(Cafe::EngineState* engine) {
    Cafe::init(engine);

    UUID textureId = Asset::loadTextureAsset(engine->assetPack, "resources/models/tree/tree.png");
    UUID shaderId = Asset::loadShaderAsset(engine->assetPack, "resources/shaders/mainVertex.glsl", "resources/shaders/mainFragment.glsl");
    UUID materialId = Asset::loadMaterialAsset(engine->assetPack, shaderId, textureId, 0);
    UUID meshId = Asset::loadMeshAsset(engine->assetPack, "resources/models/tree/tree.obj");

    {   // Camera
        UUID entityId = ECS::createEntity(engine->ecs);
        ECS::addSpatial3dComponentToEntity(engine->ecs, entityId, {0, -2, -5}, {0, 0, 0}, 1);
        ECS::addCameraComponentToEntity(engine->ecs, entityId, 0.0f, 0.0f);
    }

    {   // Light
        UUID entityId = ECS::createEntity(engine->ecs);
        ECS::addSpatial3dComponentToEntity(engine->ecs, entityId, {100.0f, 100.0f, 100.0f}, {0, 0, 0}, 1);
        ECS::addLightComponentToEntity(engine->ecs, entityId, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 0.0f});
    }

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 100; j++) {
            UUID entityId = ECS::createEntity(engine->ecs);
            ECS::addSpatial3dComponentToEntity(engine->ecs, entityId, {5.0f * i, 0, 5.0f*j}, {0, 0, 0}, 1);
            ECS::addRenderable3dComponentToEntity(engine->ecs, entityId, meshId, materialId, 0);
        }
    }
}

void handleCameraFirstPerson(ECS::EntityComponentSystem& ecs, const Input::InputState& input, f32 lastFrameTimeMs) {
    UUID cameraEntityId = 0;
    if (!ecs.activeComponents[ECS::COMPONENT_TYPE_SPATIAL_3D][cameraEntityId]) return;
    ECS::Component& spatial3d = ecs.components[ECS::COMPONENT_TYPE_SPATIAL_3D][cameraEntityId];
    const ECS::ComponentInfo& spatial3dInfo = ecs.componentTypes[ECS::COMPONENT_TYPE_SPATIAL_3D];

    Vector3f position = ECS::getField_Vector3f(spatial3d, spatial3dInfo, ECS::Spatial3d::FIELD_INDEX_POSITION);
    Vector3f rotation = ECS::getField_Vector3f(spatial3d, spatial3dInfo, ECS::Spatial3d::FIELD_INDEX_ROTATION);

    f32 moveSpeed = 0.01f * lastFrameTimeMs;
    f32 mouseSensitivity = 0.2f;
    f32 verticalViewRange = 90.0f;

    f32 x = (f32) sin((180 - rotation.y) * M_PI / 180.0f) * moveSpeed;
    f32 z = (f32) cos((180 - rotation.y) * M_PI / 180.0f) * moveSpeed;

    if (input.keys[GLFW_KEY_W]) {
        position.x += -x;
        position.z += -z;
    }
    if (input.keys[GLFW_KEY_S]) {
        position.x += x;
        position.z += z;
    }
    if (input.keys[GLFW_KEY_A]) {
        position.x += -z;
        position.z += x;
    }
    if (input.keys[GLFW_KEY_D]) {
        position.x += z;
        position.z += -x;
    }
    if (input.keys[GLFW_KEY_SPACE]) {
        position.y -= moveSpeed;
    }
    if (input.keys[GLFW_KEY_LEFT_SHIFT]) {
        position.y += moveSpeed;
    }

    f32 dx = (f32) (input.mouseX - input.oldMouseX);
    f32 dy = (f32) (input.mouseY - input.oldMouseY);

    f32 newRotationX = rotation.x - dy * mouseSensitivity;
    if (newRotationX > verticalViewRange || newRotationX < - verticalViewRange) {
        dy = 0;
    }

    rotation.x += dy * mouseSensitivity;
    rotation.y += dx * mouseSensitivity;
    rotation.x = (f32) fmod(rotation.x, 360.0f);
    rotation.y = (f32) fmod(rotation.y, 360.0f);

    ECS::setField_Vector3f(spatial3d, spatial3dInfo, ECS::Spatial3d::FIELD_INDEX_POSITION, position);
    ECS::setField_Vector3f(spatial3d, spatial3dInfo, ECS::Spatial3d::FIELD_INDEX_ROTATION, rotation);
}

void gameLoop(Cafe::EngineState* engine) {
    Render::Window& window = engine->renderState.window;
    Input::InputState& input = engine->input;
    while (!Render::shouldCloseWindow(window)) {
        Render::updateWindow(window);
        if (input.keys[GLFW_KEY_ESCAPE]) {
            Render::closeWindow(window);
            return;
        }
        if (input.mouseButtons[GLFW_MOUSE_BUTTON_LEFT]) {
            glfwSetInputMode(window.glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
        handleCameraFirstPerson(engine->ecs, input, window.lastFrameTimeMs);
        Render::render(engine->renderState, engine->assetPack, engine->ecs);
        Input::updateInputState(input);
    }
    Render::closeWindow(window);
}

int main(int argc, char* argv[]) {
    auto* engine = new Cafe::EngineState();
    initEngine(engine);
    gameLoop(engine);
    return 0;
}