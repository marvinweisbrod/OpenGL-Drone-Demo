#pragma once

#include "GameWindow.h"
#include <ShaderProgram.h>
#include <memory>
#include <AssetManager.h>
#include <vector>
#include "Renderable.h"
#include "DroneController.h"
#include "DroneAnimator.h"
#include "TextRenderer.h"

class CollectibleManager;
class DirectionalLight;

class Scene
{
public:
	Scene(GameWindow* window);
	~Scene();

	bool init();
	void shutdown();
	void render(float dt);
	void update(float dt);
	GameWindow* getWindow();

	void onKey(Key key, Action action, Modifier modifier);
	void onMouseMove(MousePosition mouseposition);
	void onMouseButton(MouseButton button, Action action, Modifier modifier);
	void onMouseScroll(double xscroll, double yscroll);
	void onFrameBufferResize(int width, int height);

private:
	std::shared_ptr<Renderable> instantiateCake(const glm::vec3& position, const glm::quat& rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
		const glm::vec3& scale = glm::vec3(1.0f, 1.0f, 1.0f));
	std::shared_ptr<Renderable> cloneRenderable(Renderable* renderable);

private:
	GameWindow* m_window;
	AssetManager m_assets;
    ShaderProgram* m_shaderMain;
	ShaderProgram* m_shaderText;
    GLuint vaoID, vboID, eboID;
	GLuint vertexCount = 0;
	float currentAspect;
	std::shared_ptr<DroneController> droneController;
	std::shared_ptr<DroneAnimator> droneAnimator;
	std::shared_ptr<CollectibleManager> collectibleManager;
	std::vector<std::shared_ptr<Renderable>> renderables;

	// Scene Objects
	std::shared_ptr<Renderable> r_ground;
	std::shared_ptr<Renderable> r_drone;
	std::shared_ptr<PointLight> pointLight;
	std::shared_ptr<SpotLight> spotLight;
	std::shared_ptr<DirectionalLight> directionalLight;
	std::shared_ptr<Camera> followCamera;
	std::shared_ptr<Camera> freeCamera;
	std::shared_ptr<Renderable> r_cake;
	std::shared_ptr<TextRenderer> textRenderer;
	glm::vec4 ambientLight;
	bool currentCameraFree = false;
	std::shared_ptr<Renderable> addObject(std::string path, std::string texDiff, std::string texSpec, std::string texEmss, float uvScale = 1.0f, bool reverseWinding = false);
	std::shared_ptr<Renderable> addDrone(std::string path, std::shared_ptr<Renderable>& parent, bool reverseWinding = false);
};

