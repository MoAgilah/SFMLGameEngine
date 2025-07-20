#pragma once

#include "../../Utilities/Point.h"
#include <SFML/Graphics.hpp>
#include <string>

class GameConstants
{
public:
	static void Init();

	static bool DRender;
	static bool SFMLUsage;
	static bool GameIsReady;
	static float FPS;
	static float Gravity;
	static float ObjectSpeed;
	static sf::Color WindowColour;
	static Point ScreenDim;
	static Point Scale;
	static std::string WindowTitle;
	static std::string TileFilePaths;
	static std::string FontFilePaths;
	static std::string ShaderFilePaths;
	static std::string SoundFilePaths;
	static std::string MusicFilePaths;
	static std::string TextureFilePaths;
};