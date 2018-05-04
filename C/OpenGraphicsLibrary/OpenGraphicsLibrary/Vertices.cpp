#include "Vertices.h"

void vertices::add_Position(vec3 position) 
{
	m_Position.push_back(position);
}

void vertices::add_Color(vec4 color) 
{
	m_Color.push_back(color);
}

void vertices::add_Endice(int endice)
{
	m_Endices.push_back(endice);
}

void vertices::add_Texture(vec2 texture)
{
	m_Textures.push_back(texture);
}