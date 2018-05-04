#pragma once

#include "Vec2.h"
#include "Vec3.h"
#include "Vec4.h"
#include "Mat4.h"

#include <vector>
#include <iostream>

class vertices
{
public:
	vertices() { std::cout << "Initialized vertices\n"; }

	void add_Position(vec3 position);
	void add_Color(vec4 color);
	void add_Endice(int endice);
	void add_Texture(vec2 texture);


	std::vector<vec3> m_Position;
	std::vector<vec4> m_Color;
	std::vector<int> m_Endices;
	std::vector<vec2> m_Textures;
	

};