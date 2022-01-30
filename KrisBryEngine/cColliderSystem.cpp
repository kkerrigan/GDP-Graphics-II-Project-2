#include "cColliderSystem.h"

#include <algorithm>

#include "cTransformComponent.h"
#include "cEulerMovementComponent.h"
#include "cRenderMeshComponent.h"
#include "cCollider.h"
#include "cPhysics.h"
#include "cDebugRenderComponent.h"
#include "cVAOMeshManager.h"
#include "cDebugRenderer.h"

cColliderSystem* cColliderSystem::getInstance() {
	static cColliderSystem instance;

	return &instance;
}

void cColliderSystem::update(double deltaTime) {
	for (std::vector<cEntity*>::iterator itObj1 = this->entitiesToUpdate.begin(); itObj1 != this->entitiesToUpdate.end(); ++itObj1) {

		// test collisions against all other objects
		for (std::vector<cEntity*>::iterator itObj2 = this->entitiesToUpdate.begin(); itObj2 != this->entitiesToUpdate.end(); ++itObj2) {
			
			if (itObj1 == itObj2) { 
				continue; 
			}

			if (this->testForCollision(*itObj1, *itObj2, deltaTime)) {
				
				cEntity* entity1 = *itObj1;
				cEntity* entity2 = *itObj2;

				cTransformComponent* pObj1Transform = static_cast<cTransformComponent*>(entity1->components[1]);
				cEulerMovementComponent* pObjec1EulerMovement = static_cast<cEulerMovementComponent*>(entity1->components[2]);
				cCollider* pObj1Collider = static_cast<cCollider*>(entity1->components[5]);

				cTransformComponent* pObj2Transform = static_cast<cTransformComponent*>(entity2->components[1]);
				cEulerMovementComponent* pObjec2EulerMovement = static_cast<cEulerMovementComponent*>(entity2->components[2]);
				cCollider* pObj2Collider = static_cast<cCollider*>(entity2->components[5]);

				if ((pObj1Collider->shapeType == pObj1Collider->SPHERE) && (pObj2Collider->shapeType == pObj2Collider->SPHERE)) {
					
				}
				else if ((pObj1Collider->shapeType == pObj1Collider->SPHERE) && (pObj2Collider->shapeType == pObj2Collider->TRIANGLE)) {
					// If it's a Sphere-Triange, make the intersection lines magenta
				
				}
			}
		}
	}
}

void cColliderSystem::registerEntity(cEntity* entity) {
	this->entitiesToUpdate.push_back(entity);
}

void cColliderSystem::unregisterEntity(cEntity* entity) {
	// TODO: remove from vector
}

