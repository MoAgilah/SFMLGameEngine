#pragma once
#include "../Utilities/Point.h"
#include <string>

class GameConstants
{
public:
	static bool DRender;
	static bool GameIsReady;
	static float FPS;
	static float Gravity;
	static float ObjectSpeed;
	static Point ScreenDim;
	static Point Scale;
	static std::string WindowTitle;
};