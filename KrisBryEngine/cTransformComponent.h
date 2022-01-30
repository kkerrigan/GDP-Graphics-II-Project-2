#ifndef _cTransformComponent_HG_
#define _cTransformComponent_HG_

#include <glm/vec3.hpp>
#include <glm/gtc/constants.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "cComponent.h"

#include <rapidjson/prettywriter.h>

class cTransformComponent : public cComponent {
public:
	glm::vec3 position;
	glm::quat orientation;
	glm::vec3 scale;

	cTransformComponent() :cComponent(1), position(glm::vec3(0.0f)), orientation(glm::vec3(0.0f)), scale(glm::vec3(1.0f)) {}

	void serializeComponent(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);
	
	glm::quat getQOrientation(void);
	void setQOrientation(glm::quat newOrientation);
	void setOrientationEulerAngles(glm::vec3 newAnglesEuler, bool bIsDegrees = false);
	void setOrientationEulerAngles(float x, float y, float z, bool bIsDegrees = false);
	void adjOrientationEulerAngles(glm::vec3 adjAngleEuler, bool bIsDegrees = false);
	void adjOrientationEulerAngles(float x, float y, float z, bool bIsDegrees = false);
	void adjMeshOrientationQ(glm::quat adjOrientQ);

	void setUniformScale(float scale);
};

#endif