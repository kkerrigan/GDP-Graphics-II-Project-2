#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp> 
#include <glm/mat4x4.hpp> 
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp> 

#include <algorithm>

#include "cEngine.h"
#include "cMeshRenderSystem.h"
#include "cRenderMeshComponent.h"
#include "cTransformComponent.h"
#include "cVAOMeshManager.h"
#include "sLight.h"
#include "cShaderManager.h"
#include "cSceneManager.h"
#include "cDebugRenderer.h"
#include "cBasicTextureManager.h"

class cPredEntityIsCloser {
public:
	cPredEntityIsCloser(glm::vec3 cameraEye_) {
		this->cameraEye = cameraEye_;
		return;
	}
	glm::vec3 cameraEye;

	bool operator() (const cEntity* P1, const cEntity* P2) {

		cTransformComponent* entityOneTrans = static_cast<cTransformComponent*>(P1->components[1]);
		cTransformComponent* entityTwoTrans = static_cast<cTransformComponent*>(P2->components[1]);
		cRenderMeshComponent* entityOneMesh = static_cast<cRenderMeshComponent*>(P1->components[3]);
		cRenderMeshComponent* entityTwoMesh = static_cast<cRenderMeshComponent*>(P2->components[3]);

		float P1_dist_to_camera = glm::distance(entityOneTrans->position, this->cameraEye);
		float P2_dist_to_camera = glm::distance(entityTwoTrans->position, this->cameraEye);

		return P1_dist_to_camera > P2_dist_to_camera;
	}
};


cMeshRenderSystem::cMeshRenderSystem() {

	// Shader uniforms
	this->matModel_UniLoc = -1;
	this->matView_UniLoc = -1;
	this->matProj_UniLoc = -1;
	this->matModelInvTrans_UniLoc = -1;
	this->objectEyePos_UniLoc = -1;
	this->objectColour_UniLoc = -1;
	this->objectSpecular_UniLoc = -1;
	this->useVertexColour_UniLoc = -1;
	this->bDontUseLighting_UniLoc = -1;
	this->colorByHeight_UniLoc = -1;
	this->wholeObjectAlphaTrans_UniLoc = -1;
	this->skyBoxCubeMap_UniLoc = -1;
	this->useSkyBoxTexture_UniLoc = -1;
	this->bShaderUniformsLoaded = false;

	this->skyBoxTextureUnitID = 30;

	this->tex00_UniLoc = -1;
	this->tex01_UniLoc = -1;
	this->tex02_UniLoc = -1;
	this->tex03_UniLoc = -1;
	this->tex04_UniLoc = -1;
	this->tex05_UniLoc = -1;
	this->tex06_UniLoc = -1;
	this->tex07_UniLoc = -1;
	this->texBW_0_UniLoc = -1;
	this->texBW_1_UniLoc = -1;
	this->bTextureUniformsLoaded = false;

	return;
}

cMeshRenderSystem::~cMeshRenderSystem() {}

cMeshRenderSystem* cMeshRenderSystem::getInstance() {
	static cMeshRenderSystem instance;

	return &instance;
}

