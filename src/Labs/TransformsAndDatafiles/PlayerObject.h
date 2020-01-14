#pragma once
#include "GameObject.h"
#include "CameraComponent.h"
#include "TransformComponent.h"
//#include "InputHandler.h"
//#include "Scene.h"
class PlayerObject : public GameObject
{
public:
	PlayerObject();
	void OnMessage(const std::string m);
	void OnUpdate(/*InputHandler& m_inputHandler, Scene& m_currentScene*/) ;

	
	int currently_looking_at = 1;
	std::string current_turn = "red";
	std::string getcurrentturn() {
		return current_turn;
	}
	bool splanetchange = true;
	bool renforcements = true;
	bool attack = false;
	bool manover = false;
	bool turnover = false;
	bool attackselection = false;
	int playernum;
	int lvlsize;
	int level;
	bool changelevel = false;
	bool planetchange = false;
	bool redalive;
	bool orangealive;
	bool greenalive;
	bool pinkalive;
	bool whitealive;
	bool purplealive;
	bool tealalive;
	bool greyalive;
};