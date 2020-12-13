#pragma once

#include "GameWindow.h"
#include <ShaderProgram.h>
#include <memory>
#include <AssetManager.h>
#include <vector>
#include "Renderable.h"
#include <memory>

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
	GameWindow* m_window;
	AssetManager m_assets;
    ShaderProgram* m_shader;
    GLuint vaoID, vboID, eboID;
	GLuint vertexCount = 0;
	float currentAspect;

	std::vector<std::shared_ptr<Renderable>> renderables;
	float timecounter=0.0f;

	// Scene Objects
	std::shared_ptr<Renderable> r_ground;
	std::shared_ptr<Renderable> r_spheres;
	std::shared_ptr<Camera> followCamera;
	std::shared_ptr<Camera> freeCamera;
	bool currentCameraFree = false;
	std::shared_ptr<Renderable> addObject(std::string path, bool reverseWinding = false);
};

