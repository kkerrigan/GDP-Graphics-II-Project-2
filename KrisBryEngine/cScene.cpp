#include "cScene.h"

#include <cstdio>
#include <fstream>
#include <sstream>
#include <iomanip>

#include <rapidjson/prettywriter.h>
#include <rapidjson/document.h>
#include <rapidjson/filereadstream.h>
#include <rapidjson/istreamwrapper.h>

#include "cLightHelper.h"
#include "cShaderManager.h"
#include "cParticleComponent.h"
#include "cEulerMovementComponent.h"
#include "cTransformComponent.h"
#include "cRenderMeshComponent.h"
#include "cMeshRenderSystem.h"

cScene::cScene() : cameraUp(glm::vec3(0.0f, 1.0f, 0.0f)), selectedEntity(0), selectedLight(0), selectedChannelGroup(0) {

	this->bIsLightDebug = false;
	this->bIsRenderDebug = false;

	return;
}

cScene::~cScene(){}

void cScene::setSceneName(std::string name) {
	this->sceneName = name;
}

void cScene::updateWindowTitle(GLFWwindow* window) {

	cEntity* curObject = this->getSelectedEntity();
	cRenderMeshComponent* curMesh = static_cast<cRenderMeshComponent*>(curObject->components[3]);
	cTransformComponent* curTransform = static_cast<cTransformComponent*>(curObject->components[1]);
	
	sLight* curLight = this->getSelectedLight();

	// Set the window title
	std::stringstream ssTitle;
	ssTitle << "Camera (x,y,z) "
		<< " Eye "
		<< this->flyCamera.eye.x << ", " << this->flyCamera.eye.y << ", " << this->flyCamera.eye.z
		<< " Selected Model[" << curMesh->friendlyName << "] position: "
		<< curTransform->position.x << ", " << curTransform->position.y << ", " << curTransform->position.z
		<< " Selected Light[" << this->selectedLight << "] position: "
		<< curLight->position.x << ", " << curLight->position.y << ", " << curLight->position.z
		<< " Const: " << curLight->atten.x << " Linear: " << curLight->atten.y << " Quad: " << std::setprecision(9) << std::showpoint << std::fixed << curLight->atten.z;

	glfwSetWindowTitle(window, ssTitle.str().c_str());

	return;
}

bool cScene::getIsScenePaused() {
	return this->isScenePaused;
}

void cScene::setIsScenePaused(bool val) {
	this->isScenePaused = val;
}

void cScene::setCameraPosition(glm::vec3 pos) {
	this->cameraPos = pos;
}

void cScene::setCameraLookAt(glm::vec3 lookAt) {
	this->cameraLookAt = lookAt;
}

void cScene::addEntityToScene(cEntity* entity) {
	this->entities.push_back(entity);
}

void cScene::removeEntityFromScene(cEntity* entiy) {

}

cEntity* cScene::getSelectedEntity() {
	return this->entities[this->selectedEntity];
}

size_t cScene::getNumberOfEntites() {

	return this->entities.size();
}

cEntity* cScene::findObjectByFriendlyName(std::string theNameToFind) {

	for (unsigned int index = 0; index != this->entities.size(); index++) {

		cRenderMeshComponent* meshComp = static_cast<cRenderMeshComponent*>(this->entities[index]->components[3]);

		if (meshComp->friendlyName == theNameToFind)
		{
			return this->entities[index];
		}
	}

	return NULL;
}

std::vector<cEntity*>& cScene::getEntities() {
	return this->entities;
}

void cScene::addLightToScene(sLight* light) {
	this->lights.push_back(light);
}

void cScene::removeLightFromScene(sLight* light) {

}

sLight* cScene::getSelectedLight() {
	return this->lights[this->selectedLight];
}

size_t cScene::getNumberOfLights() {
	return this->lights.size();
}

std::vector<sLight*> cScene::getLights() {
	return this->lights;
}

void cScene::addSoundToScene(cSoundManager::sSoundInfo* soundInfo) {
	this->sounds.push_back(soundInfo);
}

cSoundManager::sSoundInfo* cScene::getSoundInfo(unsigned index) {
	return this->sounds[index];
}

std::vector<cSoundManager::sSoundInfo*> cScene::getSounds() {
	return this->sounds;
}


