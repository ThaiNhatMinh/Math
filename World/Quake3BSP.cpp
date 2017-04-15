#include "..\iostream.h"



CQuake3BSP::CQuake3BSP()
{
	// Here we simply initialize our member variables to 0
	m_numOfVerts = 0;
	m_numOfFaces = 0;
	m_numOfIndices = 0;
	m_numOfTextures = 0;
	m_numOfLightmaps = 0;


	/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

	m_numOfNodes = 0;
	m_numOfLeafs = 0;
	m_numOfLeafFaces = 0;
	m_numOfPlanes = 0;

	/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

	// Initialize all the dynamic BSP data pointers to NULL
	m_pVerts = NULL;
	m_pFaces = NULL;
	m_pIndices = NULL;


	/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

	m_pNodes = NULL;
	m_pLeafs = NULL;
	m_pPlanes = NULL;
	m_pLeafFaces = NULL;
	m_pBrushes = NULL;
	m_pBrushSides = NULL;
	m_pTextures = NULL;
	m_pLeafBrushes = NULL;

	memset(&m_clusters, 0, sizeof(tBSPVisData));

	// We need to initialize our Min and Max and Extent variables
	m_vTraceMins = Vec3(0, 0, 0);
	m_vTraceMaxs = Vec3(0, 0, 0);
	m_vExtents = Vec3(0, 0, 0);

	// This will store the normal of the plane we collided with
	m_vCollisionNormal = Vec3(0, 0, 0);

}


void CQuake3BSP::FindTextureExtension(char *strFileName)
{
	char strJPGPath[MAX_PATH] = { 0 };
	char strTGAPath[MAX_PATH] = { 0 };
	FILE *fp = NULL;

	

	// Get the current path we are in
	GetCurrentDirectory(MAX_PATH, strJPGPath);

	// Add on a '\' and the file name to the end of the current path.
	// We create 2 seperate strings to test each image extension.
	strcat(strJPGPath, "\\");
	strcat(strJPGPath, strFileName);
	strcpy(strTGAPath, strJPGPath);

	// Add the extensions on to the file name and path
	strcat(strJPGPath, ".jpg");
	strcat(strTGAPath, ".tga");

	// Check if there is a jpeg file with the texture name
	if ((fp = fopen(strJPGPath, "rb")) != NULL)
	{
		// If so, then let's add ".jpg" onto the file name and return
		strcat(strFileName, ".jpg");
		return;
	}

	// Check if there is a targa file with the texture name
	if ((fp = fopen(strTGAPath, "rb")) != NULL)
	{
		// If so, then let's add a ".tga" onto the file name and return
		strcat(strFileName, ".tga");
		return;
	}

}

Vec3 CQuake3BSP::Trace(Vec3 vStart, Vec3 vEnd)
{
	// Initially we set our trace ratio to 1.0f, which means that we don't have
	// a collision or intersection point, so we can move freely.
	m_traceRatio = 1.0f;

	// We start out with the first node (0), setting our start and end ratio to 0 and 1.
	// We will recursively go through all of the nodes to see which brushes we should check.
	CheckNode(0, 0.0f, 1.0f, vStart, vEnd);

	// If the traceRatio is STILL 1.0f, then we never collided and just return our end position
	if (m_traceRatio == 1.0f)
	{
		return vEnd;
	}
	else	// Else COLLISION!!!!
	{
		// If we get here then it's assumed that we collided and need to move the position
		// the correct distance from the starting position a position around the intersection
		// point.  This is done by the cool equation below (described in detail at top of page).

		// Set our new position to a position that is right up to the brush we collided with
		Vec3 vNewPosition = vStart + ((vEnd - vStart) * m_traceRatio);


		// * SLIDING CODE *
		// This code is what calculates our sliding along the brush planes.
		// Like we explained in detail in Main.cpp, we get the vector of the
		// distance from the end point to the new position, then use the dot
		// product with that vector and the plane's normal to obtain the distance
		// along the normal of the plane to the new sliding position.  Finally,
		// we subtract that distance multiplied by the normal of the plane from
		// the end point, which gives us our new sliding position.

		// Get the distance from the end point to the new position we just got
		Vec3 vMove = vEnd - vNewPosition;

		// Get the distance we need to travel backwards to the new slide position.
		// This is the distance of course along the normal of the plane we collided with.
		float distance = Math::Dot(vMove, m_vCollisionNormal);

		// Get the new end position that we will end up (the slide position).
		// This is obtained by move along the plane's normal from the end point 
		// by the distance we just calculated.
		Vec3 vEndPosition = vEnd - m_vCollisionNormal*distance;

		// Since we got a new position for our sliding vector, we need to check
		// to make sure that new sliding position doesn't collide with anything else.
		vNewPosition = Trace(vNewPosition, vEndPosition);


		// Return the new position to be used by our camera (or player)
		return vNewPosition;
	}
}

