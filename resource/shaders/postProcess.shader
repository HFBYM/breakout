#shader vertex
#version 330 core
layout(location = 0) in vec4 vertex;
out vec2 texCoord;
uniform bool chaos;
uniform bool confuse;
uniform bool shake;
uniform float time;	//系统当前时间
void main()
{
	gl_Position = vec4(vertex.xy, 0.0f, 1.0f);
	vec2 tex = vertex.zw;	//纹理坐标
	if (chaos)
	{
		float strength = 0.3;
		vec2 pos = vec2(tex.x + sin(time) * strength, tex.y + cos(time) * strength);
		texCoord = pos;		//通过旋转帧缓冲的纹理坐标达到镜头旋转的效果 用位置会导致纹理无拉伸
	}
	else if (confuse)
		texCoord = vec2(1.0 - tex.x, 1.0 - tex.y);
	else
		texCoord = tex;
	if (shake)
	{
		float strength = 0.01;
		gl_Position.x += cos(time * 10) * strength;
		gl_Position.y += cos(time * 15) * strength;		//用纹理会导致出现拉伸
	}
}

#shader fragment
#version 330 core
in vec2 texCoord;				//会插值进行补全
uniform bool chaos;				//两者的uniform并不互通
uniform bool confuse;
uniform bool shake;
uniform bool blurry;

uniform sampler2D scene;
uniform vec2 offset[9];			//用于计算偏移的向量		这些参数由外部进行传递而不用每次着色器都创建一次
uniform int edge_kernel[9];		//边缘卷积核
uniform float blur_kernel[9];	//模糊卷积核
void main()
{
	gl_FragColor = vec4(0.0f);
	vec3 samples[9];	//先把像素周围的颜色采样
	if (chaos || shake || blurry)
		for (int i = 0; i < 9; i++)
			samples[i] = vec3(texture(scene, texCoord + offset[i]));
	if (chaos)
	{
		for (int i = 0; i < 9; i++)
			gl_FragColor += vec4(samples[i] * edge_kernel[i], 0.0f);
		gl_FragColor.a = 1.0f;
	}
	else if (confuse)
		gl_FragColor = vec4(1 - texture(scene, texCoord).rgb, 1.0f);	//反转颜色
	else if (shake || blurry)
	{
		for (int i = 0; i < 9; i++)
			gl_FragColor += vec4(samples[i] * blur_kernel[i], 0.0f);
		gl_FragColor.a = 1.0f;
	}
	else
		gl_FragColor = texture(scene, texCoord);
}