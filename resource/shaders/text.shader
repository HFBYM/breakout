#shader vertex
#version 330 core
layout(location = 0) in vec4 vertex;
out vec2 tex_coord;
uniform mat4 proj;	//???坐标如何表示
void main()
{
	gl_Position = proj * vec4(vertex.xy, 0.0f, 1.0f);
	tex_coord = vertex.zw;
}

#shader fragment
#version 330 core
in vec2 tex_coord;
uniform sampler2D text;
uniform vec3 color;
void main()
{
	vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, tex_coord).r);	//用其红色分量代替alpha分量
	gl_FragColor = vec4(color, 1.0f) * sampled;		//这样颜色就是自定义的 alpha分量为源图像的红色分量
}