#pragma once
#include "GameObject.h"
#include "PlayerObject.h"
#include "CameraComponent.h"
#include "SFML\Graphics.hpp"
#include "Scene.h"
#include <SFML\Graphics.hpp>
#include "ModelComponent.h"
#include <ImGUI\imgui.h>
#include <ImGUI\imgui_impl_glfw_gl3.h>
#include <ImGUI\imgui_internal.h>
#include "TransformComponent.h"
#include "ColourComponent.h"
#include "CameraComponent.h"

class GUI {
public:
	GUI();
	void gui(Game& game);


};

