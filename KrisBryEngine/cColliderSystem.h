#ifndef _cColliderSystem_HG_
#define _cColliderSystem_HG_

#include <vector>

#include "iSystem.h"

class cColliderSystem : public iSystem {
public:

	static cColliderSystem* getInstance();
	cColliderSystem(cColliderSystem const&) = delete;
	void operator=(cColliderSystem const&) = delete;

	void update(double deltaTime);
	void registerEntity(cEntity* entity);
	void unregisterEntity(cEntity* entity);

private:
	cColliderSystem() {}
	~cColliderSystem() {}

	std::vector<cEntity*> entitiesToUpdate;

	bool testForCollision(cEntity* entity1, cEntity* entity2, double deltaTime);
};

#endif
