#include "Game.h"
#include "TransformComponent.h"
#include "ColourComponent.h"
#include "CameraComponent.h"
#include "Scene.h"
#include <fstream>
#include <sstream>
#include <SFML\Graphics.hpp>
#include "ModelComponent.h"
#include <ImGUI\imgui.h>
#include <ImGUI\imgui_impl_glfw_gl3.h>
#include <ImGUI\imgui_internal.h>
#include <ImGUI\imconfig.h>
#include "IEngineCore.h"
#include "InputHandler.h"
#include "GameObject.h"
#include "PlayerObject.h"
#include "TransformComponent.h"
#include "ColourComponent.h"
#include "CameraComponent.h"
#include "VectorCombo.h"

Game::Game()
{
	m_engineInterfacePtr = nullptr;
	m_inputHandler = new InputHandler();
	m_currentScene = new Scene;
	menusetup();
}

void Game::onMessage(const std::string m) {
	
}

void Game::playermovement()
{
	if (CurrentLevel != "menu") {
		if (m_inputHandler->m_objectsthatlisten.size() >= 1) {
				TransformComponent* transform = m_currentScene->player.getComponent<TransformComponent>();

				glm::quat inverseOrientation = inverse(transform->m_orientation);

				m_currentScene->player.getComponent<CameraComponent>()->m_position = -(transform->m_position + (glm::vec3(0, 2, 5) * glm::inverse(transform->m_orientation)));
				m_currentScene->player.getComponent<CameraComponent>()->m_orientation = glm::inverse(-transform->m_orientation);
		}
	}
}

void Game::cam_Movement(std::string team, PlayerObject& Player) {
	Playerpos = Player.currently_looking_at;
	if (Player.planetchange == true && Playerpos != 0) {
		
			
		if (m_currentScene->v_LevelObjects[Playerpos].team == team) {
			glm::vec3 newcampos = m_currentScene->v_LevelObjects[Playerpos].getComponent<TransformComponent>()->position();
			Player.getComponent<TransformComponent>()->m_position = newcampos;
			std::cout << "player moved to " << m_currentScene->v_LevelObjects[Playerpos].getName() << std::endl;
			Player.planetchange = false;
			//Player.splanetchange = false;
		}
	
	}
}

void Game::att_cam_Movement(std::string team, PlayerObject& Player) {
	Playerpos = Player.currently_looking_at;
	if (Player.planetchange == true && Playerpos != 0) {
		if (m_currentScene->v_LevelObjects[Playerpos].team != team) {
			glm::vec3 newcampos = m_currentScene->v_LevelObjects[Playerpos].getComponent<TransformComponent>()->position();
			Player.getComponent<TransformComponent>()->m_position = newcampos;
			std::cout << "player moved to " << m_currentScene->v_LevelObjects[Playerpos].getName() << std::endl;
			Player.planetchange = false;
			//Player.splanetchange = false;
		}
		
	}
}

void Game::attackCalculation(std::string turn, PlayerObject& Player) {
		if (attackfrom != 0 && attacking != 0) {
			if (m_currentScene->v_LevelObjects[attackfrom].team.c_str() != m_currentScene->v_LevelObjects[attacking].team.c_str()) {
				int attackernum = m_currentScene->v_LevelObjects[attackfrom].getFleets();
				attackernum = attackernum - 1;
				int attacker = m_currentScene->v_LevelObjects[attackfrom].getFleets();
				int defender = m_currentScene->v_LevelObjects[attacking].getFleets();
				std::cout << m_currentScene->v_LevelObjects[attackfrom].name << " is attacking " << m_currentScene->v_LevelObjects[attacking].name << std::endl;
				int a = m_currentScene->v_LevelObjects[attackfrom].getComponent<TransformComponent>()->position().x;
				int a2 = m_currentScene->v_LevelObjects[attacking].getComponent<TransformComponent>()->position().x;
				int c = m_currentScene->v_LevelObjects[attackfrom].getComponent<TransformComponent>()->position().z;
				int c2 = m_currentScene->v_LevelObjects[attacking].getComponent<TransformComponent>()->position().z;
				int AC = floor(sqrt((a*a2) + (c*c2)));
				if (AC > 100) { //distance between planets

					attackover = true;
					to_far = true;
				}


			if (attackover == false) {
				do {
					int roll = Timer.getElapsedTime().asMicroseconds();
					//defendermessage = false;
					//attackermessage = false;
					srand(roll);
					int attroll = (rand() % 6 + 1);
					int defroll = (rand() % 6 + 1);
					if (attroll > defroll) {
						defender--;
						std::cout << "Defender lost a fleet" << std::endl;
						defendermessage = true;
						//imguicombatpopup();
					}
					else if (attroll < defroll) {
						attacker--;
						std::cout << "Attacker lost a fleet" << std::endl;
						//attackermessage = true;
						//imguicombatpopup();
					}
					else if (attroll == defroll) {
						attacker--;
						std::cout << "Attacker lost a fleet" << std::endl;
						//attackermessage = true;
					}
				} while (attacker != 0 && defender != 0);
				if ((attacker == 0 || defender == 0) && attackover == false) {
					if (attacker == 0) {
						std::cout << "Defenders wins" << std::endl;
						m_currentScene->v_LevelObjects[attackfrom].Fleets = 1;
						m_currentScene->v_LevelObjects[attacking].Fleets = defender;
						Player.attackselection = false;
						attackfrom = 0;
						attacking = 0;
						//defeatmessage = true;
						attackover = true;
						Player.planetchange = true;
						cam_Movement(Player.current_turn, m_currentScene->player);
					}
					if (defender == 0) {
						std::cout << "Attacker wins" << std::endl;
						attackerfleets = attacker;
						//m_currentScene->v_LevelObjects[attacking].Fleets = attacker;
						if (turn == "red") {
							m_currentScene->v_LevelObjects[attacking].addComponent(new ModelComponent(m_currentScene->RedPlanet));
						}
						else if (turn == "orange") {
							m_currentScene->v_LevelObjects[attacking].addComponent(new ModelComponent(m_currentScene->orangePlanet));
						}
						else if (turn == "green") {
							m_currentScene->v_LevelObjects[attacking].addComponent(new ModelComponent(m_currentScene->GreenPlanet));
						}
						else if (turn == "pink") {
							m_currentScene->v_LevelObjects[attacking].addComponent(new ModelComponent(m_currentScene->PinkPlanet));
						}
						else if (turn == "white") {
							m_currentScene->v_LevelObjects[attacking].addComponent(new ModelComponent(m_currentScene->WhitePlanet));
						}
						else if (turn == "purple") {
							m_currentScene->v_LevelObjects[attacking].addComponent(new ModelComponent(m_currentScene->PurplePlanet));
						}
						else if (turn == "teal") {
							m_currentScene->v_LevelObjects[attacking].addComponent(new ModelComponent(m_currentScene->TealPlanet));
						}
						else if (turn == "grey") {
							m_currentScene->v_LevelObjects[attacking].addComponent(new ModelComponent(m_currentScene->GreyPlanet));
						}
						else if (turn == "yellow") {

							m_currentScene->v_LevelObjects[attacking].addComponent(new ModelComponent(m_currentScene->YellowPlanet));
							std::cout << "ai took a planet" << std::endl;
						}
						m_currentScene->v_LevelObjects[attacking].team = turn;
						Player.attackselection = false;
						attackerfleetspos = attackfrom;
						defenderfleetspos = attacking;
						attackfrom = 0;
						attacking = 0;
						if (turn != "yellow") {
							fleetredeployment = true;
						}
						//victorymessage = true;
						attackover = true;

					}
				}
			}
			attackover = false;

		}
		else {
			attackingself = true;
		}
	}

	
}

void Game::remainingplayers(PlayerObject& player) {
	
	redplanets = 0;
	orangeplanets = 0;
	greenplanets = 0;
	pinkplanets = 0;
	whiteplanets = 0;
	purplesplanets = 0;
	tealsplanets = 0;
	greysplanets = 0;
	AIsplanets = 0;



	for (int i = 0; i < m_currentScene->v_LevelObjects.size(); i++) {
		if (m_currentScene->v_LevelObjects[i].team == "red") {
			redplanets++;
		}
		else if (m_currentScene->v_LevelObjects[i].team == "orange") {
			orangeplanets++;
		}
		else if (m_currentScene->v_LevelObjects[i].team == "green") {
			greenplanets++;
		}
		else if (m_currentScene->v_LevelObjects[i].team == "pink") {
			pinkplanets++;
		}
		else if (m_currentScene->v_LevelObjects[i].team == "white") {
			whiteplanets++;
		}
		else if (m_currentScene->v_LevelObjects[i].team == "purple") {
			purplesplanets++;
		}
		else if (m_currentScene->v_LevelObjects[i].team == "teal") {
			tealsplanets++;
		}
		else if (m_currentScene->v_LevelObjects[i].team == "grey") {
			greysplanets++;
		}
		else if (m_currentScene->v_LevelObjects[i].team == "yellow") {
			AIsplanets++;
		}


	}
	if (redplanets == 0) {
		player.redalive = false;
	}
	else { player.redalive = true; }
	if (orangeplanets == 0) {
		player.orangealive = false;
	}
	else { player.orangealive = true; }
	if (greenplanets == 0) {
		player.greenalive = false;
	}
	else { player.greenalive = true; }
	if (pinkplanets == 0) {
		player.pinkalive = false;
	}
	else { player.pinkalive = true; }
	if (whiteplanets == 0) {
		player.whitealive = false;
	}
	else { player.whitealive = true; }
	if (purplesplanets == 0) {
		player.purplealive = false;
	}
	else { player.purplealive = true; }
	if (tealsplanets == 0) {
		player.tealalive = false;
	}
	else { player.tealalive = true; }
	if (greysplanets == 0) {
		player.greyalive = false;
	}
	else { player.greyalive = true; }
	if (AIsplanets == 0) {
		AI_Player = false;
	}
	
}