void CQuake3BSP::CheckNode(int nodeIndex, float startRatio, float endRatio, Vec3 vStart, Vec3 vEnd)
{
	// Remember, the nodeIndices are stored as negative numbers when we get to a leaf, so we 
	// check if the current node is a leaf, which holds brushes.  If the nodeIndex is negative,
	// the next index is a leaf (note the: nodeIndex + 1)
	if (nodeIndex < 0)
	{
		// If this node in the BSP is a leaf, we need to negate and add 1 to offset
		// the real node index into the m_pLeafs[] array.  You could also do [~nodeIndex].
		tBSPLeaf *pLeaf = &m_pLeafs[-(nodeIndex + 1)];

		// We have a leaf, so let's go through all of the brushes for that leaf
		for (int i = 0; i < pLeaf->numOfLeafBrushes; i++)
		{
			// Get the current brush that we going to check
			tBSPBrush *pBrush = &m_pBrushes[m_pLeafBrushes[pLeaf->leafBrush + i]];

			// This is kind of an important line.  First, we check if there is actually
			// and brush sides (which store indices to the normal and plane data for the brush).
			// If not, then go to the next one.  Otherwise, we also check to see if the brush
			// is something that we want to collide with.  For instance, there are brushes for
			// water, lava, bushes, misc. sprites, etc...  We don't want to collide with water
			// and other things like bushes, so we check the texture type to see if it's a solid.
			// If the textureType can be binary-anded (&) and still be 1, then it's solid,
			// otherwise it's something that can be walked through.  That's how Quake chose to
			// do it.

			// Check if we have brush sides and the current brush is solid and collidable
			if ((pBrush->numOfBrushSides > 0) && (m_pTextures[pBrush->textureID].textureType & 1))
			{
				// Now we delve into the dark depths of the real calculations for collision.
				// We can now check the movement vector against our brush planes.
				CheckBrush(pBrush, vStart, vEnd);
			}
		}

		// Since we found the brushes, we can go back up and stop recursing at this level
		return;
	}

	// If we haven't found a leaf in the node, then we need to keep doing some dirty work
	// until we find the leafs which store the brush information for collision detection.

	// Grad the next node to work with and grab this node's plane data
	tBSPNode *pNode = &m_pNodes[nodeIndex];
	tBSPPlane *pPlane = &m_pPlanes[pNode->plane];

	// Now we do some quick tests to see which side we fall on of the node in the BSP

	// Here we use the plane equation to find out where our initial start position is
	// according the the node that we are checking.  We then grab the same info for the end pos.
	float startDistance = Math::Dot(vStart, pPlane->vNormal) - pPlane->d;
	float endDistance = Math::Dot(vEnd, pPlane->vNormal) - pPlane->d;
	float offset = 0.0f;

	// If we are doing any type of collision detection besides a ray, we need to change
	// the offset for which we are testing collision against the brushes.  If we are testing
	// a sphere against the brushes, we need to add the sphere's offset when we do the plane
	// equation for testing our movement vector (start and end position).  * More Info * For
	// more info on sphere collision, check out our tutorials on this subject.

	// If we are doing sphere collision, include an offset for our collision tests below
	if (m_traceType == TYPE_SPHERE)
		offset = m_traceRadius;

	// Here we check to see if we are working with a BOX or not
	else if (m_traceType == TYPE_BOX)
	{
		// This equation does a dot product to see how far our
		// AABB is away from the current plane we are checking.
		// Since this is a distance, we need to make it an absolute
		// value, which calls for the fabs() function (abs() for floats).

		// Get the distance our AABB is from the current splitter plane
		offset = (float)(fabs(m_vExtents.x * pPlane->vNormal.x) +
			fabs(m_vExtents.y * pPlane->vNormal.y) +
			fabs(m_vExtents.z * pPlane->vNormal.z));
	}

	// Below we just do a basic traversal down the BSP tree.  If the points are in
	// front of the current splitter plane, then only check the nodes in front of
	// that splitter plane.  Otherwise, if both are behind, check the nodes that are
	// behind the current splitter plane.  The next case is that the movement vector
	// is on both sides of the splitter plane, which makes it a bit more tricky because we now
	// need to check both the front and the back and split up the movement vector for both sides.

	// Here we check to see if the start and end point are both in front of the current node.
	// If so, we want to check all of the nodes in front of this current splitter plane.
	if (startDistance >= offset && endDistance >= offset)
	{
		// Traverse the BSP tree on all the nodes in front of this current splitter plane
		CheckNode(pNode->front, startDistance, endDistance, vStart, vEnd);
	}
	// If both points are behind the current splitter plane, traverse down the back nodes
	else if (startDistance < -offset && endDistance < -offset)
	{
		// Traverse the BSP tree on all the nodes in back of this current splitter plane
		CheckNode(pNode->back, startDistance, endDistance, vStart, vEnd);
	}
	else
	{
		// If we get here, then our ray needs to be split in half to check the nodes
		// on both sides of the current splitter plane.  Thus we create 2 ratios.
		float Ratio1 = 1.0f, Ratio2 = 0.0f, middleRatio = 0.0f;
		Vec3 vMiddle;	// This stores the middle point for our split ray

							// Start of the side as the front side to check
		int side = pNode->front;

		// Here we check to see if the start point is in back of the plane (negative)
		if (startDistance < endDistance)
		{
			// Since the start position is in back, let's check the back nodes
			side = pNode->back;

			// Here we create 2 ratios that hold a distance from the start to the
			// extent closest to the start (take into account a sphere and epsilon).
			// We use epsilon like Quake does to compensate for float errors.  The second
			// ratio holds a distance from the other size of the extents on the other side
			// of the plane.  This essential splits the ray for both sides of the splitter plane.
			float inverseDistance = 1.0f / (startDistance - endDistance);
			Ratio1 = (startDistance - offset - kEpsilon) * inverseDistance;
			Ratio2 = (startDistance + offset + kEpsilon) * inverseDistance;
		}
		// Check if the starting point is greater than the end point (positive)
		else if (startDistance > endDistance)
		{
			// This means that we are going to recurse down the front nodes first.
			// We do the same thing as above and get 2 ratios for split ray.
			// Ratio 1 and 2 are switched in contrast to the last if statement.
			// This is because the start is starting in the front of the splitter plane.
			float inverseDistance = 1.0f / (startDistance - endDistance);
			Ratio1 = (startDistance + offset + kEpsilon) * inverseDistance;
			Ratio2 = (startDistance - offset - kEpsilon) * inverseDistance;
		}

		// Make sure that we have valid numbers and not some weird float problems.
		// This ensures that we have a value from 0 to 1 as a good ratio should be :)
		if (Ratio1 < 0.0f) Ratio1 = 0.0f;
		else if (Ratio1 > 1.0f) Ratio1 = 1.0f;

		if (Ratio2 < 0.0f) Ratio2 = 0.0f;
		else if (Ratio2 > 1.0f) Ratio2 = 1.0f;

		// Just like we do in the Trace() function, we find the desired middle
		// point on the ray, but instead of a point we get a middleRatio percentage.
		// This isn't the true middle point since we are using offset's and the epsilon value.
		// We also grab the middle point to go with the ratio.
		middleRatio = startRatio + ((endRatio - startRatio) * Ratio1);
		vMiddle = vStart + ((vEnd - vStart) * Ratio1);

		// Now we recurse on the current side with only the first half of the ray
		CheckNode(side, startRatio, middleRatio, vStart, vMiddle);

		// Now we need to make a middle point and ratio for the other side of the node
		middleRatio = startRatio + ((endRatio - startRatio) * Ratio2);
		vMiddle = vStart + ((vEnd - vStart) * Ratio2);

		// Depending on which side should go last, traverse the bsp with the
		// other side of the split ray (movement vector).
		if (side == pNode->back)
			CheckNode(pNode->front, middleRatio, endRatio, vMiddle, vEnd);
		else
			CheckNode(pNode->back, middleRatio, endRatio, vMiddle, vEnd);
	}
}

