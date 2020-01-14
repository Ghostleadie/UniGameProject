#pragma once
#include "IEngineCore.h"
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <glm/mat4x4.hpp>
#include <ImGUI\imgui.h>
#include <ImGUI\imgui_impl_glfw_gl3.h>
#include <ImGUI\imgui_internal.h>
#include "CameraComponent.h"
#include "model.h"
#include "Game.h"
#include <fstream>
#include <sstream>
#include <numeric>
#include <glm/detail/type_vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm\gtx\intersect.hpp>
#include "GUI.h"


class Game;

class GLFW_EngineCore : public IEngineCore
{
public:
	~GLFW_EngineCore() override;

	bool initWindow(int width, int height, std::string windowName) override;
	bool runEngine(Game& game) override;
	void renderColouredBackground(float r, float g, float b) override;

	void setCamera(const CameraComponent* cam) override;
	void drawCube(const glm::mat4& modelMatrix) override;
	void drawModel(Model* model, glm::mat4& modelMatrix);

	glm::vec3 rayfromMouse(float mousex, float mousey, CameraComponent* cam);
	bool planetclicked(glm::vec3 ray_origin_wor, glm::vec3 ray_direction_wor, glm::vec3 planet_centre_wor, float planet_radius, float *intersection_distance);
	void glfw_mouse_click_callback(GLFWwindow *window, Game& game);
	//void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods, );
	const float planet_radius = 1.0f;
	int planetclick;
	//static void keyCallbackEvent(GLFWwindow* window, int key, int scancode, int action, int mods, PlayerObject& player);
	static int m_screenWidth;
	static int m_screenHeight;
	glm::mat4 projection;
	
private:
	GLFWwindow* m_window;
	GLuint m_defaultShaderProgram;
	
	
	static std::vector<bool> m_keyBuffer;
	static const int m_keyBufferSize = 400;

	static void mouseMoveCallbackEvent(GLFWwindow* window, double xPos, double yPos);
	static void keyCallbackEvent(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void windowResizeCallbackEvent(GLFWwindow* window, int width, int height);
	
	void setDefaultShaders();
	void initCubeModel();


};