void Game::Gamelogic(PlayerObject& Player) {
	//player 1 red
	//ImGuiIO& io = ImGui::GetIO();
	//ImGuiIO io = ImGui::GetIO();
	// Open it from an audio file
	/*
	if (!background.openFromFile("assets\Sound\background.wav"))
	{
		// error...
	}
	// Change some parameters
	background.setPosition(0, 1, 10); // change its 3D position
	background.setPitch(2);           // increase the pitch
	background.setVolume(20);         // reduce the volume
	background.setLoop(true);         // make it loop
								 // Play it
	background.play();
	
	*/
	if (CurrentLevel == "menu") {
		redplanets = 0;
		orangeplanets = 0;
		greenplanets = 0;
		pinkplanets = 0;
		whiteplanets = 0;
		purplesplanets = 0;
		tealsplanets = 0;
		greysplanets = 0;
		AIsplanets = 0;
	}


	if (CurrentLevel == "game") {
		if (GameStart == false) {
			Player.planetchange = true;
			cam_Movement("red", m_currentScene->player);
			GameStart = true;
		}
		//if (start == false) {
		//	player_turn[0] = true;
		//	start == true;
		//}
		
		



		if (Player.current_turn == "red") {
			if (Player.attackselection == false) {
				cam_Movement(std::string("red"), m_currentScene->player);
			}
			else if (Player.attackselection == true) {
				att_cam_Movement(std::string("red"), m_currentScene->player);
			}
			if (Player.renforcements == true) {
				if (renforcements_cal != true) {
					for (int i = 0; i < m_currentScene->v_LevelObjects.size(); i++) {
						if (m_currentScene->v_LevelObjects[i].team == "red") {
							renforcementnumber++;
						}
					}
					fleetdeployment = floor(renforcementnumber / 2);
					if (fleetdeployment < 3) {
						fleetdeployment = 3;
					}
					renforcements_cal = true;
				}
				if (fleetdeployment == 0) {
					Player.renforcements = false;
					Player.attack = true;
					renforcementnumber = 0;
					renforcements_cal = false;
				}
			}
			if (Player.attack == true) {
				/*if (attackfrom != 0 && attacking != 0) {
					int attackernum = m_currentScene->v_LevelObjects[attackfrom].getFleets();
					attackernum = attackernum - 1;
					int attacker = m_currentScene->v_LevelObjects[attackfrom].getFleets();
					int defender = m_currentScene->v_LevelObjects[attacking].getFleets();
					std::cout << m_currentScene->v_LevelObjects[attackfrom].name << " is attacking " << m_currentScene->v_LevelObjects[attacking].name << std::endl;
					if(attackover == false){
					do{
						int roll = Timer.getElapsedTime().asMicroseconds();
							defendermessage = false;
							attackermessage = true;
							srand(roll);
							int attroll = (rand() % 6 + 1);
							int defroll = (rand() % 6 + 1);
							if (attroll > defroll) {
								defender--;
								std::cout << "Defender lost a fleet" << std::endl;
								defendermessage = true;
								//imguicombatpopup();
							}
							else if (attroll < defroll) {
								attacker--;
								std::cout << "Attacker lost a fleet" << std::endl;
								attackermessage = true;
								//imguicombatpopup();
							}
							else if (attroll == defroll) {
								std::cout << "Draw no winner" << std::endl;
							}
					} while (attacker != 0 && defender != 0);
					if ((attacker == 0 || defender == 0) && attackover == false) {
						if (attacker == 0) {
							std::cout << "Defenders wins" << std::endl;
							m_currentScene->v_LevelObjects[attackfrom].Fleets = 1;
							m_currentScene->v_LevelObjects[attacking].Fleets = defender;
							Player.attackselection = false;
							attackfrom = 0;
							attacking = 0;
							//defeatmessage = true;
							attackover = true;
						}
						if (defender == 0) {
							std::cout << "Attacker wins" << std::endl;
							attackerfleets = attacker;
							//m_currentScene->v_LevelObjects[attacking].Fleets = attacker;
							m_currentScene->v_LevelObjects[attacking].addComponent(new ModelComponent(m_currentScene->RedPlanet));
							m_currentScene->v_LevelObjects[attacking].team = "red";
							Player.attackselection = false;
							attackerfleetspos = attackfrom;
							defenderfleetspos = attacking;
							attackfrom = 0;
							attacking = 0;
							fleetredeployment = true;
							//victorymessage = true;
							attackover = true;
							
						}
					}
				}
				attackover = false;
		
				}*/

				attackCalculation(Player.current_turn, m_currentScene->player);
			}



			//player_turn[0] = false;
			//player_turn[1] = true;
		}
		//player 2 orange
		else if (Player.current_turn == "orange") {
			if (Player.attackselection == false) {
				cam_Movement(std::string("orange"), m_currentScene->player);
			}
			else if (Player.attackselection == true) {
				att_cam_Movement(std::string("orange"), m_currentScene->player);
			}
			if (Player.renforcements == true) {
				if (renforcements_cal != true) {
					for (int i = 0; i < m_currentScene->v_LevelObjects.size(); i++) {
						if (m_currentScene->v_LevelObjects[i].team == "orange") {
							renforcementnumber++;
						}
					}
					fleetdeployment = floor(renforcementnumber / 2);
					if (fleetdeployment < 3) {
						fleetdeployment = 3;
					}
					renforcements_cal = true;
				}
				if (fleetdeployment == 0) {
					renforcementnumber = 0;
					Player.renforcements = false;
					Player.attack = true;
					renforcements_cal = false;
				}
			}
			if (Player.attack == true) {
				attackCalculation(Player.current_turn, m_currentScene->player);
			}

		}
		//player 3 green
		else if (Player.current_turn == "green") {
			if (Player.attackselection == false) {
				cam_Movement(std::string("green"), m_currentScene->player);
			}
			else if (Player.attackselection == true) {
				att_cam_Movement(std::string("green"), m_currentScene->player);
			}
			if (Player.renforcements == true) {
				if (renforcements_cal != true) {
					for (int i = 0; i < m_currentScene->v_LevelObjects.size(); i++) {
						if (m_currentScene->v_LevelObjects[i].team == "green") {
							renforcementnumber++;
						}
					}
					fleetdeployment = floor(renforcementnumber / 2);
					if (fleetdeployment < 3) {
						fleetdeployment = 3;
					}
					renforcements_cal = true;
				}
				if (fleetdeployment == 0) {
					renforcementnumber = 0;
					Player.renforcements = false;
					Player.attack = true;
					renforcements_cal = false;
				}
			}
			if (Player.attack == true) {
				attackCalculation(Player.current_turn, m_currentScene->player);
			}

		}
		//player 4 pink
		else if (Player.current_turn == "pink") {
			if (Player.attackselection == false) {
				cam_Movement(std::string("pink"), m_currentScene->player);
			}
			else if (Player.attackselection == true) {
				att_cam_Movement(std::string("pink"), m_currentScene->player);
			}
			if (Player.renforcements == true) {
				if (renforcements_cal != true) {
					for (int i = 0; i < m_currentScene->v_LevelObjects.size(); i++) {
						if (m_currentScene->v_LevelObjects[i].team == "pink") {
							renforcementnumber++;
						}
					}
					fleetdeployment = floor(renforcementnumber / 2);
					if (fleetdeployment < 3) {
						fleetdeployment = 3;
					}
					renforcements_cal = true;
				}
				if (fleetdeployment == 0) {
					renforcementnumber = 0;
					Player.renforcements = false;
					Player.attack = true;
					renforcements_cal = false;
				}
			}
			if (Player.attack == true) {
				attackCalculation(Player.current_turn, m_currentScene->player);
			}

		}
		//player 5 white
		else if (Player.current_turn == "white") {
			if (Player.attackselection == false) {
				cam_Movement(std::string("white"), m_currentScene->player);
			}
			else if (Player.attackselection == true) {
				att_cam_Movement(std::string("white"), m_currentScene->player);
			}
			if (Player.renforcements == true) {
				if (renforcements_cal != true) {
					for (int i = 0; i < m_currentScene->v_LevelObjects.size(); i++) {
						if (m_currentScene->v_LevelObjects[i].team == "white") {
							renforcementnumber++;
						}
					}
					fleetdeployment = floor(renforcementnumber / 2);
					if (fleetdeployment < 3) {
						fleetdeployment = 3;
					}
					renforcements_cal = true;
				}
				if (fleetdeployment == 0) {
					renforcementnumber = 0;
					Player.renforcements = false;
					Player.attack = true;
					renforcements_cal = false;
				}
			}
			if (Player.attack == true) {
				attackCalculation(Player.current_turn, m_currentScene->player);
			}

		}
		//player 6 purple
		else if (Player.current_turn == "purple") {
			if (Player.attackselection == false) {
				cam_Movement(std::string("purple"), m_currentScene->player);
			}
			else if (Player.attackselection == true) {
				att_cam_Movement(std::string("purple"), m_currentScene->player);
			}
			if (Player.renforcements == true) {
				if (renforcements_cal != true) {
					for (int i = 0; i < m_currentScene->v_LevelObjects.size(); i++) {
						if (m_currentScene->v_LevelObjects[i].team == "purple") {
							renforcementnumber++;
						}
					}
					fleetdeployment = floor(renforcementnumber / 2);
					if (fleetdeployment < 3) {
						fleetdeployment = 3;
					}
					renforcements_cal = true;
				}
				if (fleetdeployment == 0) {
					renforcementnumber = 0;
					Player.renforcements = false;
					Player.attack = true;
					renforcements_cal = false;
				}
			}
			if (Player.attack == true) {
				attackCalculation(Player.current_turn, m_currentScene->player);
			}
		}
		//player 7 teal
		else if (Player.current_turn == "teal") {
			if (Player.attackselection == false) {
				cam_Movement(std::string("teal"), m_currentScene->player);
			}
			else if (Player.attackselection == true) {
				att_cam_Movement(std::string("teal"), m_currentScene->player);
			}
			if (Player.renforcements == true) {
				if (renforcements_cal != true) {
					for (int i = 0; i < m_currentScene->v_LevelObjects.size(); i++) {
						if (m_currentScene->v_LevelObjects[i].team == "teal") {
							renforcementnumber++;
						}
					}
					fleetdeployment = floor(renforcementnumber / 2);
					if (fleetdeployment < 3) {
						fleetdeployment = 3;
					}
					renforcements_cal = true;
				}
				if (fleetdeployment == 0) {
					renforcementnumber = 0;
					Player.renforcements = false;
					Player.attack = true;
					renforcements_cal = false;
				}
			}
			if (Player.attack == true) {
				attackCalculation(Player.current_turn, m_currentScene->player);
			}
		}
		//player 8 grey
		else if (Player.current_turn == "grey") {
			if (Player.attackselection == false) {
				cam_Movement(std::string("grey"), m_currentScene->player);
			}
			else if (Player.attackselection == true) {
				att_cam_Movement(std::string("grey"), m_currentScene->player);
			}
			if (Player.renforcements == true) {
				if (renforcements_cal != true) {
					for (int i = 0; i < m_currentScene->v_LevelObjects.size(); i++) {
						if (m_currentScene->v_LevelObjects[i].team == "grey") {
							renforcementnumber++;
						}
					}
					fleetdeployment = floor(renforcementnumber / 2);
					if (fleetdeployment < 3) {
						fleetdeployment = 3;
					}
					renforcements_cal = true;
				}
				if (fleetdeployment == 0) {
					renforcementnumber = 0;
					Player.renforcements = false;
					Player.attack = true;
					renforcements_cal = false;
				}
			}
			if (Player.attack == true) {
				attackCalculation(Player.current_turn, m_currentScene->player);
			}
		}
	}
}

