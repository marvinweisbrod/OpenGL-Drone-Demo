#include "Scene.h"
#include <AssetManager.h>
#include "OBJLoader.h"
#include <vector>
#include <memory>
#include <exception>
#include "CollectibleManager.h"
#include "DirectionalLight.h"

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
		m_assets.addShaderProgram("main", AssetManager::createShaderProgram("assets/shaders/vertex.glsl", "assets/shaders/fragment.glsl"));
		m_shaderMain = m_assets.getShaderProgram("main");
		m_assets.addShaderProgram("text", AssetManager::createShaderProgram("assets/shaders/textVert.glsl", "assets/shaders/textFrag.glsl"));

		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);
		glCullFace(GL_BACK);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//{ // version 1
		//	float a = 0.4f;
		//	int a_int = *((int*)&a);
		//	std::cout << "1: float: " << a << " int: " << a_int << "\n";
		//}

		char name[10] = { 'L','e','s','s','o','n','s','\0' };
		std::cout << *(name + 1) << "\n";

		//{ // version 2
		//	float a = 0.4f;
		//	int a_int = ((int)a);
		//	std::cout << "2: float: " << a << " int: " << a_int << "\n";
		//}
		//

		{// Text Renderer
			textRenderer = std::make_shared<TextRenderer>(m_assets.getShaderProgram("text"));
			textRenderer->setAspect(currentAspect);

			// hier jetzt testweise ein text hinzugefügt. der CollectibleManager kann sich selbst nen text erstellen und ihn immer wieder editieren.
			//auto result = textRenderer->createTextEntry();
			//result.second->setText("Bla");
			//result.second->setPosition(glm::vec2(0.0f, 0.0f));
			//result.second->setSize(0.3f);
			//result.second->setCentered(true);
		}
		std::shared_ptr<Renderable> drone_body_transform;
		{// Drone
			r_drone = std::make_shared<Renderable>();
			renderables.push_back(r_drone);
			auto model = addDrone("assets/models/Drone_fixed2.obj", r_drone, false, drone_body_transform);
			model->scale(glm::vec3(0.005f, 0.005f, 0.005f));
			Bounds bounds = model->getTransformedBounds();
			r_drone->translateLocal(glm::vec3(0.0f, 0.00f, 0.5f));
			//model->rotate(glm::angleAxis(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
			model->setParent(r_drone.get());
			r_drone->setBounds(bounds);
		}
		{// GROUND
			auto ground = addObject("assets/models/ground.obj"
				, "assets/textures/ground2_diff.png"
				, "assets/textures/black.png"
				, "assets/textures/black.png", 90.0f);
			r_ground = ground;
			ground->scale(glm::vec3(0.5f, 0.00f, 0.5f));
			ground->translate(glm::vec3(0.0f, 0.0f, 0.0f));
		}
		{
			// Cake
			r_cake = addObject("assets/models/cake.obj", "assets/textures/cake.png", "", "", 1.0f, false);
			r_cake->setActive(false);
			r_cake->scale(glm::vec3(0.025f, 0.025f, 0.025f));
			r_cake->rotate(glm::angleAxis(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
			r_cake->translate(glm::vec3(0.0f, 0.03f, 0.0f));
		}
		{// CAM FOLLOW
			followCamera = std::make_shared<Camera>();
			followCamera->setPerspective(glm::radians(60.0f), currentAspect, 0.01f, 500.0f);
			followCamera->translate(glm::vec3(0.0f, 0.4f, -0.8f));
			followCamera->lookat(glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			//followCamera->rotateLocal(glm::angleAxis(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
			followCamera->setParent(r_drone.get());
		}
		{// CAM FREE
			freeCamera = std::make_shared<Camera>();
			freeCamera->setPerspective(glm::radians(60.0f), currentAspect, 0.01f, 500.0f);
			freeCamera->translate(glm::vec3(0.0f, 0.0f, -1.5f));
			freeCamera->lookat(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		}
		{// LIGHTS
			const bool DAYMODE = true;

			if (DAYMODE) {
				ambientLight = glm::vec4(0.5, 0.5, 0.5, 1.0);
				directionalLight = std::make_shared<DirectionalLight>(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec4(0.5f, 0.5f, 0.4f, 1.0f));
				spotLight = std::make_shared<SpotLight>(glm::vec3(0.0f, 0.0f, 34.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::radians(20.0f), glm::radians(40.0f),
					glm::vec4(0.0f, 0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.22f, 0.0019f));
			}
			else {
				ambientLight = glm::vec4(0.1, 0.1, 0.1, 1.0);
				directionalLight = std::make_shared<DirectionalLight>(glm::vec3(1.0f, -1.0f, 1.0f), glm::vec4(0.4f, 0.40f, 0.6f, 1.0f));
				spotLight = std::make_shared<SpotLight>(glm::vec3(0.0f, 0.0f, 34.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::radians(20.0f), glm::radians(40.0f),
					glm::vec4(1.0f, 0.9f, 0.7f, 1.0f), glm::vec3(1.0f, 0.22f, 0.0019f));
			}

			spotLight->setParent(drone_body_transform.get());
			pointLight = std::make_shared<PointLight>(glm::vec3(0.0f,2.0f,0.0f), glm::vec4(0.0f,0.0f,0.0f,0.0f));
		}
		{// Buildings
			// good heights for our model are ß, -2.8, -5.8, -8.8, -11.8, -14.8
			// green
			instantiateApartment(0, glm::vec3( 10.0f,  0.0f, 20.0f),  0.0f);
			instantiateApartment(0, glm::vec3( 10.0f, -2.8f,-10.0f),  0.0f);
			instantiateApartment(2, glm::vec3(-10.0f, -5.8f, 20.0f),  0.0f);
			instantiateApartment(1, glm::vec3( 10.0f, -8.8f,-20.0f),  0.0f);
			instantiateApartment(2, glm::vec3(-19.0f, -2.8f, 15.0f), 90.0f);
			instantiateApartment(1, glm::vec3( 20.0f, -2.8f,  0.0f),-90.0f);
			instantiateApartment(1, glm::vec3( -9.0f,-11.8f,  6.0f), 90.0f);
			instantiateApartment(1, glm::vec3(  6.0f,-14.8f,  6.0f), 90.0f);
			instantiateApartment(2, glm::vec3(-10.0f, -5.8f,-16.0f),  0.0f);
			instantiateApartment(0, glm::vec3( 25.0f,-11.8f, 16.0f), 90.0f);
		}

		collectibleManager = std::make_shared<CollectibleManager>(r_drone, textRenderer);
		// on buildings
		instantiateCake(glm::vec3(10.0f, 18.0f, 19.5f));
		instantiateCake(glm::vec3(-19.0f, 15.0f, 15.6f));
		instantiateCake(glm::vec3(20.0f, 15.0f, -0.8f));
		instantiateCake(glm::vec3(-10.0f, 12.0f, -16.6f));

		// between buildings
		instantiateCake(glm::vec3(10.0f, 0.0f, -15.0f));
		instantiateCake(glm::vec3(-14.0f, 0.0f, 15.0f));

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
	m_shaderMain->use();
	m_shaderMain->bind(currentCameraFree ? *freeCamera : *followCamera);
	m_shaderMain->bind(*pointLight);
	m_shaderMain->bind(*spotLight);
	m_shaderMain->bind(*directionalLight);
	m_shaderMain->setUniform("ambient", ambientLight);

	for(auto& renderable: renderables){
		renderable->render(*m_shaderMain);
	}

	// Render text without depth test
	glDisable(GL_DEPTH_TEST);
	textRenderer->render();
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
}

void Scene::update(float dt)
{
	
	// free cam movement
	if (currentCameraFree) { 
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
	else { // Drone Movement
		droneController->update(dt);
	}

	droneAnimator->update(dt);

	{ // cam switching
		if (m_window->getInput().getKeyState(Key::K1) == KeyState::Pressed)
			currentCameraFree = false;
		if (m_window->getInput().getKeyState(Key::K2) == KeyState::Pressed)
			currentCameraFree = true;
	}

	collectibleManager->update(dt);
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
	textRenderer->setAspect(currentAspect);
}

std::shared_ptr<Renderable> Scene::addObject(std::string path, std::string texDiff, std::string texSpec, std::string texEmss, float uvScale, bool reverseWinding)
{
	auto result = OBJLoader::loadOBJ(path, false, false);
	auto base = std::make_shared<Renderable>();
	renderables.push_back(base);

	Bounds baseBounds = result.bounds;
	for (auto& object : result.objects) {
		auto child = std::make_shared<Renderable>();
		renderables.push_back(child);
		child->setParent(base.get());
		for (auto& mesh : object.meshes) {
			if(reverseWinding)
				OBJLoader::reverseWinding(mesh);
			child->addMesh(std::make_shared<Mesh>(mesh.vertices, mesh.atts, mesh.indices, texDiff, texSpec, texEmss, mesh.bounds, uvScale));
		}
		//baseBounds.include(child->getBounds());
	}

	base->setBounds(baseBounds);

	return base;
}

std::shared_ptr<Renderable> Scene::addDrone(std::string path, std::shared_ptr<Renderable>& parent, bool reverseWinding, std::shared_ptr<Renderable>& out_body)
{
	auto result = OBJLoader::loadOBJ(path, false, false);
	auto base = std::make_shared<Renderable>();
	renderables.push_back(base);

	auto collection = std::make_shared<Renderable>();
	collection->setParent(base.get());
	out_body = collection;
	auto rotor0 = std::make_shared<Renderable>();
	auto rotor1 = std::make_shared<Renderable>();
	auto rotor2 = std::make_shared<Renderable>();
	auto rotor3 = std::make_shared<Renderable>();
	rotor0->setParent(collection.get());
	rotor1->setParent(collection.get());
	rotor2->setParent(collection.get());
	rotor3->setParent(collection.get());
	renderables.push_back(collection);
	renderables.push_back(rotor0);
	renderables.push_back(rotor1);
	renderables.push_back(rotor2);
	renderables.push_back(rotor3);
	
	if (result.objects.size() != 1) {
		throw std::exception("Wrong amout of objects in drone file!");
	}

	float offset = 23.0f;

	Bounds baseBounds = result.bounds;
	for (auto& object : result.objects) {
		int meshCounter = 0;
		for (auto& mesh : object.meshes) {
			if (reverseWinding)
				OBJLoader::reverseWinding(mesh);
			if (meshCounter == 0) // Body
				collection->addMesh(std::make_shared<Mesh>(mesh.vertices, mesh.atts, mesh.indices
					, "assets/textures/drone_diffuse.png"
					, "assets/textures/drone_specular.png"
					, "assets/textures/drone_emissive.png"
					, mesh.bounds));
			if (meshCounter == 1) { // Front Left
				auto temp = std::make_shared<Renderable>();
				renderables.push_back(temp);
				temp->setParent(rotor0.get());
				temp->translate(glm::vec3(-offset, 0.0f, -offset));
				rotor0->translate(glm::vec3(offset, 0.0f, offset));
				temp->addMesh(std::make_shared<Mesh>(mesh.vertices, mesh.atts, mesh.indices
					, "assets/textures/drone_diffuse.png"
					, "assets/textures/drone_specular.png"
					, "assets/textures/black.png"
					, mesh.bounds));
				//baseBounds.include(temp->getTransformedBounds());
			}
			if (meshCounter == 2) { // Back Left
				auto temp = std::make_shared<Renderable>();
				renderables.push_back(temp);
				temp->setParent(rotor1.get());
				temp->translate(glm::vec3(-offset, 0.0f, offset));
				rotor1->translate(glm::vec3(offset, 0.0f, -offset));
				temp->addMesh(std::make_shared<Mesh>(mesh.vertices, mesh.atts, mesh.indices
					, "assets/textures/drone_diffuse.png"
					, "assets/textures/drone_specular.png"
					, "assets/textures/black.png"
					, mesh.bounds));
				//baseBounds.include(temp->getTransformedBounds());
			}
			if (meshCounter == 3) { // Back Right
				auto temp = std::make_shared<Renderable>();
				renderables.push_back(temp);
				temp->setParent(rotor2.get());
				temp->translate(glm::vec3(offset, 0.0f, offset));
				rotor2->translate(glm::vec3(-offset, 0.0f, -offset));
				temp->addMesh(std::make_shared<Mesh>(mesh.vertices, mesh.atts, mesh.indices
					, "assets/textures/drone_diffuse.png"
					, "assets/textures/drone_specular.png"
					, "assets/textures/black.png"
					, mesh.bounds));
				//baseBounds.include(temp->getTransformedBounds());
			}
			if (meshCounter == 4) { // Front Right
				auto temp = std::make_shared<Renderable>();
				renderables.push_back(temp);
				temp->setParent(rotor3.get());
				temp->translate(glm::vec3(offset, 0.0f, -offset));
				rotor3->translate(glm::vec3(-offset, 0.0f, offset));
				temp->addMesh(std::make_shared<Mesh>(mesh.vertices, mesh.atts, mesh.indices
					, "assets/textures/drone_diffuse.png"
					, "assets/textures/drone_specular.png"
					, "assets/textures/black.png"
					, mesh.bounds));
				//baseBounds.include(temp->getTransformedBounds());
			}
			

			meshCounter++;
		}
	}

	//baseBounds.include(collection->getTransformedBounds());
	//baseBounds.include(rotor0->getTransformedBounds());
	//baseBounds.include(rotor1->getTransformedBounds());
	//baseBounds.include(rotor2->getTransformedBounds());
	//baseBounds.include(rotor3->getTransformedBounds());
	base->setBounds(baseBounds);

	droneController = std::make_shared<DroneController>(std::static_pointer_cast<Transform>(parent), m_window, buildings);
	droneAnimator = std::make_shared<DroneAnimator>(droneController
		, std::static_pointer_cast<Transform>(collection)
		, std::static_pointer_cast<Transform>(rotor0)
		, std::static_pointer_cast<Transform>(rotor1)
		, std::static_pointer_cast<Transform>(rotor2)
		, std::static_pointer_cast<Transform>(rotor3)
		);

	return base;
}

void Scene::instantiateApartment(int type, glm::vec3 pos, float rotation)
{
	auto apt = addApartment(type);
	apt->translate(pos);
	apt->rotate(glm::angleAxis(glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f)));
}

std::shared_ptr<Renderable> Scene::addApartment(int type)
{
	auto result = OBJLoader::loadOBJ("assets/models/apartment1.obj", false, false);
	auto base = std::make_shared<Renderable>();
	renderables.push_back(base);

	std::string diffuse = "assets/textures/apartment1/apartment1.000.png";
	if (type == 1)
		diffuse = "assets/textures/apartment1/apartment1.001.png";
	if (type == 2)
		diffuse = "assets/textures/apartment1/apartment1.002.png";

	for (auto& object : result.objects) {
		auto child = std::make_shared<Renderable>();
		renderables.push_back(child);
		child->setParent(base.get());
		for (auto& mesh : object.meshes) {
			//OBJLoader::reverseWinding(mesh);
			child->addMesh(std::make_shared<Mesh>(mesh.vertices, mesh.atts, mesh.indices, diffuse, "assets/textures/black.png", "assets/textures/black.png", mesh.bounds, 1.0f));
		}
	}

	base->setBounds(result.bounds);
	buildings.push_back(base);
	return base;
}

std::shared_ptr<Renderable> Scene::instantiateCake(const glm::vec3& position, const glm::quat& rotation, const glm::vec3& scale)
{
	std::shared_ptr<Renderable> cake = cloneRenderable(r_cake.get());
	cake->translate(position);
	cake->rotate(rotation);
	cake->scale(scale);
	cake->setActive(true);
	collectibleManager->add(cake);
	return cake;
}

std::shared_ptr<Renderable> Scene::cloneRenderable(Renderable* renderable)
{
	std::shared_ptr<Renderable> clone = std::make_shared<Renderable>(*renderable);
	renderables.push_back(clone);
	for(Transform* childTransform : renderable->getChildren())
	{
		std::shared_ptr<Renderable> child = cloneRenderable(static_cast<Renderable*>(childTransform));
		child->setParent(clone.get());
		renderables.push_back(child);
	}

	return clone;
}
