#ifndef _cCollider_HG_
#define _cCollider_HG_

#include "cComponent.h"

class cCollider : public cComponent {
public:

	enum eShape
	{
		UNKOWN_SHAPE,
		SPHERE,
		TRIANGLE,
		AABB, 
		AOB	
	};
	
	void* pTheShape;		
	eShape shapeType;	

	cCollider() : cComponent(5) {}
	~cCollider();
};

#endif
