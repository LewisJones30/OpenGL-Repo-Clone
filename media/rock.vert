
#version 400 core

layout( location = 0 ) in vec3 vPosition;
layout( location = 1 ) in vec4 vColour;
layout (location = 2) in vec2 aTexCoord;
uniform mat4 mvp;
uniform vec4 ambient;
uniform vec4 lightPos;
uniform vec3 dLight;
out vec4 fragColour;
out vec2 TexCoord;
void
main()
{
    gl_Position = mvp * vec4 (vPosition,1.0);
	fragColour = vec4(0.1,0.1,0.1,0.4) * ambient;
	TexCoord = aTexCoord;
}
