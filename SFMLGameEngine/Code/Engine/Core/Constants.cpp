#include "Constants.h"

bool  GameConstants::DRender = false;
bool  GameConstants::GameIsReady = false;
float GameConstants::FPS = 60.f;
float GameConstants::ObjectSpeed = 2.f;
Colour GameConstants::WindowColour = Colour(0, 0, 255);
Point GameConstants::ScreenDim = Point(600.f, 600.f);
Point GameConstants::Scale = Point(1.f, 1.f);
std::string GameConstants::WindowTitle = "SFML Game Engine";
std::string GameConstants::TileFilePaths = "../Resources/TileTypes.txt";
std::string GameConstants::FontFilePaths = "../SFMLGameEngine/SFMLGameEngine/Resources/Fonts/";
std::string GameConstants::ShaderFilePaths = "../SFMLGameEngine/SFMLGameEngine/Resources/Shaders/";
std::string GameConstants::SoundFilePaths = "../SFMLGameEngine/SFMLGameEngine/Resources/Sounds/";
std::string GameConstants::MusicFilePaths = "../SFMLGameEngine/SFMLGameEngine/Resources/Music/";
std::string GameConstants::TextureFilePaths = "../SFMLGameEngine/SFMLGameEngine/Resources/Textures/";

void GameConstants::Init()
{
	// Touch the values so linker pulls this file in early
	(void)DRender;
	(void)GameIsReady;
	(void)FPS;
	(void)WindowColour;
	(void)ScreenDim;
	(void)Scale;
	(void)WindowTitle;
	(void)TileFilePaths;
	(void)FontFilePaths;
	(void)ShaderFilePaths;
	(void)SoundFilePaths;
	(void)MusicFilePaths;
	(void)TextureFilePaths;
}