#ifndef _cEntity_HG_
#define _cEntity_HG_

#include <rapidjson/prettywriter.h>

#define ENTITY_MAX_COMPONENTS 64 // we have an int as our component bit field

class cEntity {	
public:
	const int UNIQUE_ID;
	const int entityType;
	int componentBitField;
	void* components[ENTITY_MAX_COMPONENTS];

	void serializeComponent(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);

	cEntity(int type);
	~cEntity();

private:
	static int nextUniqueId;

};

#endif
