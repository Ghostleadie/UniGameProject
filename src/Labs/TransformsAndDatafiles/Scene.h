#pragma once
#include "IEngineCore.h"
#include "InputHandler.h"
#include "Game.h"
#include "PlayerObject.h"
#include "ModelComponent.h"
#include "CameraComponent.h"
#include <fstream>
#include <sstream>
#include "Serializer.h"
#include <SFML\Graphics.hpp>

class Scene {
public:
	void menu();
	void createLevel(int gSize, int Nplayers, bool AI, int playerSeed);

	std::vector<GameObject> v_LevelObjects;

	PlayerObject player;
	int size;
	int tsize;
	int getobjectsize();
	char planetteam;
	Model* RedPlanet = new Model("assets/Models/sphere.obj");
	Model* orangePlanet = new Model("assets/Models/sphere2.obj");
	Model* GreenPlanet = new Model("assets/Models/sphere3.obj");
	Model* PinkPlanet = new Model("assets/Models/sphere4.obj");
	Model* WhitePlanet = new Model("assets/Models/sphere5.obj");
	Model* PurplePlanet = new Model("assets/Models/sphere6.obj");
	Model* TealPlanet = new Model("assets/Models/sphere11.obj");
	Model* GreyPlanet = new Model("assets/Models/sphere8.obj");
	Model* YellowPlanet = new Model("assets/Models/sphere9.obj");
	
	bool playerspawned = false;
	Scene() {
	}
private:
	
	bool seedgenerated = false;
	bool firstRun = true;
	float seed;
	sf::Clock Timer;
	int sTime;
	std::string s;
	std::stringstream ss;
	bool playercheck = false;
	bool player1 = true;
	bool player2 = false;
	bool player3 = false;
	bool player4 = false;
	bool player5 = false;
	bool player6 = false;
	bool player7 = false;
	bool player8 = false;
	bool AI_player = false;

	
	int seedpos;
	int numberofElements;
	int w;
	int x;
	int y;
	int z;
};

/*class Planet : public JsonSerializer
{
public:
	Planet(void);
	virtual void Jserialize(Json::Value& root);
	virtual void Jdeserialize(Json::Value& root);
private:
	std::string name;
	std::string object;
	int positionx;
	int positiony;
	int positionz;
};*/