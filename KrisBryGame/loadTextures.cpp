#include <cBasicTextureManager.h>
#include <iostream>

void loadTextures() {
	
	cBasicTextureManager* pBasicTextureManager = cBasicTextureManager::getInstance();
	pBasicTextureManager->SetBasePath("assets/textures");

	if (!pBasicTextureManager->Create2DTextureFromBMPFile("boo.bmp", true)) {
		std::cout << "Did not load boo.bmp" << std::endl;
	}

	if (!pBasicTextureManager->Create2DTextureFromBMPFile("bowser.bmp", true)) {
		std::cout << "Did not load bowser.bmp" << std::endl;
	}

	if (!pBasicTextureManager->Create2DTextureFromBMPFile("castle.bmp", true)) {
		std::cout << "Did not load castle.bmp" << std::endl;
	}

	if (!pBasicTextureManager->Create2DTextureFromBMPFile("fire.bmp", true)) {
		std::cout << "Did not load fire.bmp" << std::endl;
	}

	if (!pBasicTextureManager->Create2DTextureFromBMPFile("goomba.bmp", true)) {
		std::cout << "Did not load goomba.bmp" << std::endl;
	}

	if (!pBasicTextureManager->Create2DTextureFromBMPFile("green_pipe.bmp", true)) {
		std::cout << "Did not load green_pipe.bmp" << std::endl;
	}

	if (!pBasicTextureManager->Create2DTextureFromBMPFile("luigi.bmp", true)) {
		std::cout << "Did not load luigi.bmp" << std::endl;
	}
	
	if (!pBasicTextureManager->Create2DTextureFromBMPFile("mario.bmp", true)) {
		std::cout << "Did not load mario.bmp" << std::endl;
	}

	if (!pBasicTextureManager->Create2DTextureFromBMPFile("old_lantern.bmp", true)) {
		std::cout << "Did not load old_latern.bmp" << std::endl;
	}

	if (!pBasicTextureManager->Create2DTextureFromBMPFile("Peach.bmp", true)) {
		std::cout << "Did not load Peach.bmp" << std::endl;
	}

	if (!pBasicTextureManager->Create2DTextureFromBMPFile("pirahna_plant.bmp", true)) {
		std::cout << "Did not load pirahna_plant.bmp" << std::endl;
	}

	if (!pBasicTextureManager->Create2DTextureFromBMPFile("star.bmp", true)) {
		std::cout << "Did not load star.bmp" << std::endl;
	}

	if (!pBasicTextureManager->Create2DTextureFromBMPFile("Toad.bmp", true)) {
		std::cout << "Did not load Toad.bmp" << std::endl;
	}

	if (!pBasicTextureManager->Create2DTextureFromBMPFile("torch.bmp", true)) {
		std::cout << "Did not load torch.bmp" << std::endl;
	}

	if (!pBasicTextureManager->Create2DTextureFromBMPFile("yoshi.bmp", true)) {
		std::cout << "Did not load yoshi.bmp" << std::endl;
	}

	if (!pBasicTextureManager->Create2DTextureFromBMPFile("metal.bmp", true)) {
		std::cout << "Did not load metal.bmp" << std::endl;
	}

	if (!pBasicTextureManager->Create2DTextureFromBMPFile("rust.bmp", true)) {
		std::cout << "Did not load rust.bmp" << std::endl;
	}

	if (!pBasicTextureManager->Create2DTextureFromBMPFile("wood.bmp", true)) {
		std::cout << "Did not load wood.bmp" << std::endl;
	}

	if (!pBasicTextureManager->Create2DTextureFromBMPFile("aged_wood.bmp", true)) {
		std::cout << "Did not load aged_wood.bmp" << std::endl;
	}

	if (!pBasicTextureManager->Create2DTextureFromBMPFile("old_metal.bmp", true)) {
		std::cout << "Did not load old_metal.bmp" << std::endl;
	}

	if (!pBasicTextureManager->Create2DTextureFromBMPFile("flames.bmp", true)) {
		std::cout << "Did not load flames.bmp" << std::endl;
	}

	if (!pBasicTextureManager->Create2DTextureFromBMPFile("sprial.bmp", true)) {
		std::cout << "Did not load sprial.bmp" << std::endl;
	}

	// Load cube maps
	pBasicTextureManager->SetBasePath("assets/textures/cubemaps");
	std::string errorString;

	/*
	if (pBasicTextureManager->CreateCubeTextureFromBMPFiles("CityCubeMap",
		"SpaceBox_right1_posX.bmp", "SpaceBox_left2_negX.bmp",
		"SpaceBox_top3_posY.bmp", "SpaceBox_bottom4_negY.bmp",
		"SpaceBox_front5_posZ.bmp", "SpaceBox_back6_negZ.bmp", true, errorString))
	{
		std::cout << "Loaded the city cube map OK" << std::endl;
	}
	else
	{
		std::cout << "Error: city cube map DIDN't load. On no!" << std::endl;
	}*/

	if ( pBasicTextureManager->CreateCubeTextureFromBMPFiles("CityCubeMap", 
		 "FullMoonLeft2048.bmp", "FullMoonRight2048.bmp",	
		 "FullMoonDown2048.bmp", "FullMoonUp2048.bmp",
		 "FullMoonFront2048.bmp", "FullMoonBack2048.bmp", true, errorString ) )
	{
		std::cout << "Loaded the night sky cube map OK" << std::endl;
	}
	else
	{
		std::cout << "Error: night sky cube map DIDN't load. On no!" << std::endl;
	}

	//if ( pBasicTextureManager->CreateCubeTextureFromBMPFiles("CityCubeMap", 
	//	 "TropicalSunnyDayLeft2048.bmp", "TropicalSunnyDayRight2048.bmp",		// Alternate these
	//	 "TropicalSunnyDayDown2048.bmp", "TropicalSunnyDayUp2048.bmp", 			// Rotate these 90 degrees
	//	 "TropicalSunnyDayFront2048.bmp", "TropicalSunnyDayBack2048.bmp", true, errorString ) )
	//{
	//	std::cout << "Loaded the city cube map OK" << std::endl;
	//}
	//else
	//{
	//	std::cout << "Error: city cube map DIDN't load. On no!" << std::endl;
	//}

	return;
}