#include "TextRenderer.h"
#include <vector>

TextRenderer::TextRenderer(ShaderProgram* textShader) 
	: m_shader(textShader)
	, fontTexture("assets/textures/FontBig.png", true)
{
	std::vector<glm::vec2> vertices{
		glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, 0.0f), 
		glm::vec2(0.0f, 1.0f), 
		glm::vec2(1.0f, 1.0f)
	};
	std::vector<GLuint> indices{ 0,1,2,3 };

	// create buffers
	glGenVertexArrays(1, &vaoID);
	glGenBuffers(1, &vboID);
	glGenBuffers(1, &iboID);

	// bind VAO
	glBindVertexArray(vaoID);

	// configure VBO
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), 0);
	glEnableVertexAttribArray(0);

	// configure element array buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), indices.data(), GL_STATIC_DRAW);
	// unbind vao
	glBindVertexArray(0);
}

void TextRenderer::render()
{
	m_shader->use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, fontTexture.getTexId());
	m_shader->setUniform("fontTex", 0);
	m_shader->setUniform("aspect", m_aspectRatio);

	for (auto& textEntry : textEntries) {
		auto& current = textEntry.second;
		if (!current.isEnabled()) continue;
		m_shader->setUniform("text", current.getData(), current.getNumCharacters());
		m_shader->setUniform("numChars", current.getNumCharacters());
		m_shader->setUniform("color", current.getColor());
		m_shader->setUniform("textSize", current.getSize());
		m_shader->setUniform("textPos", current.getPosition());
		m_shader->setUniform("positioning", current.getPositioning());

		glBindVertexArray(vaoID);
		glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, 0);
		
	}
	glBindVertexArray(0);
}

std::pair<unsigned, TextRenderer::TextEntry*> TextRenderer::createTextEntry()
{
	unsigned id = textEntryIdCounter++;
	TextRenderer::TextEntry temp;
	textEntries.emplace(id, temp);
	auto* reference = &textEntries.at(id);
	return std::pair<unsigned, TextRenderer::TextEntry*>(id, reference);
}

void TextRenderer::deleteTextEntry(unsigned id)
{
	textEntries.erase(id);
}

TextRenderer::TextEntry& TextRenderer::getTextEntryById(unsigned id)
{
	return textEntries.at(id);
}

void TextRenderer::TextEntry::setText(std::string t)
{
	for (int i = 0; i < 64 && i < t.size();++i) {
		int temp = 0;
		temp = t[i]; // does this really just write the bytes? if this tries to convert anything i'm f'ed.
		text[i] = temp;
	}
	numCharacters = (t.size() <= 64) ? static_cast<int>(t.size()) : 64;
}