/*namespace ImGui
{
	static auto vector_getter = [](void* vec, int idx, const char** out_text)
	{
		auto& vector = *static_cast<std::vector<std::string>*>(vec);
		if (idx < 0 || idx >= static_cast<int>(vector.size())) { return false; }
		*out_text = vector.at(idx).c_str();
		return true;
	};

	bool Combo(const char* label, int* currIndex, std::vector<std::string>& values)
	{
		if (values.empty()) { return false; }
		return Combo(label, currIndex, vector_getter,
			static_cast<void*>(&values), values.size());
	}

	bool ListBox(const char* label, int* currIndex, std::vector<std::string>& values)
	{
		if (values.empty()) { return false; }
		return ListBox(label, currIndex, vector_getter,
			static_cast<void*>(&values), values.size());
	}

}*/

void Game::playerendturn(PlayerObject& player) {
	while (m_currentScene->v_LevelObjects[player.currently_looking_at].team != player.current_turn) {
		player.currently_looking_at = player.currently_looking_at + 1;
		if (player.currently_looking_at == m_currentScene->v_LevelObjects.size()) {
			player.currently_looking_at = 1;
		}
	}
	player.planetchange = true;
	cam_Movement(player.current_turn, m_currentScene->player);
}

void Game::gui(PlayerObject& player)
{

	if (Playerpos == m_currentScene->v_LevelObjects.size()) {
		Playerpos = 1;
	}
	else if (Playerpos == 0) {
		Playerpos = m_currentScene->v_LevelObjects.size();
	}

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoResize;
	window_flags |= ImGuiWindowFlags_NoCollapse;
	window_flags |= ImGuiWindowFlags_NoMove;
	bool* p_open = false;
	static bool* AI = false;
	static int clicked = 0;
	static int b1clicked = 0;
	static int b2clicked = 0;



	ImGui_ImplGlfwGL3_NewFrame();
	//ImGui::SetNextWindowSize(ImVec2(250, 300));
	if (ImGui::Begin("Risk", p_open, window_flags)) {
		ImGui::SetWindowPos(ImVec2(0, 0));
		ImGui::SetWindowSize(ImVec2(wWidth*0.20, wHeight*0.35));

		if (Ismenu == true) {
			ImGui::Text("Welcome, to a game of RISK.");
			if (ImGui::Button("Start"))
				ImGui::OpenPopup("Game Set Up");
			if (ImGui::BeginPopupModal("Game Set Up", NULL, window_flags))
			{
				ImGui::Text("How many players do you want?");
				static int players = 0;
				ImGui::Combo("players", &players, " 2 \0 3 \0 4 \0 5 \0 6 \0 7 \0 8 \0\0");
				ImGui::Checkbox("Add an AI Player", &AI_Player);
				ImGui::Text("Leave seed input blank for a random seed");
				static int seed = 0;
				ImGui::InputInt("Seed input", &seed);
				if (ImGui::Button("Start"))
					clicked++;
				if (seed != 0) {
					playerseed = seed;
					}
				if (clicked & 1)
				{

					if (players == 0) {
						std::cout << "2 players" << std::endl;
						Ismenu = false;
						gamesize = 1;
						CurrentLevel = "game";
						test = true;
						Nplayer = 2;
						gameover = false;
						m_currentScene->createLevel(gamesize, Nplayer, AI_Player, playerseed);
					}
					if (players == 1) {
						std::cout << "3 players" << std::endl;
						Ismenu = false;
						sTime = Timer.getElapsedTime().asMilliseconds();
						srand(sTime);
						seed = (rand() % 500 + 1);
						gamesize = 1;
						CurrentLevel = "game";
						Nplayer = 3;
						gameover = false;
						m_currentScene->createLevel(gamesize, Nplayer, AI_Player, playerseed);
					}
					if (players == 2) {
						std::cout << "4 players" << std::endl;
						Ismenu = false;
						gamesize = 1;
						CurrentLevel = "game";
						Nplayer = 4;
						gameover = false;
						m_currentScene->createLevel(gamesize, Nplayer, AI_Player, playerseed);
					}
					if (players == 3) {
						std::cout << "5 players" << std::endl;
						Ismenu = false;
						gamesize = 2;
						CurrentLevel = "game";
						Nplayer = 5;
						gameover = false;
						m_currentScene->createLevel(gamesize, Nplayer, AI_Player, playerseed);
					}
					if (players == 4) {
						std::cout << "6 players" << std::endl;
						Ismenu = false;
						gamesize = 2;
						CurrentLevel = "game";
						Nplayer = 6;
						gameover = false;
						m_currentScene->createLevel(gamesize, Nplayer, AI_Player, playerseed);
					}
					if (players == 5) {
						std::cout << "7 players" << std::endl;
						Ismenu = false;
						gamesize = 2;
						CurrentLevel = "game";
						Nplayer = 7;
						gameover = false;
						m_currentScene->createLevel(gamesize, Nplayer, AI_Player, playerseed);
					}
					if (players == 6) {
						std::cout << "8 players" << std::endl;
						Ismenu = false;
						gamesize = 3;
						CurrentLevel = "game";
						Nplayer = 8;
						gameover = false;
						m_currentScene->createLevel(gamesize, Nplayer, AI_Player, playerseed);
					}
					if (AI_Player == true) {
						std::cout << "AI player added" << std::endl;
						
					}
					ImGui::CloseCurrentPopup();
				}
				clicked = 0;
				if (ImGui::Button("Back"))
					ImGui::CloseCurrentPopup();
				ImGui::EndPopup();
			}
			if (ImGui::Button("Tutorial")) 
				ImGui::OpenPopup("Tutorial");
				if (ImGui::BeginPopupModal("Tutorial", NULL, window_flags))
				{
					ImGui::Text("Phase 1: The renforcement stage. This is where you can add more fleets to your planets so that you can attack other planets or have a more defended planet.");
					ImGui::Text("Phase 2: The attack stage. In this stage the player can attack as many planets as they want as long as they still have 1 fleet to defend the planet they are attacking from.");
					ImGui::Text("Phase 3: The re-deployment stage. This is where you can move fleets from one planet to another one of your planets.");
					if (ImGui::Button("Close"))
						ImGui::CloseCurrentPopup();
					ImGui::EndPopup();
				}
			
			if (ImGui::Button("Exit"))
				std::_Exit(EXIT_FAILURE);
			ImGui::Text("Created by Jack Brewer.");
		}
		if (Ismenu == false) {
			if (AI_Turn == false) {
				//char turn = player.current_turn.c_str;

				//ImGui::Text(turn); ImGui::SameLine(); ImGui::Text(" turn");
				if (player.renforcements == true) {

					ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + 300.0f);
					ImGui::Text("This is the renforcement stage. Cycle to the planet or planets you want to add extra space fleets to then click renforce planet. Once all your fleets renforcements are gone you can move onto the next phase.");
					ImGui::Text("%s", player.current_turn.c_str()); ImGui::SameLine(); ImGui::Text("teams turn");
					int planet = player.currently_looking_at;
					int ren = fleetdeployment;
					int newnum = m_currentScene->v_LevelObjects[planet].getFleets();
					int starting_number = m_currentScene->v_LevelObjects[planet].Fleets;
					static int current_number = 0;
					ImGui::SliderInt("Fleets", &current_number, 0.0f, ren);
					//ImGui::Text((const char*)ren);  ImGui::SameLine(); ImGui::Text(" space fleets left for renforcements.");
					if (ImGui::Button("Add Fleets")) {
						fleetdeployment = ren - current_number;

						newnum = newnum + current_number;

						m_currentScene->v_LevelObjects[planet].Fleets = newnum;
						current_number = 0;
						//imguiUpdate(m_currentScene->player);
					}

					ImGui::Text("You are currently at "); ImGui::SameLine(); ImGui::Text("%s", m_currentScene->v_LevelObjects[planet].getName().c_str());
					ImGui::Text("Planets current space fleets: "); ImGui::SameLine(); ImGui::Text("%i", newnum);
					ImGui::Text("%i", fleetdeployment);  ImGui::SameLine(); ImGui::Text(" space fleets left for renforcements.");



					//static bool AI_Player = false;
					//ImGui::Checkbox("Enable Extra AI Player", &AI_Player);

				}
				if (player.attack == true) {
					ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + 300.0f);
					ImGui::Text("This is the attack phase. Please Move to the planet you want to attack from then select the plan attack button. You'll then be able to navigate between all of the planets you don't own when you have found a planet you want to attack press the attack button");
					int Fleets = m_currentScene->v_LevelObjects[Playerpos].getFleets();
					ImGui::Text("You are currently at "); ImGui::SameLine(); ImGui::Text((const char*)m_currentScene->v_LevelObjects[Playerpos].getName().c_str());
					ImGui::Text("Planets current space fleets: "); ImGui::SameLine(); ImGui::Text("%i", Fleets);
					if (player.attackselection == false) {

						if (ImGui::Button("Plan Attack")) {
							if (m_currentScene->v_LevelObjects[Playerpos].Fleets == 1) {
								to_few = true;
							}
							else
							{
								attackfrom = Playerpos;
								player.attackselection = true;
							}
						}
						if (to_few == true) {
							ImGui::OpenPopup("Not Enough Fleets");
							if (ImGui::BeginPopupModal("Not Enough Fleets"))
							{
								ImGui::Text("You do not have enough fleets to attack");
								if (ImGui::Button("Select new staging area")) {
									to_few = false;
									ImGui::CloseCurrentPopup();
								}
								ImGui::EndPopup();
							}
						}
						if (attackingself == true) {
							ImGui::OpenPopup("Your attacking yourself");
							if (ImGui::BeginPopupModal("Your attacking yourself"))
							{
								ImGui::Text("That not a smart idea! No need for civil war");
								if (ImGui::Button("Attack the true enenmy")) {
									attackingself = false;
									ImGui::CloseCurrentPopup();
								}
								ImGui::EndPopup();
							}
						}

						if (ImGui::Button("Next Stage")) {
							player.attack = false;
							player.manover = true;
							resupply = false;

						}
					}
					else if (player.attackselection == true && fleetdeployment != true) {
						if (to_far == false) {


							if (ImGui::Button("Attack")) {
								attacking = Playerpos;

							}

							if (ImGui::Button("Next Stage")) {
								player.attack = false;
								player.manover = true;
								resupply = false;

							}

						}
						else if (to_far == true) {
							ImGui::OpenPopup("To far away");
							if (ImGui::BeginPopupModal("To far away", NULL, window_flags))
							{
								ImGui::Text("Your fleets don't have enough fual to attack that target from here please select another target.");
								if (ImGui::Button("Select New Target")) {
									to_far = false;
									attacking = 0;
									ImGui::CloseCurrentPopup();

								}
								ImGui::EndPopup();
							}
						}
					}
					if (fleetredeployment == true) {
						ImGui::OpenPopup("Fleet Deployment");
						if (ImGui::BeginPopupModal("Fleet Deployment", NULL, window_flags))
						{
							static int current_number = 0;
							ImGui::SliderInt("Fleets", &current_number, 0.0f, attackerfleets - 1);
							//ImGui::Text((const char*)ren);  ImGui::SameLine(); ImGui::Text(" space fleets left for renforcements.");
							ImGui::Text("%i", attackerfleets - current_number -1); ImGui::SameLine(); ImGui::Text("Fleets will return to defend your staging planet.");
							ImGui::Text("%i", current_number); ImGui::SameLine(); ImGui::Text("Fleets will defend your new planet.");

							if (ImGui::Button("Re deploy Fleets")) {
								std::cout << "fleets redeployed" << std::endl;
								m_currentScene->v_LevelObjects[attackerfleetspos].Fleets = attackerfleets - current_number;
								m_currentScene->v_LevelObjects[defenderfleetspos].Fleets = current_number;
								current_number = 0;
								fleetredeployment = false;
								ImGui::CloseCurrentPopup();
								//imguiUpdate(m_currentScene->player);
							}
							ImGui::EndPopup();
						}
					}

					
					}
				}
				if (player.manover == true) {
						ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + 300.0f);
						int planet = player.currently_looking_at;
						ImGui::Text("This is the resupply stage. you can move some of your fleets from one planet to another.");
						ImGui::Text("You are currently at "); ImGui::SameLine(); ImGui::Text((const char*)m_currentScene->v_LevelObjects[Playerpos].getName().c_str());
						ImGui::Text("Planets current space fleets: "); ImGui::SameLine(); ImGui::Text("%i", m_currentScene->v_LevelObjects[player.currently_looking_at].getFleets());


						if (resupply == false) {

							int fleet_numbers = m_currentScene->v_LevelObjects[planet].Fleets;
							static int movedfleets = 1;

							ImGui::SliderInt("Fleets", &movedfleets, 1.0f, fleet_numbers);

							if (ImGui::Button("Redeploy Fleets")) {
								redeployed_fleets = movedfleets;
								m_currentScene->v_LevelObjects[planet].Fleets = fleet_numbers - movedfleets;
								resupply = true;
							}

						}
						else {
							ImGui::Text("%i", redeployed_fleets); ImGui::SameLine(); ImGui::Text(" Fleets are re-deploying to a planet of your choosing.");
							if (ImGui::Button("Redeploy Fleets")) {
								int currentNfleets = m_currentScene->v_LevelObjects[planet].Fleets;
								m_currentScene->v_LevelObjects[planet].Fleets = currentNfleets + redeployed_fleets;
								redeployed_fleets = 0;

							}
							
						}
					
						if (ImGui::Button("End Turn##test")) {
							remainingplayers(m_currentScene->player);
							player.manover = false;
							player.renforcements = true;
							if (player.current_turn == "red") {
								if (player.orangealive == true) {
									player.current_turn = "orange";
									playerendturn(m_currentScene->player);
								}
								else {
									if (player.greenalive == true) {
										player.current_turn = "green";
										playerendturn(m_currentScene->player);
									}
									else if (player.pinkalive == true) {
										player.current_turn = "pink";
										playerendturn(m_currentScene->player);
									}
									else if (player.whitealive == true) {
										player.current_turn = "white";
										playerendturn(m_currentScene->player);
									}
									else if (player.purplealive == true) {
										player.current_turn = "purple";
										playerendturn(m_currentScene->player);
									}
									else if (player.tealalive == true) {
										player.current_turn = "teal";
										playerendturn(m_currentScene->player);
									}
									else if (player.greyalive == true) {
										player.current_turn = "grey";
										playerendturn(m_currentScene->player);
									}
									else if (AI_Player == true) {
										player.current_turn = "yellow";
										AI_Turn = true;
										AI_Behave(player);
									}

								}
							}
							else if (player.current_turn == "orange") {
								if (player.greenalive == true) {
									if (Nplayer == 2) {
										if (AI_Player == true) {
											player.current_turn = "yellow";
											AI_Turn = true;
											AI_Behave(player);
										}
										else {
											player.current_turn = "red";
											playerendturn(m_currentScene->player);
										}
									}
									else {
										player.current_turn = "green";
										playerendturn(m_currentScene->player);
									}

								}
								else {
									if (player.pinkalive == true) {
										player.current_turn = "pink";
										playerendturn(m_currentScene->player);
									}
									else if (player.whitealive == true) {
										player.current_turn = "white";
										playerendturn(m_currentScene->player);
									}
									else if (player.purplealive == true) {
										player.current_turn = "purple";
										playerendturn(m_currentScene->player);
									}
									else if (player.tealalive == true) {
										player.current_turn = "teal";
										playerendturn(m_currentScene->player);
									}
									else if (player.greyalive == true) {
										player.current_turn = "grey";
										playerendturn(m_currentScene->player);
									}
									else if (AI_Player == true) {
										player.current_turn = "yellow";
										AI_Turn = true;
										AI_Behave(player);
									}
									else if (player.redalive == true) {
										player.current_turn = "red";
										playerendturn(m_currentScene->player);
									}
								}
							}

							else if (player.current_turn == "green") {
								if (player.pinkalive == true) {
									if (Nplayer == 3) {
										if (AI_Player == true) {
											player.current_turn = "yellow";
											AI_Turn = true;
											AI_Behave(player);
										}
										else {
											player.current_turn = "red";
											playerendturn(m_currentScene->player);
										}
									}
									else {
										player.current_turn = "pink";
										playerendturn(m_currentScene->player);
									}
								}
								else {
									if (player.whitealive == true) {
										player.current_turn = "white";
										playerendturn(m_currentScene->player);
									}
									else if (player.purplealive == true) {
										player.current_turn = "purple";
										playerendturn(m_currentScene->player);
									}
									else if (player.tealalive == true) {
										player.current_turn = "teal";
										playerendturn(m_currentScene->player);
									}
									else if (player.greyalive == true) {
										player.current_turn = "grey";
										playerendturn(m_currentScene->player);
									}
									else if (AI_Player == true) {
										player.current_turn = "yellow";
										AI_Turn = true;
										AI_Behave(player);
									}
									else if (player.redalive == true) {
										player.current_turn = "red";
										playerendturn(m_currentScene->player);
									}
									else if (player.orangealive == true) {
										player.current_turn = "orange";
										playerendturn(m_currentScene->player);
									}


								}
							}
							else if (player.current_turn == "pink") {
								if (player.whitealive == true) {
									if (Nplayer == 4) {
										if (AI_Player == true) {
											player.current_turn = "yellow";
											AI_Turn = true;
											AI_Behave(player);
										}
										else {
											player.current_turn = "red";
											playerendturn(m_currentScene->player);
										}
									}
									else {
										player.current_turn = "white";
										playerendturn(m_currentScene->player);
									}
								}
								else {
									if (player.purplealive == true) {
										player.current_turn = "purple";
										playerendturn(m_currentScene->player);
									}
									else if (player.tealalive == true) {
										player.current_turn = "teal";
										playerendturn(m_currentScene->player);
									}
									else if (player.greyalive == true) {
										player.current_turn = "grey";
										playerendturn(m_currentScene->player);
									}
									else if (AI_Player == true) {
										player.current_turn = "yellow";
										AI_Turn = true;
										AI_Behave(player);
									}
									else if (player.redalive == true) {
										player.current_turn = "red";
										playerendturn(m_currentScene->player);
									}
									else if (player.orangealive == true) {
										player.current_turn = "orange";
										playerendturn(m_currentScene->player);
									}

									else if (player.greenalive == true) {
										player.current_turn = "green";
										playerendturn(m_currentScene->player);
									}

								}
							}
							else if (player.current_turn == "white") {
								if (player.purplealive == true) {
									if (Nplayer == 5) {
										if (AI_Player == true) {
											player.current_turn = "yellow";
											AI_Turn = true;
											AI_Behave(player);
										}
										else {
											player.current_turn = "red";
											playerendturn(m_currentScene->player);
										}
									}
									else {
										player.current_turn = "purple";
										playerendturn(m_currentScene->player);
									}
								}
								else {
									if (player.tealalive == true) {
										player.current_turn = "teal";
										playerendturn(m_currentScene->player);
									}
									else if (player.greyalive == true) {
										player.current_turn = "grey";
										playerendturn(m_currentScene->player);
									}
									else if (AI_Player == true) {
										player.current_turn = "yellow";
										AI_Turn = true;
										AI_Behave(player);
									}
									else if (player.redalive == true) {
										player.current_turn = "red";
										playerendturn(m_currentScene->player);
									}
									else if (player.orangealive == true) {
										player.current_turn = "orange";
										playerendturn(m_currentScene->player);
									}

									else if (player.greenalive == true) {
										player.current_turn = "green";
										playerendturn(m_currentScene->player);
									}
									else if (player.pinkalive == true) {
										player.current_turn = "pink";
										playerendturn(m_currentScene->player);
									}

								}
							}
							else if (player.current_turn == "purple") {
								if (player.tealalive == true) {
									if (Nplayer == 6) {
										if (AI_Player == true) {
											player.current_turn = "yellow";
											AI_Turn = true;
											AI_Behave(player);
										}
										else {
											player.current_turn = "red";
											playerendturn(m_currentScene->player);
										}
									}
									else {
										player.current_turn = "teal";
										playerendturn(m_currentScene->player);
									}
								}
								else {
									if (player.greyalive == true) {
										player.current_turn = "grey";
										playerendturn(m_currentScene->player);
									}
									else if (AI_Player == true) {
										player.current_turn = "yellow";
										AI_Turn = true;
										AI_Behave(player);
									}
									else if (player.redalive == true) {
										player.current_turn = "red";
										playerendturn(m_currentScene->player);
									}
									else if (player.orangealive == true) {
										player.current_turn = "orange";
										playerendturn(m_currentScene->player);
									}

									else if (player.greenalive == true) {
										player.current_turn = "green";
										playerendturn(m_currentScene->player);
									}
									else if (player.pinkalive == true) {
										player.current_turn = "pink";
										playerendturn(m_currentScene->player);
									}
									else if (player.whitealive == true) {
										player.current_turn = "white";
										playerendturn(m_currentScene->player);
									}

								}
							}
							else if (player.current_turn == "teal") {
								if (player.greyalive == true) {
									if (Nplayer == 7) {
										if (AI_Player == true) {
											player.current_turn = "yellow";
											AI_Turn = true;
											AI_Behave(player);
										}
										else {
											player.current_turn = "red";
											playerendturn(m_currentScene->player);
										}
									}
									else {
										player.current_turn = "grey";
										playerendturn(m_currentScene->player);
									}
								}
								else {
									if (AI_Player == true) {
										player.current_turn = "yellow";
										AI_Turn = true;
										AI_Behave(player);
									}
									else if (player.redalive == true) {
										player.current_turn = "red";
										playerendturn(m_currentScene->player);
									}
									else if (player.orangealive == true) {
										player.current_turn = "orange";
										playerendturn(m_currentScene->player);
									}
									else if (player.greenalive == true) {
										player.current_turn = "green";
										playerendturn(m_currentScene->player);
									}
									else if (player.pinkalive == true) {
										player.current_turn = "pink";
										playerendturn(m_currentScene->player);
									}
									else if (player.whitealive == true) {
										player.current_turn = "white";
										playerendturn(m_currentScene->player);
									}
									else if (player.purplealive == true) {
										player.current_turn = "purple";
										playerendturn(m_currentScene->player);
									}

								}
							}
							else if (player.current_turn == "grey") {
								if (player.redalive == true) {
									if (AI_Player == true) {
										player.current_turn = "yellow";
										AI_Turn = true;
										AI_Behave(player);
									}
									else {
										player.current_turn = "red";
										playerendturn(m_currentScene->player);
									}
								}
								else {
									if (player.orangealive == true) {
										player.current_turn = "orange";
										playerendturn(m_currentScene->player);
									}
									else if (player.greenalive == true) {
										player.current_turn = "green";
										playerendturn(m_currentScene->player);
									}
									else if (player.pinkalive == true) {
										player.current_turn = "pink";
										playerendturn(m_currentScene->player);
									}
									else if (player.whitealive == true) {
										player.current_turn = "white";
										playerendturn(m_currentScene->player);
									}
									else if (player.purplealive == true) {
										player.current_turn = "purple";
										playerendturn(m_currentScene->player);
									}
									else if (player.tealalive == true) {
										player.current_turn = "teal";
										playerendturn(m_currentScene->player);
									}
									else if (player.greyalive == true) {
										player.current_turn = "grey";
										playerendturn(m_currentScene->player);
									}
									else if (AI_Player == true) {
										player.current_turn = "yellow";
										AI_Turn = true;
										AI_Behave(player);
									}
								}
							}
						}
						
}
			}
			//ImGui::ListBox("", &listboxGameobjectCurrentIndex, getGameObjectNameFunc, static_cast<void*>(&m_currentScene->v_LevelObjects), test, 30);

			

			/* if (defendermessage == true) {
				 ImGui::OpenPopup("Defender Fleet Lost");
				 if (ImGui::BeginPopup("Defender Fleet Lost", NULL)) {
					 ImGui::Text("Defenders Fleet has been destroyed.");
					 if (ImGui::Button("Close")) {
						 //fleetredeployment = false;
						 //imguiUpdate(m_currentScene->player);
					 }
					 ImGui::EndPopup();
				 }
			 }
			 if (attackermessage == true) {
				 ImGui::OpenPopup("Attack Fleets Lost");
				 if (ImGui::BeginPopup("Defender Fleet Lost", NULL)) {
					 ImGui::Text("Defenders Fleet has been destroyed.");
					 if (ImGui::Button("close")) {
						 //fleetredeployment = false;
						 ImGui::EndPopup();
						 //imguiUpdate(m_currentScene->player);
					 }
					 ImGui::EndPopup();
				 }
			 }
			 if (to_far == true) {
				 ImGui::OpenPopup("Notification");
				 if (ImGui::BeginPopup("Notification", NULL)) {
					 ImGui::Text("Your Fleets don't have enough fuel to attack from here. Select a new staging point.");
					 if (ImGui::Button("close")) {

						 player.attackselection = false;
						 ImGui::EndPopup();
						 //imguiUpdate(m_currentScene->player);
					 }
					 ImGui::EndPopup();
				 }
			 }
*/
		}
	
	ImGui::End();



	ImGuiWindowFlags window2_flags = ImGuiWindowFlags_NoResize;
	window_flags |= ImGuiWindowFlags_NoCollapse;

	if (ImGui::Begin("Game Information", NULL, window2_flags)) {
		ImGui::SetWindowPos(ImVec2(wWidth - (wWidth*0.20), 0));
		ImGui::SetWindowSize(ImVec2(wWidth*0.20, wHeight*0.25));
		redscore = 0;
		orangescore = 0;
		greenscore = 0;
		pinkscore = 0;
		whitescore = 0;
		purplescore = 0;
		tealscore = 0;
		greyscore = 0;
		AIscore = 0;
		if (gameover == true) {
			ImGui::Text("%s", player.current_turn.c_str()); ImGui::SameLine(); ImGui::Text("Team have won this game.");
		}
		if (CurrentLevel == "game") {

			if (Nplayer == 2) {
				for (int i = 0; i < m_currentScene->v_LevelObjects.size(); i++) {
					if (m_currentScene->v_LevelObjects[i].team == "red") {
						redscore++;
						if (redscore == m_currentScene->v_LevelObjects.size() - 1) {
							Ismenu = true;
							CurrentLevel = "menu";
							m_currentScene->playerspawned = false;
							gameover = true;
							menusetup();
						}
					}
					else if (m_currentScene->v_LevelObjects[i].team == "orange") {
						orangescore++;
						if (orangescore == m_currentScene->v_LevelObjects.size() - 1) {
							Ismenu = true;
							CurrentLevel = "menu";
							m_currentScene->playerspawned = false;
							gameover = true;
							menusetup();
						}
					}
				}
				ImGui::Text("%i", redscore); ImGui::SameLine; ImGui::Text("Planets controlled by red team");
				ImGui::Text("%i", orangescore); ImGui::SameLine; ImGui::Text("Planets controlled by orange team");
			}
			if (Nplayer == 3) {
				for (int i = 0; i < m_currentScene->v_LevelObjects.size(); i++) {
					if (m_currentScene->v_LevelObjects[i].team == "red") {
						redscore++;
						if (redscore == m_currentScene->v_LevelObjects.size() - 1) {
							Ismenu = true;
							CurrentLevel = "menu";
							m_currentScene->playerspawned = false;
							gameover = true;
							menusetup();
						}
					}
					else if (m_currentScene->v_LevelObjects[i].team == "orange") {
						orangescore++;
						if (orangescore == m_currentScene->v_LevelObjects.size() - 1) {
							Ismenu = true;
							CurrentLevel = "menu";
							m_currentScene->playerspawned = false;
							gameover = true;
							menusetup();
						}
					}
					else if (m_currentScene->v_LevelObjects[i].team == "green") {
						greenscore++;
						if (greenscore == m_currentScene->v_LevelObjects.size() - 1) {
							Ismenu = true;
							CurrentLevel = "menu";
							m_currentScene->playerspawned = false;
							gameover = true;
							menusetup();
						}
					}
				}

				ImGui::Text("%i", redscore); ImGui::SameLine; ImGui::Text("Planets controlled by red team");
				ImGui::Text("%i", orangescore); ImGui::SameLine; ImGui::Text("Planets controlled by orange team");
				ImGui::Text("%i", greenscore); ImGui::SameLine; ImGui::Text("Planets controlled by green team");
			}
			if (Nplayer == 4) {
				for (int i = 0; i < m_currentScene->v_LevelObjects.size(); i++) {
					if (m_currentScene->v_LevelObjects[i].team == "red") {
						redscore++;
						if (redscore == m_currentScene->v_LevelObjects.size() - 1) {
							Ismenu = true;
							CurrentLevel = "menu";
							m_currentScene->playerspawned = false;
							gameover = true;
							menusetup();
						}
					}
					else if (m_currentScene->v_LevelObjects[i].team == "orange") {
						orangescore++;
						if (orangescore == m_currentScene->v_LevelObjects.size() - 1) {
							Ismenu = true;
							CurrentLevel = "menu";
							m_currentScene->playerspawned = false;
							gameover = true;
							menusetup();
						}
					}
					else if (m_currentScene->v_LevelObjects[i].team == "green") {
						greenscore++;
						if (greenscore == m_currentScene->v_LevelObjects.size() - 1) {
							Ismenu = true;
							CurrentLevel = "menu";
							m_currentScene->playerspawned = false;
							gameover = true;
							menusetup();
						}
					}
					else if (m_currentScene->v_LevelObjects[i].team == "pink") {
						pinkscore++;
						if (pinkscore == m_currentScene->v_LevelObjects.size() - 1) {
							Ismenu = true;
							CurrentLevel = "menu";
							m_currentScene->playerspawned = false;
							gameover = true;
							menusetup();
						}
					}
				}

				ImGui::Text("%i", redscore); ImGui::SameLine; ImGui::Text("Planets controlled by red team");
				ImGui::Text("%i", orangescore); ImGui::SameLine; ImGui::Text("Planets controlled by orange team");
				ImGui::Text("%i", greenscore); ImGui::SameLine; ImGui::Text("Planets controlled by green team");
				ImGui::Text("%i", pinkscore); ImGui::SameLine; ImGui::Text("Planets controlled by pink team");
			}
			if (Nplayer == 5) {
				for (int i = 0; i < m_currentScene->v_LevelObjects.size(); i++) {
					if (m_currentScene->v_LevelObjects[i].team == "red") {
						redscore++;
						if (redscore == m_currentScene->v_LevelObjects.size() - 1) {
							Ismenu = true;
							CurrentLevel = "menu";
							m_currentScene->playerspawned = false;
							gameover = true;
							menusetup();
						}
					}
					else if (m_currentScene->v_LevelObjects[i].team == "orange") {
						orangescore++;
						if (orangescore == m_currentScene->v_LevelObjects.size() - 1) {
							Ismenu = true;
							CurrentLevel = "menu";
							m_currentScene->playerspawned = false;
							gameover = true;
							menusetup();
						}
					}
					else if (m_currentScene->v_LevelObjects[i].team == "green") {
						greenscore++;
						if (greenscore == m_currentScene->v_LevelObjects.size() - 1) {
							Ismenu = true;
							CurrentLevel = "menu";
							m_currentScene->playerspawned = false;
							gameover = true;
							menusetup();
						}
					}
					else if (m_currentScene->v_LevelObjects[i].team == "pink") {
						pinkscore++;
						if (pinkscore == m_currentScene->v_LevelObjects.size() - 1) {
							Ismenu = true;
							CurrentLevel = "menu";
							m_currentScene->playerspawned = false;
							gameover = true;
							menusetup();
						}
					}
					else if (m_currentScene->v_LevelObjects[i].team == "white") {
						whitescore++;
						if (whitescore == m_currentScene->v_LevelObjects.size() - 1) {
							Ismenu = true;
							CurrentLevel = "menu";
							m_currentScene->playerspawned = false;
							gameover = true;
							menusetup();
						}
					}
				}
				ImGui::Text("%i", redscore); ImGui::SameLine; ImGui::Text("Planets controlled by red team");
				ImGui::Text("%i", orangescore); ImGui::SameLine; ImGui::Text("Planets controlled by orange team");
				ImGui::Text("%i", greenscore); ImGui::SameLine; ImGui::Text("Planets controlled by green team");
				ImGui::Text("%i", pinkscore); ImGui::SameLine; ImGui::Text("Planets controlled by pink team");
				ImGui::Text("%i", whitescore); ImGui::SameLine; ImGui::Text("Planets controlled by white team");
			}
			if (Nplayer == 6) {
				for (int i = 0; i < m_currentScene->v_LevelObjects.size(); i++) {
					if (m_currentScene->v_LevelObjects[i].team == "red") {
						redscore++;
						if (redscore == m_currentScene->v_LevelObjects.size() - 1) {
							Ismenu = true;
							CurrentLevel = "menu";
							m_currentScene->playerspawned = false;
							gameover = true;
							menusetup();
						}
					}
					else if (m_currentScene->v_LevelObjects[i].team == "orange") {
						orangescore++;
						if (orangescore == m_currentScene->v_LevelObjects.size() - 1) {
							Ismenu = true;
							CurrentLevel = "menu";
							m_currentScene->playerspawned = false;
							gameover = true;
							menusetup();
						}
					}
					else if (m_currentScene->v_LevelObjects[i].team == "green") {
						greenscore++;
						if (greenscore == m_currentScene->v_LevelObjects.size() - 1) {
							Ismenu = true;
							CurrentLevel = "menu";
							m_currentScene->playerspawned = false;
							gameover = true;
							menusetup();
						}
					}
					else if (m_currentScene->v_LevelObjects[i].team == "pink") {
						pinkscore++;
						if (pinkscore == m_currentScene->v_LevelObjects.size() - 1) {
							Ismenu = true;
							CurrentLevel = "menu";
							m_currentScene->playerspawned = false;
							gameover = true;
							menusetup();
						}
					}
					else if (m_currentScene->v_LevelObjects[i].team == "white") {
						whitescore++;
						if (whitescore == m_currentScene->v_LevelObjects.size() - 1) {
							Ismenu = true;
							CurrentLevel = "menu";
							m_currentScene->playerspawned = false;
							gameover = true;
							menusetup();
						}
					}
					else if (m_currentScene->v_LevelObjects[i].team == "purple") {
						purplescore++;
						if (purplescore == m_currentScene->v_LevelObjects.size() - 1) {
							Ismenu = true;
							CurrentLevel = "menu";
							m_currentScene->playerspawned = false;
							gameover = true;
							menusetup();
						}
					}
				}

				ImGui::Text("%i", redscore); ImGui::SameLine; ImGui::Text("Planets controlled by red team");
				ImGui::Text("%i", orangescore); ImGui::SameLine; ImGui::Text("Planets controlled by orange team");
				ImGui::Text("%i", greenscore); ImGui::SameLine; ImGui::Text("Planets controlled by green team");
				ImGui::Text("%i", pinkscore); ImGui::SameLine; ImGui::Text("Planets controlled by pink team");
				ImGui::Text("%i", whitescore); ImGui::SameLine; ImGui::Text("Planets controlled by white team");
				ImGui::Text("%i", purplescore); ImGui::SameLine; ImGui::Text("Planets controlled by purple team");
			}
			if (Nplayer == 7) {
				for (int i = 0; i < m_currentScene->v_LevelObjects.size(); i++) {
					if (m_currentScene->v_LevelObjects[i].team == "red") {
						redscore++;
						if (redscore == m_currentScene->v_LevelObjects.size() - 1) {
							Ismenu = true;
							CurrentLevel = "menu";
							m_currentScene->playerspawned = false;
							gameover = true;
							menusetup();
						}
					}
					else if (m_currentScene->v_LevelObjects[i].team == "orange") {
						orangescore++;
						if (orangescore == m_currentScene->v_LevelObjects.size() - 1) {
							Ismenu = true;
							CurrentLevel = "menu";
							m_currentScene->playerspawned = false;
							gameover = true;
							menusetup();
						}
					}
					else if (m_currentScene->v_LevelObjects[i].team == "green") {
						greenscore++;
						if (greenscore == m_currentScene->v_LevelObjects.size() - 1) {
							Ismenu = true;
							CurrentLevel = "menu";
							m_currentScene->playerspawned = false;
							gameover = true;
							menusetup();
						}
					}
					else if (m_currentScene->v_LevelObjects[i].team == "pink") {
						pinkscore++;
						if (pinkscore == m_currentScene->v_LevelObjects.size() - 1) {
							Ismenu = true;
							CurrentLevel = "menu";
							m_currentScene->playerspawned = false;
							gameover = true;
							menusetup();
						}
					}
					else if (m_currentScene->v_LevelObjects[i].team == "white") {
						whitescore++;
						if (whitescore == m_currentScene->v_LevelObjects.size() - 1) {
							Ismenu = true;
							CurrentLevel = "menu";
							m_currentScene->playerspawned = false;
							gameover = true;
							menusetup();
						}
					}
					else if (m_currentScene->v_LevelObjects[i].team == "purple") {
						purplescore++;
						if (purplescore == m_currentScene->v_LevelObjects.size() - 1) {
							Ismenu = true;
							CurrentLevel = "menu";
							m_currentScene->playerspawned = false;
							gameover = true;
							menusetup();
						}
					}
					else if (m_currentScene->v_LevelObjects[i].team == "teal") {
						tealscore++;
						if (tealscore == m_currentScene->v_LevelObjects.size() - 1) {
							Ismenu = true;
							CurrentLevel = "menu";
							m_currentScene->playerspawned = false;
							gameover = true;
							menusetup();
						}
					}
				}
				ImGui::Text("%i", redscore); ImGui::SameLine; ImGui::Text("Planets controlled by red team");
				ImGui::Text("%i", orangescore); ImGui::SameLine; ImGui::Text("Planets controlled by orange team");
				ImGui::Text("%i", greenscore); ImGui::SameLine; ImGui::Text("Planets controlled by green team");
				ImGui::Text("%i", pinkscore); ImGui::SameLine; ImGui::Text("Planets controlled by pink team");
				ImGui::Text("%i", whitescore); ImGui::SameLine; ImGui::Text("Planets controlled by white team");
				ImGui::Text("%i", purplescore); ImGui::SameLine; ImGui::Text("Planets controlled by purple team");
				ImGui::Text("%i", tealscore); ImGui::SameLine; ImGui::Text("Planets controlled by teal team");
			}
			if (Nplayer == 8) {
				for (int i = 0; i < m_currentScene->v_LevelObjects.size(); i++) {
					if (m_currentScene->v_LevelObjects[i].team == "red") {
						redscore++;
						if (redscore == m_currentScene->v_LevelObjects.size() - 1) {
							Ismenu = true;
							CurrentLevel = "menu";
							m_currentScene->playerspawned = false;
							gameover = true;
							menusetup();
						}
					}
					else if (m_currentScene->v_LevelObjects[i].team == "orange") {
						orangescore++;
						if (orangescore == m_currentScene->v_LevelObjects.size() - 1) {
							Ismenu = true;
							CurrentLevel = "menu";
							m_currentScene->playerspawned = false;
							gameover = true;
							menusetup();
						}
					}
					else if (m_currentScene->v_LevelObjects[i].team == "green") {
						greenscore++;
						if (greenscore == m_currentScene->v_LevelObjects.size() - 1) {
							Ismenu = true;
							CurrentLevel = "menu";
							m_currentScene->playerspawned = false;
							gameover = true;
							menusetup();
						}
					}
					else if (m_currentScene->v_LevelObjects[i].team == "pink") {
						pinkscore++;
						if (pinkscore == m_currentScene->v_LevelObjects.size() - 1) {
							Ismenu = true;
							CurrentLevel = "menu";
							m_currentScene->playerspawned = false;
							gameover = true;
							menusetup();
						}
					}
					else if (m_currentScene->v_LevelObjects[i].team == "white") {
						whitescore++;
						if (whitescore == m_currentScene->v_LevelObjects.size() - 1) {
							Ismenu = true;
							CurrentLevel = "menu";
							m_currentScene->playerspawned = false;
							gameover = true;
							menusetup();
						}
					}
					else if (m_currentScene->v_LevelObjects[i].team == "purple") {
						purplescore++;
						if (purplescore == m_currentScene->v_LevelObjects.size() - 1) {
							Ismenu = true;
							CurrentLevel = "menu";
							m_currentScene->playerspawned = false;
							gameover = true;
							menusetup();
						}
					}
					else if (m_currentScene->v_LevelObjects[i].team == "teal") {
						tealscore++;
						if (tealscore == m_currentScene->v_LevelObjects.size() - 1) {
							Ismenu = true;
							CurrentLevel = "menu";
							m_currentScene->playerspawned = false;
							gameover = true;
							menusetup();
						}
					}
					else if (m_currentScene->v_LevelObjects[i].team == "grey") {
						greyscore++;
						if (tealscore == m_currentScene->v_LevelObjects.size() - 1) {
							Ismenu = true;
							CurrentLevel = "menu";
							m_currentScene->playerspawned = false;
							gameover = true;
							menusetup();
						}

					}
				}
					ImGui::Text("%i", redscore); ImGui::SameLine; ImGui::Text("Planets controlled by red team");
					ImGui::Text("%i", orangescore); ImGui::SameLine; ImGui::Text("Planets controlled by orange team");
					ImGui::Text("%i", greenscore); ImGui::SameLine; ImGui::Text("Planets controlled by green team");
					ImGui::Text("%i", pinkscore); ImGui::SameLine; ImGui::Text("Planets controlled by pink team");
					ImGui::Text("%i", whitescore); ImGui::SameLine; ImGui::Text("Planets controlled by white team");
					ImGui::Text("%i", purplescore); ImGui::SameLine; ImGui::Text("Planets controlled by purple team");
					ImGui::Text("%i", tealscore); ImGui::SameLine; ImGui::Text("Planets controlled by teal team");
					ImGui::Text("%i", greyscore); ImGui::SameLine; ImGui::Text("Planets controlled by grey team");
				}
				

			//}
			if (AI_Player == true) {
				for (int i = 0; i < m_currentScene->v_LevelObjects.size(); i++) {
					if (m_currentScene->v_LevelObjects[i].team == "yellow") {
						AIscore++;
						if (AIscore == m_currentScene->v_LevelObjects.size() - 1) {
							Ismenu = true;
							CurrentLevel = "menu";
							m_currentScene->playerspawned = false;
							gameover = true;
							menusetup();
						}
					}
				}
				ImGui::Text("%i", AIscore); ImGui::SameLine; ImGui::Text("Planets controlled by yellow team");
			}

			if (gameover == true) {
				ImGui::Text("%s", winner); ImGui::SameLine(); ImGui::Text("Team is the winner.");
			}
			//if (defeatmessage){}

		}
		ImGui::End();

		ImGuiWindowFlags window3_flags = ImGuiWindowFlags_NoResize;
		window_flags |= ImGuiWindowFlags_NoCollapse;
		window_flags |= ImGuiWindowFlags_NoMove;

		if (ImGui::Begin("Planet Locations", NULL, window3_flags)) {
			ImGui::SetWindowPos(ImVec2(0, wHeight*0.35));
			ImGui::SetWindowSize(ImVec2(wWidth*0.20, wHeight*0.15));
			if (Ismenu == false) {
				if (player.renforcements == true || player.attackselection == false) {
					ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + 250.0f);
					ImGui::Text("To move to different planets use the drop down list and the move button to move to a different planet.");
					int first = 1;
					while (m_currentScene->v_LevelObjects[first].team != player.current_turn) {
						first++;
					}
					static const char* viewindex = m_currentScene->v_LevelObjects[first].name.c_str();
					if (ImGui::BeginCombo("", viewindex)) // The second parameter is the label previewed before opening the combo.
					{
						for (int n = 0; n < m_currentScene->v_LevelObjects.size(); n++)
						{
							if (m_currentScene->v_LevelObjects[n].team == player.current_turn) {
								bool is_selected = (viewindex == m_currentScene->v_LevelObjects[n].name.c_str());
								if (ImGui::Selectable(m_currentScene->v_LevelObjects[n].name.c_str(), is_selected))
									viewindex = m_currentScene->v_LevelObjects[n].name.c_str();

								if (is_selected) {
									ImGui::SetItemDefaultFocus();

								}
							}
						}
						ImGui::EndCombo();
					}
					if (ImGui::Button("Move")) {
						for (int i = 0; i < m_currentScene->v_LevelObjects.size(); i++) {
							if (viewindex == m_currentScene->v_LevelObjects[i].name.c_str()) {
								player.currently_looking_at = i;
								player.planetchange = true;
								cam_Movement(m_currentScene->v_LevelObjects[i].team, m_currentScene->player);

							}
						}
					}
				}
				else if (player.attackselection == true) {
					ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + 250.0f);
					ImGui::Text("To move to different planets use the drop down list and the move button to move to a different planet.");
					int second = 1;
					while (m_currentScene->v_LevelObjects[second].team == player.current_turn) {
						second++;
					}
					static const char* viewindex = m_currentScene->v_LevelObjects[second].name.c_str();
					if (ImGui::BeginCombo("", viewindex)) // The second parameter is the label previewed before opening the combo.
					{
						for (int n = 0; n < m_currentScene->v_LevelObjects.size(); n++)
						{
							if (m_currentScene->v_LevelObjects[n].team != player.current_turn) {
								bool is_selected = (viewindex == m_currentScene->v_LevelObjects[n].name.c_str());
								if (ImGui::Selectable(m_currentScene->v_LevelObjects[n].name.c_str(), is_selected))
									viewindex = m_currentScene->v_LevelObjects[n].name.c_str();

								if (is_selected) {
									ImGui::SetItemDefaultFocus();

								}
								// Set the initial focus when opening the combo (scrolling + for keyboard navigation support in the upcoming navigation branch)
							}
						}
						ImGui::EndCombo();
					}
					if (ImGui::Button("Move")) {
						for (int i = 0; i < m_currentScene->v_LevelObjects.size(); i++) {
							if (viewindex == m_currentScene->v_LevelObjects[i].name.c_str()) {
								player.currently_looking_at = i;
								player.planetchange = true;
								cam_Movement(m_currentScene->v_LevelObjects[i].team, m_currentScene->player);

							}
						}
					}
				}




				/*static auto getGameObjectNameFunc = [](void* vec, int idx, const char**
					out_text)
				{
					// need a cast as it is passed in as void* - a bit messy
					std::vector<GameObject*>* vector = static_cast<std::vector<GameObject*>*>(vec);
					// only get the name if it’s a valid index
					if (idx < 0 || idx >= vector->size())
						return false;
					*out_text = vector->at(idx)->name.c_str();
					return true;
				};

				static int listboxGameobjectCurrentIndex = 0;
				ImGui::ListBox("", &listboxGameobjectCurrentIndex,
					getGameObjectNameFunc, // a lambda function we have to make
					static_cast<void*>(&m_currentScene->v_LevelObjects),
					m_currentScene->v_LevelObjects.size(),
					30);*/


					/*IMGUI_API bool List(const char* label, int* current_item, bool(*Getnames)(void* data, int idx, const char** out_text), void* data, int items_count, int height_in_items = -1);
					static int current = 1;

					List("Planet Navigation", &current,
						[](void* vec, int idx, const char** out_text) {
						std::vector<GameObject>* Gvector = reinterpret_cast<std::vector<GameObject>*>(vec);
						if (idx < 0 || idx >= Gvector->size()) {
							return false;
						}
						*out_text = Gvector->at(idx).getnavname();
						return true;
					}, reinterpret_cast<void*>(&m_currentScene->v_LevelObjects), m_currentScene->v_LevelObjects.size());*/

					/*static int listboxGameobjectCurrentIndex = -1;

					static auto getGameObjectNameFunc = [](void* vec, int idx, const char**
						out_text)
					{
						// need a cast as it is passed in as void* - a bit messy
						std::vector<GameObject*>* vector = static_cast<std::vector<GameObject*>*>(vec);
						// only get the name if it’s a valid index
						if (idx < 0 || idx >= vector->size())
							return false;
						*out_text = vector->at(idx)->getnavname();
						return true;
					};

					ImGui::ListBox("Planet Navigation", &listboxGameobjectCurrentIndex, getGameObjectNameFunc, static_cast<void*>(&m_currentScene->v_LevelObjects), m_currentScene->v_LevelObjects.size(), 30);*/


					//static int Pselected = -1;
					//ImGui::_Combo("Planet Navigation", Pselected, ImGui::vector_getter)
					//ImGui::ListBox("Planet Navigation", &Pselected, m_currentScene->v_planetNav);
					//ImGui::BeginCombo()
			}
		}
	}
	ImGui::End();

}