double oldTime;
void cScene::handleCommand(iCommand* command) {

	double now = glfwGetTime();
	double delta = now - oldTime;
	oldTime = now;

	if (delta > 0.1f) {
		delta = 0.1f;
	}


	// camera controls
	cMoveCameraForwardCommand* moveCamZCommand = dynamic_cast<cMoveCameraForwardCommand*>(command);
	if (moveCamZCommand) {
		moveCamZCommand->execute(&this->flyCamera);
	}

	cMoveCameraLeftRightCommand* moveCamXCommand = dynamic_cast<cMoveCameraLeftRightCommand*>(command);
	if (moveCamXCommand) {
		moveCamXCommand->execute(&this->flyCamera);
	}

	cMoveCameraUpDownCommand* moveCamYCommand = dynamic_cast<cMoveCameraUpDownCommand*>(command);
	if (moveCamYCommand) {
		moveCamYCommand->execute(&this->flyCamera);
	}

	cRollCameraCommand* rollCameraCommand = dynamic_cast<cRollCameraCommand*>(command);
	if (rollCameraCommand) {
		rollCameraCommand->execute(&this->flyCamera);
	}

	// light controls
	cMoveLightForwardCommand* moveLightForwardCommand = dynamic_cast<cMoveLightForwardCommand*>(command);
	if (moveLightForwardCommand) {
		moveLightForwardCommand->execute(this->getSelectedLight());
	}

	cMoveLightLeftRightCommand* moveLightLeftRightCommand = dynamic_cast<cMoveLightLeftRightCommand*>(command);
	if (moveLightLeftRightCommand) {
		moveLightLeftRightCommand->execute(this->getSelectedLight());
	}

	cMoveLightUpDownCommand* moveUpDownCommand = dynamic_cast<cMoveLightUpDownCommand*>(command);
	if (moveUpDownCommand) {
		moveUpDownCommand->execute(this->getSelectedLight());
	}

	//attenuation
	cLightChangeConstAttenuationCommand* changeLightConstAttenCommand = dynamic_cast<cLightChangeConstAttenuationCommand*>(command);
	if (changeLightConstAttenCommand) {
		changeLightConstAttenCommand->execute(this->getSelectedLight());
	}

	cLightChangeLinAttenuationCommand* changeLightLinAttenCommand = dynamic_cast<cLightChangeLinAttenuationCommand*>(command);
	if (changeLightLinAttenCommand) {
		changeLightLinAttenCommand->execute(this->getSelectedLight());
	}

	cLightChangeQuadAttenuationCommand* changeLightQuadAttenCommand = dynamic_cast<cLightChangeQuadAttenuationCommand*>(command);
	if (changeLightQuadAttenCommand) {
		changeLightQuadAttenCommand->execute(this->getSelectedLight());
	}
	
	// entity controls
	cTransformComponent* selectedEntityTransformComp = (cTransformComponent*)this->getSelectedEntity()->components[1];
	cRenderMeshComponent* selectedEntityMeshComp = (cRenderMeshComponent*)this->getSelectedEntity()->components[3];
	
	cMoveEntiyForwardCommand* moveEntityZCommand = dynamic_cast<cMoveEntiyForwardCommand*>(command);
	if (moveEntityZCommand) {
		moveEntityZCommand->execute(selectedEntityTransformComp);
	}

	cMoveEntiyLeftRightCommand* moveEntityXCommand = dynamic_cast<cMoveEntiyLeftRightCommand*>(command);
	if (moveEntityXCommand) {
		moveEntityXCommand->execute(selectedEntityTransformComp);
	}

	cMoveEntiyUpDownCommand* moveEntityYCommand = dynamic_cast<cMoveEntiyUpDownCommand*>(command);
	if (moveEntityYCommand) {
		moveEntityYCommand->execute(selectedEntityTransformComp);
	}

	cRotateEntiyXCommand* moveEntityRotXCommand = dynamic_cast<cRotateEntiyXCommand*>(command);
	if (moveEntityRotXCommand) {
		moveEntityRotXCommand->execute(selectedEntityTransformComp);
	}

	cRotateEntiyYCommand* moveEntityRotYCommand = dynamic_cast<cRotateEntiyYCommand*>(command);
	if (moveEntityRotYCommand) {
		moveEntityRotYCommand->execute(selectedEntityTransformComp);
	}

	cRotateEntiyZCommand* moveEntityRotZCommand = dynamic_cast<cRotateEntiyZCommand*>(command);
	if (moveEntityRotZCommand) {
		moveEntityRotZCommand->execute(selectedEntityTransformComp);
	}

	// specular power
	cChangeEntitySpecularPower* moveEntityChangeSpecPowerCommand = dynamic_cast<cChangeEntitySpecularPower*>(command);
	if (moveEntityChangeSpecPowerCommand) {
		moveEntityChangeSpecPowerCommand->execute(selectedEntityMeshComp);
	}

	//end the command's lifecycle
	delete command;
}

bool cScene::serialize() {
	
	rapidjson::StringBuffer buffer;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);

	writer.StartObject();
	writer.String("sceneName");
	writer.String(this->sceneName.c_str());
	writer.String("selectedEntity");
	writer.Int(this->selectedEntity);
	writer.String("selectedLight");
	writer.Int(this->selectedLight);

	writer.String(("camera"));
	writer.StartObject();

	writer.String("position");
	writer.StartObject();
	writer.String("x");
	writer.Double(this->flyCamera.eye.x);
	writer.String("y");
	writer.Double(this->flyCamera.eye.y);
	writer.String("z");
	writer.Double(this->flyCamera.eye.z);
	writer.EndObject();

	writer.String("orientation");
	writer.StartObject();
	writer.String("x");
	writer.Double(this->flyCamera.getQOrientation().x);
	writer.String("y");
	writer.Double(this->flyCamera.getQOrientation().y);
	writer.String("z");
	writer.Double(this->flyCamera.getQOrientation().z);
	writer.String("w");
	writer.Double(this->flyCamera.getQOrientation().w);
	writer.EndObject();

	writer.String("up");
	writer.StartObject();
	writer.String("x");
	writer.Double(this->flyCamera.getUpVector().x);
	writer.String("y");
	writer.Double(this->flyCamera.getUpVector().y);
	writer.String("z");
	writer.Double(this->flyCamera.getUpVector().z);
	writer.EndObject();
	writer.EndObject();


	writer.String(("lights"));
	writer.StartArray();
	for (std::vector<sLight*>::iterator light = this->lights.begin(); light != this->lights.end(); ++light) {
		(*light)->serializeComponent(writer);
	}
	writer.EndArray();

	writer.String(("entities"));
	writer.StartArray();
	for (std::vector<cEntity*>::iterator entity = this->entities.begin(); entity != this->entities.end(); ++entity) {
		(*entity)->serializeComponent(writer);
	}
	writer.EndArray();

	writer.String(("sounds"));
	writer.StartArray();
	for (std::vector<cSoundManager::sSoundInfo*>::iterator sound = this->sounds.begin(); sound != this->sounds.end(); ++sound) {
		writer.StartObject();
		(*sound)->serializeSound(writer);
		writer.EndObject();
	}
	writer.EndArray();

	writer.EndObject();

	// Write the JSON object to a file
	std::ofstream file("./assets/maps/" + this->sceneName);

	if (!file.is_open()) {
		return false;
	}
	file << buffer.GetString();

	return true;
}