void cMeshRenderSystem::update(double deltaTime) {

	cSceneManager* sceneManager = cSceneManager::getInstance();
	cScene* activeScene = sceneManager->getActiveScene();

	// Switch to the shader we want
	cShaderManager::getInstance()->useShaderProgram("BasicUberShader");
	GLint program = cShaderManager::getInstance()->getIDFromFriendlyName("BasicUberShader");

	float ratio;
	int width, height;
	glm::mat4x4 matProjection = glm::mat4(1.0f);
	glm::mat4x4	matView = glm::mat4(1.0f);

	glfwGetFramebufferSize(cEngine::getInstance()->getWindow(), &width, &height);
	ratio = width / (float)height;
	glViewport(0, 0, width, height);

	glEnable(GL_DEPTH);			 // Enables the KEEPING of the depth information
	glEnable(GL_DEPTH_TEST);	// When drawing, checked the existing depth
	glEnable(GL_CULL_FACE);		// Discared "back facing" triangles

	matProjection = glm::perspective(0.6f,
		ratio,			// Aspect ratio
		0.1f,			// Near clipping plane
		10000.0f);		// Far clipping plane

	matView = glm::lookAt(activeScene->flyCamera.eye, activeScene->flyCamera.getAtInWorldSpace(), activeScene->flyCamera.getUpVector());

	// Load the shader uniform locations if not already done
	if (!this->bShaderUniformsLoaded) {
		this->loadShaderUniforms(program);
	}

	glUniformMatrix4fv(this->matView_UniLoc, 1, GL_FALSE, glm::value_ptr(matView));
	glUniformMatrix4fv(this->matProj_UniLoc, 1, GL_FALSE, glm::value_ptr(matProjection));

	// Draw Sky box first
	this->drawSkyBox();

	std::vector<cEntity*> vec_transparentObj;

	for (unsigned int objIndex = 0; objIndex != this->entitiesToUpdate.size(); objIndex++) {
		glm::mat4x4 matModel = glm::mat4(1.0f);

		if (static_cast<cRenderMeshComponent*>(this->entitiesToUpdate[objIndex]->components[3])->materialDiffuse.a >= 1.0f) {
			this->drawObject(this->entitiesToUpdate[objIndex], matModel, program);
		}
		else {
			vec_transparentObj.push_back(this->entitiesToUpdate[objIndex]);
		}
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	std::sort(vec_transparentObj.begin(), vec_transparentObj.end(), cPredEntityIsCloser(activeScene->flyCamera.eye));
	for (unsigned int index = 0; index != vec_transparentObj.size(); ++index) {

		glm::mat4x4 matModel = glm::mat4(1.0f);
		this->drawObject(vec_transparentObj[index], matModel, program);
	}

	glDisable(GL_BLEND);

	cDebugRenderer::getInstance()->RenderDebugObjects(matView, matProjection, deltaTime);
}

void cMeshRenderSystem::drawSkyBox() {

	cScene* activeScene = cSceneManager::getInstance()->getActiveScene();
	cEntity* pSkyBox = activeScene->findObjectByFriendlyName("SkyBoxObject");
	cTransformComponent* pSkyboxTransComp = static_cast<cTransformComponent*>(pSkyBox->components[1]);
	cRenderMeshComponent* pSkyboxMeshComp = static_cast<cRenderMeshComponent*>(pSkyBox->components[3]);

	// Draw the skybox first 
	pSkyboxTransComp->position = activeScene->flyCamera.eye;
	pSkyboxMeshComp->bIsVisible = true;
	pSkyboxMeshComp->bIsWireFrame = false;

	// Bind the cube map texture to the cube map in the shader
	glActiveTexture(this->skyBoxTextureUnitID + GL_TEXTURE0);
	int cubeMapTextureID = cBasicTextureManager::getInstance()->getTextureIDFromName("CityCubeMap");

	// Cube map is now bound to texture unit 30
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTextureID);

	glUniform1i(this->skyBoxCubeMap_UniLoc, skyBoxTextureUnitID);
	glUniform1f(this->useSkyBoxTexture_UniLoc, (float)GL_TRUE);

	glm::mat4 matIdentity = glm::mat4(1.0f);
	GLint program = cShaderManager::getInstance()->getIDFromFriendlyName("BasicUberShader");

	this->drawObject(pSkyBox, matIdentity, program);

	pSkyboxMeshComp->bIsVisible = false;
	glUniform1f(this->useSkyBoxTexture_UniLoc, (float)GL_FALSE);

	return;
}

void cMeshRenderSystem::loadShaderUniforms(GLint shaderProgramID) {

	this->matModel_UniLoc = glGetUniformLocation(shaderProgramID, "matModel");
	this->matView_UniLoc = glGetUniformLocation(shaderProgramID, "matView");
	this->matProj_UniLoc = glGetUniformLocation(shaderProgramID, "matProj");
	this->matModelInvTrans_UniLoc = glGetUniformLocation(shaderProgramID, "matModelInvTrans");
	this->objectEyePos_UniLoc = glGetUniformLocation(shaderProgramID, "eyeLocation");
	this->objectColour_UniLoc = glGetUniformLocation(shaderProgramID, "objectDiffuse");
	this->objectSpecular_UniLoc = glGetUniformLocation(shaderProgramID, "objectSpecular");
	this->useVertexColour_UniLoc = glGetUniformLocation(shaderProgramID, "useVertexColour");
	this->bDontUseLighting_UniLoc = glGetUniformLocation(shaderProgramID, "bDontUseLighting");
	this->colorByHeight_UniLoc = glGetUniformLocation(shaderProgramID, "colorBasedOnHeight");
	this->wholeObjectAlphaTrans_UniLoc = glGetUniformLocation(shaderProgramID, "wholeObjectAlphaTransparency");
	this->skyBoxCubeMap_UniLoc = glGetUniformLocation(shaderProgramID, "textureSkyBox");
	this->useSkyBoxTexture_UniLoc = glGetUniformLocation(shaderProgramID, "useSkyBoxTexture");
	this->bShaderUniformsLoaded = true;
}

