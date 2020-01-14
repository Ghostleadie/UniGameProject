#include "GLFW_EngineCore.h"



std::vector<bool> GLFW_EngineCore::m_keyBuffer;
int GLFW_EngineCore::m_screenWidth;
int GLFW_EngineCore::m_screenHeight;

GLFW_EngineCore::~GLFW_EngineCore()
{
	
	// cleanup
	glfwTerminate();
}

bool GLFW_EngineCore::initWindow(int width, int height, std::string windowName)
{
	
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	m_screenWidth = width;
	m_screenHeight = height;

	m_window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
	if (m_window == nullptr)
	{
		std::cout << "Failed to create GLFW m_window" << std::endl;
		glfwTerminate();
		return false;
	}

	ImGui::CreateContext();
	ImGui_ImplGlfwGL3_Init(m_window, true);

	glfwMakeContextCurrent(m_window);
	
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialise GLAD" << std::endl;
		return false;
	}

	// callback functions
	glfwSetFramebufferSizeCallback(m_window, windowResizeCallbackEvent);
	glfwSetKeyCallback(m_window, keyCallbackEvent);

	// make space for the keybuffer
	m_keyBuffer.resize(m_keyBufferSize);
	std::fill(m_keyBuffer.begin(), m_keyBuffer.end(), false);

	// set the shaders to the given default ones
	setDefaultShaders();

	// set the drawable model as a cube
	// note: this will be changed later when we can draw many kinds of objects
	initCubeModel();

	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// enable depth test
	glEnable(GL_DEPTH_TEST);

	return true;
}

bool GLFW_EngineCore::runEngine(Game& game)
{
	// for this example just give the game direct access to the engine
	// there are other ways this could be handled
	game.m_engineInterfacePtr = this;
	double mousex, mousey;
	int winx, winy;
	
	ImGuiIO& io = ImGui::GetIO();
	glfwGetFramebufferSize(m_window, &game.wWidth, &game.wHeight);
	// message loop
	while (!glfwWindowShouldClose(m_window))
	{
		glfwGetWindowSize(m_window, &winx, &winy);
		glfwGetCursorPos(m_window, &mousex, &mousey);
		
		//glfwSetMouseButtonCallback(m_window, glfw_mouse_click_callback(m_window,io.MouseDown[0],game));
		if (game.CurrentLevel != "menu") {
			if (io.MouseDown[1]) {
				game.m_inputHandler->mouseinput(mousex, mousey, winx, winy);
				glfwSetCursorPos(m_window, winx / 2, winy / 2);
			}
		}
		
		game.m_inputHandler->handleInputs(m_keyBuffer);
		//menu screen
		game.Gamelogic(game.m_currentScene->player);
		//game.menu();
		game.gui(game.m_currentScene->player);
		//gui.gui(game);
		game.playermovement();
		game.render(); // prepare game to send info to the renderer in engine core
		
		if (game.CurrentLevel == "game") {
			//glfwSetMouseButtonCallback(m_window, glfw_mouse_click_callback)
			glm::vec3 playerpos = game.m_currentScene->player.getComponent<CameraComponent>()->position();
			//if (io.MouseDown[2]) {
				//std::cout << "Player Camera pos x: " << playerpos.x << " y: " << playerpos.y << " z: " << playerpos.z << std::endl;
			//}
			//glfw_mouse_click_callback(m_window, game);
			//rayfromMouse(mousex, mousey, game.m_currentScene->player.getComponent<CameraComponent>());
		}

		ImGui::Render();
		ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
		// swap buffers
		glfwSwapBuffers(m_window);
		glfwPollEvents();
	}

	return true;
}

