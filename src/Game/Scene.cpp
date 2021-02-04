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
		m_assets.addShaderProgram("skybox", AssetManager::createShaderProgram("assets/shaders/skyboxVert.glsl", "assets/shaders/skyboxFrag.glsl"));
		m_assets.addShaderProgram("geometryPass", AssetManager::createShaderProgram("assets/shaders/geometryPassVert.glsl", "assets/shaders/geometryPassFrag.glsl"));
		m_shaderGeometryPass = m_assets.getShaderProgram("geometryPass");
		m_assets.addShaderProgram("lightingPass", AssetManager::createShaderProgram("assets/shaders/lightingPassVert.glsl", "assets/shaders/lightingPassFrag.glsl"));
		m_shaderLightingPass = m_assets.getShaderProgram("lightingPass");
		m_assets.addShaderProgram("visualization", AssetManager::createShaderProgram("assets/shaders/lightingPassVert.glsl", "assets/shaders/visualizationFrag.glsl"));
		m_shaderVisualization = m_assets.getShaderProgram("visualization");

		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);
		glCullFace(GL_BACK);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		{// Text Renderer
			textRenderer = std::make_shared<TextRenderer>(m_assets.getShaderProgram("text"));
			textRenderer->setAspect(currentAspect);

			// hier jetzt testweise ein text hinzugefügt. der CollectibleManager kann sich selbst nen text erstellen und ihn immer wieder editieren.
			//auto result = textRenderer->createTextEntry();
			//result.second->setText("Bla");
			//result.second->setPosition(glm::vec2(0.0f, 0.0f));
			//result.second->setSize(0.3f);
			//result.second->setCentered(true);

			auto result = textRenderer->createTextEntry();
			viewModeTextId = result.first;
			result.second->setPosition(glm::vec2(0.0f, 0.9f));
			result.second->setSize(0.1f);
			result.second->setEnabled(false);
		}
		{
			std::vector<std::string> faces
			{
				"assets/textures/skybox2/right.jpg",
				"assets/textures/skybox2/left.jpg",
				"assets/textures/skybox2/top.jpg",
				"assets/textures/skybox2/bottom.jpg",
				"assets/textures/skybox2/front.jpg",
				"assets/textures/skybox2/back.jpg"
			};
			skyboxRenderer = std::make_shared<SkyboxRenderer>(m_assets.getShaderProgram("skybox"), &faces);
		}
		std::shared_ptr<Renderable> drone_body_transform;
		{// Drone
			r_drone = std::make_shared<Renderable>();
			renderables.push_back(r_drone);
			auto model = addDrone("assets/models/Drone_fixed2.obj", r_drone, false, drone_body_transform);
			model->scale(glm::vec3(0.005f, 0.005f, 0.005f));
			Bounds bounds = model->getTransformedBounds();
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
			ambientLight = glm::vec4(0.5, 0.5, 0.5, 1.0);
			directionalLight = std::make_shared<DirectionalLight>(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec4(0.5f, 0.5f, 0.4f, 1.0f));
			spotLight = std::make_shared<SpotLight>(glm::vec3(0.0f, 0.0f, 34.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::radians(20.0f), glm::radians(40.0f),
				glm::vec4(0.0f, 0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.22f, 0.0019f));

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

		flowManager = std::make_shared<FlowManager>(droneController, textRenderer, collectibleManager, m_window);

		// Initialize gbuffer
		glGenFramebuffers(1, &gBuffer); GLERR
		glBindFramebuffer(GL_FRAMEBUFFER, gBuffer); GLERR
		GLsizei framebufferWidth = m_window->getFrameBufferWidth();
		GLsizei framebufferHeight = m_window->getFrameBufferHeight();

		// Position color buffer
		glGenTextures(1, &gPosition); GLERR
		glBindTexture(GL_TEXTURE_2D, gPosition); GLERR
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, framebufferWidth, framebufferHeight, 0, GL_RGBA, GL_FLOAT, nullptr); GLERR
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); GLERR
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); GLERR
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0); GLERR

		// Normal color buffer
		glGenTextures(1, &gNormal); GLERR
		glBindTexture(GL_TEXTURE_2D, gNormal); GLERR
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, framebufferWidth, framebufferHeight, 0, GL_RGBA, GL_FLOAT, nullptr); GLERR
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); GLERR
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); GLERR
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0); GLERR

		// Color color buffer
		glGenTextures(1, &gAlbedo); GLERR
		glBindTexture(GL_TEXTURE_2D, gAlbedo); GLERR
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, framebufferWidth, framebufferHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr); GLERR
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); GLERR
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); GLERR
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedo, 0); GLERR

		// Emissive color buffer
		glGenTextures(1, &gEmissive); GLERR
		glBindTexture(GL_TEXTURE_2D, gEmissive); GLERR
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, framebufferWidth, framebufferHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr); GLERR
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); GLERR
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); GLERR
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, gEmissive, 0); GLERR

		// Specular + shine color buffer
		glGenTextures(1, &gSpecShine); GLERR
		glBindTexture(GL_TEXTURE_2D, gSpecShine); GLERR
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, framebufferWidth, framebufferHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr); GLERR
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); GLERR
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); GLERR
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, gSpecShine, 0); GLERR

		// Depth buffer object, using render buffer
		glGenRenderbuffers(1, &gDepth); GLERR
		glBindRenderbuffer(GL_RENDERBUFFER, gDepth); GLERR
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, framebufferWidth, framebufferHeight); GLERR
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, gDepth); GLERR
		// using color buffer
		//glGenTextures(1, &gDepth);
		//glBindTexture(GL_TEXTURE_2D, gDepth);
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, framebufferWidth, framebufferHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr); GLERR
		//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, gDepth, 0); GLERR

		// Set which color attachments we use
		unsigned int attachments[5] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4 };
		glDrawBuffers(5, attachments); GLERR

		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER); GLERR
		if(status != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cerr << "GBuffer failed to initialize: " << status << "\n";
			exit(1);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0); GLERR

		// Generate full screen quad.
		glGenVertexArrays(1, &quadVao); GLERR
		glBindVertexArray(quadVao); GLERR

		static const GLfloat quadVertexBufferData[] = {
			1, -1, 1.0f, 0.0f,
			1, 1, 1.0f, 1.0f,
			-1, -1, 0.0f, 0.0f,
			-1, 1, 0.0f, 1.0f
		};

		GLuint quadVbo;
		glGenBuffers(1, &quadVbo); GLERR
		glBindBuffer(GL_ARRAY_BUFFER, quadVbo); GLERR
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertexBufferData), quadVertexBufferData, GL_STATIC_DRAW); GLERR
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4, 0); GLERR
		glEnableVertexAttribArray(0); GLERR
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4, (const void*) (sizeof(GLfloat) * 2)); GLERR
		glEnableVertexAttribArray(1); GLERR

		//initial opengl state
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f); GLERR

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
	// Deferred shading
	GLsizei width = m_window->getFrameBufferWidth();
	GLsizei height = m_window->getFrameBufferHeight();

	// --------------------------------
	// Geometry pass
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer); GLERR
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GLERR
	m_shaderGeometryPass->use();

	// Setup uniforms
	m_shaderGeometryPass->bind(currentCameraFree ? *freeCamera : *followCamera);

	// Render all meshes to the framebuffer
	for(auto& renderable : renderables)
	{
		renderable->render(*m_shaderGeometryPass);
	}
	
	// -------------------------------
	// Light pass
	glBindFramebuffer(GL_FRAMEBUFFER, 0); GLERR
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GLERR

	if(viewMode == ViewMode::kDefault || viewMode == ViewMode::kDeferredOnly)
	{
		m_shaderLightingPass->use();

		// Setup uniforms
		glm::mat4 viewMatrix = currentCameraFree ? freeCamera->getViewMatrix() : followCamera->getViewMatrix();
		m_shaderLightingPass->setUniform("viewMatInv", glm::inverse(viewMatrix), false);
		m_shaderLightingPass->bind(*pointLight);
		m_shaderLightingPass->bind(*spotLight);
		m_shaderLightingPass->bind(*directionalLight);
		m_shaderLightingPass->setUniform("ambient", ambientLight);
		glActiveTexture(GL_TEXTURE0); GLERR
		glBindTexture(GL_TEXTURE_2D, gPosition); GLERR
		m_shaderLightingPass->setUniform("gPosition", 0);
		glActiveTexture(GL_TEXTURE1); GLERR
		glBindTexture(GL_TEXTURE_2D, gNormal); GLERR
		m_shaderLightingPass->setUniform("gNormal", 1);
		glActiveTexture(GL_TEXTURE2); GLERR
		glBindTexture(GL_TEXTURE_2D, gAlbedo); GLERR
		m_shaderLightingPass->setUniform("gAlbedo", 2);
		glActiveTexture(GL_TEXTURE3); GLERR
		glBindTexture(GL_TEXTURE_2D, gEmissive); GLERR
		m_shaderLightingPass->setUniform("gEmissive", 3);
		glActiveTexture(GL_TEXTURE4); GLERR
		glBindTexture(GL_TEXTURE_2D, gSpecShine); GLERR
		m_shaderLightingPass->setUniform("gSpecShine", 4);
	}
	else
	{
		m_shaderVisualization->use();

		// Setup uniforms
		glActiveTexture(GL_TEXTURE0); GLERR
		glm::mat4 channels(1.0f);
		channels[3][3] = 0.0f;
		glm::vec4 additive(0.0f, 0.0f, 0.0f, 1.0f);
		switch(viewMode)
		{
		case ViewMode::kPosition:
			glBindTexture(GL_TEXTURE_2D, gPosition); GLERR
			break;

		case ViewMode::kNormal:
			glBindTexture(GL_TEXTURE_2D, gNormal); GLERR
			break;

		case ViewMode::kAlbedo:
			glBindTexture(GL_TEXTURE_2D, gAlbedo); GLERR
			break;

		case ViewMode::kSpec:
			glBindTexture(GL_TEXTURE_2D, gSpecShine); GLERR
			channels[1][1] = 0.0f;
			channels[2][2] = 0.0f;
			channels[0][1] = 1.0f;
			channels[0][2] = 1.0f;
			break;

		case ViewMode::kEmissive:
			glBindTexture(GL_TEXTURE_2D, gEmissive); GLERR
			break;

		case ViewMode::kShine:
			glBindTexture(GL_TEXTURE_2D, gSpecShine); GLERR
			channels[0][0] = 0.0f;
			channels[1][1] = 1.0f;
			channels[2][2] = 0.0f;
			channels[1][0] = 1.0f;
			channels[1][2] = 1.0f;
			break;
		}
		m_shaderVisualization->setUniform("gTexture", 0);
		m_shaderVisualization->setUniform("channels", channels, false);
		m_shaderVisualization->setUniform("additive", additive);
	}

	// Render the quad
	glBindVertexArray(quadVao);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4); GLERR

	// ---------------------------------------
	// Forward pass

	// Render the skybox (if necessary)
	if(viewMode == ViewMode::kDefault)
	{
		// Copy the depth buffer from the gbuffer to the default framebuffer
		glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// render skybox after all other objects so we dont need to calculate hidden fragments
		skyboxRenderer->render(currentCameraFree ? *freeCamera : *followCamera);
	}

	// Render text without depth test
	glDisable(GL_DEPTH_TEST);
	textRenderer->render();
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
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
	flowManager->update();
}

