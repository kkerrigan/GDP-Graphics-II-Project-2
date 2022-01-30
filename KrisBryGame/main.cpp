#include <iostream>
#include <cEngine.h>
#include <cConsole.h>
#include <cShaderManager.h>
#include <cVAOMeshManager.h>
#include <cEntityBuilder.h>
#include <cDebugRenderer.h>
#include <cSoundManager.h>

// forward declare some functions
bool loadShaders();
void loadTextures();

void updateCallback(double deltaTime);

void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void keyboardCallbackASYNC(GLFWwindow* window);

void mouseCallbackAsync(GLFWwindow* window);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void cursor_enter_callback(GLFWwindow* window, int entered);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

void loadModels();
void loadScene();
void loadLights();
void loadTerrainAABBHierarchy();

int main() {

	cConsole console;

	// initialize the engine and window
	cEngine* pEngine = cEngine::getInstance();
	sWindowSettings windowSettings(1024, 720, "Graphics Checkpoint 6");

	if (!pEngine->initialize(&windowSettings)) {
		std::cout << "Failed to initialize engine." << std::endl;
		exit(EXIT_FAILURE);
	}

	if (!loadShaders()) {
		std::cout << "Failed to load shaders." << std::endl;
		exit(EXIT_FAILURE);
	}

	// Set up SOund Manager
	cSoundManager* pSoundManager = cSoundManager::getInstance();
	pSoundManager->initialize();
	pSoundManager->setBasePath("./assets/sounds/");

	// set lots of callbacks
	pEngine->setUpdateCallback(updateCallback);
	pEngine->setKeyboardCallback(keyboardCallback);
	pEngine->setKeyboardCallbackAsync(keyboardCallbackASYNC);
	pEngine->setMouseCallbackAsync(mouseCallbackAsync);
	pEngine->setMouseCursorPositionCallback(cursor_position_callback);
	pEngine->setMouseButtonCallback(mouse_button_callback);
	pEngine->setMouseEnterCallback(cursor_enter_callback);
	pEngine->setMouseScrollCallback(scroll_callback);

	// setup debug renderer
	cDebugRenderer::getInstance()->initialize();

	//load textures
	loadTextures();

	//load model files
	loadModels();

	// load the entities
	loadScene();

	// load the terrain aabb Hierarchy (for broad phase collision detection)
	//loadTerrainAABBHierarchy();

	// load lights
	//loadLights();

	// start the game loop (hang on..!)
	pEngine->startGameLoop();

	cSoundManager::getInstance()->shutDown();

	return 0;
}