void CQuake3BSP::CheckBrush(tBSPBrush * pBrush, Vec3 vStart, Vec3 vEnd)
{
	float startRatio = -1.0f;		// Like in BrushCollision.htm, start a ratio at -1
	float endRatio = 1.0f;			// Set the end ratio to 1
	bool startsOut = false;			// This tells us if we starting outside the brush

									// This function actually does the collision detection between our movement
									// vector and the brushes in the world data.  We will go through all of the
									// brush sides and check our start and end ratio against the planes to see if
									// they pass each other.  We start the startRatio at -1 and the endRatio at
									// 1, but as we set the ratios to their intersection points (ratios), then
									// they slowly move toward each other.  If they pass each other, then there
									// is definitely not a collision.

									// Go through all of the brush sides and check collision against each plane
	for (int i = 0; i < pBrush->numOfBrushSides; i++)
	{
		// Here we grab the current brush side and plane in this brush
		tBSPBrushSide *pBrushSide = &m_pBrushSides[pBrush->brushSide + i];
		tBSPPlane *pPlane = &m_pPlanes[pBrushSide->plane];

		// Let's store a variable for the offset (like for sphere collision)
		float offset = 0.0f;

		// If we are testing sphere collision we need to add the sphere radius
		if (m_traceType == TYPE_SPHERE)
			offset = m_traceRadius;

		// Test the start and end points against the current plane of the brush side.
		// Notice that we add an offset to the distance from the origin, which makes
		// our sphere collision work.
		float startDistance = Math::Dot(vStart, pPlane->vNormal) - (pPlane->d + offset);
		float endDistance = Math::Dot(vEnd, pPlane->vNormal) - (pPlane->d + offset);

		// This is the last beefy part of code in this tutorial.  In this
		// section we need to do a few special checks to see which extents
		// we should use.  We do this by checking the x,y,z of the normal.
		// If the vNormal.x is less than zero, we want to use the Max.x
		// value, otherwise use the Min.x value.  We do these checks because
		// we want the corner of the bounding box that is closest to the plane to
		// test for collision.  Write it down on paper and see how this works.
		// We want to grab the closest corner (x, y, or z value that is...) so we
		// dont go through the wall.  This works because the bounding box is axis aligned.

		// Store the offset that we will check against the plane
		Vec3 vOffset = Vec3(0, 0, 0);

		// If we are using AABB collision
		if (m_traceType == TYPE_BOX)
		{
			// Grab the closest corner (x, y, or z value) that is closest to the plane
			vOffset.x = (pPlane->vNormal.x < 0) ? m_vTraceMaxs.x : m_vTraceMins.x;
			vOffset.y = (pPlane->vNormal.y < 0) ? m_vTraceMaxs.y : m_vTraceMins.y;
			vOffset.z = (pPlane->vNormal.z < 0) ? m_vTraceMaxs.z : m_vTraceMins.z;

			// Use the plane equation to grab the distance our start position is from the plane.
			// We need to add the offset to this to see if the box collides with the plane,
			// even if the position doesn't.
			startDistance = Math::Dot(vStart + vOffset, pPlane->vNormal) - pPlane->d;

			// Get the distance our end position is from this current brush plane
			endDistance = Math::Dot(vEnd + vOffset, pPlane->vNormal) - pPlane->d;
		}

		// Make sure we start outside of the brush's volume
		if (startDistance > 0)	startsOut = true;

		// Stop checking since both the start and end position are in front of the plane
		if (startDistance > 0 && endDistance > 0)
			return;

		// Continue on to the next brush side if both points are behind or on the plane
		if (startDistance <= 0 && endDistance <= 0)
			continue;

		// If the distance of the start point is greater than the end point, we have a collision!
		if (startDistance > endDistance)
		{
			// This gets a ratio from our starting point to the approximate collision spot
			float Ratio1 = (startDistance - kEpsilon) / (startDistance - endDistance);

			// If this is the first time coming here, then this will always be true,
			// since startRatio starts at -1.0f.  We want to find the closest collision,
			// so we still continue to check all of the brushes before quitting.
			if (Ratio1 > startRatio)
			{
				// Set the startRatio (currently the closest collision distance from start)
				startRatio = Ratio1;
				m_bCollided = true;		// Let us know we collided!
				m_vCollisionNormal = pPlane->vNormal;
			}
		}
		else
		{
			// Get the ratio of the current brush side for the endRatio
			float Ratio = (startDistance + kEpsilon) / (startDistance - endDistance);

			// If the ratio is less than the current endRatio, assign a new endRatio.
			// This will usually always be true when starting out.
			if (Ratio < endRatio)
				endRatio = Ratio;
		}
	}

	// If we didn't start outside of the brush we don't want to count this collision - return;
	if (startsOut == false)
	{
		return;
	}

	// If our startRatio is less than the endRatio there was a collision!!!
	if (startRatio < endRatio)
	{
		// Make sure the startRatio moved from the start and check if the collision
		// ratio we just got is less than the current ratio stored in m_traceRatio.
		// We want the closest collision to our original starting position.
		if (startRatio > -1 && startRatio < m_traceRatio)
		{
			// If the startRatio is less than 0, just set it to 0
			if (startRatio < 0)
				startRatio = 0;

			// Store the new ratio in our member variable for later
			m_traceRatio = startRatio;
		}
	}
}


