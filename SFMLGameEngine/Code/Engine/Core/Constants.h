#pragma once

#include "../../Utilities/Traits.h"
#include "../../Utilities/Point.h"
#include "../../Utilities/Colour.h"
#include <string>

#if USE_SFML

#include "../../Platform/SFML/Drawables/SFFont.h"
#include "../../Platform/SFML/Resource/SFShader.h"

using ActiveFontTrait = FontTrait<SFFont>;
using ActiveShaderTrait = ShaderTrait<SFShader>;

#endif


class GameConstants
{
public:
	static void Init();

	static bool DRender;
	static bool GameIsReady;
	static float FPS;
	static float Gravity;
	static float ObjectSpeed;
	static Colour WindowColour;
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