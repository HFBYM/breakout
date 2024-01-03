#shader vertex
#version 330 core
layout(location = 0) in vec4 vertex;	//两个局部坐标 两个纹理坐标
out vec2 texCoord;
uniform mat4 model;
uniform mat4 proj;
void main()
{
	texCoord = vertex.zw;
	gl_Position = proj * model * vec4(vertex.xy, 0.0f, 1.0f);	//不需要摄像机投影
}

#shader fragment
#version 330 core
in vec2 texCoord;

uniform sampler2D image;	//纹理采样器
uniform vec3 spriteColor;
void main()
{
	gl_FragColor = vec4(spriteColor, 1.0f) * texture(image, texCoord);
}