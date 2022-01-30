#include <cVAOMeshManager.h>
#include <cShaderManager.h>
#include <iostream>

void loadModels() {
	
	cVAOMeshManager* pMeshManager = cVAOMeshManager::getInstance();
	pMeshManager->setBasePath("./assets/models/");
	int shaderProgramId = cShaderManager::getInstance()->getIDFromFriendlyName("BasicUberShader");

	{
		sModelDrawInfo modelInfo;
		modelInfo.meshFileName = "Peach_Castle_xyz_n_uv.ply";
		if (!pMeshManager->LoadModelIntoVAO(modelInfo, shaderProgramId)) {
			std::cout << "Didn't load " << modelInfo.meshFileName << std::endl;
			std::cout << pMeshManager->GetLastError() << std::endl;
		}
	}

	{
		sModelDrawInfo modelInfo;
		modelInfo.meshFileName = "boo_xyz_n_uv.ply";
		if (!pMeshManager->LoadModelIntoVAO(modelInfo, shaderProgramId)) {
			std::cout << "Didn't load " << modelInfo.meshFileName << std::endl;
			std::cout << pMeshManager->GetLastError() << std::endl;
		}
	}

	{
		sModelDrawInfo modelInfo;
		modelInfo.meshFileName = "bowser_xyz_n_uv.ply";
		if (!pMeshManager->LoadModelIntoVAO(modelInfo, shaderProgramId)) {
			std::cout << "Didn't load " << modelInfo.meshFileName << std::endl;
			std::cout << pMeshManager->GetLastError() << std::endl;
		}
	}

	{
		sModelDrawInfo modelInfo;
		modelInfo.meshFileName = "fire_xyz_n_uv.ply";
		if (!pMeshManager->LoadModelIntoVAO(modelInfo, shaderProgramId)) {
			std::cout << "Didn't load " << modelInfo.meshFileName << std::endl;
			std::cout << pMeshManager->GetLastError() << std::endl;
		}
	}

	{
		sModelDrawInfo modelInfo;
		modelInfo.meshFileName = "goomba_xyz_n_uv.ply";
		if (!pMeshManager->LoadModelIntoVAO(modelInfo, shaderProgramId)) {
			std::cout << "Didn't load " << modelInfo.meshFileName << std::endl;
			std::cout << pMeshManager->GetLastError() << std::endl;
		}
	}

	{
		sModelDrawInfo modelInfo;
		modelInfo.meshFileName = "GreenPipe_xyz_n_uv.ply";
		if (!pMeshManager->LoadModelIntoVAO(modelInfo, shaderProgramId)) {
			std::cout << "Didn't load " << modelInfo.meshFileName << std::endl;;
			std::cout << pMeshManager->GetLastError() << std::endl;
		}
	}

	{
		sModelDrawInfo modelInfo;
		modelInfo.meshFileName = "cube_flat_shaded_xyz_n.ply";
		if (!pMeshManager->LoadModelIntoVAO(modelInfo, shaderProgramId)) {
			std::cout << "Didn't load " << modelInfo.meshFileName << std::endl;
			std::cout << pMeshManager->GetLastError() << std::endl;
		}
	}

	{
		sModelDrawInfo modelInfo;
		modelInfo.meshFileName = "lantern_xyz_n_uv.ply";
		if (!pMeshManager->LoadModelIntoVAO(modelInfo, shaderProgramId)) {
			std::cout << "Didn't load " << modelInfo.meshFileName << std::endl;
			std::cout << pMeshManager->GetLastError() << std::endl;
		}
	}

	{
		sModelDrawInfo modelInfo;
		modelInfo.meshFileName = "luigi_xyz_n_uv.ply";
		if (!pMeshManager->LoadModelIntoVAO(modelInfo, shaderProgramId)) {
			std::cout << "Didn't load " << modelInfo.meshFileName << std::endl;
			std::cout << pMeshManager->GetLastError() << std::endl;
		}
	}

	{
		sModelDrawInfo modelInfo;
		modelInfo.meshFileName = "mario_xyz_n_uv.ply";
		if (!pMeshManager->LoadModelIntoVAO(modelInfo, shaderProgramId)) {
			std::cout << "Didn't load " << modelInfo.meshFileName << std::endl;
			std::cout << pMeshManager->GetLastError() << std::endl;
		}
	}

	{
		sModelDrawInfo modelInfo;
		modelInfo.meshFileName = "Peach_xyz_n_uv.ply";
		if (!pMeshManager->LoadModelIntoVAO(modelInfo, shaderProgramId)) {
			std::cout << "Didn't load " << modelInfo.meshFileName << std::endl;
			std::cout << pMeshManager->GetLastError() << std::endl;
		}
	}

	{
		sModelDrawInfo modelInfo;
		modelInfo.meshFileName = "pirahna_plant_xyz_n_uv.ply";
		if (!pMeshManager->LoadModelIntoVAO(modelInfo, shaderProgramId)) {
			std::cout << "Didn't load " << modelInfo.meshFileName << std::endl;
			std::cout << pMeshManager->GetLastError() << std::endl;
		}
	}

	{
		sModelDrawInfo modelInfo;
		modelInfo.meshFileName = "star_xyz_n_uv.ply";
		if (!pMeshManager->LoadModelIntoVAO(modelInfo, shaderProgramId)) {
			std::cout << "Didn't load " << modelInfo.meshFileName << std::endl;
			std::cout << pMeshManager->GetLastError() << std::endl;
		}
	}

	{
		sModelDrawInfo modelInfo;
		modelInfo.meshFileName = "toad_xyz_n_uv.ply";
		if (!pMeshManager->LoadModelIntoVAO(modelInfo, shaderProgramId)) {
			std::cout << "Didn't load " << modelInfo.meshFileName << std::endl;
			std::cout << pMeshManager->GetLastError() << std::endl;
		}
	}

	{
		sModelDrawInfo modelInfo;
		modelInfo.meshFileName = "torch_xyz_n_uv.ply";
		if (!pMeshManager->LoadModelIntoVAO(modelInfo, shaderProgramId)) {
			std::cout << "Didn't load " << modelInfo.meshFileName << std::endl;
			std::cout << pMeshManager->GetLastError() << std::endl;
		}
	}

	{
		sModelDrawInfo modelInfo;
		modelInfo.meshFileName = "yoshi_xyz_n_uv.ply";
		if (!pMeshManager->LoadModelIntoVAO(modelInfo, shaderProgramId)) {
			std::cout << "Didn't load " << modelInfo.meshFileName << std::endl;
			std::cout << pMeshManager->GetLastError() << std::endl;
		}
	}

	{
		sModelDrawInfo modelInfo;
		modelInfo.meshFileName = "Sphere_320_faces_xyz_n_GARBAGE_uv.ply";
		if (!pMeshManager->LoadModelIntoVAO(modelInfo, shaderProgramId)) {
			std::cout << "Didn't load " << modelInfo.meshFileName << std::endl;
			std::cout << pMeshManager->GetLastError() << std::endl;
		}
	}


	{
		sModelDrawInfo modelInfo;
		modelInfo.meshFileName = "Sphere_320_faces_xyz_n_GARBAGE_uv_INVERTED_NORMALS.ply";
		if (!pMeshManager->LoadModelIntoVAO(modelInfo, shaderProgramId)) {
			std::cout << "Didn't load " << modelInfo.meshFileName << std::endl;
			std::cout << pMeshManager->GetLastError() << std::endl;
		}
	}

	{
		sModelDrawInfo modelInfo;
		modelInfo.meshFileName = "1x1_Star_1_Quad_2_Sided_xyz_n_uv.ply";
		if (!pMeshManager->LoadModelIntoVAO(modelInfo, shaderProgramId)) {
			std::cout << "Didn't load " << modelInfo.meshFileName << std::endl;
			std::cout << pMeshManager->GetLastError() << std::endl;
		}
	}

	{
		sModelDrawInfo modelInfo;
		modelInfo.meshFileName = "bun_res3_xyz_n_uv.ply";
		if (!pMeshManager->LoadModelIntoVAO(modelInfo, shaderProgramId)) {
			std::cout << "Didn't load " << modelInfo.meshFileName << std::endl;
			std::cout << pMeshManager->GetLastError() << std::endl;
		}
	}
}