#shader vertex
#version 330 core
layout(location = 0) in vec4 vertex;	//两个坐标 两个纹理
out vec2 texCoord;
out vec4 particleColor;
uniform mat4 proj;
uniform vec2 offset;
void main()
{
	float scale = 10.0f;
	texCoord = vertex.zw;
	gl_Position = proj * vec4((vertex.xy * scale) + offset, 0.0f, 1.0f);	//略去了model的移动和scale
}

#shader fragment
#version 330 core
in vec2 texCoord;
uniform sampler2D sprite;
uniform vec4 particleColor;
void main()
{
	gl_FragColor = texture(sprite, texCoord) * particleColor;
}