bool cScene::deserialize(std::string filename) {
	
	this->sceneName = filename;
	std::ifstream ifs("./assets/maps/" + filename);

	if (!ifs.is_open()) {
		return false;
	}

	rapidjson::IStreamWrapper isw(ifs);
	rapidjson::Document document;
	document.ParseStream(isw);

	//scene name
	assert(document.HasMember("sceneName"));
	assert(document["sceneName"].IsString());
	this->sceneName = document["sceneName"].GetString();

	//selected entity
	assert(document.HasMember("selectedEntity"));
	assert(document["selectedEntity"].IsInt());
	this->selectedEntity = document["selectedEntity"].GetInt();

	//selected light
	assert(document.HasMember("selectedLight"));
	assert(document["selectedLight"].IsInt());
	this->selectedLight = document["selectedLight"].GetInt();

	//camera
	assert(document.HasMember("camera"));
	assert(document["camera"].IsObject());

	// position
	assert(document["camera"].HasMember("position"));
	assert(document["camera"]["position"].IsObject());

	assert(document["camera"]["position"].HasMember("x"));
	assert(document["camera"]["position"]["x"].IsDouble());
	this->flyCamera.eye.x = (float)document["camera"]["position"]["x"].GetDouble();

	assert(document["camera"]["position"].HasMember("y"));
	assert(document["camera"]["position"]["y"].IsDouble());
	this->flyCamera.eye.y = (float)document["camera"]["position"]["y"].GetDouble();

	assert(document["camera"]["position"].HasMember("z"));
	assert(document["camera"]["position"]["z"].IsDouble());
	this->flyCamera.eye.z = (float)document["camera"]["position"]["z"].GetDouble();

	// look at
	glm::quat orientation;
	assert(document["camera"].HasMember("orientation"));
	assert(document["camera"]["orientation"].IsObject());

	assert(document["camera"]["orientation"].HasMember("x"));
	assert(document["camera"]["orientation"]["x"].IsDouble());
	orientation.x = (float)document["camera"]["orientation"]["x"].GetDouble();

	assert(document["camera"]["orientation"].HasMember("y"));
	assert(document["camera"]["orientation"]["y"].IsDouble());
	orientation.y = (float)document["camera"]["orientation"]["y"].GetDouble();

	assert(document["camera"]["orientation"].HasMember("z"));
	assert(document["camera"]["orientation"]["z"].IsDouble());
	orientation.z = (float)document["camera"]["orientation"]["z"].GetDouble();

	assert(document["camera"]["orientation"].HasMember("w"));
	assert(document["camera"]["orientation"]["w"].IsDouble());
	orientation.w = (float)document["camera"]["orientation"]["w"].GetDouble();

	this->flyCamera.adjMeshOrientationQ(orientation);


	// camera up
	assert(document["camera"].HasMember("up"));
	assert(document["camera"]["up"].IsObject());

	assert(document["camera"]["up"].HasMember("x"));
	assert(document["camera"]["up"]["x"].IsDouble());
	this->cameraUp.x = (float)document["camera"]["up"]["x"].GetDouble();

	assert(document["camera"]["up"].HasMember("y"));
	assert(document["camera"]["up"]["y"].IsDouble());
	this->cameraUp.y = (float)document["camera"]["up"]["y"].GetDouble();

	assert(document["camera"]["up"].HasMember("z"));
	assert(document["camera"]["up"]["z"].IsDouble());
	this->cameraUp.z = (float)document["camera"]["up"]["z"].GetDouble();

	//lights
	assert(document.HasMember("lights"));
	assert(document["lights"].IsArray());
	rapidjson::GenericArray < false, rapidjson::Value::ValueType> lights = (document["lights"].GetArray());

	unsigned int cameraCount = 0;
	cLightManager* pLightManager = cLightManager::getInstance();
	for (rapidjson::Value::ConstValueIterator itr = lights.Begin(); itr != lights.End(); ++itr) { // Ok
		sLight* light = pLightManager->getLightByIndex(cameraCount);

		//position
		assert(itr->HasMember("position"));
		assert((*itr)["position"].IsObject());

		assert((*itr)["position"].HasMember("x"));
		assert((*itr)["position"]["x"].IsDouble());
		light->position.x = (float)(*itr)["position"]["x"].GetDouble();

		assert((*itr)["position"].HasMember("y"));
		assert((*itr)["position"]["y"].IsDouble());
		light->position.y = (float)(*itr)["position"]["y"].GetDouble();

		assert((*itr)["position"].HasMember("z"));
		assert((*itr)["position"]["z"].IsDouble());
		light->position.z = (float)(*itr)["position"]["z"].GetDouble();

		//specular
		assert(itr->HasMember("specular"));
		assert((*itr)["specular"].IsObject());

		assert((*itr)["specular"].HasMember("r"));
		assert((*itr)["specular"]["r"].IsDouble());
		light->specular.x = (float)(*itr)["specular"]["r"].GetDouble();

		assert((*itr)["specular"].HasMember("g"));
		assert((*itr)["specular"]["g"].IsDouble());
		light->specular.y = (float)(*itr)["specular"]["g"].GetDouble();

		assert((*itr)["specular"].HasMember("b"));
		assert((*itr)["specular"]["b"].IsDouble());
		light->specular.z = (float)(*itr)["specular"]["b"].GetDouble();

		assert((*itr)["specular"].HasMember("a"));
		assert((*itr)["specular"]["a"].IsDouble());
		light->specular.w = (float)(*itr)["specular"]["a"].GetDouble();

		//diffuse
		assert(itr->HasMember("diffuse"));
		assert((*itr)["diffuse"].IsObject());

		assert((*itr)["diffuse"].HasMember("r"));
		assert((*itr)["diffuse"]["r"].IsDouble());
		light->diffuse.x = (float)(*itr)["diffuse"]["r"].GetDouble();

		assert((*itr)["diffuse"].HasMember("g"));
		assert((*itr)["diffuse"]["g"].IsDouble());
		light->diffuse.y = (float)(*itr)["diffuse"]["g"].GetDouble();

		assert((*itr)["diffuse"].HasMember("b"));
		assert((*itr)["diffuse"]["b"].IsDouble());
		light->diffuse.z = (float)(*itr)["diffuse"]["b"].GetDouble();

		assert((*itr)["diffuse"].HasMember("a"));
		assert((*itr)["diffuse"]["a"].IsDouble());
		light->diffuse.w = (float)(*itr)["diffuse"]["a"].GetDouble();

		//attenuation
		assert(itr->HasMember("attenuation"));
		assert((*itr)["attenuation"].IsObject());

		assert((*itr)["attenuation"].HasMember("constant"));
		assert((*itr)["attenuation"]["constant"].IsDouble());
		light->atten.x = (float)(*itr)["attenuation"]["constant"].GetDouble();

		assert((*itr)["attenuation"].HasMember("linear"));
		assert((*itr)["attenuation"]["linear"].IsDouble());
		light->atten.y = (float)(*itr)["attenuation"]["linear"].GetDouble();

		assert((*itr)["attenuation"].HasMember("quadratic"));
		assert((*itr)["attenuation"]["quadratic"].IsDouble());
		light->atten.z = (float)(*itr)["attenuation"]["quadratic"].GetDouble();

		assert((*itr)["attenuation"].HasMember("cutoffDist"));
		assert((*itr)["attenuation"]["cutoffDist"].IsDouble());
		light->atten.w = (float)(*itr)["attenuation"]["cutoffDist"].GetDouble();

		//direction
		assert(itr->HasMember("direction"));
		assert((*itr)["direction"].IsObject());

		assert((*itr)["direction"].HasMember("x"));
		assert((*itr)["direction"]["x"].IsDouble());
		light->direction.x = (float)(*itr)["direction"]["x"].GetDouble();

		assert((*itr)["direction"].HasMember("y"));
		assert((*itr)["direction"]["y"].IsDouble());
		light->direction.y = (float)(*itr)["direction"]["y"].GetDouble();

		assert((*itr)["direction"].HasMember("z"));
		assert((*itr)["direction"]["z"].IsDouble());
		light->direction.z = (float)(*itr)["direction"]["z"].GetDouble();

		assert((*itr)["direction"].HasMember("w"));
		assert((*itr)["direction"]["w"].IsDouble());
		light->direction.w = (float)(*itr)["direction"]["w"].GetDouble();

		//param1
		assert(itr->HasMember("param1"));
		assert((*itr)["param1"].IsObject());

		assert((*itr)["param1"].HasMember("lightType"));
		assert((*itr)["param1"]["lightType"].IsDouble());
		light->param1.x = (float)(*itr)["param1"]["lightType"].GetDouble();

		assert((*itr)["param1"].HasMember("innerAngle"));
		assert((*itr)["param1"]["innerAngle"].IsDouble());
		light->param1.y = (float)(*itr)["param1"]["innerAngle"].GetDouble();

		assert((*itr)["param1"].HasMember("outerAngle"));
		assert((*itr)["param1"]["outerAngle"].IsDouble());
		light->param1.z = (float)(*itr)["param1"]["outerAngle"].GetDouble();

		assert((*itr)["param1"].HasMember("w"));
		assert((*itr)["param1"]["w"].IsDouble());
		light->param1.w = (float)(*itr)["param1"]["w"].GetDouble();

		//param2
		assert(itr->HasMember("param2"));
		assert((*itr)["param2"].IsObject());

		assert((*itr)["param2"].HasMember("isOn"));
		assert((*itr)["param2"]["isOn"].IsDouble());
		light->param2.x = (float)(*itr)["param2"]["isOn"].GetDouble();

		assert((*itr)["param2"].HasMember("y"));
		assert((*itr)["param2"]["y"].IsDouble());
		light->param2.y = (float)(*itr)["param2"]["y"].GetDouble();

		assert((*itr)["param2"].HasMember("z"));
		assert((*itr)["param2"]["z"].IsDouble());
		light->param2.z = (float)(*itr)["param2"]["z"].GetDouble();

		assert((*itr)["param2"].HasMember("w"));
		assert((*itr)["param2"]["w"].IsDouble());
		light->param2.z = (float)(*itr)["param2"]["w"].GetDouble();

		this->addLightToScene(light);

		cameraCount++;
	}

	// sounds non-3d
	assert(document.HasMember("sounds"));
	assert(document["sounds"].IsArray());

	rapidjson::GenericArray < false, rapidjson::Value::ValueType> sounds = (document["sounds"].GetArray());

	for (rapidjson::Value::ConstValueIterator compItr = sounds.Begin(); compItr != sounds.End(); ++compItr) {

		cSoundManager::sSoundInfo* soundInfo = new cSoundManager::sSoundInfo();

		//acceleration
		assert(compItr->HasMember("fileName"));
		assert((*compItr)["fileName"].IsString());
		soundInfo->fileName = (*compItr)["fileName"].GetString();

		cSoundManager::getInstance()->load3dSound(soundInfo, FMOD_LOOP_NORMAL | FMOD_3D_LINEARROLLOFF);

		assert(compItr->HasMember("soundType"));
		assert((*compItr)["soundType"].IsInt());
		soundInfo->type = (*compItr)["soundType"].GetInt();

		assert(compItr->HasMember("channelGroup"));
		assert((*compItr)["channelGroup"].IsInt());
		soundInfo->channelGroupIndex = (*compItr)["channelGroup"].GetInt();

		assert(compItr->HasMember("isPaused"));
		assert((*compItr)["isPaused"].IsBool());
		soundInfo->isPaused = (*compItr)["isPaused"].GetBool();

		assert(compItr->HasMember("isCompressed"));
		assert((*compItr)["isCompressed"].IsBool());
		soundInfo->isCompressed = (*compItr)["isCompressed"].GetBool();

		assert(compItr->HasMember("frequency"));
		assert((*compItr)["frequency"].IsDouble());
		soundInfo->frequency = (float)(*compItr)["frequency"].GetDouble();

		assert(compItr->HasMember("volume"));
		assert((*compItr)["volume"].IsDouble());
		soundInfo->volume = (float)(*compItr)["volume"].GetDouble();

		assert(compItr->HasMember("balance"));
		assert((*compItr)["balance"].IsDouble());
		soundInfo->balance = (float)(*compItr)["balance"].GetDouble();

		assert(compItr->HasMember("pitch"));
		assert((*compItr)["pitch"].IsDouble());
		soundInfo->pitch = (float)(*compItr)["pitch"].GetDouble();

		assert(compItr->HasMember("soundLength"));
		assert((*compItr)["soundLength"].IsUint());
		soundInfo->soundLength = (*compItr)["soundLength"].GetUint();

		assert(compItr->HasMember("channelPosition"));
		assert((*compItr)["channelPosition"].IsUint());
		soundInfo->channelPosition = (*compItr)["channelPosition"].GetUint();

		assert(compItr->HasMember("minDistance"));
		assert((*compItr)["minDistance"].IsDouble());
		soundInfo->f3dMinDistance = (float)(*compItr)["minDistance"].GetDouble();

		assert(compItr->HasMember("maxDistance"));
		assert((*compItr)["maxDistance"].IsDouble());
		soundInfo->f3dMaxDistance = (float)(*compItr)["maxDistance"].GetDouble();

		// "play the sound" to get a channels and others stuff
		cSoundManager::getInstance()->playSound(soundInfo);

		cSoundManager::getInstance()->setPaused(soundInfo, soundInfo->isPaused);
		cSoundManager::getInstance()->setSoundFrequency(*soundInfo, soundInfo->frequency);
		cSoundManager::getInstance()->setSoundVolume(*soundInfo, soundInfo->volume);
		cSoundManager::getInstance()->setSoundPitch(*soundInfo, soundInfo->pitch);
		cSoundManager::getInstance()->setSoundCurrentPosition(*soundInfo, soundInfo->channelPosition);

		this->addSoundToScene(soundInfo);

	}

	//entities
	assert(document.HasMember("entities"));
	assert(document["entities"].IsArray());
	rapidjson::GenericArray < false, rapidjson::Value::ValueType> entities = (document["entities"].GetArray()); //this is super templated library (the type is crazy)
	for (rapidjson::Value::ConstValueIterator itr = entities.Begin(); itr != entities.End(); ++itr) { // Ok

		assert((*itr).HasMember("entityType"));
		assert((*itr)["entityType"].IsInt());

		// create an instance of the entity
		cEntity* entity = cEntityBuilder::getInstance()->createEntity((*itr)["entityType"].GetInt());

		// update the components to match the file
		assert((*itr).HasMember("components"));
		assert((*itr)["components"].IsArray());
		auto components = ((*itr)["components"].GetArray()); //this is super templated library (the type is crazy)
		for (rapidjson::Value::ConstValueIterator compItr = components.Begin(); compItr != components.End(); ++compItr) {

			assert((*compItr).HasMember("componentType"));
			assert((*compItr)["componentType"].IsInt());

			switch ((*compItr)["componentType"].GetInt()) {
			case 1:
			{
				cTransformComponent* transformComp = (cTransformComponent*)(entity->components[1]);

				//position
				assert(compItr->HasMember("position"));
				assert((*compItr)["position"].IsObject());

				assert((*compItr)["position"].HasMember("x"));
				assert((*compItr)["position"]["x"].IsDouble());
				transformComp->position.x = (float)(*compItr)["position"]["x"].GetDouble();

				assert((*compItr)["position"].HasMember("y"));
				assert((*compItr)["position"]["y"].IsDouble());
				transformComp->position.y = (float)(*compItr)["position"]["y"].GetDouble();

				assert((*compItr)["position"].HasMember("z"));
				assert((*compItr)["position"]["z"].IsDouble());
				transformComp->position.z = (float)(*compItr)["position"]["z"].GetDouble();

				//rotation
				assert(compItr->HasMember("orientation"));
				assert((*compItr)["orientation"].IsObject());

				glm::vec3 eulerOrientation;

				assert((*compItr)["orientation"].HasMember("x"));
				assert((*compItr)["orientation"]["x"].IsDouble());
				eulerOrientation.x = (float)(*compItr)["orientation"]["x"].GetDouble();

				assert((*compItr)["orientation"].HasMember("y"));
				assert((*compItr)["orientation"]["y"].IsDouble());
				eulerOrientation.y = (float)(*compItr)["orientation"]["y"].GetDouble();

				assert((*compItr)["orientation"].HasMember("z"));
				assert((*compItr)["orientation"]["z"].IsDouble());
				eulerOrientation.z = (float)(*compItr)["orientation"]["z"].GetDouble();

				transformComp->setOrientationEulerAngles(eulerOrientation, true);

				//scale
				assert(compItr->HasMember("scale"));
				assert((*compItr)["scale"].IsObject());

				assert((*compItr)["scale"].HasMember("x"));
				assert((*compItr)["scale"]["x"].IsDouble());
				transformComp->scale.x = (float)(*compItr)["scale"]["x"].GetDouble();

				assert((*compItr)["scale"].HasMember("y"));
				assert((*compItr)["scale"]["y"].IsDouble());
				transformComp->scale.y = (float)(*compItr)["scale"]["y"].GetDouble();

				assert((*compItr)["scale"].HasMember("z"));
				assert((*compItr)["scale"]["z"].IsDouble());
				transformComp->scale.z = (float)(*compItr)["scale"]["z"].GetDouble();

				this->addEntityToScene(entity);
				break;
			}

			case 2:
			{
				cEulerMovementComponent* eulerMoveComp = (cEulerMovementComponent*)(entity->components[2]);

				//acceleration
				assert(compItr->HasMember("acceleration"));
				assert((*compItr)["acceleration"].IsObject());

				assert((*compItr)["acceleration"].HasMember("x"));
				assert((*compItr)["acceleration"]["x"].IsDouble());
				eulerMoveComp->acceleration.x = (float)(*compItr)["acceleration"]["x"].GetDouble();

				assert((*compItr)["acceleration"].HasMember("y"));
				assert((*compItr)["acceleration"]["y"].IsDouble());
				eulerMoveComp->acceleration.y = (float)(*compItr)["acceleration"]["y"].GetDouble();

				assert((*compItr)["acceleration"].HasMember("z"));
				assert((*compItr)["acceleration"]["z"].IsDouble());
				eulerMoveComp->acceleration.z = (float)(*compItr)["acceleration"]["z"].GetDouble();

				//velocity
				assert(compItr->HasMember("velocity"));
				assert((*compItr)["velocity"].IsObject());

				assert((*compItr)["velocity"].HasMember("x"));
				assert((*compItr)["velocity"]["x"].IsDouble());
				eulerMoveComp->velocity.x = (float)(*compItr)["velocity"]["x"].GetDouble();

				assert((*compItr)["velocity"].HasMember("y"));
				assert((*compItr)["velocity"]["y"].IsDouble());
				eulerMoveComp->velocity.y = (float)(*compItr)["velocity"]["y"].GetDouble();

				assert((*compItr)["velocity"].HasMember("z"));
				assert((*compItr)["velocity"]["z"].IsDouble());
				eulerMoveComp->velocity.z = (float)(*compItr)["velocity"]["z"].GetDouble();

				break;
			}

			case 3:
			{
				cRenderMeshComponent* renderComp = (cRenderMeshComponent*)(entity->components[3]);

				//acceleration
				assert(compItr->HasMember("meshName"));
				assert((*compItr)["meshName"].IsString());
				renderComp->meshName = (*compItr)["meshName"].GetString();

				assert(compItr->HasMember("friendlyName"));
				assert((*compItr)["friendlyName"].IsString());
				renderComp->friendlyName = (*compItr)["friendlyName"].GetString();

				assert(compItr->HasMember("isWireframe"));
				assert((*compItr)["isWireframe"].IsBool());
				renderComp->bIsWireFrame = (*compItr)["isWireframe"].GetBool();

				assert(compItr->HasMember("isVisible"));
				assert((*compItr)["isVisible"].IsBool());
				renderComp->bIsVisible = (*compItr)["isVisible"].GetBool();

				assert(compItr->HasMember("useVertexColor"));
				assert((*compItr)["useVertexColor"].IsBool());
				renderComp->bUseVertexColour = (*compItr)["useVertexColor"].GetBool();

				assert(compItr->HasMember("dontLight"));
				assert((*compItr)["dontLight"].IsBool());
				renderComp->bDontLight = (*compItr)["dontLight"].GetBool();

				assert(compItr->HasMember("color"));
				assert((*compItr)["color"].IsObject());

				assert((*compItr)["color"].HasMember("r"));
				assert((*compItr)["color"]["r"].IsDouble());
				renderComp->materialDiffuse.r = (float)(*compItr)["color"]["r"].GetDouble();

				assert((*compItr)["color"].HasMember("g"));
				assert((*compItr)["color"]["g"].IsDouble());
				renderComp->materialDiffuse.g = (float)(*compItr)["color"]["g"].GetDouble();

				assert((*compItr)["color"].HasMember("b"));
				assert((*compItr)["color"]["b"].IsDouble());
				renderComp->materialDiffuse.b = (float)(*compItr)["color"]["b"].GetDouble();

				assert((*compItr)["color"].HasMember("a"));
				assert((*compItr)["color"]["a"].IsDouble());
				renderComp->materialDiffuse.a = (float)(*compItr)["color"]["a"].GetDouble();

				//textures
				assert(compItr->HasMember("textures"));
				assert((*compItr)["textures"].IsArray());
				rapidjson::GenericArray < true, rapidjson::Value::ValueType> textures = ((*compItr)["textures"].GetArray());

				for (rapidjson::Value::ConstValueIterator texture = textures.Begin(); texture != textures.End(); ++texture) {
					sTextureInfo textureInfo;

					assert(texture->HasMember("name"));
					assert((*texture)["name"].IsString());
					textureInfo.name = (*texture)["name"].GetString();

					assert(texture->HasMember("strength"));
					assert((*texture)["strength"].IsDouble());
					textureInfo.strength = (float)(*texture)["strength"].GetDouble();

					renderComp->textures.push_back(textureInfo);
				}


				assert(compItr->HasMember("specular"));
				assert((*compItr)["specular"].IsObject());

				assert((*compItr)["specular"].HasMember("r"));
				assert((*compItr)["specular"]["r"].IsDouble());
				renderComp->materialSpecular.r = (float)(*compItr)["specular"]["r"].GetDouble();

				assert((*compItr)["specular"].HasMember("g"));
				assert((*compItr)["specular"]["g"].IsDouble());
				renderComp->materialSpecular.g = (float)(*compItr)["specular"]["g"].GetDouble();

				assert((*compItr)["specular"].HasMember("b"));
				assert((*compItr)["specular"]["b"].IsDouble());
				renderComp->materialSpecular.b = (float)(*compItr)["specular"]["b"].GetDouble();

				assert((*compItr)["specular"].HasMember("shinyness"));
				assert((*compItr)["specular"]["shinyness"].IsDouble());
				renderComp->materialSpecular.a = (float)(*compItr)["specular"]["shinyness"].GetDouble();

				break;
			}

			case 6:
			{
				c3dSoundComponent* soundComp = (c3dSoundComponent*)(entity->components[6]);

				//acceleration
				assert(compItr->HasMember("fileName"));
				assert((*compItr)["fileName"].IsString());
				soundComp->soundInfo->fileName = (*compItr)["fileName"].GetString();

				cSoundManager::getInstance()->load3dSound(soundComp->soundInfo, FMOD_LOOP_NORMAL | FMOD_3D_LINEARROLLOFF);

				assert(compItr->HasMember("soundType"));
				assert((*compItr)["soundType"].IsInt());
				soundComp->soundInfo->type = (*compItr)["soundType"].GetInt();

				assert(compItr->HasMember("channelGroup"));
				assert((*compItr)["channelGroup"].IsInt());
				soundComp->soundInfo->channelGroupIndex = (*compItr)["channelGroup"].GetInt();

				assert(compItr->HasMember("isPaused"));
				assert((*compItr)["isPaused"].IsBool());
				soundComp->soundInfo->isPaused = (*compItr)["isPaused"].GetBool();

				assert(compItr->HasMember("isCompressed"));
				assert((*compItr)["isCompressed"].IsBool());
				soundComp->soundInfo->isCompressed = (*compItr)["isCompressed"].GetBool();

				assert(compItr->HasMember("frequency"));
				assert((*compItr)["frequency"].IsDouble());
				soundComp->soundInfo->frequency = (float)(*compItr)["frequency"].GetDouble();

				assert(compItr->HasMember("volume"));
				assert((*compItr)["volume"].IsDouble());
				soundComp->soundInfo->volume = (float)(*compItr)["volume"].GetDouble();

				assert(compItr->HasMember("balance"));
				assert((*compItr)["balance"].IsDouble());
				soundComp->soundInfo->balance = (float)(*compItr)["balance"].GetDouble();

				assert(compItr->HasMember("pitch"));
				assert((*compItr)["pitch"].IsDouble());
				soundComp->soundInfo->pitch = (float)(*compItr)["pitch"].GetDouble();

				assert(compItr->HasMember("soundLength"));
				assert((*compItr)["soundLength"].IsUint());
				soundComp->soundInfo->soundLength = (*compItr)["soundLength"].GetUint();

				assert(compItr->HasMember("channelPosition"));
				assert((*compItr)["channelPosition"].IsUint());
				soundComp->soundInfo->channelPosition = (*compItr)["channelPosition"].GetUint();

				assert(compItr->HasMember("minDistance"));
				assert((*compItr)["minDistance"].IsDouble());
				soundComp->soundInfo->f3dMinDistance = (float)(*compItr)["minDistance"].GetDouble();

				assert(compItr->HasMember("maxDistance"));
				assert((*compItr)["maxDistance"].IsDouble());
				soundComp->soundInfo->f3dMaxDistance = (float)(*compItr)["maxDistance"].GetDouble();

				// "play the sound" to get a channels and others stuff
				cSoundManager::getInstance()->playSound(soundComp->soundInfo);

				//move the 3d sound
				cTransformComponent* transComp = (cTransformComponent*)entity->components[1];
				FMOD_VECTOR position;
				position.x = transComp->position.x;
				position.y = transComp->position.y;
				position.z = transComp->position.z;

				FMOD_VECTOR velocity = { 0.0f,0.0f,0.0f };

				cSoundManager::getInstance()->mresult = soundComp->soundInfo->channel->set3DAttributes(&position, &velocity);
				cSoundManager::getInstance()->checkError();

				cSoundManager::getInstance()->mresult = soundComp->soundInfo->channel->set3DMinMaxDistance(soundComp->soundInfo->f3dMinDistance, soundComp->soundInfo->f3dMaxDistance);
				cSoundManager::getInstance()->checkError();

				cSoundManager::getInstance()->setPaused(soundComp->soundInfo, soundComp->soundInfo->isPaused);
				cSoundManager::getInstance()->setSoundFrequency(*soundComp->soundInfo, soundComp->soundInfo->frequency);
				cSoundManager::getInstance()->setSoundVolume(*soundComp->soundInfo, soundComp->soundInfo->volume);
				cSoundManager::getInstance()->setSoundPitch(*soundComp->soundInfo, soundComp->soundInfo->pitch);
				cSoundManager::getInstance()->setSoundCurrentPosition(*soundComp->soundInfo, soundComp->soundInfo->channelPosition);

				cSoundManager::getInstance()->update();

				break;
			}
			case 7:
			{
				cParticleComponent* particleComp = static_cast<cParticleComponent*>(entity->components[7]);
				cParticleComponent::sParticleCreationParams particleParams;

				assert(compItr->HasMember("minLifeTime"));
				assert((*compItr)["minLifeTime"].IsDouble());
				particleParams.minLifeTime = (float)(*compItr)["minLifeTime"].GetDouble();

				assert(compItr->HasMember("maxLifeTime"));
				assert((*compItr)["maxLifeTime"].IsDouble());
				particleParams.maxLifeTime = (float)(*compItr)["maxLifeTime"].GetDouble();

				assert(compItr->HasMember("totalParticles"));
				assert((*compItr)["totalParticles"].IsUint());
				particleParams.totalNumberOfParticles = (*compItr)["totalParticles"].GetUint();

				//position
				assert(compItr->HasMember("position"));
				assert((*compItr)["position"].IsObject());

				assert((*compItr)["position"].HasMember("x"));
				assert((*compItr)["position"]["x"].IsDouble());
				particleParams.position.x = (float)(*compItr)["position"]["x"].GetDouble();

				assert((*compItr)["position"].HasMember("y"));
				assert((*compItr)["position"]["y"].IsDouble());
				particleParams.position.y = (float)(*compItr)["position"]["y"].GetDouble();

				assert((*compItr)["position"].HasMember("z"));
				assert((*compItr)["position"]["z"].IsDouble());
				particleParams.position.z = (float)(*compItr)["position"]["z"].GetDouble();

				// min intital velocity
				assert(compItr->HasMember("minInitialVelocity"));
				assert((*compItr)["minInitialVelocity"].IsObject());

				assert((*compItr)["minInitialVelocity"].HasMember("x"));
				assert((*compItr)["minInitialVelocity"]["x"].IsDouble());
				particleParams.minInitialVelocity.x = (float)(*compItr)["minInitialVelocity"]["x"].GetDouble();

				assert((*compItr)["minInitialVelocity"].HasMember("y"));
				assert((*compItr)["minInitialVelocity"]["y"].IsDouble());
				particleParams.minInitialVelocity.y = (float)(*compItr)["minInitialVelocity"]["y"].GetDouble();

				assert((*compItr)["minInitialVelocity"].HasMember("z"));
				assert((*compItr)["minInitialVelocity"]["z"].IsDouble());
				particleParams.minInitialVelocity.z = (float)(*compItr)["minInitialVelocity"]["z"].GetDouble();

				// max initial velocity
				assert(compItr->HasMember("maxInitialVelocity"));
				assert((*compItr)["maxInitialVelocity"].IsObject());

				assert((*compItr)["maxInitialVelocity"].HasMember("x"));
				assert((*compItr)["maxInitialVelocity"]["x"].IsDouble());
				particleParams.maxInitialVelocity.x = (float)(*compItr)["maxInitialVelocity"]["x"].GetDouble();

				assert((*compItr)["maxInitialVelocity"].HasMember("y"));
				assert((*compItr)["maxInitialVelocity"]["y"].IsDouble());
				particleParams.maxInitialVelocity.y = (float)(*compItr)["maxInitialVelocity"]["y"].GetDouble();

				assert((*compItr)["maxInitialVelocity"].HasMember("z"));
				assert((*compItr)["maxInitialVelocity"]["z"].IsDouble());
				particleParams.maxInitialVelocity.z = (float)(*compItr)["maxInitialVelocity"]["z"].GetDouble();

				// acceleration
				assert(compItr->HasMember("acceleration"));
				assert((*compItr)["acceleration"].IsObject());

				assert((*compItr)["acceleration"].HasMember("x"));
				assert((*compItr)["acceleration"]["x"].IsDouble());
				particleParams.acceleration.x = (float)(*compItr)["acceleration"]["x"].GetDouble();

				assert((*compItr)["acceleration"].HasMember("y"));
				assert((*compItr)["acceleration"]["y"].IsDouble());
				particleParams.acceleration.y = (float)(*compItr)["acceleration"]["y"].GetDouble();

				assert((*compItr)["acceleration"].HasMember("z"));
				assert((*compItr)["acceleration"]["z"].IsDouble());
				particleParams.acceleration.z = (float)(*compItr)["acceleration"]["z"].GetDouble();

				// min initial orientation
				assert(compItr->HasMember("minInitialOrientation"));
				assert((*compItr)["minInitialOrientation"].IsObject());

				assert((*compItr)["minInitialOrientation"].HasMember("x"));
				assert((*compItr)["minInitialOrientation"]["x"].IsDouble());
				particleParams.minInitialOrientationEuler.x = (float)(*compItr)["minInitialOrientation"]["x"].GetDouble();

				assert((*compItr)["minInitialOrientation"].HasMember("y"));
				assert((*compItr)["minInitialOrientation"]["y"].IsDouble());
				particleParams.minInitialOrientationEuler.y = (float)(*compItr)["minInitialOrientation"]["y"].GetDouble();

				assert((*compItr)["minInitialOrientation"].HasMember("z"));
				assert((*compItr)["minInitialOrientation"]["z"].IsDouble());
				particleParams.minInitialOrientationEuler.z = (float)(*compItr)["minInitialOrientation"]["z"].GetDouble();

				// max initial orientation
				assert(compItr->HasMember("maxInitialOrientation"));
				assert((*compItr)["maxInitialOrientation"].IsObject());

				assert((*compItr)["maxInitialOrientation"].HasMember("x"));
				assert((*compItr)["maxInitialOrientation"]["x"].IsDouble());
				particleParams.maxInitialOrientationEuler.x = (float)(*compItr)["maxInitialOrientation"]["x"].GetDouble();

				assert((*compItr)["maxInitialOrientation"].HasMember("y"));
				assert((*compItr)["maxInitialOrientation"]["y"].IsDouble());
				particleParams.maxInitialOrientationEuler.y = (float)(*compItr)["maxInitialOrientation"]["y"].GetDouble();

				assert((*compItr)["maxInitialOrientation"].HasMember("z"));
				assert((*compItr)["maxInitialOrientation"]["z"].IsDouble());
				particleParams.maxInitialOrientationEuler.z = (float)(*compItr)["maxInitialOrientation"]["z"].GetDouble();

				// min orientation change
				assert(compItr->HasMember("minOrientationChange"));
				assert((*compItr)["minOrientationChange"].IsObject());

				assert((*compItr)["minOrientationChange"].HasMember("x"));
				assert((*compItr)["minOrientationChange"]["x"].IsDouble());
				particleParams.minOrientationChangeEuler.x = (float)(*compItr)["minOrientationChange"]["x"].GetDouble();

				assert((*compItr)["minOrientationChange"].HasMember("y"));
				assert((*compItr)["minOrientationChange"]["y"].IsDouble());
				particleParams.minOrientationChangeEuler.y = (float)(*compItr)["minOrientationChange"]["y"].GetDouble();

				assert((*compItr)["minOrientationChange"].HasMember("z"));
				assert((*compItr)["minOrientationChange"]["z"].IsDouble());
				particleParams.minOrientationChangeEuler.z = (float)(*compItr)["minOrientationChange"]["z"].GetDouble();

				// max orientation change
				assert(compItr->HasMember("maxOrientationChange"));
				assert((*compItr)["maxOrientationChange"].IsObject());

				assert((*compItr)["maxOrientationChange"].HasMember("x"));
				assert((*compItr)["maxOrientationChange"]["x"].IsDouble());
				particleParams.maxOrientationChangeEuler.x = (float)(*compItr)["maxOrientationChange"]["x"].GetDouble();

				assert((*compItr)["maxOrientationChange"].HasMember("y"));
				assert((*compItr)["maxOrientationChange"]["y"].IsDouble());
				particleParams.maxOrientationChangeEuler.y = (float)(*compItr)["maxOrientationChange"]["y"].GetDouble();

				assert((*compItr)["maxOrientationChange"].HasMember("z"));
				assert((*compItr)["maxOrientationChange"]["z"].IsDouble());
				particleParams.maxOrientationChangeEuler.z = (float)(*compItr)["maxOrientationChange"]["z"].GetDouble();

				assert(compItr->HasMember("minInitialScale"));
				assert((*compItr)["minInitialScale"].IsDouble());
				particleParams.minInitialScale = (float)(*compItr)["minInitialScale"].GetDouble();

				assert(compItr->HasMember("maxInitialScale"));
				assert((*compItr)["maxInitialScale"].IsDouble());
				particleParams.maxInitialScale = (float)(*compItr)["maxInitialScale"].GetDouble();

				assert(compItr->HasMember("minScaleChange"));
				assert((*compItr)["minScaleChange"].IsDouble());
				particleParams.minScaleChange = (float)(*compItr)["minScaleChange"].GetDouble();

				assert(compItr->HasMember("maxScaleChange"));
				assert((*compItr)["maxScaleChange"].IsDouble());
				particleParams.maxScaleChange = (float)(*compItr)["maxScaleChange"].GetDouble();

				assert(compItr->HasMember("deathFadeTime"));
				assert((*compItr)["deathFadeTime"].IsDouble());
				particleParams.deathTransparencyFadeTimeSeconds = (float)(*compItr)["deathFadeTime"].GetDouble();

				assert(compItr->HasMember("minNewParticles"));
				assert((*compItr)["minNewParticles"].IsUint());
				particleParams.minNumberNewParticles = (*compItr)["minNewParticles"].GetUint();

				assert(compItr->HasMember("maxNewParticles"));
				assert((*compItr)["maxNewParticles"].IsUint());
				particleParams.maxNumberNewParticles = (*compItr)["maxNewParticles"].GetUint();

				assert(compItr->HasMember("minTimeBetweenGen"));
				assert((*compItr)["minTimeBetweenGen"].IsDouble());
				particleParams.minTimeBetweenParticleGenSeconds = (float)(*compItr)["minTimeBetweenGen"].GetDouble();

				assert(compItr->HasMember("maxTimeBetweenGen"));
				assert((*compItr)["maxTimeBetweenGen"].IsDouble());
				particleParams.maxTimeBetweenParticleGenSeconds = (float)(*compItr)["maxTimeBetweenGen"].GetDouble();

				particleComp->init(particleParams);

				break;
			}
			}
		}
	}

	return true;
}

