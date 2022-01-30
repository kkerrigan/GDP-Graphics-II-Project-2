#include "cEntity.h"
#include "cEulerMovementComponent.h"
#include "cRenderMeshComponent.h"
#include "cTransformComponent.h"
#include "c3dSoundComponent.h"
#include "cParticleComponent.h"


cEntity::cEntity(int type) : UNIQUE_ID(++nextUniqueId), componentBitField(0), entityType(type) {

	// initialize all the component pointers to null
	for (int i = 0; i != ENTITY_MAX_COMPONENTS; ++i) {
		this->components[i] = nullptr;
	}
}

cEntity::~cEntity() {}

void cEntity::serializeComponent(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) {

	writer.StartObject();
	writer.String("id");
	writer.Int(this->UNIQUE_ID);

	writer.String("entityType");
	writer.Int(this->entityType);

	writer.String("componentBits");
	writer.Int(this->componentBitField);

	// Call serialize for each component
	writer.String(("components"));
	writer.StartArray();
	for (unsigned int i = 0; i < ENTITY_MAX_COMPONENTS; ++i) {
		if (static_cast<cEulerMovementComponent*>(this->components[i]) != nullptr) {

			switch (i) {
			case 1:
				static_cast<cTransformComponent*>(this->components[i])->serializeComponent(writer);
				break;

			case 2:
				static_cast<cEulerMovementComponent*>(this->components[i])->serializeComponent(writer);
				break;

			case 3:
				static_cast<cRenderMeshComponent*>(this->components[i])->serializeComponent(writer);
				break;

			case 6:
				static_cast<c3dSoundComponent*>(this->components[i])->serializeComponent(writer);
				break;
			case 7:
				static_cast<cParticleComponent*>(this->components[i])->serializeComponent(writer);
				break;
			}
		}
	}
	writer.EndArray();
	writer.EndObject();

}

int cEntity::nextUniqueId = 0;
