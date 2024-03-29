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
#include "SkyboxRenderer.h"
#include "FlowManager.h"

class CollectibleManager;
class DirectionalLight;

enum class ViewMode
{
	kDefault,
	kDeferredOnly,
	kPosition,
	kNormal,
	kAlbedo,
	kSpec,
	kEmissive,
	kShine,
};

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
	void setViewMode(ViewMode mode);
	void setDayMode(bool day = true);

private:
	GameWindow* m_window;
	AssetManager m_assets;
    ShaderProgram* m_shaderMain;
	ShaderProgram* m_shaderText;
	ShaderProgram* m_shaderGeometryPass;
	ShaderProgram* m_shaderLightingPass;
	ShaderProgram* m_shaderVisualization;
    GLuint vaoID, vboID, eboID;
	GLuint vertexCount = 0;
	float currentAspect;
	std::shared_ptr<DroneController> droneController;
	std::shared_ptr<DroneAnimator> droneAnimator;
	std::shared_ptr<CollectibleManager> collectibleManager;
	std::vector<std::shared_ptr<Renderable>> renderables;
	std::shared_ptr<FlowManager> flowManager;
	bool dayMode = true;

	// Scene Objects
	std::shared_ptr<Renderable> r_ground;
	std::shared_ptr<Renderable> r_drone;
	std::vector<std::shared_ptr<PointLight>> pointLights;
	std::shared_ptr<SpotLight> spotLight;
	std::shared_ptr<DirectionalLight> directionalLight;
	std::shared_ptr<Camera> followCamera;
	std::shared_ptr<Camera> freeCamera;
	std::shared_ptr<Renderable> r_cake;
	std::shared_ptr<TextRenderer> textRenderer;
	std::shared_ptr<SkyboxRenderer> skyboxRenderer;
	std::vector<std::shared_ptr<Renderable>> buildings;
	glm::vec4 ambientLight;
	bool currentCameraFree = false;
	std::shared_ptr<Renderable> addObject(std::string path, std::string texDiff, std::string texSpec, std::string texEmss, float uvScale = 1.0f, bool reverseWinding = false);
	std::shared_ptr<Renderable> addDrone(std::string path, std::shared_ptr<Renderable>& parent, bool reverseWinding, std::shared_ptr<Renderable>& out_body);
	void instantiateApartment(int type, glm::vec3 pos, float rotation);
	std::shared_ptr<Renderable> addApartment(int type);

	// Deferred shading
	unsigned int gBuffer;
	unsigned int gPosition;
	unsigned int gNormal;
	unsigned int gAlbedo;
	unsigned int gEmissive;
	unsigned int gSpecShine;
	unsigned int gDepth;
	GLuint quadVao;
	ViewMode viewMode = ViewMode::kDefault;
	unsigned int viewModeTextId;
};