void Scene::setViewMode(ViewMode mode)
{
	if(viewMode != mode)
	{
		viewMode = mode;

		TextRenderer::TextEntry& viewModeText = textRenderer->getTextEntryById(viewModeTextId);
		viewModeText.setEnabled(mode != ViewMode::kDefault);
		switch(mode)
		{
		case ViewMode::kAlbedo:
			viewModeText.setText("Albedo");
			break;

		case ViewMode::kDeferredOnly:
			viewModeText.setText("Deferred only");
			break;

		case ViewMode::kEmissive:
			viewModeText.setText("Emissive");
			break;

		case ViewMode::kNormal:
			viewModeText.setText("Normal");
			break;

		case ViewMode::kPosition:
			viewModeText.setText("Position");
			break;

		case ViewMode::kShine:
			viewModeText.setText("Shine");
			break;

		case ViewMode::kSpec:
			viewModeText.setText("Specular");
			break;
		}
	}
}

void Scene::setDayMode(bool day)
{
	if(dayMode != day)
	{
		dayMode = day;
		if(dayMode)
		{
			ambientLight = glm::vec4(0.5, 0.5, 0.5, 1.0);
			directionalLight->getColor() = glm::vec4(0.5f, 0.5f, 0.4f, 1.0f);
			directionalLight->setDirection(glm::vec3(1.0f, 1.0f, 1.0f));
			spotLight->getColor() = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
		}
		else
		{
			ambientLight = glm::vec4(0.1, 0.1, 0.1, 1.0);
			directionalLight->getColor() = glm::vec4(0.4f, 0.4f, 0.6f, 1.0f);
			directionalLight->setDirection(glm::vec3(1.0f, -1.0f, 1.0f));
			spotLight->getColor() = glm::vec4(1.0f, 0.9f, 0.7f, 1.0f);
		}
	}
}

GameWindow * Scene::getWindow()
{
	return m_window;
}

void Scene::onKey(Key key, Action action, Modifier modifier)
{
	if(action == Action::Down)
	{
		if(key == Key::K3)
		{
			setViewMode(ViewMode::kDefault);
		}
		else if(key == Key::K4)
		{
			setViewMode(ViewMode::kDeferredOnly);
		}
		else if(key == Key::K5)
		{
			setViewMode(ViewMode::kPosition);
		}
		else if(key == Key::K6)
		{
			setViewMode(ViewMode::kNormal);
		}
		else if(key == Key::K7)
		{
			setViewMode(ViewMode::kAlbedo);
		}
		else if(key == Key::K8)
		{
			setViewMode(ViewMode::kSpec);
		}
		else if(key == Key::K9)
		{
			setViewMode(ViewMode::kEmissive);
		}
		else if(key == Key::K0)
		{
			setViewMode(ViewMode::kShine);
		}
		else if(key == Key::T)
		{
			setDayMode(!dayMode);
		}
	}
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
	parent->translateLocal(glm::vec3(0.0f, 0.00f, 0.5f));
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
