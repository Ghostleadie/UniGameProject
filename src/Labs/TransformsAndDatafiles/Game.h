#pragma once
#include "IEngineCore.h"
#include "InputHandler.h"
#include "GameObject.h"
#include "PlayerObject.h"
#include "CameraComponent.h"
#include "Scene.h"
#include <fstream>
#include <sstream>
#include "TransformComponent.h"
#include "ColourComponent.h"
#include "CameraComponent.h"
#include <json/json/Json/json.h>
#include <SFML\Graphics.hpp>
#include "ModelComponent.h"
#include <ImGUI\imgui.h>
#include <ImGUI\imgui_impl_glfw_gl3.h>
#include <ImGUI\imgui_internal.h>
#include <SFML\Audio\Music.hpp>

class Game
{
public:
	Game();
	IEngineCore* m_engineInterfacePtr;
	InputHandler* m_inputHandler;
	Scene* m_currentScene;

	//sf::Music background;

	void playermovement();
	void render();
	void Gamelogic(PlayerObject& Player);
	void AI_Behave(PlayerObject& Player);
	void gui(PlayerObject& Player);
	void menusetup();
	void onMessage(const std::string m);
	void cam_Movement(std::string team, PlayerObject& Player);
	void att_cam_Movement(std::string team, PlayerObject& Player);
	void attackCalculation(std::string turn, PlayerObject& Player);
	void remainingplayers(PlayerObject& player);
	void playerendturn(PlayerObject& player);
	
	//game logic variable
	std::string CurrentLevel;
	std::vector<std::string> v_Levels;
	bool gameover = false;
	bool to_far = false;
	std::string winner;

	//gui variables
	bool Ismenu = true;
	bool GameStart = false;
	int attackerfleets;
	int defenderfleets;
	int attackerfleetspos;
	int defenderfleetspos;
	int fleetdeployment;
	bool menu = true;
	sf::Clock Timer;
	int sTime;
	float seed;
	int gamesize;
	int wWidth, wHeight;
	bool fleetredeployment;
	bool resupply = false;
	int redeployed_fleets;
	bool renforcements_cal = false;
	int attackfrom;
	int attacking;
	int Playerpos;
	int Nplayer;
	int renforcementnumber = 0;
	bool to_few;
	bool attackingself;
	
	int playerseed = 0;
	
	int redscore = 0;
	int orangescore = 0;
	int greenscore = 0;
	int pinkscore = 0;
	int whitescore = 0;
	int purplescore = 0;
	int tealscore = 0;
	int greyscore = 0;
	int AIscore = 0;

	int redplanets = 0;
	int orangeplanets = 0;
	int greenplanets = 0;
	int pinkplanets = 0;
	int whiteplanets = 0;
	int purplesplanets = 0;
	int tealsplanets = 0;
	int greysplanets = 0;
	int AIsplanets = 0;

	
	//log
	bool defendermessage = false;
	bool attackermessage = false;
	bool defeatmessage = false;
	bool victorymessage = false;

	//AI
	bool AI_renforcements_cal = false;
	int AIattackfleet;
	bool AI_Range = true;
	bool AI_RangeCheck = true;
	bool AI_Turn = false;
	bool AI_Attack = false;
	bool AI_Player = false;
	
	
	
	//float randomX;
	//float randomZ;
	glm::vec3 scale;
	
	bool test = false;
	bool rclicked = false;
	bool attackover = false;
	bool imUpdate = false;
	

	int Getgamesize();

private:
	GameObject m_playerBackground;
	//GameObject m_playerCube;
	//GameObject m_playerCube2;

	//std::vector<GameObject> v_playerCubes;

	//std::string s;
	//std::stringstream ss;
	//
	//int numberofElements;
	//int w;
	//int x;
	//int y;
	//int z;

	//CameraComponent m_camera;
};
