#include "cComponentFactory.h"

#include "cTransformComponent.h"
#include "cEulerMovementComponent.h"
#include "cRenderMeshComponent.h"
#include "cDebugRenderComponent.h"
#include "cCollider.h"
#include "c3dSoundComponent.h"
#include "cParticleComponent.h"

void ballDebugRendering(cEntity* enitiy);

cComponentFactory::cComponentFactory(){
}


cComponentFactory::~cComponentFactory(){
	// cleanup the components
	for (cComponent* component : this->allocatedComponents) {
		delete component;
	}

}

cComponentFactory* cComponentFactory::getInstance() {
	static cComponentFactory instance;

	return &instance;
}

cComponent* cComponentFactory::createComponent(int componentType) {

	cComponent* result = nullptr;

	// see the ComponentIds.txt for types
	switch (componentType) {
	case 1:
		result = new cTransformComponent();
		break;

	case 2:
		result = new cEulerMovementComponent();
		break;

	case 3:
		result = new cRenderMeshComponent();
		break;

	case 4:
		result = new cDebugRenderComponent(ballDebugRendering);
		break;

	case 5:
		result = new cCollider();
		break;

	case 6:
		result = new c3dSoundComponent(new cSoundManager::sSoundInfo());
		break;
	case 7:
		result = new cParticleComponent();
	}

	if (result != nullptr) {
		this->allocatedComponents.push_back(result);
	}

	return result;
}
