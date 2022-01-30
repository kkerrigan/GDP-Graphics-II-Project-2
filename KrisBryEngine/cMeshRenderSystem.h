#ifndef _cMeshRenderSystem_HG_
#define _cMeshRenderSystem_HG_

#include <vector>
#include <glm/mat4x4.hpp>

#include "iSystem.h"
#include "cRenderMeshComponent.h"

class cMeshRenderSystem : public iSystem {
public:

	// Shader Uniforms
	GLint matModel_UniLoc;
	GLint matView_UniLoc;
	GLint matProj_UniLoc;
	GLint matModelInvTrans_UniLoc;
	GLint objectEyePos_UniLoc;
	GLint objectColour_UniLoc;
	GLint objectSpecular_UniLoc;
	GLint useVertexColour_UniLoc;
	GLint bDontUseLighting_UniLoc;
	GLint colorByHeight_UniLoc;
	GLint wholeObjectAlphaTrans_UniLoc;
	GLint skyBoxCubeMap_UniLoc;
	GLint useSkyBoxTexture_UniLoc;
	bool bShaderUniformsLoaded;

	GLuint skyBoxTextureUnitID;

	// Texture uniforms
	GLint tex00_UniLoc;
	GLint tex01_UniLoc;
	GLint tex02_UniLoc;
	GLint tex03_UniLoc;
	GLint tex04_UniLoc;
	GLint tex05_UniLoc;
	GLint tex06_UniLoc;
	GLint tex07_UniLoc;
	GLint texBW_0_UniLoc;
	GLint texBW_1_UniLoc;
	bool bTextureUniformsLoaded;

	// Singleton setup
	static cMeshRenderSystem* getInstance();
	cMeshRenderSystem(cMeshRenderSystem const&) = delete;
	void operator=(cMeshRenderSystem const&) = delete;

	void loadShaderUniforms(GLint shaderProgramID);
	void update(double deltaTime);
	void registerEntity(cEntity* entity);
	void unregisterEntity(cEntity* entity);
	void loadTextureUniforms(GLint shaderProgramID);
	void bindTextures(cRenderMeshComponent* pCurrentMesh, GLuint shaderProgramID);
	void drawSkyBox();
	void drawObject(cEntity* pCurrentMesh, glm::mat4x4 &matModel, int shaderProgramID);

private:
	std::vector<cEntity*> entitiesToUpdate;

	cMeshRenderSystem();
	~cMeshRenderSystem();
};

#endif