void GLFW_EngineCore::renderColouredBackground(float r, float g, float b)
{
	glClearColor(r, g, b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/*glm::vec3 GLFW_EngineCore::rayfromMouse(float mousex, float mousey, CameraComponent* cam) {
	ImGuiIO& io = ImGui::GetIO();
	float x = (2.0f * mousex) / m_screenWidth - 1.0f;
	float y = 1.0f - (2.0f * mousey) / m_screenHeight;
	float z = 1.0f;
	//normalised
	glm::vec3 ray_norm = glm::vec3(x, y, z);
	// Homogeneous Coordinates
	glm::vec4 ray_Hom = glm::vec4(ray_norm.x, ray_norm.y, -1.0, 1.0);
	//glm::inverse(ray_Hom);
	glm::mat4 ray_proj = projection;
	glm::inverse(ray_proj);
	glm::vec4 ray_eye = ray_proj * ray_Hom;
	ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0, 0);
	glm::mat4 ray_view = cam->getViewMatrix();
	glm::inverse(ray_view);
	glm::vec3 ray_world = ray_view * ray_eye;
	ray_world = glm::normalize(ray_world);
	if (io.MouseDown[2]) {
		std::cout << " x: " << ray_world.x << " y: " << ray_world.y << " z: " << ray_world.z;
	}
	return ray_world;
	

}*/

/*bool GLFW_EngineCore::planetclicked(glm::vec3 ray_origin_wor, glm::vec3 ray_direction_wor, glm::vec3 planet_centre_wor, float planet_radius,float *intersection_distance) {
	//first method

	glm::vec3 Dist2Planet = ray_origin_wor - planet_centre_wor;
	//float a = glm::dot(ray_direction_wor, ray_direction_wor);
	//float b = 2.0 * glm::dot(ray_direction_wor, (0.0f - Dist2Planet));
	//float c = (glm::dot((0.0f - Dist2Planet), (0.0f - Dist2Planet))) - (planet_radius * planet_radius);
	//float quad = (b * b) + (-4.0) *a *c;
	//second

	//float a = glm::dot(ray_direction_wor, ray_direction_wor);
	float b = glm::dot(ray_direction_wor, (0.0f - Dist2Planet));
	float c = (glm::dot((0.0f - Dist2Planet), (0.0f - Dist2Planet))) - (planet_radius * planet_radius);
	float quad = (b * b) - c;
	//glm::vec3 glm::intersectRaySphere<>(ray_origin_wor, quad, planet_centre_wor, planet_radius, *intersection_distance);
	//check if ray misses planet completely
	if (quad < 0.0f) {
		std::cout << "fucked" << std::endl;
		return false;
	}
	//checking if ray hits twice
	if (quad > 0.0f) {
		glm::sqrt(quad);
		//first
		//float insect_a = (-b + quad) / (2.0 * a);
		//float insect_b = (-b - quad) / (2.0 * a);
		//second
		float insect_a = -b + quad;
		float insect_b = -b - quad;
		*intersection_distance = insect_b;
		//std::cout << "intersection b "<< insect_b << std::endl;;
		//check if behind camera
		if (insect_a < 0.0) {
			if (insect_b < 0.0) {
				std::cout << "missed" << std::endl;;
				return false;
			}
		}
		else if (insect_b < 0.0) {
			*intersection_distance = insect_a;
			//std::cout << "intersection b " << insect_a << std::endl;;
		}
		return true;
	}
	//skimming the edge
	if (quad == 0.0f) {
		float edge = -b + glm::sqrt(quad);
		if (edge < 0.0f) {
			return false;
		}
		*intersection_distance = edge;
		std::cout <<  "edge " << edge << std::endl;;
		return true;
	}
	return false;
	
}*/

/*void GLFW_EngineCore::glfw_mouse_click_callback(GLFWwindow *window, Game& game) {
	ImGuiIO& io = ImGui::GetIO();
	if (io.MouseClicked[0]) {
		std::cout << "mouse clicked" << std::endl;;
		double mousexpos, mouseypos;
		glfwGetCursorPos(m_window, &mousexpos, &mouseypos);

		glm::vec3 ray_war = rayfromMouse((float)mousexpos, (float)mouseypos, game.m_currentScene->player.getComponent<CameraComponent>());
		
		int closestPlanet = -1;
		float closestInsec = 0.0f;
		int tsize = game.m_currentScene->v_LevelObjects.size();
		for (int i = 0; i < tsize; i++) {
			float t_dist = 0.0f;
			if (planetclicked(game.m_currentScene->player.getComponent<CameraComponent>()->m_position, ray_war, game.m_currentScene->v_LevelObjects[i].getComponent<TransformComponent>()->position(), planet_radius, &t_dist)) {
				if (closestPlanet == -1 || t_dist < closestInsec) {
					closestPlanet = i;
					//std::cout << "closest planet: " << closestPlanet << std::endl;
					closestInsec = t_dist;
					//std::cout << "closest intersection: " << closestInsec << std::endl;
				}
			}
		}
		planetclick = closestPlanet;
		if (planetclick != -1 && planetclick !=0) {
			std::string name = game.m_currentScene->v_LevelObjects[planetclick].getName();
			std::cout << name << std::endl;
			//glm::vec3 a = game.m_currentScene->player.getComponent<CameraComponent>()->m_position;
			
			//glm::intersectRaySphere()
		}
	}
}*/


//-----------------------------Private functions------------------------------
void GLFW_EngineCore::keyCallbackEvent(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	//m_keyBuffer[69] = ((action == GLFW_RELEASE || action == GLFW_RELEASE));
	//m_keyBuffer[81] = ((action == GLFW_RELEASE || action == GLFW_RELEASE));
	if (key == GLFW_KEY_UNKNOWN || key > m_keyBufferSize)
	{
		return;
	}
	m_keyBuffer[key] = ((action == GLFW_PRESS || action == GLFW_REPEAT));
	

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	
}

void GLFW_EngineCore::windowResizeCallbackEvent(GLFWwindow* window, int width, int height)
{
	// change the opengl viewport to match the new m_window size
	m_screenWidth = width;
	m_screenHeight = height;
	glViewport(0, 0, width, height);
}

// loading some default shaders to get things up and running
void GLFW_EngineCore::setDefaultShaders()
{
	// Load contents of vertex file
	std::ifstream inFile("assets/shaders/defaultShader.vert");
	if (!inFile) {
		fprintf(stderr, "Error opening file: shader\n");
		exit(1);
	}

	std::stringstream code;
	code << inFile.rdbuf();
	inFile.close();
	std::string codeStr(code.str());
	const GLchar* vertex_shader[] = { codeStr.c_str() };

	// Load contents of fragment file
	std::ifstream inFile2("assets/shaders/defaultShader.frag");
	if (!inFile2) {
		fprintf(stderr, "Error opening file: shader\n");
		exit(1);
	}

	std::stringstream code2;
	code2 << inFile2.rdbuf();
	inFile2.close();
	std::string codeStr2(code2.str());
	const GLchar* fragment_shader[] = { codeStr2.c_str() };
	
	// vertex shader
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, vertex_shader, NULL);
	glCompileShader(vertexShader);	
	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// fragment shader
	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, fragment_shader, NULL);
	glCompileShader(fragmentShader);
	// check for shader compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// link shaders
	m_defaultShaderProgram = glCreateProgram();
	glAttachShader(m_defaultShaderProgram, vertexShader);
	glAttachShader(m_defaultShaderProgram, fragmentShader);
	glLinkProgram(m_defaultShaderProgram);
	// check for linking errors
	glGetProgramiv(m_defaultShaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(m_defaultShaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glUseProgram(m_defaultShaderProgram);
}

// a simple function to initialise a cube model in memory
void GLFW_EngineCore::initCubeModel()
{
	// set up vertex and normal data
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};
	
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	
}

void GLFW_EngineCore::setCamera(const CameraComponent* cam)
{
	// set the view and projection components of our shader to the camera values
	projection = glm::perspective(glm::radians(cam->m_fov), (float)m_screenWidth / (float)m_screenHeight, 0.1f, 100.0f);
	glUniformMatrix4fv(glGetUniformLocation(m_defaultShaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	glUniformMatrix4fv(glGetUniformLocation(m_defaultShaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(cam->getViewMatrix()));

	// be sure to activate shader when setting uniforms/drawing objects
	glUniform3f(glGetUniformLocation(m_defaultShaderProgram, "objectColour"), 1.0f, 0.6f, 0.61f);
	glUniform3f(glGetUniformLocation(m_defaultShaderProgram, "lightColour"), 1.0f, 1.0f, 1.0f);
	glUniform3f(glGetUniformLocation(m_defaultShaderProgram, "lightPos"), 0.0f, 2.0f, -2.0f);
	glUniform3fv(glGetUniformLocation(m_defaultShaderProgram, "viewPos"), 1, glm::value_ptr(cam->position()));
	
}

void GLFW_EngineCore::drawCube(const glm::mat4& modelMatrix)
{
	// set the model component of our shader to the cube model
	glUniformMatrix4fv(glGetUniformLocation(m_defaultShaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));

	// the only thing we can draw so far is the cube, so we know it is bound already
	// this will obviously have to change later
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void GLFW_EngineCore::drawModel(Model* model, glm::mat4& modelMatrix)
{
	// set the model component of our shader to the object model
	glUniformMatrix4fv(glGetUniformLocation(m_defaultShaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));

	model->render(m_defaultShaderProgram);
}
