#include "cTransformComponent.h"

void cTransformComponent::serializeComponent(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) {
	writer.StartObject();

	writer.String("componentType");
	writer.Int(this->COMPONENT_TYPE_ID);

	writer.String("position");
	writer.StartObject();
	writer.String("x");
	writer.Double(this->position.x);
	writer.String("y");
	writer.Double(this->position.y);
	writer.String("z");
	writer.Double(this->position.z);
	writer.EndObject();

	//TODO: Update to serialize Quat
	glm::vec3 eulerOrientation = glm::degrees(glm::eulerAngles<float>(this->orientation));

	writer.String("orientation");
	writer.StartObject();
	writer.String("x");
	writer.Double(eulerOrientation.x);
	writer.String("y");
	writer.Double(eulerOrientation.y);
	writer.String("z");
	writer.Double(eulerOrientation.z);
	writer.EndObject();

	writer.String("scale");
	writer.StartObject();
	writer.String("x");
	writer.Double(this->scale.x);
	writer.String("y");
	writer.Double(this->scale.y);
	writer.String("z");
	writer.Double(this->scale.z);
	writer.EndObject();

	writer.EndObject();
}

glm::quat cTransformComponent::getQOrientation(void) { return this->orientation; };

void cTransformComponent::setQOrientation(glm::quat newOrientation)
{
	this->orientation = newOrientation;
}

void cTransformComponent::setOrientationEulerAngles(glm::vec3 newAnglesEuler, bool bIsDegrees) {
	
	if (bIsDegrees) {
		newAnglesEuler = glm::vec3(glm::radians(newAnglesEuler.x), glm::radians(newAnglesEuler.y), glm::radians(newAnglesEuler.z));
	}

	this->orientation = glm::quat(glm::vec3(newAnglesEuler.x, newAnglesEuler.y, newAnglesEuler.z));
	return;
}

void cTransformComponent::setOrientationEulerAngles(float x, float y, float z, bool bIsDegrees) {
	this->setOrientationEulerAngles(glm::vec3(x, y, z), bIsDegrees);
}

void cTransformComponent::adjOrientationEulerAngles(glm::vec3 adjAngleEuler, bool bIsDegrees) {
	
	if (bIsDegrees) {
		adjAngleEuler = glm::vec3(glm::radians(adjAngleEuler.x), glm::radians(adjAngleEuler.y), glm::radians(adjAngleEuler.z));
	}

	// Step 1: make a quaternion that represents the angle we want to rotate
	glm::quat rotationAdjust(adjAngleEuler);

	// Step 2: Multiply this quaternion by the existing quaternion. This "adds" the angle we want.
	this->orientation *= rotationAdjust;
}

void cTransformComponent::adjOrientationEulerAngles(float x, float y, float z, bool bIsDegrees) {
	this->adjOrientationEulerAngles(glm::vec3(x, y, z), bIsDegrees);
}

void cTransformComponent::adjMeshOrientationQ(glm::quat adjOrientQ) {
	this->orientation *= adjOrientQ;
}

void cTransformComponent::setUniformScale(float scale) {

	this->scale = glm::vec3(scale, scale, scale);

	return;
}