void cScene::setLightDebugSphere(cEntity* pSphere) {

	sLight* pCurLight = this->getSelectedLight();

	int program = cShaderManager::getInstance()->getIDFromFriendlyName("BasicUberShader");

	// Create debug object
	cTransformComponent* pSphereTrans = static_cast<cTransformComponent*>(pSphere->components[1]);
	cRenderMeshComponent* pSphereMesh = static_cast<cRenderMeshComponent*>(pSphere->components[3]);
	pSphereTrans->position = glm::vec3(pCurLight->position);
	pSphereMesh->setDiffuseColour(glm::vec3(1.0f, 1.0f, 1.0f));
	pSphereTrans->setUniformScale(0.1f);

	glm::mat4 matBall = glm::mat4(1.0f);
	cMeshRenderSystem::getInstance()->drawObject(pSphere, matBall, program);

	cLightHelper* pLightHelper = new cLightHelper();

	// calculate 90% distance
	float distance90Percent = pLightHelper->calcApproxDistFromAtten(0.90f, this->ACCURACY_OF_DISTANCE, this->INFINITE_DISTANCE,
		pCurLight->atten.x, pCurLight->atten.y, pCurLight->atten.z);
	pSphereTrans->setUniformScale(distance90Percent);
	pSphereMesh->setDiffuseColour(glm::vec3(1.0f, 1.0f, 0.0f));
	cMeshRenderSystem::getInstance()->drawObject(pSphere, matBall, program);

	// calculate 50% brightness
	float distance50Percent = pLightHelper->calcApproxDistFromAtten(0.50f, this->ACCURACY_OF_DISTANCE, this->INFINITE_DISTANCE,
		pCurLight->atten.x, pCurLight->atten.y, pCurLight->atten.z);
	pSphereTrans->setUniformScale(distance50Percent);
	pSphereMesh->setDiffuseColour(glm::vec3(0.0f, 1.0f, 0.0f));
	cMeshRenderSystem::getInstance()->drawObject(pSphere, matBall, program);

	// calculate 25% brightness
	float distance25Percent = pLightHelper->calcApproxDistFromAtten(0.25f, this->ACCURACY_OF_DISTANCE, this->INFINITE_DISTANCE,
		pCurLight->atten.x, pCurLight->atten.y, pCurLight->atten.z);
	pSphereTrans->setUniformScale(distance25Percent);
	pSphereMesh->setDiffuseColour(glm::vec3(1.0f, 0.0f, 0.0f));
	cMeshRenderSystem::getInstance()->drawObject(pSphere, matBall, program);

	// calculate 1% brightness
	float distance1Percent = pLightHelper->calcApproxDistFromAtten(0.01f, this->ACCURACY_OF_DISTANCE, this->INFINITE_DISTANCE,
		pCurLight->atten.x, pCurLight->atten.y, pCurLight->atten.z);
	pSphereTrans->setUniformScale(distance1Percent);
	pSphereMesh->setDiffuseColour(glm::vec3(0.0f, 0.0f, 1.0f));
	cMeshRenderSystem::getInstance()->drawObject(pSphere, matBall, program);

	return;
}