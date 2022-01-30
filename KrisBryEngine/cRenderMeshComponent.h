#ifndef _cRenderMeshComponent_HG_
#define _cRenderMeshComponent_HG_

#include <vector>
#include <string>

#include "Global.h"
#include "cComponent.h"
#include "sTextureInfo.h"

#include <rapidjson/prettywriter.h>

class cRenderMeshComponent : public cComponent {
public:
	int shaderId;
	
	std::string meshName;
	std::string friendlyName;
	
	bool bIsWireFrame;	
	bool bIsVisible;
	bool bUseVertexColour;
	bool bDontLight;
	bool colorByHeight;

	glm::vec4 materialDiffuse;
	void setDiffuseColour(glm::vec3 newDiffuseColour);
	void setAlphaTransparency(float newAlpha);

	glm::vec4 materialSpecular;
	void setSpecularColour(glm::vec3 newSpecularColour);
	void setSpecularPower(float specPower);

	std::vector<cRenderMeshComponent*> vec_pChildObjectsToDraw;
	std::vector<sTextureInfo> textures;

	cRenderMeshComponent();

	void serializeComponent(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);
};

#endif