bool CQuake3BSP::LoadBSP(const char *strFileName)
{
	FILE *fp = NULL;
	int i = 0;

	// Check if the .bsp file could be opened
	if ((fp = fopen(strFileName, "rb")) == NULL)
	{
		// Display an error message and quit if the file can't be found.
		Log::Message(Log::LOG_ERROR, "Can't open file: " + string(strFileName));
		return false;
	}


	// Initialize the header and lump structures
	tBSPHeader header = { 0 };
	tBSPLump lumps[kMaxLumps] = { 0 };

	// Read in the header and lump data
	fread(&header, 1, sizeof(tBSPHeader), fp);
	fread(&lumps, kMaxLumps, sizeof(tBSPLump), fp);

	// Now we know all the information about our file.  We can
	// then allocate the needed memory for our member variables.

	// Allocate the vertex memory
	m_numOfVerts = lumps[kVertices].length / sizeof(tBSPVertex);
	m_pVerts = new tBSPVertex[m_numOfVerts];

	// Allocate the face memory
	m_numOfFaces = lumps[kFaces].length / sizeof(tBSPFace);
	m_pFaces = new tBSPFace[m_numOfFaces];

	// Allocate the index memory
	m_numOfIndices = lumps[kIndices].length / sizeof(int);
	m_pIndices = new int[m_numOfIndices];

	// Allocate memory to read in the texture information.
	m_numOfTextures = lumps[kTextures].length / sizeof(tBSPTexture);
	m_pTextures = new tBSPTexture[m_numOfTextures];

	// Allocate memory to read in the lightmap data.  
	m_numOfLightmaps = lumps[kLightmaps].length / sizeof(tBSPLightmap);
	tBSPLightmap *pLightmaps = new tBSPLightmap[m_numOfLightmaps];

	// Seek to the position in the file that stores the vertex information
	fseek(fp, lumps[kVertices].offset, SEEK_SET);

	// Go through all of the vertices that need to be read and swap axises
	for (i = 0; i < m_numOfVerts; i++)
	{
		// Read in the current vertex
		fread(&m_pVerts[i], 1, sizeof(tBSPVertex), fp);

		// Swap the y and z values, and negate the new z so Y is up.
		float temp = m_pVerts[i].vPosition.y;
		m_pVerts[i].vPosition.y = m_pVerts[i].vPosition.z;
		m_pVerts[i].vPosition.z = -temp;
	}

	// Seek to the position in the file that stores the index information
	fseek(fp, lumps[kIndices].offset, SEEK_SET);

	// Read in all the index information
	fread(m_pIndices, m_numOfIndices, sizeof(int), fp);

	// Seek to the position in the file that stores the face information
	fseek(fp, lumps[kFaces].offset, SEEK_SET);

	// Read in all the face information
	fread(m_pFaces, m_numOfFaces, sizeof(tBSPFace), fp);

	// Seek to the position in the file that stores the texture information
	fseek(fp, lumps[kTextures].offset, SEEK_SET);

	// Read in all the texture information
	fread(m_pTextures, m_numOfTextures, sizeof(tBSPTexture), fp);

	// Go through all of the textures
	for (i = 0; i < m_numOfTextures; i++)
	{
		// Find the extension if any and append it to the file name
		FindTextureExtension(m_pTextures[i].strName);

		// Create a texture from the image
		m_textures[i] = Resources::LoadTexture(m_pTextures[i].strName);
	}

	// Seek to the position in the file that stores the lightmap information
	fseek(fp, lumps[kLightmaps].offset, SEEK_SET);

	// Go through all of the lightmaps and read them in
	for (i = 0; i < m_numOfLightmaps; i++)
	{
		// Read in the RGB data for each lightmap
		fread(&pLightmaps[i], 1, sizeof(tBSPLightmap), fp);

		// Create a texture map for each lightmap that is read in.  The lightmaps
		// are always 128 by 128.
		char filename[64];
		sprintf(filename, "lightmap_%i", i);
		m_lightMap[i] = Resources::LoadTexMemory(filename,(unsigned char *)pLightmaps[i].imageBits, 128, 128);
	}

	// Delete the image bits because we are already done with them
	delete[] pLightmaps;


	/////// * /////////// * /////////// * NEW * /////// * /////////// * /////////// *

	// In this function we read from a bunch of new lumps.  These include
	// the BSP nodes, the leafs, the leaf faces, BSP splitter planes and
	// visibility data (clusters).

	// Store the number of nodes and allocate the memory to hold them
	m_numOfNodes = lumps[kNodes].length / sizeof(tBSPNode);
	m_pNodes = new tBSPNode[m_numOfNodes];

	// Seek to the position in the file that hold the nodes and store them in m_pNodes
	fseek(fp, lumps[kNodes].offset, SEEK_SET);
	fread(m_pNodes, m_numOfNodes, sizeof(tBSPNode), fp);

	// Store the number of leafs and allocate the memory to hold them
	m_numOfLeafs = lumps[kLeafs].length / sizeof(tBSPLeaf);
	m_pLeafs = new tBSPLeaf[m_numOfLeafs];

	// Seek to the position in the file that holds the leafs and store them in m_pLeafs
	fseek(fp, lumps[kLeafs].offset, SEEK_SET);
	fread(m_pLeafs, m_numOfLeafs, sizeof(tBSPLeaf), fp);

	for (i = 0; i < m_numOfLeafs; i++)
	{
	

		int temp = m_pLeafs[i].min.y;
		m_pLeafs[i].min.y = m_pLeafs[i].min.z;
		m_pLeafs[i].min.z = -temp;
		temp = m_pLeafs[i].max.y;
		m_pLeafs[i].max.y = m_pLeafs[i].max.z;
		m_pLeafs[i].max.z = -temp;
	}
	// Store the number of leaf faces and allocate the memory for them
	m_numOfLeafFaces = lumps[kLeafFaces].length / sizeof(int);
	m_pLeafFaces = new int[m_numOfLeafFaces];

	// Seek to the leaf faces lump, then read it's data
	fseek(fp, lumps[kLeafFaces].offset, SEEK_SET);
	fread(m_pLeafFaces, m_numOfLeafFaces, sizeof(int), fp);

	// Store the number of planes, then allocate memory to hold them
	m_numOfPlanes = lumps[kPlanes].length / sizeof(tBSPPlane);
	m_pPlanes = new tBSPPlane[m_numOfPlanes];

	// Seek to the planes lump in the file, then read them into m_pPlanes
	fseek(fp, lumps[kPlanes].offset, SEEK_SET);
	fread(m_pPlanes, m_numOfPlanes, sizeof(tBSPPlane), fp);

	// Go through every plane and convert it's normal to the Y-axis being up
	for (i = 0; i < m_numOfPlanes; i++)
	{
		float temp = m_pPlanes[i].vNormal.y;
		m_pPlanes[i].vNormal.y = m_pPlanes[i].vNormal.z;
		m_pPlanes[i].vNormal.z = -temp;
	}

	// Seek to the position in the file that holds the visibility lump
	fseek(fp, lumps[kVisData].offset, SEEK_SET);

	// Check if there is any visibility information first
	if (lumps[kVisData].length)
	{
		// Read in the number of vectors and each vector's size
		fread(&(m_clusters.numOfClusters), 1, sizeof(int), fp);
		fread(&(m_clusters.bytesPerCluster), 1, sizeof(int), fp);

		// Allocate the memory for the cluster bitsets
		int size = m_clusters.numOfClusters * m_clusters.bytesPerCluster;
		m_clusters.pBitsets = new byte[size];

		// Read in the all the visibility bitsets for each cluster
		fread(m_clusters.pBitsets, 1, sizeof(byte) * size, fp);
	}
	// Otherwise, we don't have any visibility data (prevents a crash)
	else
		m_clusters.pBitsets = NULL;

	// Like we do for other data, we read get the size of brushes and allocate memory
	m_numOfBrushes = lumps[kBrushes].length / sizeof(int);
	m_pBrushes = new tBSPBrush[m_numOfBrushes];

	// Here we read in the brush information from the BSP file
	fseek(fp, lumps[kBrushes].offset, SEEK_SET);
	fread(m_pBrushes, m_numOfBrushes, sizeof(tBSPBrush), fp);

	// Get the size of brush sides, then allocate memory for it
	m_numOfBrushSides = lumps[kBrushSides].length / sizeof(int);
	m_pBrushSides = new tBSPBrushSide[m_numOfBrushSides];

	// Read in the brush sides data
	fseek(fp, lumps[kBrushSides].offset, SEEK_SET);
	fread(m_pBrushSides, m_numOfBrushSides, sizeof(tBSPBrushSide), fp);

	// Read in the number of leaf brushes and allocate memory for it
	m_numOfLeafBrushes = lumps[kLeafBrushes].length / sizeof(int);
	m_pLeafBrushes = new int[m_numOfLeafBrushes];

	// Finally, read in the leaf brushes for traversing the bsp tree with brushes
	fseek(fp, lumps[kLeafBrushes].offset, SEEK_SET);
	fread(m_pLeafBrushes, m_numOfLeafBrushes, sizeof(int), fp);

	// Close the file
	fclose(fp);

	// Here we allocate enough bits to store all the faces for our bitset
	m_FacesDrawn.Resize(m_numOfFaces);

	// Return a success
	return true;
}