void cMeshRenderSystem::loadTextureUniforms(GLint shaderProgramID) {

	this->tex00_UniLoc = glGetUniformLocation(shaderProgramID, "texture00");		// uniform sampler2D texture00;
	this->tex01_UniLoc = glGetUniformLocation(shaderProgramID, "texture01");		// uniform sampler2D texture01;
	this->tex02_UniLoc = glGetUniformLocation(shaderProgramID, "texture02");		// uniform sampler2D texture02;
	this->tex03_UniLoc = glGetUniformLocation(shaderProgramID, "texture03");		// uniform sampler2D texture03;
	this->tex04_UniLoc = glGetUniformLocation(shaderProgramID, "texture04");		// uniform sampler2D texture04;
	this->tex05_UniLoc = glGetUniformLocation(shaderProgramID, "texture05");		// uniform sampler2D texture05;
	this->tex06_UniLoc = glGetUniformLocation(shaderProgramID, "texture06");		// uniform sampler2D texture06;
	this->tex07_UniLoc = glGetUniformLocation(shaderProgramID, "texture07");		// uniform sampler2D texture07;
	this->texBW_0_UniLoc = glGetUniformLocation(shaderProgramID, "texBlendWeights[0]");	// uniform vec4 texBlendWeights[2];
	this->texBW_1_UniLoc = glGetUniformLocation(shaderProgramID, "texBlendWeights[1]");	// uniform vec4 texBlendWeights[2];
	this->bTextureUniformsLoaded = true;

	return;
}

void cMeshRenderSystem::registerEntity(cEntity* entity) {
	this->entitiesToUpdate.push_back(entity);
}

void cMeshRenderSystem::unregisterEntity(cEntity* entity) {
	// TODO: remove from vector
}

void cMeshRenderSystem::bindTextures(cRenderMeshComponent* pCurrentMesh, GLuint shaderProgramID) {

	// Load texture uniforms if they are not already set
	if (!this->bTextureUniformsLoaded) {
		this->loadTextureUniforms(shaderProgramID);
	}

	float blendWeights[8] = { 0 };

	for (int texBindIndex = 0; texBindIndex != pCurrentMesh->textures.size(); texBindIndex++) {

		// Bind to the the "texBindIndex" texture unit
		glActiveTexture(GL_TEXTURE0 + texBindIndex);

		// Connect the specific texture to THIS texture unit
		std::string texName = pCurrentMesh->textures[texBindIndex].name;

		GLuint texID = cBasicTextureManager::getInstance()->getTextureIDFromName(texName);

		glBindTexture(GL_TEXTURE_2D, texID);

		// Use a switch to pick the texture sampler and weight (strength)
		// BECAUSE the samplers can't be in an array
		switch (texBindIndex)
		{
		case 0:		// uniform sampler2D texture00  AND texBlendWeights[0].x;
			glUniform1i(this->tex00_UniLoc, texBindIndex);
			break;
		case 1:		// uniform sampler2D texture01  AND texBlendWeights[0].y;
			glUniform1i(this->tex01_UniLoc, texBindIndex);
			break;
		case 2:
			glUniform1i(this->tex02_UniLoc, texBindIndex);
			break;
		case 3:
			glUniform1i(this->tex03_UniLoc, texBindIndex);
			break;
		case 4:		// uniform sampler2D texture04  AND texBlendWeights[1].x;
			glUniform1i(this->tex04_UniLoc, texBindIndex);
			break;
		case 5:
			glUniform1i(this->tex05_UniLoc, texBindIndex);
			break;
		case 6:
			glUniform1i(this->tex06_UniLoc, texBindIndex);
			break;
		case 7:
			glUniform1i(this->tex07_UniLoc, texBindIndex);
			break;
		}//switch ( texBindIndex )

		// Set the blend weight (strengt)
		blendWeights[texBindIndex] = pCurrentMesh->textures[texBindIndex].strength;
	}

	// Set the weights (strengths) in the shader
	glUniform4f(this->texBW_0_UniLoc, blendWeights[0], blendWeights[1], blendWeights[2], blendWeights[3]);
	glUniform4f(this->texBW_1_UniLoc, blendWeights[4], blendWeights[5], blendWeights[6], blendWeights[7]);

	return;
}

