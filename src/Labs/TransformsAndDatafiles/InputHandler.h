#pragma once
#include <map>
#include "GameObject.h"
#include "Game.h"
#include "PlayerObject.h"
#include "CameraComponent.h"
#include <vector>
#include "TransformComponent.h"
#include <ImGUI\imgui.h>
#include <ImGUI\imgui_impl_glfw_gl3.h>
#include <ImGUI\imgui_internal.h>


class InputCommand
{
public:
	virtual ~InputCommand() {}
	virtual void execute(GameObject& PlayerObject) = 0;
	virtual void execute(PlayerObject& player) = 0;
	//ImGuiIO& io = ImGui::GetIO();
};

class MoveLeftCommand : public InputCommand {
public:
	void execute(GameObject& PlayerObject) override {
		
	}
	void execute(PlayerObject& Player) override {
		if (Player.getComponent<TransformComponent>()) {
			Player.getComponent<TransformComponent>()->OnMessage("moveleft");
		}
	}
};

class MoveUpCommand : public InputCommand {
public:
	void execute(GameObject& PlayerObject) override {
		
	}
	void execute(PlayerObject& Player) override {
		if (Player.getComponent<TransformComponent>()) {
			Player.getComponent<TransformComponent>()->OnMessage("moveforward");
		}
	}
};

class MoveRightCommand : public InputCommand {
public:
	void execute(GameObject& PlayerObject) override {
	}
	void execute(PlayerObject& Player) override {
		if (Player.getComponent<TransformComponent>()) {
			Player.getComponent<TransformComponent>()->OnMessage("moveright");
		}
	}
};

class MoveDownCommand : public InputCommand {
public:
	void execute(GameObject& PlayerObject) override {
	}
	void execute(PlayerObject& Player) override {
		if (Player.getComponent<TransformComponent>()){
			Player.getComponent<TransformComponent>()->OnMessage("movebackward");
	}
}
};

class Changetolevel1 : public InputCommand {
public:
	void execute(GameObject& PlayerObject) override {
	}
	void execute(PlayerObject& Player) override {

		Player.changelevel = true;
		Player.level = 2;
	}
};

class Changetolevel2 : public InputCommand {
public:
	void execute(GameObject& PlayerObject) override {

	}
	void execute(PlayerObject& Player) override {

		Player.changelevel = true;
		Player.level = 3;
	}
};

class Changetolevel3 : public InputCommand {
public:
	void execute(GameObject& PlayerObject) override {
		
	}
	void execute(PlayerObject& Player) override {
		Player.changelevel = true;
		Player.level = 4;
	}
};

class ChangePerspective : public InputCommand {
public:
	void execute(GameObject& PlayerObject) override {
		
		}
	void execute(PlayerObject& Player) override {
			
	}
};

class nextPlanet : public InputCommand {
public:
	void execute(GameObject& PlayerObject) override {

	}
	void execute(PlayerObject& Player) override {
			//bool firstrun = false;
			//if (firstrun != true) {
			//	int lookingat = Player.currently_looking_at;
			//	lookingat = lookingat + 1;
			//	int maxsize = Player.lvlsize;
			//	if (lookingat == maxsize) {
			//		lookingat = 1;
			//	}
			//	Player.currently_looking_at = lookingat;
			//	Player.planetchange = true;
				//Player.splanetchange = true;
			//}

		
	}
	
};

class PreviousPlanet : public InputCommand {
public:
	void execute(GameObject& PlayerObject) override {
		
	}
	void execute(PlayerObject& Player) override {
			//bool firstrun = false;
			//int lookingat = Player.currently_looking_at;
			//int maxsize = Player.lvlsize;
			//int pressed = 0;
			//if (firstrun != true) {
			//	if (lookingat == 0) {
			//		lookingat = maxsize - 1;
			//		Player.currently_looking_at = lookingat;
			//		Player.planetchange = true;
			//	}
			//	else {
			//		lookingat = lookingat - 1;
			//		Player.currently_looking_at = lookingat;
			//		Player.planetchange = true;
			//	}
			//}


		
	}
	
};

class CameraMovement {
public:
	int sensitivity = 120;
	void execute(PlayerObject& obj, double m_x, double m_y, int window_x, int window_y) {
		float midx = window_x / 2;
		float midy = window_y / 2;

		double hMovement = (m_x - midx) / sensitivity;
		double vMovement = (m_y - midy) / sensitivity;

		//obj.getComponent<TransformComponent>()->pitch(vMovement);

		obj.getComponent<TransformComponent>()->yaw(hMovement);
	}
};




struct InputHandler
{	
	
	std::map<int, InputCommand*> m_controlMapping;
	std::vector<PlayerObject*> m_objectsthatlisten;
	CameraMovement camMove;
	InputHandler()
	{
		//key mapping
		m_controlMapping[65] = new MoveLeftCommand;
		m_controlMapping[87] = new MoveUpCommand;
		m_controlMapping[68] = new MoveRightCommand;
		m_controlMapping[83] = new MoveDownCommand;
		m_controlMapping[49] = new Changetolevel1;
		m_controlMapping[50] = new Changetolevel2;
		m_controlMapping[51] = new Changetolevel3;
		m_controlMapping[69] = new nextPlanet;
		m_controlMapping[81] = new PreviousPlanet;
	}

	void handleInputs(const std::vector<bool>& keyBuffer)
	{
		for(const auto& mapEntry : m_controlMapping)
		{
			if(keyBuffer[mapEntry.first])
			{
				for(auto object : m_objectsthatlisten)
				mapEntry.second->execute(*object);
			}
		}

	}
	void Addinput(PlayerObject* playerinput) {
		m_objectsthatlisten.push_back(playerinput);
	}
	void mouseinput(double m_x, double m_y, int window_x, int window_y) {
		if (m_objectsthatlisten.size() >= 1) {
				camMove.execute(*m_objectsthatlisten[0], m_x, m_y, window_x, window_y);

		}
	}
};