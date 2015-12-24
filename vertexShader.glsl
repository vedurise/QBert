#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec3 vertexNormal;
layout(location = 4) in vec2 textVertexPosition;
layout(location = 5) in vec2 textVertexUV;

// Output data ; will be interpolated for each fragment.
out vec2 UV;
out vec4 vertNorm;
out vec4 vertPos;

uniform float currElement_V;
// Values that stay constant for the whole mesh.
uniform mat4 MVP;

void main()
{	

	if(currElement_V == 1)
	{
		vec2 vertexPosition_homo = textVertexPosition - vec2(400,300);
		vertexPosition_homo /= vec2(400,300);
		gl_Position =  vec4(vertexPosition_homo, 0, 1);
		UV = textVertexUV;
	}
	else if(currElement_V == 0)
	{
		gl_Position =  MVP * vec4(vertexPosition,1);
		UV = vertexUV;
		vertNorm = vec4(vertexNormal, 0);
		vertPos = vec4(vertexPosition, 1);
	}

}
