#define USE_GLFW 1
#ifdef USE_GLFW
	#include "GLFW_EngineCore.h"
#endif

#include "Game.h"

int main(int argc, char* argv[])
{
	IEngineCore* engineCore;

#ifdef USE_GLFW
	engineCore = new GLFW_EngineCore;
#endif

	if (!engineCore->initWindow(1920, 1080, "Risk Game"))
		return -1;

	Game RiskGame;

	engineCore->runEngine(RiskGame);

	return 0;
}