void CQuake3BSP::Destroy()
{
	// If we still have valid memory for our vertices, free them
	if (m_pVerts)
	{
		delete[] m_pVerts;
		m_pVerts = NULL;
	}

	// If we still have valid memory for our faces, free them
	if (m_pFaces)
	{
		delete[] m_pFaces;
		m_pFaces = NULL;
	}

	// If we still have valid memory for our indices, free them
	if (m_pIndices)
	{
		delete[] m_pIndices;
		m_pIndices = NULL;
	}

	// If we still have valid memory for our nodes, free them
	if (m_pNodes)
	{
		delete[] m_pNodes;		m_pNodes = NULL;
	}

	// If we still have valid memory for our leafs, free them
	if (m_pLeafs)
	{
		delete[] m_pLeafs;		m_pLeafs = NULL;
	}

	// If we still have valid memory for our leaf faces, free them
	if (m_pLeafFaces)
	{
		delete[] m_pLeafFaces;	m_pLeafFaces = NULL;
	}

	// If we still have valid memory for our planes, free them
	if (m_pPlanes)
	{
		delete[] m_pPlanes;	m_pPlanes = NULL;
	}

	// If we still have valid memory for our clusters, free them
	if (m_clusters.pBitsets)
	{
		delete[] m_clusters.pBitsets;		m_clusters.pBitsets = NULL;
	}

}

