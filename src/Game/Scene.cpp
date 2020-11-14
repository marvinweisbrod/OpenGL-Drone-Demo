#include "Scene.h"
#include <AssetManager.h>

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

		// create the vertex data
		// Praktikum 2: 2.3.1
		GLfloat vertices[] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 0.0f,
			 0.0f,  1.0f, 1.0f, 0.0f, 0.0f,
			-0.5f,  0.5f, 0.0f, 1.0f, 0.0f
		};

		GLuint indices[] = {
			0, 1, 2,
			0, 2, 4,
			4, 2, 3
		};
		vertexCount = 9;

		// Praktikum 2: 2.3.3
		//GLfloat vertices[] = {
		//	-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // unten links
		//	 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // unten rechts
		//	 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // oben rechts
		//	 0.0f, -0.5f, 1.0f, 0.0f, 0.0f, // unten mitte
		//	-0.5f,  0.5f, 0.0f, 1.0f, 0.0f  // oben links
		//};

		//GLuint indices[] = {
		//	0, 3, 4,
		//	1, 2, 3,
		//};
		//vertexCount = 6;




		// create buffers
		glGenVertexArrays(1, &vaoID);
		glGenBuffers(1, &vboID);
		glGenBuffers(1, &eboID);

		// bind VAO
		glBindVertexArray(vaoID);

		// configure VBO
		glBindBuffer(GL_ARRAY_BUFFER, vboID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (2 * sizeof(float)));
		glEnableVertexAttribArray(1);

		// configure element array buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// unbind vao
		glBindVertexArray(0);


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

	glBindVertexArray(vaoID);
	glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);


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