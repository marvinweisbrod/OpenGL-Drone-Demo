#include "Game.h"


Game::Game() : 
GameWindow(	1280,				//width
			720,				//height
			false,				//fullscreen
			false,				//vsync
			3,					//OpenGL Version Major
			3,					//OpenGL Version Minor	=> Here the OpenGL Version is 3.3
			"Testgame",			//Window title
			4,					//MSAA samples for default framebuffer
			false,				//use latest available OpenGL version (instead of the one specified above)
			120.0)				//Update frequency
{}

Game::~Game()
{

}

//Initialization here. (i.e. load a scene, load textures ...)
void Game::init()
{
	m_scene = std::unique_ptr<Scene>(new Scene(this));
	m_scene->init();
	setCursorVisible(false);
}

//cleanup. Free resources here.
void Game::shutdown()
{
	m_scene->shutdown();
}

//Update Game Logic here
void Game::update(GLdouble dtime)
{
	m_scene->update(static_cast<float>(dtime));
}

//Render a frame
void Game::render(GLdouble dtime)
{
	m_scene->render(static_cast<float>(dtime));
}

//Keyboard events
void Game::onKey(Key key, Action action, Modifier modifier)
{
	if (key == Key::Escape && action == Action::Down)
		quit();		
	m_scene->onKey(key, action, modifier);
}

//Mouse move events
void Game::onMouseMove(MousePosition mouseposition)
{
	m_scene->onMouseMove(mouseposition);
}

//Mouse Button events
void Game::onMouseButton(MouseButton button, Action action, Modifier modifier)
{
	m_scene->onMouseButton(button, action, modifier);
}

//Mouse scroll events
void Game::onMouseScroll(double xscroll, double yscroll)
{
	m_scene->onMouseScroll(xscroll, yscroll);
}

//Window resize events
void Game::onFrameBufferResize(int width, int height)
{
	m_scene->onFrameBufferResize(width, height);
}
