#include "GUI.h"

GUI::GUI() {

}

void GUI::gui(Game& game) {
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoResize;
	window_flags |= ImGuiWindowFlags_NoCollapse;
	window_flags |= ImGuiWindowFlags_NoMove;
	bool* p_open = false;
	static bool* AI = false;
	static int clicked = 0;


	ImGui_ImplGlfwGL3_NewFrame();
	ImGui::SetNextWindowSize(ImVec2(250, 300));
	ImGui::Begin("Risk", p_open, window_flags);
	if (game.Ismenu == true) {
		ImGui::Text("Welcome, to a game of RISK.");
		if (ImGui::Button("Start"))
			ImGui::OpenPopup("Game Set Up");
		if (ImGui::BeginPopupModal("Game Set Up", NULL, window_flags))
		{
			ImGui::Text("How many players do you want?");
			static int players = 0;
			ImGui::Combo("players", &players, " 2 \0 3 \0 4 \0 5 \0 6 \0 7 \0 8 \0\0");
			//static bool AI_Player = false;
			//ImGui::Checkbox("Enable Extra AI Player", &AI_Player);
			if (ImGui::Button("Start"))
				clicked++;

			if (clicked & 1)
			{

				if (players == 0) {
					cout << "test1";
					game.Ismenu = false;
					game.sTime = game.Timer.getElapsedTime().asMilliseconds();
					srand(game.sTime);
					game.seed = (rand() % 500 + 1);
					game.gamesize = 1;
					game.CurrentLevel = "game";
					game.test = true;
					game.Nplayer = 2;
					game.m_currentScene->createLevel(game.gamesize, game.Nplayer);

				}
				if (players == 1) {
					cout << "test2";
					game.Ismenu = false;
					game.sTime = game.Timer.getElapsedTime().asMilliseconds();
					srand(game.sTime);
					game.seed = (rand() % 500 + 1);
					game.gamesize = 1;
					game.CurrentLevel = "game";
					game.Nplayer = 3;
					game.m_currentScene->createLevel(game.gamesize, game.Nplayer);
				}
				if (players == 2) {
					cout << "test3";
					game.Ismenu = false;
					game.sTime = game.Timer.getElapsedTime().asMilliseconds();
					srand(game.sTime);
					game.seed = (rand() % 500 + 1);
					game.gamesize = 1;
					game.CurrentLevel = "game";
					game.Nplayer = 4;
					game.m_currentScene->createLevel(game.gamesize, game.Nplayer);
				}
				if (players == 3) {
					cout << "test4";
					game.Ismenu = false;
					game.sTime = game.Timer.getElapsedTime().asMilliseconds();
					srand(game.sTime);
					game.seed = (rand() % 500 + 1);
					game.gamesize = 2;
					game.CurrentLevel = "game";
					game.Nplayer = 5;
					game.m_currentScene->createLevel(game.gamesize, game.Nplayer);
				}
				if (players == 4) {
					cout << "test5";
					game.Ismenu = false;
					game.sTime = game.Timer.getElapsedTime().asMilliseconds();
					srand(game.sTime);
					game.seed = (rand() % 500 + 1);
					game.gamesize = 2;
					game.CurrentLevel = "game";
					game.Nplayer = 6;
					game.m_currentScene->createLevel(game.gamesize, game.Nplayer);
				}
				if (players == 5) {
					cout << "test6";
					game.Ismenu = false;
					game.sTime = game.Timer.getElapsedTime().asMilliseconds();
					srand(game.sTime);
					game.seed = (rand() % 500 + 1);
					game.gamesize = 2;
					game.CurrentLevel = "game";
					game.Nplayer = 7;
					game.m_currentScene->createLevel(game.gamesize, game.Nplayer);
				}
				if (players == 6) {
					cout << "test7";
					game.Ismenu = false;
					game.sTime = game.Timer.getElapsedTime().asMilliseconds();
					srand(game.sTime);
					game.seed = (rand() % 500 + 1);
					game.gamesize = 3;
					game.CurrentLevel = "game";
					game.Nplayer = 8;
					game.m_currentScene->createLevel(game.gamesize, game.Nplayer);
				}
				if (players == 7) {
					cout << "test8";
					game.Ismenu = false;
					game.sTime = game.Timer.getElapsedTime().asMilliseconds();
					srand(game.sTime);
					game.seed = (rand() % 500 + 1);
					game.gamesize = 3;
					game.CurrentLevel = "game";
					game.Nplayer = 2;
					game.m_currentScene->createLevel(game.gamesize, game.Nplayer);
				}
				ImGui::CloseCurrentPopup();
			}
			clicked = 0;
			if (ImGui::Button("Back"))
				ImGui::CloseCurrentPopup();
			ImGui::EndPopup();
		}
		ImGui::Button("Tutorial");
		if (ImGui::Button("Settings"))
			ImGui::OpenPopup("Game Settings");
		if (ImGui::BeginPopupModal("Game Settings", NULL, window_flags))
		{
			ImGui::Text("Audio");

			//ImGui::DragInt("drag int 0..100", &i2, 1, 0, 100, "%.0f%%");



			if (ImGui::Button("Back"))
				ImGui::CloseCurrentPopup();
			ImGui::EndPopup();
		}
		if (ImGui::Button("Exit"))
			std::_Exit(EXIT_FAILURE);
		ImGui::Text("Created by Jack Brewer.");
	}
	if (game.Ismenu == false) {

		//char turn = player.current_turn.c_str;

		//ImGui::Text(turn); ImGui::SameLine(); ImGui::Text(" turn");
		if (game.m_currentScene->player.renforcements == true) {
			ImGui::Text("You are currently at "); ImGui::SameLine(); ImGui::Text((const char*)game.m_currentScene->v_LevelObjects[game.testpos].getName().c_str());
			if (ImGui::Button("Renforce Planet")) {
				ImGui::OpenPopup("Game Set Up");
				if (ImGui::BeginPopupModal("Game Set Up", NULL, window_flags))
				{
					ImGui::Text("How many soldier do you want?");
					int planet = game.m_currentScene->player.currently_looking_at;
					int ren = game.renforcementnumber;
					int starting_number = game.m_currentScene->v_LevelObjects[planet].troops;
					static int current_number = game.m_currentScene->v_LevelObjects[planet].troops;
					ImGui::SliderInt("renforcements", &current_number, starting_number, ren);
					if (ImGui::Button("Add Troops")) {

						game.m_currentScene->v_LevelObjects[planet].troops = current_number;
					}
					//static bool AI_Player = false;
					//ImGui::Checkbox("Enable Extra AI Player", &AI_Player);
					//					if (ImGui::Button(""))
					//						clicked++;

					//					if (clicked & 1)
					//					{
					//					}
				}


			}

			if (ImGui::Button("Next Stage")) {
				if (game.m_currentScene->player.renforcements == true) {

				}
				else if (game.m_currentScene->player.attack == true) {

				}
				else if (game.m_currentScene->player.manover == true) {

				}
				else if (game.m_currentScene->player.turnover == true) {
					if (game.m_currentScene->player.current_turn == "red") {
						game.m_currentScene->player.current_turn == "orange";
					}
					else if (game.m_currentScene->player.current_turn == "red") {
						game.m_currentScene->player.current_turn == "orange";

					}
					else if (game.m_currentScene->player.current_turn == "red") {
						game.m_currentScene->player.current_turn == "orange";

					}
					else if (game.m_currentScene->player.current_turn == "red") {
						game.m_currentScene->player.current_turn == "orange";

					}
					else if (game.m_currentScene->player.current_turn == "red") {
						game.m_currentScene->player.current_turn == "orange";

					}
					else if (game.m_currentScene->player.current_turn == "red") {
						game.m_currentScene->player.current_turn == "orange";

					}
					else if (game.m_currentScene->player.current_turn == "red") {
						game.m_currentScene->player.current_turn == "orange";

					}
					else if (game.m_currentScene->player.current_turn == "red") {
						game.m_currentScene->player.current_turn == "orange";

					}
				}



				/*
				static auto vector_getter = [](void* vec, int idx, const char** out_text)
				{
				auto& vector = *static_cast<std::vector<GameObject>*>(vec);
				if (idx < 0 || idx >= static_cast<int>(vector.size())) { return false; }
				*out_text = vector.at(idx).getName();
				return true;
				};

				bool ListBox(const char* label, int* currIndex, std::vector<GameObject>&values) {
				if (values.empty()) {
				return false;
				}
				return ListBox(label, currIndex, vector_getter, static_cast<void*>(&values), values.size());
				};*/




				/*static int listboxGameobjectCurrentIndex = -1;

				static auto getGameObjectNameFunc = [](void* vec, int idx, const char** out_text)
				{
				// need a cast as it is passed in as void* - a bit messy
				std::vector<GameObject*>* vector = static_cast<std::vector<GameObject*>*>(vec);
				// only get the name if it’s a valid index
				if (idx < 0 || idx >= vector->size())
				return false;
				*out_text = vector->at(idx)->getName();
				return true;
				};*/

				//ImGui::ListBox("", &listboxGameobjectCurrentIndex, getGameObjectNameFunc, static_cast<void*>(&m_currentScene->v_LevelObjects), test, 30);
			}
		}
	}
	ImGui::End();
}
