class vec4
{
public:

	vec4() { m_X = 0.0f; m_Y = 0.0f; m_Z = 0.0f; m_W = 0.0f; }
	vec4(float x, float y, float z, float w) { m_X = x; m_Y = y; m_Z = z; m_W = w;}

	float m_X;
	float m_Y;
	float m_Z;
	float m_W;
};