int CQuake3BSP::FindLeaf(const Vec3 & pos)
{
	int i = 0;
	float distance = 0.0f;
	
	while (i >= 0)
	{
		const tBSPNode& node = m_pNodes[i];
		const tBSPPlane& plane = m_pPlanes[node.plane];
		distance = plane.vNormal.x*pos.x + plane.vNormal.y*pos.y + plane.vNormal.z*pos.z - plane.d;

		if (distance > 0.0f) i = node.front;// in front of plane 
		else i = node.back;
	}

	// Return the leaf index (same thing as saying:  return -(i + 1)).
	return ~i;
}

int CQuake3BSP::IsClusterVisible(int current, int test)
{
	if (!m_clusters.pBitsets || current < 0) return 1;
	
	byte visSet = m_clusters.pBitsets[(current * m_clusters.bytesPerCluster) + (test / 8)];
	int result = visSet & (1 << (test & 7));

	return result;
}

Vec3 CQuake3BSP::TraceRay(Vec3 vStart, Vec3 vEnd)
{
	// We don't use this function, but we set it up to allow us to just check a
	// ray with the BSP tree brushes.  We do so by setting the trace type to TYPE_RAY.
	m_traceType = TYPE_RAY;

	// Run the normal Trace() function with our start and end 
	// position and return a new position
	return Trace(vStart, vEnd);
}

