#pragma once
#include "ShaderProgram.h"
#include <libheaders.h>
#include <string>
#include <unordered_map>
#include <utility>
#include "Texture2D.h"

class TextRenderer {
public:
	TextRenderer(ShaderProgram* textShader);
	void render();

	class TextEntry {
	public:
		void setText(std::string t);
		void setColor(glm::vec4 c) { color = c; };
		void setPosition(glm::vec2 p) { position = p; };
		void setSize(GLfloat s) { size = s; };
		void setCentered(bool c) { centered = c; };
		void setEnabled(bool state) { enabled = state; };
		int* getData() { return text; };
		float getSize() { return size; };
		glm::vec4& getColor() { return color; };
		glm::vec2& getPosition() { return position; };
		int getNumCharacters() { return numCharacters; };
		bool getCentered() { return centered; };
		bool isEnabled() { return enabled; };
	private:
		glm::vec4 color = glm::vec4(1.0f,1.0f,1.0f,1.0f);
		glm::vec2 position = glm::vec2(0.0f, 0.0f);
		float size = 0.5f;
		int text[64];
		int numCharacters = 0;
		bool centered = false;
		bool enabled = true;
	};

	std::pair<unsigned, TextRenderer::TextEntry*> createTextEntry();
	void setAspect(float aspect) { m_aspectRatio = aspect; };
	void deleteTextEntry(unsigned id);
	TextRenderer::TextEntry& getTextEntryById(unsigned id);

private:
	ShaderProgram* m_shader;
	std::unordered_map<unsigned, TextEntry> textEntries;
	GLuint vaoID = 0;
	GLuint vboID = 0;
	GLuint iboID = 0;
	Texture2D fontTexture;
	unsigned textEntryIdCounter = 0;
	float m_aspectRatio = 1.0f;
};