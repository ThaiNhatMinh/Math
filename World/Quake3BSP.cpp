#include "..\iostream.h"



CQuake3BSP::CQuake3BSP()
{
	// Here we simply initialize our member variables to 0
	m_numOfVerts = 0;
	m_numOfFaces = 0;
	m_numOfIndices = 0;
	m_numOfTextures = 0;

	m_pVerts = NULL;
	m_pFaces = NULL;
	m_pIndices = NULL;
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
	// We create a local pointer of tBSPTextures because we don't need
	// that information once we create texture maps from it.
	m_numOfTextures = lumps[kTextures].length / sizeof(tBSPTexture);
	tBSPTexture *pTextures = new tBSPTexture[m_numOfTextures];

	// Seek to the position in the file that stores the vertex information
	fseek(fp, lumps[kVertices].offset, SEEK_SET);

	// Since Quake has the Z-axis pointing up, we want to convert the data so
	// that Y-axis is pointing up (like normal!) :)

	// Go through all of the vertices that need to be read
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
	fread(pTextures, m_numOfTextures, sizeof(tBSPTexture), fp);

	// Now that we have the texture information, we need to load the
	// textures.  Since the texture names don't have an extension, we need
	// to find it first.

	// Go through all of the textures
	for (i = 0; i < m_numOfTextures; i++)
	{
		// Find the extension if any and append it to the file name
		Log::Message(Log::LOG_DEBUG, pTextures[i].strName);
		FindTextureExtension(pTextures[i].strName);

		// Create a texture from the image
		m_textures[i] = Resources::LoadTexture(pTextures[i].strName);
		
	}

	// We can now free all the texture information since we already loaded them
	delete[] pTextures;

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

}

CQuake3BSP::~CQuake3BSP()
{
	// Call our destroy function
	Destroy();
}