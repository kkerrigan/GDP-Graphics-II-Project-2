#include <Global.h>
#include <cSceneManager.h>
#include <cEntityBuilder.h>
#include <cTransformComponent.h>
#include <cRenderMeshComponent.h>
#include <c3dSoundComponent.h>
#include <iCommand.h>

bool isShiftDown(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)) { return true; }
	if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT)) { return true; }
	// both are up
	return false;
}

bool isCtrlDown(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL)) { return true; }
	if (glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL)) { return true; }
	// both are up
	return false;
}

bool isAltDown(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_LEFT_ALT)) { return true; }
	if (glfwGetKey(window, GLFW_KEY_RIGHT_ALT)) { return true; }
	// both are up
	return false;
}

bool areAllModifiersUp(GLFWwindow* window) {
	if (isShiftDown(window)) { return false; }
	if (isCtrlDown(window)) { return false; }
	if (isAltDown(window)) { return false; }
	// Yup, they are all UP
	return true;
}

void keyboardCallbackASYNC(GLFWwindow* window) {
	/*
	* All keypresses get sent to the "mediator" (scene) and are handled there. The command do no know the
	* actual object they are going to act on until they are executed
	*/
	
	cScene* pScene = cSceneManager::getInstance()->getActiveScene();
	
	const float CAMERA_SPEED_SLOW = 0.1f;
	const float CAMERA_SPEED_FAST = 1.0f;

	float cameraSpeed = CAMERA_SPEED_SLOW;
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		cameraSpeed = CAMERA_SPEED_FAST;
	}

	// If no keys are down, move the camera
	if (areAllModifiersUp(window)) {
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			pScene->handleCommand(new cMoveCameraForwardCommand(cameraSpeed));
		}
		
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
			pScene->handleCommand(new cMoveCameraForwardCommand(-cameraSpeed));
		}

		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			pScene->handleCommand(new cMoveCameraLeftRightCommand(-cameraSpeed));
		}

		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			pScene->handleCommand(new cMoveCameraLeftRightCommand(cameraSpeed));
		}

		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
			pScene->handleCommand(new cMoveCameraUpDownCommand(-cameraSpeed));
		}

		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
			pScene->handleCommand(new cMoveCameraUpDownCommand(cameraSpeed));
		}

		if (glfwGetKey(window, GLFW_KEY_G)) {
			cEntity* entityToBlendTexture = pScene->getEntities()[0];
			cRenderMeshComponent* meshComp = (cRenderMeshComponent*)entityToBlendTexture->components[3];

			meshComp->textures[0].strength -= 0.01f;
			if (meshComp->textures[0].strength < 0.0f) {
				meshComp->textures[0].strength = 0.0f;
			}

			meshComp->textures[1].strength = 1.0f - meshComp->textures[0].strength;
		}

		if (glfwGetKey(window, GLFW_KEY_H)) { 
			cEntity* entityToBlendTexture = pScene->getEntities()[0];
			cRenderMeshComponent* meshComp = (cRenderMeshComponent*) entityToBlendTexture->components[3];

			meshComp->textures[0].strength += 0.01f;
			if (meshComp->textures[0].strength > 1.0f) {
				meshComp->textures[0].strength = 1.0f;
			}

			meshComp->textures[1].strength = 1.0f - meshComp->textures[0].strength;
		}
	}

	// Shift down? Manipulate light objects
	if (isShiftDown(window)) {
		
		sLight* selectedLight = pScene->getSelectedLight();

		if (glfwGetKey(window, GLFW_KEY_W)) { // Forward
			pScene->handleCommand(new cMoveLightForwardCommand(cameraSpeed));
		}

		if (glfwGetKey(window, GLFW_KEY_S)) { // Backward
			pScene->handleCommand(new cMoveLightForwardCommand(-cameraSpeed));
		}

		if (glfwGetKey(window, GLFW_KEY_A)) {  // Left
			pScene->handleCommand(new cMoveLightLeftRightCommand(-cameraSpeed));
		}

		if (glfwGetKey(window, GLFW_KEY_D)) { // Right
			pScene->handleCommand(new cMoveLightLeftRightCommand(cameraSpeed));
		}

		if (glfwGetKey(window, GLFW_KEY_Q)) { // Up
			pScene->handleCommand(new cMoveLightUpDownCommand(cameraSpeed));
		}

		if (glfwGetKey(window, GLFW_KEY_E)) { // Down
			pScene->handleCommand(new cMoveLightUpDownCommand(-cameraSpeed));
		}

		if (glfwGetKey(window, GLFW_KEY_1)) { // Decrease Const	
			pScene->handleCommand(new cLightChangeConstAttenuationCommand(0.99f));
		}

		if (glfwGetKey(window, GLFW_KEY_2)) { //Increase Const	
			pScene->handleCommand(new cLightChangeConstAttenuationCommand(1.01f));
		}

		if (glfwGetKey(window, GLFW_KEY_3)) { // Decrease Linear	
			pScene->handleCommand(new cLightChangeLinAttenuationCommand(0.99f));
		}

		if (glfwGetKey(window, GLFW_KEY_4)) { // Increase Linear	
			pScene->handleCommand(new cLightChangeLinAttenuationCommand(1.01f));
		}

		if (glfwGetKey(window, GLFW_KEY_5)) { // Decrease Quadratic	
			pScene->handleCommand(new cLightChangeQuadAttenuationCommand(0.99f));
		}

		if (glfwGetKey(window, GLFW_KEY_6)) { // Increase Quadratic	
			pScene->handleCommand(new cLightChangeQuadAttenuationCommand(1.01f));
		}
	}

	// Alt down? Manipultate Enity objects
	if (isAltDown(window)) {
		
		cTransformComponent* selectedEntityTransformComp = (cTransformComponent*)pScene->getSelectedEntity()->components[1];
		cRenderMeshComponent* selectedEntityMeshComp = (cRenderMeshComponent*)pScene->getSelectedEntity()->components[3];

		if (glfwGetKey(window, GLFW_KEY_W)) {
			pScene->handleCommand(new cMoveEntiyForwardCommand(cameraSpeed));
		}

		if (glfwGetKey(window, GLFW_KEY_S)) {
			pScene->handleCommand(new cMoveEntiyForwardCommand(-cameraSpeed));
		}

		if (glfwGetKey(window, GLFW_KEY_A)) {
			pScene->handleCommand(new cMoveEntiyLeftRightCommand(cameraSpeed));
		}
		if (glfwGetKey(window, GLFW_KEY_D)) {
			pScene->handleCommand(new cMoveEntiyLeftRightCommand(-cameraSpeed));
		}
		if (glfwGetKey(window, GLFW_KEY_Q)) {
			pScene->handleCommand(new cMoveEntiyUpDownCommand(cameraSpeed));
		}

		if (glfwGetKey(window, GLFW_KEY_E)) {
			pScene->handleCommand(new cMoveEntiyUpDownCommand(-cameraSpeed));
		}

		if (glfwGetKey(window, GLFW_KEY_KP_ADD)) {
			pScene->handleCommand(new cChangeEntitySpecularPower(1.01f));
		}

		if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT)) {
			pScene->handleCommand(new cChangeEntitySpecularPower(0.99f));
		}

		if (glfwGetKey(window, GLFW_KEY_I)) {
			pScene->handleCommand(new cRotateEntiyXCommand(1.0f * cameraSpeed));
		}

		if (glfwGetKey(window, GLFW_KEY_K)) {
			pScene->handleCommand(new cRotateEntiyXCommand(-1.0f * cameraSpeed));
		}

		if (glfwGetKey(window, GLFW_KEY_U)) {
			pScene->handleCommand(new cRotateEntiyYCommand(1.0f * cameraSpeed));
		}

		if (glfwGetKey(window, GLFW_KEY_O)) {
			pScene->handleCommand(new cRotateEntiyYCommand(-1.0f * cameraSpeed));
		}

		if (glfwGetKey(window, GLFW_KEY_J)) {
			pScene->handleCommand(new cRotateEntiyZCommand(1.0f * cameraSpeed));
		}

		if (glfwGetKey(window, GLFW_KEY_L)) {
			pScene->handleCommand(new cRotateEntiyZCommand(-1.0f * cameraSpeed));
		}

		// Scale
		if (glfwGetKey(window, GLFW_KEY_COMMA)) { 
			
			cEntity* pEntity = pScene->getSelectedEntity();
			cTransformComponent* pTransComp = static_cast<cTransformComponent*>(pEntity->components[1]);

			pTransComp->scale *= 1.01f;
		}
		
		if (glfwGetKey(window, GLFW_KEY_PERIOD)) { 
			
			cEntity* pEntity = pScene->getSelectedEntity();
			cTransformComponent* pTransComp = static_cast<cTransformComponent*>(pEntity->components[1]);

			pTransComp->scale *= 0.99f;
		}
	}

	// Is Ctrl down? Other operations
	if (isCtrlDown(window)) {

		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) { // Roll camera to the left
			pScene->handleCommand(new cRollCameraCommand(-cameraSpeed));
		}

		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) { // Roll camera to the right
			pScene->handleCommand(new cRollCameraCommand(+cameraSpeed));
		}
	}
}