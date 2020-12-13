#include "Scene.h"
#include <AssetManager.h>
#include "OBJLoader.h"
#include <vector>
#include <memory>

Scene::Scene(GameWindow * window) :
	m_window(window)
{
	assert(window != nullptr);

	currentAspect = static_cast<float>(m_window->getFrameBufferWidth()) / static_cast<float>(m_window->getFrameBufferHeight());
}

Scene::~Scene()
{}

bool Scene::init()
{
	try
	{
		//Load shader
		m_assets.addShaderProgram("shader", AssetManager::createShaderProgram("assets/shaders/vertex.glsl", "assets/shaders/fragment.glsl"));
		m_shader = m_assets.getShaderProgram("shader");

		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);
		glCullFace(GL_BACK);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		{
			auto sphere = addObject("assets/models/sphere.obj", false);
			sphere->scale(glm::vec3(0.5f, 0.5f, 0.5f));
			r_spheres = sphere;
			// "ears"
			auto sphere2 = addObject("assets/models/sphere.obj", false);
			auto sphere3 = addObject("assets/models/sphere.obj", false);
			sphere2->scale(glm::vec3(0.6f, 0.6f, 0.6f));
			sphere3->scale(glm::vec3(0.6f, 0.6f, 0.6f));
			sphere2->translate(glm::vec3(-1.0f,1.0f,0.0f));
			sphere3->translate(glm::vec3(1.0f, 1.0f, 0.0f));
			sphere2->setParent(sphere.get());
			sphere3->setParent(sphere.get());
		}
		{
			auto ground = addObject("assets/models/ground.obj", false);
			r_ground = ground;

			ground->scale(glm::vec3(0.05f, 0.05f, 0.05f));
			ground->translate(glm::vec3(0.0f, -1.0f, 0.0f));
			//ground->rotate(glm::angleAxis(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
		}
		{	// camera child of sphere
			followCamera = std::make_shared<Camera>();
			followCamera->setPerspective(glm::radians(60.0f), currentAspect, 0.01f, 100.0f);
			followCamera->translate(glm::vec3(3.0f, 3.0f, -3.5f));
			followCamera->lookat(glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			followCamera->setParent(r_spheres.get());
		}
		{	// camera free moving
			freeCamera = std::make_shared<Camera>();
			freeCamera->setPerspective(glm::radians(60.0f), currentAspect, 0.01f, 100.0f);
			freeCamera->translate(glm::vec3(0.0f, 0.0f, -1.5f));
			freeCamera->lookat(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		}


		//initial opengl state
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        std::cout << "Scene initialization done\n";

        return true;
	}
	catch (std::exception& ex)
	{
	    throw std::logic_error("Scene initialization failed:\n" + std::string(ex.what()) + "\n");
	}
}

void Scene::shutdown()
{

}

void Scene::render(float dt)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_shader->use();
	m_shader->bind(currentCameraFree ? *freeCamera : *followCamera);
	timecounter += dt*0.5f;
	if (timecounter >= 1.0f) timecounter = 0.0f;
	m_shader->setUniform("color", timecounter);

	for(auto& renderable: renderables){
		renderable->render(*m_shader);
	}

}

void Scene::update(float dt)
{
	if (currentCameraFree) { // free cam movement
		glm::vec3 translation(0.0f, 0.0f, 0.0f);
		if (m_window->getInput().getKeyState(Key::W) == KeyState::Pressed)
			translation.z += -1.0f * dt;
		if (m_window->getInput().getKeyState(Key::S) == KeyState::Pressed)
			translation.z += 1.0f * dt;
		if (m_window->getInput().getKeyState(Key::A) == KeyState::Pressed)
			translation.x += -1.0f * dt;
		if (m_window->getInput().getKeyState(Key::D) == KeyState::Pressed)
			translation.x += 1.0f * dt;
		if (m_window->getInput().getKeyState(Key::LeftShift) == KeyState::Pressed)
			translation.y += 1.0f * dt;
		if (m_window->getInput().getKeyState(Key::LeftCtrl) == KeyState::Pressed)
			translation.y += -1.0f * dt;
		freeCamera->translateLocal(translation);
	} 
	else {  // sphere movement
		glm::vec3 translation(0.0f, 0.0f, 0.0f);
		if (m_window->getInput().getKeyState(Key::Up) == KeyState::Pressed)
			translation.y += 1.0f * dt;
		if (m_window->getInput().getKeyState(Key::Down) == KeyState::Pressed)
			translation.y += -1.0f * dt;
		if (m_window->getInput().getKeyState(Key::Left) == KeyState::Pressed)
			translation.x += 1.0f * dt;
		if (m_window->getInput().getKeyState(Key::Right) == KeyState::Pressed)
			translation.x += -1.0f * dt;
		r_spheres->translate(translation);
	}
	{ // cam switching
		if (m_window->getInput().getKeyState(Key::K1) == KeyState::Pressed)
			currentCameraFree = false;
		if (m_window->getInput().getKeyState(Key::K2) == KeyState::Pressed)
			currentCameraFree = true;
	}
}

GameWindow * Scene::getWindow()
{
	return m_window;
}

void Scene::onKey(Key key, Action action, Modifier modifier)
{

}

void Scene::onMouseMove(MousePosition mouseposition)
{
	if (currentCameraFree) {
		glm::vec3 camDir = freeCamera->getDirection();
		float xMovement = -(mouseposition.X - mouseposition.oldX);
		float yMovement = mouseposition.Y - mouseposition.oldY;
		auto rotDirection = glm::vec3(xMovement, yMovement, 0.0f);
		float length = glm::length(rotDirection);
		rotDirection = glm::normalize(rotDirection);
		rotDirection = freeCamera->getRotation() * rotDirection;
		auto cross = glm::cross(rotDirection, camDir);
		auto rotation = glm::angleAxis(glm::radians(length * 0.2f), cross);
		freeCamera->lookinto(rotation * camDir);
	}
}

void Scene::onMouseButton(MouseButton button, Action action, Modifier modifier)
{

}

void Scene::onMouseScroll(double xscroll, double yscroll)
{

}

void Scene::onFrameBufferResize(int width, int height)
{
	currentAspect = static_cast<float>(m_window->getFrameBufferWidth()) / static_cast<float>(m_window->getFrameBufferHeight());
	if(freeCamera)
		freeCamera->setPerspective(glm::radians(60.0f), currentAspect, 0.01f, 100.0f);
}

std::shared_ptr<Renderable> Scene::addObject(std::string path, bool reverseWinding)
{
	auto result = OBJLoader::loadOBJ(path, false, false);
	auto base = std::make_shared<Renderable>();
	renderables.push_back(base);

	for (auto& object : result.objects) {
		auto child = std::make_shared<Renderable>();
		renderables.push_back(child);
		child->setParent(base.get());
		for (auto& mesh : object.meshes) {
			if(reverseWinding)
				OBJLoader::reverseWinding(mesh);
			child->addMesh_inplace(mesh.vertices, mesh.atts, mesh.indices);
		}
	}
	return base;
}