void cMeshRenderSystem::drawObject(cEntity* pEntity, glm::mat4x4 &matModel, int shaderProgramID) {

	cScene* activeScene = cSceneManager::getInstance()->getActiveScene();
	cTransformComponent* transformComponent = static_cast<cTransformComponent*>(pEntity->components[1]);
	cRenderMeshComponent* meshComponent = static_cast<cRenderMeshComponent*>(pEntity->components[3]);

	// Is this object visible
	if (!meshComponent->bIsVisible) {
		return;
	}

	this->bindTextures(meshComponent, shaderProgramID);

	matModel = glm::mat4x4(1.0f);

	glm::mat4 matTranslation = glm::translate(glm::mat4(1.0f),
		transformComponent->position);

	matModel = matModel * matTranslation;

	glm::mat4 matQrotation = glm::mat4(transformComponent->orientation);
	matModel *= matQrotation;

	// Calculate the inverse transpose before the scaling
	glm::mat4 matModelInvTrans = glm::inverse(glm::transpose(matModel));

	// And now scale
	glm::mat4 matScale = glm::scale(glm::mat4(1.0f),
		transformComponent->scale);
	matModel = matModel * matScale;

	glUseProgram(shaderProgramID);

	glUniformMatrix4fv(this->matModel_UniLoc, 1, GL_FALSE, glm::value_ptr(matModel));
	glUniformMatrix4fv(this->matModelInvTrans_UniLoc, 1, GL_FALSE, glm::value_ptr(matModelInvTrans));

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);	

	// Take the 4th value from the material and pass it as alpha
	glUniform1f(this->wholeObjectAlphaTrans_UniLoc, meshComponent->materialDiffuse.a);

	glUniform4f(this->objectColour_UniLoc,
		meshComponent->materialDiffuse.r,
		meshComponent->materialDiffuse.g,
		meshComponent->materialDiffuse.b,
		meshComponent->materialDiffuse.a);

	glUniform3f(this->objectEyePos_UniLoc, activeScene->flyCamera.eye.x, activeScene->flyCamera.eye.y, activeScene->flyCamera.eye.z);
	glUniform4f(this->objectSpecular_UniLoc, meshComponent->materialSpecular.r, meshComponent->materialSpecular.g, meshComponent->materialSpecular.b, meshComponent->materialSpecular.a);

	if (meshComponent->bUseVertexColour)
	{
		glUniform1f(this->useVertexColour_UniLoc, (float)GL_TRUE);
	}
	else
	{
		glUniform1f(this->useVertexColour_UniLoc, (float)GL_FALSE);
	}

	if (meshComponent->colorByHeight) {
		glUniform1f(this->colorByHeight_UniLoc, (float)GL_TRUE);
	}
	else {
		glUniform1f(this->colorByHeight_UniLoc, (float)GL_FALSE);
	}

	if (meshComponent->bDontLight)
	{
		glUniform1f(this->bDontUseLighting_UniLoc, (float)GL_TRUE);
	}
	else
	{
		glUniform1f(this->bDontUseLighting_UniLoc, (float)GL_FALSE);
	}

	if (meshComponent->bIsWireFrame)
	{
		// Yes, draw it wireframe
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDisable(GL_CULL_FACE);	// Discared "back facing" triangles
		//glDisable( GL_DEPTH );		// Enables the KEEPING of the depth information
		//glDisable( GL_DEPTH_TEST );	// When drawing, checked the existing depth
	}
	else
	{
		// No, it's "solid" (or "Filled")
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glEnable(GL_CULL_FACE);	// Discared "back facing" triangles
		//glEnable( GL_DEPTH );		// Enables the KEEPING of the depth information
		//glEnable( GL_DEPTH_TEST );	// When drawing, checked the existing depth
	}


	sModelDrawInfo modelInfo;
	modelInfo.meshFileName = meshComponent->meshName;

	if (cVAOMeshManager::getInstance()->FindDrawInfoByModelName(modelInfo)) {

		glBindVertexArray(modelInfo.VAO_ID);

		glDrawElements(GL_TRIANGLES,
			modelInfo.numberOfIndices,
			GL_UNSIGNED_INT,
			0);

		glBindVertexArray(0);

	}

	return;
}