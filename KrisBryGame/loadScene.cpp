#include <cVAOMeshManager.h>
#include <cEntityBuilder.h>
#include <cSceneManager.h>
#include <cLightManager.h>
#include <cSoundManager.h>


cEntity* pSphere = nullptr;
cEntity* pCube = nullptr;

sLight* pMovingSpotLight = nullptr;

void loadScene() {

	// create a scene for the entities
	cSceneManager* pSceneManager = cSceneManager::getInstance();
	cScene* pSandboxScene = pSceneManager->createScene("sandbox.json");
	pSceneManager->setActiveScene("sandbox.json");

	//initialize the light manager before loading them
	cLightManager::getInstance()->initializeLights();

	pSphere = cEntityBuilder::getInstance()->createEntity(0);
	pCube = cEntityBuilder::getInstance()->createEntity(17);

	//load from the file
	pSandboxScene->deserialize("sandbox.json");

	return;
}