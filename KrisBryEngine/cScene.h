/*
* Mediator for the scene
*/

#ifndef _cScene_HG_
#define _cScene_HG_

#include <glm/vec3.hpp>
#include <vector>
#include <string>

#include "cEntity.h"
#include "sLight.h"
#include "iCommand.h"
#include "cFlyCamera.h"

#include "cEntityBuilder.h"
#include "cLightManager.h"
#include "cSoundManager.h"
#include "c3dSoundComponent.h"

class cScene {
public:
	cScene();
	~cScene();

	int selectedEntity;
	int selectedLight;
	int selectedChannelGroup;

	std::string sceneName;

	cFlyCamera flyCamera;
	bool isCursorInWindow;

	//TODO: remove old camera
	glm::vec3 cameraPos;
	glm::vec3 cameraLookAt;
	glm::vec3 cameraUp;

	// Scene modifiers
	void setSceneName(std::string name);
	void updateWindowTitle(GLFWwindow* window);
	bool getIsScenePaused();
	void setIsScenePaused(bool val);
	void handleCommand(iCommand* command);

	// Scene cameras
	void setCameraPosition(glm::vec3 pos);
	void setCameraLookAt(glm::vec3 lookAt);
	
	// Scene entities
	void addEntityToScene(cEntity* entity);
	void removeEntityFromScene(cEntity* entiy);
	cEntity* getSelectedEntity();
	cEntity* findObjectByFriendlyName(std::string theNameToFind);
	size_t getNumberOfEntites();
	std::vector<cEntity*>& getEntities();

	// Scene lights
	void addLightToScene(sLight* light);
	void removeLightFromScene(sLight* light);
	sLight* getSelectedLight();
	size_t getNumberOfLights();
	std::vector<sLight*> getLights();

	// Scene non-3D sounds
	void addSoundToScene(cSoundManager::sSoundInfo* soundInfo);
	cSoundManager::sSoundInfo* getSoundInfo(unsigned index);
	std::vector<cSoundManager::sSoundInfo*> getSounds();

	// Save/Load
	bool serialize();
	bool deserialize(std::string filename);

	// Debug Functions
	bool bIsLightDebug;
	void setLightDebugSphere(cEntity* pSphere);

	bool bIsRenderDebug;

private:
	std::vector<cEntity*> entities;
	std::vector<sLight*> lights;
	std::vector<cSoundManager::sSoundInfo*> sounds;
	bool isScenePaused;

	// Debug light constants
	const float ACCURACY_OF_DISTANCE = 0.01f;
	const float INFINITE_DISTANCE = 1000.0f;
};

#endif
