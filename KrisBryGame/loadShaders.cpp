#include <Global.h>
#include <cShaderManager.h>
#include <iostream>

bool loadShaders() {
	// load shaders
	cShaderManager* pShaderManager = cShaderManager::getInstance();
	pShaderManager->setBasePath("./assets/shaders/");

	cShaderManager::cShader vertexShader;
	cShaderManager::cShader fragmentShader;

	vertexShader.fileName = "vertex01.glsl";
	vertexShader.shaderType = cShaderManager::cShader::VERTEX_SHADER;

	fragmentShader.fileName = "fragment01.glsl";
	fragmentShader.shaderType = cShaderManager::cShader::FRAGMENT_SHADER;

	if (pShaderManager->createProgramFromFile("BasicUberShader", vertexShader, fragmentShader)) {
		std::cout << "Compiled shaders OK." << std::endl;
		
		return true;
	} else {
		std::cout << "OH NO! Compile error" << std::endl;
		std::cout << pShaderManager->getLastError() << std::endl;

		return false;
	}
}