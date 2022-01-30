#include <physicsShapes.h>
#include <cVAOMeshManager.h>

cAABBTerrianBroadPhaseHierarchy g_AABBTerrianHierarchy;
#define AABB_HEIRARCHY_HALF_LENGTH 5.0f


void loadTerrainIntoTerrainAABBHierarchy(std::string meshName) {
	sModelDrawInfo terrainMeshInfo;
	
	terrainMeshInfo.meshFileName = meshName;

	cVAOMeshManager::getInstance()->FindDrawInfoByModelName(terrainMeshInfo);
	// How big is our AABBs? Side length?
	//float sideLength = AABB_HEIRARCHY_HALF_LENGTH * 2;		

	for (unsigned int triIndex = 0; triIndex != terrainMeshInfo.numberOfTriangles; triIndex++)
	{
		// for each triangle, for each vertex, determine which AABB the triangle should be in
		// (if your mesh has been transformed, then you need to transform the tirangles 
		//  BEFORE you do this... or just keep the terrain UNTRANSFORMED)
		sPlyTriangle currentTri = terrainMeshInfo.pTriangles[triIndex];
		sPlyVertex currentVerts[3];
		currentVerts[0] = terrainMeshInfo.pVerticesFromFile[currentTri.vertex_index_1];
		currentVerts[1] = terrainMeshInfo.pVerticesFromFile[currentTri.vertex_index_2];
		currentVerts[2] = terrainMeshInfo.pVerticesFromFile[currentTri.vertex_index_3];
		
		// This is the structure we are eventually going to store in the AABB map...
		sAABB_Triangle curAABBTri;

		//verts
		curAABBTri.verts[0].x = currentVerts[0].x;
		curAABBTri.verts[0].y = currentVerts[0].y;
		curAABBTri.verts[0].z = currentVerts[0].z;
		curAABBTri.verts[1].x = currentVerts[1].x;
		curAABBTri.verts[1].y = currentVerts[1].y;
		curAABBTri.verts[1].z = currentVerts[1].z;
		curAABBTri.verts[2].x = currentVerts[2].x;
		curAABBTri.verts[2].y = currentVerts[2].y;
		curAABBTri.verts[2].z = currentVerts[2].z;

		//normals
		curAABBTri.norms[0].x = currentVerts[0].nx;
		curAABBTri.norms[0].y = currentVerts[0].ny;
		curAABBTri.norms[0].z = currentVerts[0].nz;
		curAABBTri.norms[1].x = currentVerts[1].nx;
		curAABBTri.norms[1].y = currentVerts[1].ny;
		curAABBTri.norms[1].z = currentVerts[1].nz;
		curAABBTri.norms[2].x = currentVerts[2].nx;
		curAABBTri.norms[2].y = currentVerts[2].ny;
		curAABBTri.norms[2].z = currentVerts[2].nz;

		// Is the triangle "too big", and if so, split it (take centre and make 3 more)
		// (Pro Tip: "too big" is the SMALLEST side is greater than HALF the AABB length)
		// Use THOSE triangles as the test (and recursively do this if needed),
		// +++BUT+++ store the ORIGINAL triangle info NOT the subdivided one
		// 
		// For the student to complete... 
		// 
		for (unsigned int vertIndex = 0; vertIndex != 3; vertIndex++)
		{
			// What AABB is "this" vertex in? 
			unsigned long long AABB_ID = cAABBTerrianBroadPhase::generateIdFromPosition(curAABBTri.verts[0], AABB_HEIRARCHY_HALF_LENGTH);
			
			// Do we have this AABB alredy? 
			std::map< unsigned long long/*ID AABB*/, cAABBTerrianBroadPhase* >::iterator itAABB = g_AABBTerrianHierarchy.m_mapAABBs.find(AABB_ID);
			if (itAABB == g_AABBTerrianHierarchy.m_mapAABBs.end())
			{
				// We DON'T have an AABB, yet
				
				// Determine the AABB location for this point
				// (like the generateID() method...)
				// get the center 

				glm::vec3 center = cAABBTerrianBroadPhase::getCenterFromPosition(curAABBTri.verts[0], AABB_HEIRARCHY_HALF_LENGTH);
			
				cAABBTerrianBroadPhase* pAABB = new cAABBTerrianBroadPhase(center, AABB_HEIRARCHY_HALF_LENGTH);
				g_AABBTerrianHierarchy.m_mapAABBs[AABB_ID] = pAABB;

				// Then set the iterator to the AABB, by running find again
				itAABB = g_AABBTerrianHierarchy.m_mapAABBs.find(AABB_ID);

			}

			itAABB->second->triangles.push_back(curAABBTri);

		}
	}
	
	return;
}


void loadTerrainAABBHierarchy() {

	/*cAABBTerrainBroadPhase* aabb1 = new cAABBTerrainBroadPhase(glm::vec3(0.0f, AABB_HEIRARCHY_HALF_LENGTH, 0.0f), AABB_HEIRARCHY_HALF_LENGTH);
	g_AABBTerrainHierarchy.m_mapAABBs[aabb1->getId()] = aabb1;

	cAABBTerrainBroadPhase* aabb2 = new cAABBTerrainBroadPhase(glm::vec3(3.0f, AABB_HEIRARCHY_HALF_LENGTH, 0.0f), AABB_HEIRARCHY_HALF_LENGTH);

	g_AABBTerrainHierarchy.m_mapAABBs[aabb2->getId()] = aabb2;
*/
	
	//int a = cAABBTerrainBroadPhase::generateIdFromPosition(glm::vec3(0.0f), AABB_HEIRARCHY_HALF_LENGTH);
	//int b = cAABBTerrainBroadPhase::generateIdFromPosition(glm::vec3(1.0f, 0.0f, 0.0f), AABB_HEIRARCHY_HALF_LENGTH);
	//int c = cAABBTerrainBroadPhase::generateIdFromPosition(glm::vec3(-1.0f, 0.0f, 0.0f), AABB_HEIRARCHY_HALF_LENGTH);

	//int d = cAABBTerrainBroadPhase::generateIdFromPosition(glm::vec3(6.0f, 0.0f, 0.0f), AABB_HEIRARCHY_HALF_LENGTH);
	//int e = cAABBTerrainBroadPhase::generateIdFromPosition(glm::vec3(7.0f, 0.0f, 0.0f), AABB_HEIRARCHY_HALF_LENGTH);
	//int f = cAABBTerrainBroadPhase::generateIdFromPosition(glm::vec3(11.0f, 0.0f, 0.0f), AABB_HEIRARCHY_HALF_LENGTH);

	loadTerrainIntoTerrainAABBHierarchy("Peach_Castle_xyz_n_uv.ply");
}