Vec3 CQuake3BSP::TraceSphere(Vec3 vStart, Vec3 vEnd, float radius)
{
	// In this tutorial we are doing sphere collision, so this is the function
	// that we will be doing to initiate our collision checks.

	// Here we initialize the type of trace (SPHERE) and initialize other data
	m_traceType = TYPE_SPHERE;
	m_bCollided = false;
	m_traceRadius = radius;

	// Get the new position that we will return to the camera or player
	Vec3 vNewPosition = Trace(vStart, vEnd);

	// Return the new position to be changed for the camera or player
	return vNewPosition;
}

Vec3 CQuake3BSP::TraceBox(Vec3 vStart, Vec3 vEnd, Vec3 vMin, Vec3 vMax)
{
	m_traceType = TYPE_BOX;			// Set the trace type to a BOX
	m_vTraceMaxs = vMax;			// Set the max value of our AABB
	m_vTraceMins = vMin;			// Set the min value of our AABB
	m_bCollided = false;			// Reset the collised flag

									// Here is a little tricky piece of code that basically takes the largest values
									// of the min and max values and stores them in a vector called vExtents.  This means
									// that we are storing the largest size of our box along each x, y, z value.  We use this
									// as our offset (like with sphere collision) to determine if our box collides with
									// any brushes.  If you aren't familiar with these "Terse" operations, it means that
									// we check if (i.e): if(-m_vTraceMins.x > m_vTraceMaxs.x), then return -m_vTraceMins.x,
									// otherwise, take the positive of x: m_vTraceMins.x.  We do this for each x, y, z slot.
									// This is smaller code than doing a bunch of if statements... but yes, harder to read :)

									// Grab the extend of our box (the largest size for each x, y, z axis)
	m_vExtents = Vec3(-m_vTraceMins.x > m_vTraceMaxs.x ? -m_vTraceMins.x : m_vTraceMaxs.x,
		-m_vTraceMins.y > m_vTraceMaxs.y ? -m_vTraceMins.y : m_vTraceMaxs.y,
		-m_vTraceMins.z > m_vTraceMaxs.z ? -m_vTraceMins.z : m_vTraceMaxs.z);


	// Check if our movement collided with anything, then get back our new position
	Vec3 vNewPosition = Trace(vStart, vEnd);

	// Return our new position
	return vNewPosition;
}

CQuake3BSP::~CQuake3BSP()
{
	// Call our destroy function
	Destroy();
}