void Game::AI_Behave(PlayerObject& Player){
	std::cout << "test 1" << std::endl;
	if (AI_Turn == true) {
		AI_renforcements_cal = false;
		int vunPlanet = 5;
		for (int i = 1; i < m_currentScene->v_LevelObjects.size(); i++) {
			if (m_currentScene->v_LevelObjects[i].team == "yellow") {
				std::cout << "test 2" << std::endl;
				if (m_currentScene->v_LevelObjects[i].Fleets < vunPlanet) {
					vunPlanet = i;
					std::cout << m_currentScene->v_LevelObjects[vunPlanet].name << " selected" << std::endl;
				}
			}
		}
		if (AI_renforcements_cal != true) {
			std::cout << "test 3" << std::endl;
			for (int i = 0; i < m_currentScene->v_LevelObjects.size(); i++) {
				if (m_currentScene->v_LevelObjects[i].team == "yellow") {
					renforcementnumber++;
				}
			}
			fleetdeployment = floor(renforcementnumber / 2);
			if (fleetdeployment < 3) {
				fleetdeployment = 3;
			}
			renforcements_cal = true;
		}
		for (int i = 1; i < m_currentScene->v_LevelObjects.size(); i++) {
			if (m_currentScene->v_LevelObjects[i].team == "yellow") {
				std::cout << "test 2" << std::endl;
				if (m_currentScene->v_LevelObjects[i].Fleets > vunPlanet) {
					vunPlanet = i;
				}
			}
		}
		attackfrom = vunPlanet;
		int vunfleet = m_currentScene->v_LevelObjects[vunPlanet].Fleets;
		m_currentScene->v_LevelObjects[vunPlanet].Fleets = vunfleet + fleetdeployment;
		fleetdeployment = 0;
		if (fleetdeployment == 0) {
			AI_Attack = true;
			renforcements_cal = false;
		}

	}
	if (AI_Attack == true) {
		if (AI_Range == true) {
			AI_RangeCheck = false;
			int vunAttPlanet = 5;
			for (int i = 1; i < m_currentScene->v_LevelObjects.size(); i++) {
				if (m_currentScene->v_LevelObjects[i].team != "yellow") {
					if (m_currentScene->v_LevelObjects[i].Fleets <= vunAttPlanet) {
						
						if (AI_RangeCheck != true) {
							int a = m_currentScene->v_LevelObjects[attackfrom].getComponent<TransformComponent>()->position().x;
							int a2 = m_currentScene->v_LevelObjects[i].getComponent<TransformComponent>()->position().x;
							int c = m_currentScene->v_LevelObjects[attackfrom].getComponent<TransformComponent>()->position().z;
							int c2 = m_currentScene->v_LevelObjects[i].getComponent<TransformComponent>()->position().z;
							int AC = floor(sqrt((a*a2) + (c*c2)));
							if (AC < 75) {
								AI_RangeCheck = true;
								vunAttPlanet = i;
							}

						}
					}
				}
			}
			attacking = vunAttPlanet;
		}
		std::cout << "attacking planet" << std::endl;
		attackCalculation(Player.current_turn, m_currentScene->player);
		AI_Attack = false;
		
	}
	AI_Turn = false;
	if (Player.redalive == true) {
		Player.current_turn = "red";
		playerendturn(m_currentScene->player);
	}
	else if (Player.orangealive == true) {
		Player.current_turn = "orange";
		playerendturn(m_currentScene->player);
	}
	else if (Player.greenalive == true) {
		Player.current_turn = "green";
		playerendturn(m_currentScene->player);
	}
	else if (Player.pinkalive == true) {
		Player.current_turn = "pink";
		playerendturn(m_currentScene->player);
	}
	else if (Player.whitealive == true) {
		Player.current_turn = "white";
		playerendturn(m_currentScene->player);
	}
	else if (Player.purplealive == true) {
		Player.current_turn = "purple";
		playerendturn(m_currentScene->player);
	}
	else if (Player.tealalive == true) {
		Player.current_turn = "teal";
		playerendturn(m_currentScene->player);
	}
	else if (Player.greyalive == true) {
		Player.current_turn = "grey";
		playerendturn(m_currentScene->player);
	}
	
}


