#include "Scene.h"
#include <AssetManager.h>
#include "OBJLoader.h"

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

		//{ // 3.2,3.4
		//	std::vector<Vertex> vertices;
		//	vertices.emplace_back(glm::vec3(-0.5f, -0.5f, 0.0f));
		//	vertices.emplace_back(glm::vec3(0.5f, -0.5f, 0.0f));
		//	vertices.emplace_back(glm::vec3(0.5f, 0.5f, 0.0f));
		//	vertices.emplace_back(glm::vec3(0.0f, 1.0f, 0.0f));
		//	vertices.emplace_back(glm::vec3(-0.5f, 0.5f, 0.0f));

		//	std::vector<Index> indices;
		//	indices.push_back(0);
		//	indices.push_back(1);
		//	indices.push_back(2);

		//	indices.push_back(0);
		//	indices.push_back(2);
		//	indices.push_back(4);

		//	indices.push_back(4);
		//	indices.push_back(2);
		//	indices.push_back(3);

		//	std::vector<VertexAttribute> attributes;
		//	attributes.push_back(VertexAttribute{ 3, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, position) });
		//	attributes.push_back(VertexAttribute{ 2, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, uv) });
		//	attributes.push_back(VertexAttribute{ 3, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, normal) });
		//	attributes.push_back(VertexAttribute{ 3, GL_FLOAT, sizeof(Vertex), offsetof(Vertex, tangent) });

		//	renderables.push_back(Renderable());
		//	auto& renderable = renderables.back();
		//	renderable.addMesh_inplace(vertices, attributes, indices);
		//	renderable.initialize();
		//}

		{ // 3.3 (sphere required)
			auto result = OBJLoader::loadOBJ("assets/models/icosahedron.obj", false, false);

			for (auto& object : result.objects) {
				renderables.push_back(Renderable());
				auto& renderable = renderables.back();

				for (auto& mesh : object.meshes) {
					renderable.addMesh_inplace(mesh.vertices, mesh.atts, mesh.indices);
				}
				renderable.initialize();
			}
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
	glClear(GL_COLOR_BUFFER_BIT);
	m_shader->use();

	timecounter += dt;
	if (timecounter >= 1.0f) timecounter = 0.0f;
	m_shader->setUniform("color", timecounter);

	for(auto& renderable: renderables){
		renderable.render();
	}

}

void Scene::update(float dt)
{

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