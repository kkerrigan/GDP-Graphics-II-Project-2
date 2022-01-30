#include <cLightManager.h>
#include <cSceneManager.h>
#include <cGLColourHelper.h>

void loadLights() {
	cSceneManager* sceneManager = cSceneManager::getInstance();
	cScene* pScene = sceneManager->getActiveScene();

	cLightManager* lightManager = cLightManager::getInstance();
	lightManager->initializeLights();

	cGLColourHelper* pColourHelper = cGLColourHelper::getInstance();

	// GRAPHICS PROJECT 2
	/*sLight* light1 = lightManager->getLightByIndex(1);
	light1->param2 = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
	light1->position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	light1->atten.y = 0.0005f;
	light1->atten.z = 0.00000000001f;
	light1->diffuse = glm::vec4(pColourHelper->getColourRGB(cGLColourHelper::FIRE_BRICK), 1.0f);
	pScene->addLightToScene(light1);*/

	/*sLight* light1 = lightManager->getLightByIndex(2);
	light1->param2 = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
	light1->position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	light1->atten.y = 0.0005f;
	light1->atten.z = 0.00000000001f;
	light1->diffuse = glm::vec4(pColourHelper->getColourRGB(cGLColourHelper::FIRE_BRICK), 1.0f);
	pScene->addLightToScene(light1);*/

	/*sLight* light1 = lightManager->getLightByIndex(4);
	light1->param2 = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
	light1->position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	light1->atten.y = 0.0005f;
	light1->atten.z = 0.00000000001f;
	light1->diffuse = glm::vec4(pColourHelper->getColourRGB(cGLColourHelper::FIRE_BRICK), 1.0f);
	pScene->addLightToScene(light1);*/

	/*sLight* light1 = lightManager->getLightByIndex(5);
	light1->param2 = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
	light1->position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	light1->atten.y = 0.0005f;
	light1->atten.z = 0.00000000001f;
	light1->diffuse = glm::vec4(pColourHelper->getColourRGB(cGLColourHelper::LIGHT_YELLOW), 1.0f);
	pScene->addLightToScene(light1);*/

	/*sLight* light1 = lightManager->getLightByIndex(6);
	light1->param2 = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
	light1->position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	light1->atten.y = 0.0005f;
	light1->atten.z = 0.00000000001f;
	light1->diffuse = glm::vec4(pColourHelper->getColourRGB(cGLColourHelper::LIGHT_YELLOW), 1.0f);
	pScene->addLightToScene(light1);*/

	/*sLight* light1 = lightManager->getLightByIndex(7);
	light1->param2 = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
	light1->position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	light1->atten.y = 0.0005f;
	light1->atten.z = 0.00000000001f;
	light1->diffuse = glm::vec4(pColourHelper->getColourRGB(cGLColourHelper::LIGHT_YELLOW), 1.0f);
	pScene->addLightToScene(light1);*/

	/*sLight* light1 = lightManager->getLightByIndex(8);
	light1->param2 = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
	light1->position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	light1->atten.y = 0.0005f;
	light1->atten.z = 0.00000000001f;
	light1->diffuse = glm::vec4(pColourHelper->getColourRGB(cGLColourHelper::LIGHT_YELLOW), 1.0f);
	pScene->addLightToScene(light1);*/

	/*sLight* light1 = lightManager->getLightByIndex(9);
	light1->param2 = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
	light1->position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	light1->atten.y = 0.0005f;
	light1->atten.z = 0.00000000001f;
	light1->diffuse = glm::vec4(pColourHelper->getColourRGB(cGLColourHelper::LIGHT_YELLOW), 1.0f);
	pScene->addLightToScene(light1);*/

	/*sLight* light1 = lightManager->getLightByIndex(10);
	light1->param2 = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
	light1->position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	light1->atten.y = 0.0005f;
	light1->atten.z = 0.00000000001f;
	light1->diffuse = glm::vec4(pColourHelper->getColourRGB(cGLColourHelper::LIGHT_YELLOW), 1.0f);
	pScene->addLightToScene(light1);*/

	/*sLight* light1 = lightManager->getLightByIndex(12);
	light1->param2 = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
	light1->position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	light1->atten.y = 0.0005f;
	light1->atten.z = 0.00000000001f;
	light1->diffuse = glm::vec4(pColourHelper->getColourRGB(cGLColourHelper::FIRE_BRICK), 1.0f);
	pScene->addLightToScene(light1);*/

	/*sLight* light1 = lightManager->getLightByIndex(13);
	light1->param2 = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
	light1->position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	light1->atten.y = 0.0005f;
	light1->atten.z = 0.00000000001f;
	light1->diffuse = glm::vec4(pColourHelper->getColourRGB(cGLColourHelper::FIRE_BRICK), 1.0f);
	pScene->addLightToScene(light1);*/

	/*sLight* light1 = lightManager->getLightByIndex(14);
	light1->param2 = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
	light1->position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	light1->atten.y = 0.0005f;
	light1->atten.z = 0.00000000001f;
	light1->diffuse = glm::vec4(pColourHelper->getColourRGB(cGLColourHelper::FIRE_BRICK), 1.0f);
	pScene->addLightToScene(light1);*/

	// Spotlights
	/*sLight* light1 = lightManager->getLightByIndex(0);
	light1->param2 = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
	light1->position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	light1->atten.y = 0.0005f;
	light1->atten.z = 0.00000000001f;
	light1->diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	light1->SetLightType(sLight::SPOT_LIGHT);
	light1->SetSpotConeAngles(25.0f, 25.0f);
	light1->SetRelativeDirection(glm::vec3(0.0f, -1.0f, 0.0f));
	pScene->addLightToScene(light1);*/

	/*sLight* light1 = lightManager->getLightByIndex(15);
	light1->param2 = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
	light1->position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	light1->atten.y = 0.0005f;
	light1->atten.z = 0.00000000001f;
	light1->diffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	light1->SetLightType(sLight::SPOT_LIGHT);
	light1->SetSpotConeAngles(25.0f, 25.0f);
	light1->SetRelativeDirection(glm::vec3(0.0f, -1.0f, 0.0f));
	pScene->addLightToScene(light1);*/
}