/*void Game::planetClicked(PlayerObject& Player) {
	std::string turn = Player.getcurrentturn();
	if (turn == "red") {
		
	}
	else if (Player.current_turn == "red") {

	}
	else if (Player.current_turn == "red") {

	}
	else if (Player.current_turn == "red") {

	}
	else if (Player.current_turn == "red") {

	}
	else if (Player.current_turn == "red") {

	}
	else if (Player.current_turn == "red") {

	}
	else if (Player.current_turn == "red") {

	}
};*/

int Game::Getgamesize(){
	int Gsize = gamesize;
	return(Gsize);
};

void Game::render()
{
	
	// e.g. pass object details to the engine to render the next frame
	m_engineInterfacePtr->renderColouredBackground(0,0,0);

	// update the camera (probably don't need to do this each frame)
	m_engineInterfacePtr->setCamera(m_currentScene->player.getComponent<CameraComponent>());
	// draws all the models to the screen
	for(int i = 1; i < m_currentScene->v_LevelObjects.size(); i++){
		m_engineInterfacePtr->drawModel(m_currentScene->v_LevelObjects[i].getComponent<ModelComponent>()->themodel,m_currentScene->v_LevelObjects[i].getComponent<TransformComponent>()->getModelMatrix());
	}
	


}

void Game::menusetup() {
	m_currentScene->menu();
	m_inputHandler->Addinput(&m_currentScene->player);
}

