#include "Game.h"
#include <fstream>
#include <sstream>
#include <json/json/Json/json.h>
#include "ModelComponent.h"
#include "CameraComponent.h"
#include "TransformComponent.h"
#include "Serializer.h"
#include "Planet.h"

void Scene::menu() {
	glm::vec3 pos(0, 0, 0);
	size = 1;
	v_LevelObjects.resize(size);
	player.addComponent(new TransformComponent(pos));
	//player.lvlsize = size;
	//player.addComponent(new ModelComponent(myPlayerModel));
	player.addComponent(new CameraComponent(pos));
	seedgenerated = false;
	//std::cout << "player spawned at " << "x: " << x << " y: " << y << " Z: " << z;
}

void Scene::createLevel(int gSize, int Nplayers, bool AI, int playerSeed)
{
	if (seedgenerated == false && playerSeed == 0) {
		sTime = Timer.getElapsedTime().asMilliseconds();
		srand(sTime);
		seed = (rand() % 500 + 1);
		seedgenerated = true;
		std::cout << "Your seed is " << seed << std::endl;
	}
	else {
		seed = playerSeed;
	}
	if (gSize == 1) {
		srand(seed);
		size = (rand() % 10 + 15);
		seedpos = (rand() % 50 + 51);
	}
	if (gSize == 2) {
		srand(seed);
		size = (rand() % 30 + 25);
		seedpos = (rand() % 100 + 76);
	}
	if (gSize == 3) {
		srand(seed);
		size = (rand() % 40 + 25);
		seedpos = (rand() % 100 + 101);
	}

	int tsize = size;
	v_LevelObjects.resize(size);
	for (int i = 0; i < size; i++) {
		
		//std::cout << "planet " << i << " loaded / ";
		
		//test1 = test1 + i;

		float x, y, z;

		x = (rand() % seedpos + 1);
		y = 0;
		z = (rand() % seedpos + 1);

		glm::vec3 pos(x, y, z);

		

		if (playerspawned == false) {

			player.addComponent(new TransformComponent(pos));
			player.lvlsize = size;
			player.addComponent(new CameraComponent(pos));
			std::cout << "player spawned at " << "x: " << x << " y: " << y << " Z: " << z << std::endl;
			playerspawned = true;
		}else
		{
			std::string name = "planet " + std::to_string(i);
			v_LevelObjects[i].name = name;
			v_LevelObjects[i].navname = v_LevelObjects[i].getName().c_str();
			std::cout << v_LevelObjects[i].name << "spawned" << std::endl;
			playercheck = true;
			v_LevelObjects[i].Fleets = 2;
			v_LevelObjects[i].addComponent(new TransformComponent(pos));
			if (playerspawned == true) {
				int check = 1;
				for (int p = 0; p < i; p++) {
					int a = v_LevelObjects[i].getComponent<TransformComponent>()->position().x;
					int a2 = v_LevelObjects[check].getComponent<TransformComponent>()->position().x;
					int c = v_LevelObjects[i].getComponent<TransformComponent>()->position().z;
					int c2 = v_LevelObjects[check].getComponent<TransformComponent>()->position().z;
					int AC = floor(sqrt((a*a2) + (c*c2)));
					if (AC < 10) {
						float x, y, z;
						sTime = Timer.getElapsedTime().asMicroseconds();
						srand(sTime);
						x = (rand() % seedpos + 1);
						z = (rand() % seedpos + 1);
			
						v_LevelObjects[i].getComponent<TransformComponent>()->m_position.x = x;
						v_LevelObjects[i].getComponent<TransformComponent>()->m_position.z = z;
					}
					check++;
				}
			
			}
			if (Nplayers == 2) {
				if (playercheck == true) {
					if (player1 == true) {
						v_LevelObjects[i].team = "red";
						v_LevelObjects[i].addComponent(new ModelComponent(RedPlanet));
						player1 = false;
						player2 = true;
						playercheck = false;
					} else if (player2 == true) {
						firstRun = false;
						v_LevelObjects[i].team = "orange";
						v_LevelObjects[i].addComponent(new ModelComponent(orangePlanet));
						if (AI == true) {
							player2 = false;
							AI_player = true;
							playercheck = false;
						}
						else {
							player2 = false;
							player1 = true;
							playercheck = false;
						}
					}
					else if (AI == true) {
						v_LevelObjects[i].team = "yellow";
						v_LevelObjects[i].addComponent(new ModelComponent(YellowPlanet));
						AI_player = false;
						player1 = true;
						playercheck = false;
					}
				}

				//v_LevelObjects[i].addComponent(new ModelComponent(Planet));
			}
			if (Nplayers == 3) {
				if (playercheck == true) {
					if (player1 == true) {
						v_LevelObjects[i].team = "red";
						v_LevelObjects[i].addComponent(new ModelComponent(RedPlanet));
						player1 = false;
						player2 = true;
						playercheck = false;
					} else if (player2 == true) {
						v_LevelObjects[i].team = "orange";
						v_LevelObjects[i].addComponent(new ModelComponent(orangePlanet));
						player2 = false;
						player3 = true;
						playercheck = false;
					} else if (player3 == true) {
						v_LevelObjects[i].team = "green";
						v_LevelObjects[i].addComponent(new ModelComponent(GreenPlanet));
						if (AI == true) {
							player3 = false;
							AI_player = true;
							playercheck = false;
						}
						else {
							player3 = false;
							player1 = true;
							playercheck = false;
						}
					}
					else if (AI == true) {
						v_LevelObjects[i].team = "yellow";
						v_LevelObjects[i].addComponent(new ModelComponent(YellowPlanet));
						AI_player = false;
						player1 = true;
						playercheck = false;
					}
				}
			}
			if (Nplayers == 4) {
				if (playercheck == true) {
					if (player1 == true) {
						v_LevelObjects[i].team = "red";
						v_LevelObjects[i].addComponent(new ModelComponent(RedPlanet));
						player1 = false;
						player2 = true;
						playercheck = false;
					} else if (player2 == true) {
						v_LevelObjects[i].team = "orange";
						v_LevelObjects[i].addComponent(new ModelComponent(orangePlanet));
						player2 = false;
						player3 = true;
						playercheck = false;
					} else if (player3 == true) {
						v_LevelObjects[i].team = "green";
						v_LevelObjects[i].addComponent(new ModelComponent(GreenPlanet));
						player3 = false;
						player4 = true;
						playercheck = false;
					}else if (player4 == true) {
						v_LevelObjects[i].team = "pink";
						v_LevelObjects[i].addComponent(new ModelComponent(PinkPlanet));
						if (AI == true) {
							player4 = false;
							AI_player = true;
							playercheck = false;
						}
						else {
							player4 = false;
							player1 = true;
							playercheck = false;
						}
					}
					else if (AI == true) {
						v_LevelObjects[i].team = "yellow";
						v_LevelObjects[i].addComponent(new ModelComponent(YellowPlanet));
						AI_player = false;
						player1 = true;
						playercheck = false;
					}
				}
				//v_LevelObjects[i].addComponent(new ModelComponent(Planet));
			}
			if (Nplayers == 5) {
				if (playercheck == true) {
					if (player1 == true) {
						v_LevelObjects[i].team = "red";
						v_LevelObjects[i].addComponent(new ModelComponent(RedPlanet));
						player1 = false;
						player2 = true;
						playercheck = false;
					}else if (player2 == true) {
						v_LevelObjects[i].team = "orange";
						v_LevelObjects[i].addComponent(new ModelComponent(orangePlanet));
						player2 = false;
						player3 = true;
						playercheck = false;
					}else if (player3 == true) {
						v_LevelObjects[i].team = "green";
						v_LevelObjects[i].addComponent(new ModelComponent(GreenPlanet));
						player3 = false;
						player4 = true;
						playercheck = false;
					}else if (player4 == true) {
						v_LevelObjects[i].team = "pink";
						v_LevelObjects[i].addComponent(new ModelComponent(PinkPlanet));
						player4 = false;
						player5 = true;
						playercheck = false;
					}else if (player5 == true) {
						v_LevelObjects[i].team = "white";
						v_LevelObjects[i].addComponent(new ModelComponent(WhitePlanet));
						if (AI == true) {
							player5 = false;
							AI_player = true;
							playercheck = false;
						}
						else {
							player5 = false;
							player1 = true;
							playercheck = false;
						}
					}
					else if (AI == true) {
						v_LevelObjects[i].team = "yellow";
						v_LevelObjects[i].addComponent(new ModelComponent(YellowPlanet));
						AI_player = false;
						player1 = true;
						playercheck = false;
					}
				}
				//v_LevelObjects[i].addComponent(new ModelComponent(Planet));
			}
			if (Nplayers == 6) {
				if (playercheck == true) {
					if (player1 == true) {
						v_LevelObjects[i].team = "red";
						v_LevelObjects[i].addComponent(new ModelComponent(RedPlanet));
						player1 = false;
						player2 = true;
						playercheck = false;
					} else if (player2 == true) {
						v_LevelObjects[i].team = "orange";
						v_LevelObjects[i].addComponent(new ModelComponent(orangePlanet));
						player2 = false;
						player3 = true;
						playercheck = false;
					}else if (player3 == true) {
						v_LevelObjects[i].team = "green";
						v_LevelObjects[i].addComponent(new ModelComponent(GreenPlanet));
						player3 = false;
						player4 = true;
						playercheck = false;
					}else if (player4 == true) {
						v_LevelObjects[i].team = "pink";
						v_LevelObjects[i].addComponent(new ModelComponent(PinkPlanet));
						player4 = false;
						player5 = true;
						playercheck = false;
					}else if (player5 == true) {
						v_LevelObjects[i].team = "white";
						v_LevelObjects[i].addComponent(new ModelComponent(WhitePlanet));
						player5 = false;
						player6 = true;
						playercheck = false;
					}else if (player6 == true) {
						v_LevelObjects[i].team = "purple";
						v_LevelObjects[i].addComponent(new ModelComponent(PurplePlanet));
						if (AI == true) {
							player6 = false;
							AI_player = true;
							playercheck = false;
						}
						else {
							player6 = false;
							player1 = true;
							playercheck = false;
						}
					}
					else if (AI == true) {
						v_LevelObjects[i].team = "yellow";
						v_LevelObjects[i].addComponent(new ModelComponent(YellowPlanet));
						AI_player = false;
						player1 = true;
						playercheck = false;
					}
				}
				//v_LevelObjects[i].addComponent(new ModelComponent(Planet));
			}
			if (Nplayers == 7) {
				if (playercheck == true) {
					if (player1 == true) {
						v_LevelObjects[i].team = "red";
						v_LevelObjects[i].addComponent(new ModelComponent(RedPlanet));
						player1 = false;
						player2 = true;
						playercheck = false;
					}else if (player2 == true) {
						v_LevelObjects[i].team = "orange";
						v_LevelObjects[i].addComponent(new ModelComponent(orangePlanet));
						player2 = false;
						player3 = true;
						playercheck = false;
					}else if (player3 == true) {
						v_LevelObjects[i].team = "green";
						v_LevelObjects[i].addComponent(new ModelComponent(GreenPlanet));
						player3 = false;
						player4 = true;
						playercheck = false;
					}else if (player4 == true) {
						v_LevelObjects[i].team = "pink";
						v_LevelObjects[i].addComponent(new ModelComponent(PinkPlanet));
						player4 = false;
						player5 = true;
						playercheck = false;
					}else if (player5 == true) {
						v_LevelObjects[i].team = "white";
						v_LevelObjects[i].addComponent(new ModelComponent(WhitePlanet));
						player5 = false;
						player6 = true;
						playercheck = false;
					}else if (player6 == true) {
						v_LevelObjects[i].team = "purple";
						v_LevelObjects[i].addComponent(new ModelComponent(PurplePlanet));
						player6 = false;
						player7 = true;
						playercheck = false;
					}else if (player7 == true) {
						v_LevelObjects[i].team = "teal";
						v_LevelObjects[i].addComponent(new ModelComponent(TealPlanet));
						if (AI == true) {
							player7 = false;
							AI_player = true;
							playercheck = false;
						}
						else {
							player7 = false;
							player1 = true;
							playercheck = false;
						}
					}
					else if (AI == true) {
						v_LevelObjects[i].team = "yellow";
						v_LevelObjects[i].addComponent(new ModelComponent(YellowPlanet));
						AI_player = false;
						player1 = true;
						playercheck = false;
					}
				}
				//v_LevelObjects[i].addComponent(new ModelComponent(Planet));
			}
			if (Nplayers == 8) {
				if (playercheck == true) {
					if (player1 == true) {
						v_LevelObjects[i].team = "red";
						v_LevelObjects[i].addComponent(new ModelComponent(RedPlanet));
						player1 = false;
						player2 = true;
						playercheck = false;
					}else if (player2 == true) {
						v_LevelObjects[i].team = "orange";
						v_LevelObjects[i].addComponent(new ModelComponent(orangePlanet));
						player2 = false;
						player3 = true;
						playercheck = false;
					}else if (player3 == true) {
						v_LevelObjects[i].team = "green";
						v_LevelObjects[i].addComponent(new ModelComponent(GreenPlanet));
						player3 = false;
						player4 = true;
						playercheck = false;
					}else if (player4 == true) {
						v_LevelObjects[i].team = "pink";
						v_LevelObjects[i].addComponent(new ModelComponent(PinkPlanet));
						player4 = false;
						player5 = true;
						playercheck = false;
					}else if (player5 == true) {
						v_LevelObjects[i].team = "white";
						v_LevelObjects[i].addComponent(new ModelComponent(WhitePlanet));
						player5 = false;
						player6 = true;
						playercheck = false;
					}else if (player6 == true) {
						v_LevelObjects[i].team = "purple";
						v_LevelObjects[i].addComponent(new ModelComponent(PurplePlanet));
						player6 = false;
						player7 = true;
						playercheck = false;
					}else if (player7 == true) {
						v_LevelObjects[i].team = "teal";
						v_LevelObjects[i].addComponent(new ModelComponent(TealPlanet));
						player7 = false;
						player8 = true;
						playercheck = false;
					}else if (player8 == true) {
						v_LevelObjects[i].team = "grey";
						v_LevelObjects[i].addComponent(new ModelComponent(GreyPlanet));
						if (AI == true) {
							player8 = false;
							AI_player = true;
							playercheck = false;
						}
						else {
							player8 = false;
							player1 = true;
							playercheck = false;
						}
					}
					else if (AI == true) {
						v_LevelObjects[i].team = "yellow";
						v_LevelObjects[i].addComponent(new ModelComponent(YellowPlanet));
						AI_player = false;
						player1 = true;
						playercheck = false;
					}
				}
				//v_LevelObjects[i].addComponent(new ModelComponent(Planet));
			}
			
		}
		}
	/*	for (int i = 1; i < tsize; i++) {
			int a = v_LevelObjects[i].getComponent<TransformComponent>()->m_position.x;
			int c = v_LevelObjects[i].getComponent<TransformComponent>()->m_position.z;
			for (int p = tsize; p < 1; p--) {
				int a2 = v_LevelObjects[p].getComponent<TransformComponent>()->m_position.x;
				int c2 = v_LevelObjects[p].getComponent<TransformComponent>()->m_position.z;
				int poscheck = sqrt((a*a2) + (c*c2));
				if (poscheck <= 150) {
					float x, y, z;
					srand(seed);
					x = (rand() % seedpos + 1);
					y = 0;
					z = (rand() % seedpos + 1);

					glm::vec3 pos2(x, y, z);
					v_LevelObjects[p].getComponent<TransformComponent>()->m_position.x = pos2.x;
					v_LevelObjects[p].getComponent<TransformComponent>()->m_position.z = pos2.z;
					//v_LevelObjects[p].addComponent(new TransformComponent(pos2));
				}
			}*/
	
		
	

	// add a check for if objects are inside each other
}

int Scene::getobjectsize() {
	return size;
}

//void Scene::CreateLevelJson() {
	//std::fstream jsonData;
	//Json::Value root;
	//Json::Reader reader;
	//std::ofstream jsonLevel;
	//Jserializer js;
	//Game game;
	//int size = game.sizeseed;
	//char** planet = new char*[size];
	
	//for (int i = 1; i < size; i++) {
	//	planet[i] = "{\"name\" : planet, \"object\" : ModelCube, \"position\" : [0, 0, ,0]}";
	//	js.deserialize()
	//}
	
//}
