#include <Global.h>
#include <cSceneManager.h>
#include <cEntityBuilder.h>
#include <cRenderMeshComponent.h>
#include <cSoundManager.h>
#include <cParticleComponent.h>

bool isShiftDown(GLFWwindow* window);
bool isCtrlDown(GLFWwindow* window);
bool isAltDown(GLFWwindow* window);
bool areAllModifiersUp(GLFWwindow* window);

void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	
	cSceneManager* pSceneManager = cSceneManager::getInstance();
	cScene* pScene = pSceneManager->getActiveScene();
	sLight* selectedLight = pScene->getSelectedLight();
	cEntity* selectedEntity = pScene->getSelectedEntity();

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}

	if (key == GLFW_KEY_P && action == GLFW_PRESS) {
		pScene->setIsScenePaused(!pScene->getIsScenePaused());
	}

	if (areAllModifiersUp(window)) {

		if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
			if (pScene->selectedEntity == pScene->getNumberOfEntites() - 1) {
				pScene->selectedEntity = 0;
			}
			else {
				pScene->selectedEntity++;
			}
		}

		if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
			if (pScene->selectedEntity != 0) {
				pScene->selectedEntity--;
			}
		}

		if (key == GLFW_KEY_B && action == GLFW_PRESS) {
			std::vector<cEntity*> entities = pScene->getEntities();

			for (unsigned int i = 0; i != entities.size(); ++i) {
				cParticleComponent* pCurParticle = static_cast<cParticleComponent*>(entities[i]->components[7]);

				if (pCurParticle != nullptr) {
					pCurParticle->particleGenerationOn();
				}
			}
		}

		if (key == GLFW_KEY_N && action == GLFW_PRESS) {
			std::vector<cEntity*> entities = pScene->getEntities();

			for (unsigned int i = 0; i != entities.size(); ++i) {
				cParticleComponent* pCurParticle = static_cast<cParticleComponent*>(entities[i]->components[7]);

				if (pCurParticle != nullptr) {
					pCurParticle->particleGenerationOff();
				}
			}
		}
	}

	// Shift down? Manipulate light objects
	if (isShiftDown(window)) {

		if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
			if (pScene->selectedLight == pScene->getNumberOfLights() - 1) {
				pScene->selectedLight = 0;
			}
			else {
				pScene->selectedLight++;
			}
		}

		if (glfwGetKey(window, GLFW_KEY_DOWN))
		{
			if (pScene->selectedLight != 0) {
				pScene->selectedLight--;
			}
		}

		if (glfwGetKey(window, GLFW_KEY_7)) { // Turn on/off debug light
			pScene->bIsLightDebug = !pScene->bIsLightDebug;
		}

		if (glfwGetKey(window, GLFW_KEY_SPACE)) {	// Turn light on/off
			selectedLight->param2.x = selectedLight->param2.x ? 0.0f : 1.0f;
		}
	}

	// Alt down? Manipultate Enity objects
	if (isAltDown(window)) {

		if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
			cRenderMeshComponent* meshComonent = (cRenderMeshComponent*)selectedEntity->components[3];

			meshComonent->bIsWireFrame = !meshComonent->bIsWireFrame;
		}
	}

	// Ctrl down? Other options
	if (isCtrlDown(window)) {
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			pSceneManager->getActiveScene()->serialize();
		}

		if (key == GLFW_KEY_D && action == GLFW_PRESS) {
			pScene->bIsRenderDebug = !pScene->bIsRenderDebug;
		}
	}
}