#version 330 core
out vec4 FragColor;
  
in vec4 ourColor;
in vec2 TexCoord;
in vec3 VertexCoordinate;

uniform sampler2D texture;

void main()
{
	FragColor = texture(texture, TexCoord);
}