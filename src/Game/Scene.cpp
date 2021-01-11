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

		{// BIKE
			r_bike = std::make_shared<Renderable>();
			renderables.push_back(r_bike);
			auto model = addBike("assets/models/HQ_Movie cycle.obj");
			model->scale(glm::vec3(0.5f, 0.5f, 0.5f));
			model->translateLocal(glm::vec3(0.0f, 0.00f, -0.02f));
			model->rotate(glm::angleAxis(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
			model->setParent(r_bike.get());
		}
		{// GROUND
			auto ground = addObject("assets/models/ground.obj"
				, "assets/textures/ground_diff.png"
				, "assets/textures/ground_spec.png"
				, "assets/textures/ground_emit.png", 100.0f);
			r_ground = ground;
			ground->scale(glm::vec3(0.5f, 0.01f, 0.5f));
			ground->translate(glm::vec3(0.0f, 0.0f, 0.0f));
		}
		{// CAM FOLLOW
			followCamera = std::make_shared<Camera>();
			followCamera->setPerspective(glm::radians(60.0f), currentAspect, 0.01f, 100.0f);
			followCamera->translate(glm::vec3(-2.0f, 2.0f, 0.0f));
			followCamera->lookat(glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			//followCamera->rotateLocal(glm::angleAxis(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
			followCamera->setParent(r_bike.get());
		}
		{// CAM FREE
			freeCamera = std::make_shared<Camera>();
			freeCamera->setPerspective(glm::radians(60.0f), currentAspect, 0.01f, 100.0f);
			freeCamera->translate(glm::vec3(0.0f, 0.0f, -1.5f));
			freeCamera->lookat(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		}
		{// LIGHTS
			ambientLight = glm::vec4(0.1, 0.1, 0.1, 1.0);
			pointLight = std::make_shared<PointLight>(glm::vec3(0.0f,2.0f,0.0f), glm::vec4(0.8f,0.8f,1.0f,1.0f));
			spotLight = std::make_shared<SpotLight>(glm::vec3(1.0f, 0.25f, 0.0f), glm::vec3(1.0f,0.0f,0.0f), glm::radians(30.0f), glm::radians(60.0f), glm::vec4(1.0f, 0.9f, 0.7f, 1.0f));
			spotLight->setParent(r_bike.get());
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
	m_shader->bind(*pointLight);
	m_shader->bind(*spotLight);
	m_shader->setUniform("ambient", ambientLight);

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
	else {  // bike movement
		glm::vec3 translation(0.0f, 0.0f, 0.0f);
		float turn = 0.0f;
		if (m_window->getInput().getKeyState(Key::Up) == KeyState::Pressed)
			translation.x += 1.0f * dt;
		if (m_window->getInput().getKeyState(Key::Down) == KeyState::Pressed)
			translation.x += -1.0f * dt;
		if (m_window->getInput().getKeyState(Key::Left) == KeyState::Pressed)
			turn += 1.0f * dt;
		if (m_window->getInput().getKeyState(Key::Right) == KeyState::Pressed)
			turn += -1.0f * dt;
		if (turn != 0.0f) {
			auto rotation = glm::angleAxis(glm::radians(turn*100), glm::vec3(0.0,1.0,0.0));
			r_bike->rotateLocal(rotation);
		}
		
		r_bike->translateLocal(translation*3.0f);
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
		float xMovement = static_cast<float>(-(mouseposition.X - mouseposition.oldX));
		float yMovement = static_cast<float>(mouseposition.Y - mouseposition.oldY);
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

std::shared_ptr<Renderable> Scene::addObject(std::string path, std::string texDiff, std::string texSpec, std::string texEmss, float uvScale, bool reverseWinding)
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
			child->addMesh(std::make_shared<Mesh>(mesh.vertices, mesh.atts, mesh.indices, texDiff, texSpec, texEmss, uvScale));
		}
	}
	return base;
}

std::shared_ptr<Renderable> Scene::addBike(std::string path, bool reverseWinding)
{
	auto result = OBJLoader::loadOBJ(path, false, false);
	auto base = std::make_shared<Renderable>();
	renderables.push_back(base);

	for (auto& object : result.objects) {
		auto child = std::make_shared<Renderable>();
		renderables.push_back(child);
		child->setParent(base.get());
		int meshCounter = 0;
		for (auto& mesh : object.meshes) {
			if (reverseWinding)
				OBJLoader::reverseWinding(mesh);
			if (meshCounter == 0)
				child->addMesh(std::make_shared<Mesh>(mesh.vertices, mesh.atts, mesh.indices
				, "assets/textures/MG_MovieCycle_Body_DIFF.tga"
				, "assets/textures/MG_MovieCycle_Body_SPEC.tga"
				, "assets/textures/MG_MovieCycle_Body_EMSS.tga"));
			if (meshCounter == 1)
				child->addMesh(std::make_shared<Mesh>(mesh.vertices, mesh.atts, mesh.indices
					, "assets/textures/black.png"
					, "assets/textures/black.png"
					, "assets/textures/black.png"));
			if (meshCounter == 2)
				child->addMesh(std::make_shared<Mesh>(mesh.vertices, mesh.atts, mesh.indices
					, "assets/textures/MG_MovieCycle_Tire_DIFF.tga"
					, "assets/textures/MG_MovieCycle_Tire_SPEC.tga"
					, "assets/textures/MG_MovieCycle_Tire_EMSS.tga"));
			if (meshCounter == 3)
				child->addMesh(std::make_shared<Mesh>(mesh.vertices, mesh.atts, mesh.indices
					, "assets/textures/MG_MovieCycle_Engine_DIFF.tga"
					, "assets/textures/MG_MovieCycle_Engine_SPEC.tga"
					, "assets/textures/MG_MovieCycle_Engine_EMSS.tga"));
			if (meshCounter == 4)
				child->addMesh(std::make_shared<Mesh>(mesh.vertices, mesh.atts, mesh.indices
					, "assets/textures/MG_Player_Body_DIFF.tga"
					, "assets/textures/MG_Player_Body_SPEC.tga"
					, "assets/textures/MG_Player_Body_EMSS.tga"));
			if (meshCounter == 5)
				child->addMesh(std::make_shared<Mesh>(mesh.vertices, mesh.atts, mesh.indices
					, "assets/textures/MG_Player_Helmet_DIFF.tga"
					, "assets/textures/MG_Player_Helmet_SPEC.tga"
					, "assets/textures/MG_Player_Helmet_EMSS.tga"));
			if (meshCounter == 6)
				child->addMesh(std::make_shared<Mesh>(mesh.vertices, mesh.atts, mesh.indices
					, "assets/textures/MG_Player_Disc_DIFF.tga"
					, "assets/textures/MG_Player_Disc_SPEC.tga"
					, "assets/textures/MG_Player_Disc_EMSS.tga"));
			if (meshCounter == 7)
				child->addMesh(std::make_shared<Mesh>(mesh.vertices, mesh.atts, mesh.indices
					, "assets/textures/MG_Player_Baton_DIFF.tga"
					, "assets/textures/MG_Player_Baton_SPEC.tga"
					, "assets/textures/MG_Player_Baton_EMSS.tga"));


			meshCounter++;
		}
	}
	return base;
}
