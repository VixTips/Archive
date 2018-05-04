#pragma once

class vec3
{
public:
	vec3() {m_X = 0.0f; m_Y = 0.0f; m_Z = 0.0f;}
	vec3(float x, float y, float z){ m_X = x; m_Y = y; m_Z = z; }

	float m_X;
	float m_Y;
	float m_Z;
};