bool cColliderSystem::testForCollision(cEntity* entity1, cEntity* entity2, double deltaTime) {
	cTransformComponent* pObj1Transform = static_cast<cTransformComponent*>(entity1->components[1]);
	cEulerMovementComponent* pObjec1EulerMovement = static_cast<cEulerMovementComponent*>(entity1->components[2]);
	cCollider* pObj1Collider = static_cast<cCollider*>(entity1->components[5]);

	cTransformComponent* pObj2Transform = static_cast<cTransformComponent*>(entity2->components[1]);
	cEulerMovementComponent* pObjec2EulerMovement = static_cast<cEulerMovementComponent*>(entity2->components[2]);
	cCollider* pObj2Collider = static_cast<cCollider*>(entity2->components[5]);

	if (pObj1Collider->pTheShape == NULL) { return false; /*print error?*/ }
	if (pObj2Collider->pTheShape == NULL) { return false; /*print error?*/ }

	// Sphere - sphere
	if ((pObj1Collider->shapeType == pObj1Collider->SPHERE) && (pObj2Collider->shapeType == pObj2Collider->SPHERE)) {
		return cPhysics::areSpheresPenetrating(entity1, entity2);
	}
	else if ((pObj1Collider->shapeType == pObj1Collider->SPHERE) && (pObj2Collider->shapeType == pObj2Collider->TRIANGLE)) {
		return cPhysics::sphereTraingleTest(entity1, entity2);
	}

	else if ((pObj1Collider->shapeType == pObj1Collider->SPHERE) && (pObj2Collider->shapeType == pObj2Collider->AABB)) {
		
		std::vector<sClosestPointData> closestPoints;

		sModelDrawInfo cubeInfo;
		cubeInfo.meshFileName = "cube_flat_shaded_xyz_n.ply";
		cVAOMeshManager::getInstance()->FindDrawInfoByModelName(cubeInfo);

		cPhysics::calculateClosestPointsOnMesh(cubeInfo, pObj1Transform->position, closestPoints);

		int closestIndex = 0;
		float minDistance = glm::abs(glm::distance(pObj1Transform->position, closestPoints[0].thePoint));
		for (unsigned int i = 0; i < closestPoints.size(); ++i) {
			float dist = std::abs(glm::distance(pObj1Transform->position, closestPoints[i].thePoint));
			if ( dist < minDistance) {
				minDistance = dist;
				closestIndex = i;
			}
		}

		sTriangle tri;
		int point1 = cubeInfo.pTriangles[closestPoints[closestIndex].triangleIndex].vertex_index_1;
		int point2 = cubeInfo.pTriangles[closestPoints[closestIndex].triangleIndex].vertex_index_2;
		int point3 = cubeInfo.pTriangles[closestPoints[closestIndex].triangleIndex].vertex_index_3;
		
		tri.v[0] = glm::vec3(cubeInfo.pVerticesFromFile[point1].x, cubeInfo.pVerticesFromFile[point1].y, cubeInfo.pVerticesFromFile[point1].z);
		tri.v[1] = glm::vec3(cubeInfo.pVerticesFromFile[point2].x, cubeInfo.pVerticesFromFile[point2].y, cubeInfo.pVerticesFromFile[point2].z);
		tri.v[2] = glm::vec3(cubeInfo.pVerticesFromFile[point3].x, cubeInfo.pVerticesFromFile[point3].y, cubeInfo.pVerticesFromFile[point3].z);
		
		sSphere* pSphereA = (sSphere*)(pObj1Collider->pTheShape);
		sTriangle* pTri = &tri;

		glm::vec3 closestPointToTri = cPhysics::closestPtPointTriangle(pObj1Transform->position, pTri->v[0], pTri->v[1], pTri->v[2]);

		//// is this point LESS THAN the radius of the sphere? 
		float dist = glm::abs(glm::distance(closestPointToTri, pObj1Transform->position));
		if (dist <= pSphereA->radius + 0.1f) {
			//get the normal
			glm::vec3 norm1 = glm::vec3(cubeInfo.pVerticesToVBO[point1].nx, cubeInfo.pVerticesToVBO[point1].ny, cubeInfo.pVerticesToVBO[point1].nz);
			glm::vec3 norm2 = glm::vec3(cubeInfo.pVerticesToVBO[point2].nx, cubeInfo.pVerticesToVBO[point2].ny, cubeInfo.pVerticesToVBO[point2].nz);
			glm::vec3 norm3 = glm::vec3(cubeInfo.pVerticesToVBO[point3].nx, cubeInfo.pVerticesToVBO[point3].ny, cubeInfo.pVerticesToVBO[point3].nz);

			glm::vec3 normal = (norm1 + norm2 + norm3) / glm::vec3(3.0f,3.0f,3.0f);
		
			glm::vec3 reflected =-normal * (-normal * pObjec1EulerMovement->velocity * 0.9f);
			if (pObjec1EulerMovement->velocity.y < 0.01f) {
				pObjec1EulerMovement->velocity.y = 0.0f;
			}

			pObjec1EulerMovement->velocity -= reflected;
			
			return true;
		}

		return false;
	}

	return false;
}