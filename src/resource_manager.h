#pragma once
class mString;
class Shader;
class Texture2D;

/// @brief manager the shaders and textures and is a static class
class ResourceManager
{
private:
	ResourceManager() = default;
	~ResourceManager() = default;
public:
	/// @brief it should be initialized once only
	static void init();

	/// @brief clear all the shaders and textures when program terminated and should clear once only
	static void clear();
	
	static mString readFile(const mString& file);

	static Shader& getShader(const mString& name);
	static Texture2D& getTexture(const mString& name);
};