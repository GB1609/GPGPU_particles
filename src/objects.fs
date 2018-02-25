#version 330 core
out vec4 FragColor;

uniform bool cone;

void main()
{
	if(cone)
	{
	FragColor = vec4(0.2,0.2,0.8, 1.0f);
	}
	else
	{
	FragColor = vec4(1.0f,1.0f,1.0f, 1.0f);
	}
}