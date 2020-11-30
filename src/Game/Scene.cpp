#include "Scene.h"
#include <AssetManager.h>
#include "OBJLoader.h"
#include <vector>
#include <memory>

Scene::Scene(GameWindow * window) :
	m_window(window)
{
	assert(window != nullptr);
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
			//auto sphere = addObject("assets/models/sphere.obj");
			//sphere->scale(glm::vec3(0.6f, 0.6f, 0.6f));
			//r_spheres = sphere;
			//// "ears"
			//auto sphere2 = addObject("assets/models/icosahedron.obj");
			//auto sphere3 = addObject("assets/models/icosahedron.obj");
			//sphere2->scale(glm::vec3(0.6f, 0.6f, 0.6f));
			//sphere3->scale(glm::vec3(0.6f, 0.6f, 0.6f));
			//sphere2->translate(glm::vec3(-1.0f,1.0f,0.0f));
			//sphere3->translate(glm::vec3(1.0f, 1.0f, 0.0f));
			//sphere2->setParent(sphere);
			//sphere3->setParent(sphere);
		}
		{
			auto ground = addObject("assets/models/ground.obj", true);
			r_ground = ground;

			ground->scale(glm::vec3(0.01f, 0.01f, 0.01f));
			ground->rotate(glm::angleAxis(glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
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

	timecounter += dt*0.5f;
	if (timecounter >= 1.0f) timecounter = 0.0f;
	m_shader->setUniform("color", timecounter);

	for(auto& renderable: renderables){
		renderable->render(*m_shader);
	}

}

void Scene::update(float dt)
{
	//glm::vec3 translation(0.0f,0.0f,0.0f);
	//if (m_window->getInput().getKeyState(Key::W) == KeyState::Pressed)
	//	translation.y += 1.0f * dt;
	//if (m_window->getInput().getKeyState(Key::S) == KeyState::Pressed)
	//	translation.y += -1.0f * dt;
	//if (m_window->getInput().getKeyState(Key::A) == KeyState::Pressed)
	//	translation.x += -1.0f * dt;
	//if (m_window->getInput().getKeyState(Key::D) == KeyState::Pressed)
	//	translation.x += 1.0f * dt;
	//r_spheres->translate(translation);

	glm::quat rotation(0.0f, 0.0f, 0.0f, 0.0f);
	if (m_window->getInput().getKeyState(Key::W) == KeyState::Pressed)
		rotation = glm::angleAxis(glm::radians(30.0f * dt), glm::vec3(1.0f, 0.0f, 0.0f));
	if (m_window->getInput().getKeyState(Key::S) == KeyState::Pressed)
		rotation = glm::angleAxis(glm::radians(-30.0f * dt), glm::vec3(1.0f, 0.0f, 0.0f));
	if (m_window->getInput().getKeyState(Key::A) == KeyState::Pressed)
		rotation = glm::angleAxis(glm::radians(30.0f * dt), glm::vec3(0.0f, 1.0f, 0.0f));
	if (m_window->getInput().getKeyState(Key::D) == KeyState::Pressed)
		rotation = glm::angleAxis(glm::radians(-30.0f * dt), glm::vec3(0.0f, 1.0f, 0.0f));
	r_ground->rotate(rotation);
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

}

void Scene::onMouseButton(MouseButton button, Action action, Modifier modifier)
{

}

void Scene::onMouseScroll(double xscroll, double yscroll)
{

}

void Scene::onFrameBufferResize(int width, int height)
{

}

Renderable* Scene::addObject(std::string path, bool reverseWinding)
{
	auto result = OBJLoader::loadOBJ(path, false, false);
	auto base = new Renderable();
	renderables.push_back(base);

	for (auto& object : result.objects) {
		auto child = new Renderable();
		renderables.push_back(child);
		child->setParent(base);
		for (auto& mesh : object.meshes) {
			if(reverseWinding)
				OBJLoader::reverseWinding(mesh);
			child->addMesh_inplace(mesh.vertices, mesh.atts, mesh.indices);
		}
	}
	return base;
}
