#include <cSceneManager.h>
#include <cShaderManager.h>
#include <cConsole.h>
#include <cTransformComponent.h>
#include <cRenderMeshComponent.h>
#include <cMeshRenderSystem.h>

#include <cDebugRenderer.h>
#include <cEngine.h>

#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

#include <cLightHelper.h>

#include <iCommand.h>
#include <physicsShapes.h>

extern cEntity* pSphere;
extern cEntity* pCube;

extern cAABBTerrianBroadPhaseHierarchy g_AABBTerrianHierarchy;

double nextLightSwitchTime = 0;

void showModelNormals();

float randomFloat(float a, float b) {
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;
}

void updateCallback(double deltaTime) {

	cShaderManager::getInstance()->useShaderProgram("BasicUberShader");
	GLint program = cShaderManager::getInstance()->getIDFromFriendlyName("BasicUberShader");

	cSceneManager* pSceneManager = cSceneManager::getInstance();
	cScene* pScene = pSceneManager->getActiveScene();

	for (std::map< cAABBTerrianBroadPhase::id_type, cAABBTerrianBroadPhase* >::iterator it = g_AABBTerrianHierarchy.m_mapAABBs.begin(); it != g_AABBTerrianHierarchy.m_mapAABBs.end(); ++it) {
		
		cTransformComponent* transform = (cTransformComponent*)pCube->components[1];
		transform->position = (*it).second->center;
		transform->scale = glm::vec3(10.0f);
		
		cRenderMeshComponent* render = (cRenderMeshComponent*)pCube->components[3];
		render->bIsVisible = true;
		render->setDiffuseColour(glm::vec3(1.0f, 0.0f, 0.0f));
		
		glm::mat4x4 mat(1.0f);
		cMeshRenderSystem::getInstance()->drawObject(pCube, mat, program);

		render->bIsVisible = false;
	}

	// Update Window title
	pScene->updateWindowTitle(cEngine::getInstance()->getWindow());

	// Render debug spheres for lights?
	if (pScene->bIsLightDebug) {
		pScene->setLightDebugSphere(pSphere);
	}

	// Rotate the star
	cEntity* pStar = pScene->findObjectByFriendlyName("Star");
	float degreesThisFrame = (float)deltaTime * 15.0f;		// 15 degrees per second

	cTransformComponent* pStarTrans = static_cast<cTransformComponent*>(pStar->components[1]);
	pStarTrans->adjOrientationEulerAngles(glm::vec3(0.0f, degreesThisFrame, 0.0f), true);

	// Debug render
	if (pScene->bIsRenderDebug) {
		showModelNormals();
	}
}