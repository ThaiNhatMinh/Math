#pragma once
#include "..\iostream.h"


struct DtxHeader
{
	unsigned int iResType;
	int iVersion;
	unsigned short usWidth;
	unsigned short usHeight;
	unsigned short usMipmaps;
	unsigned short usSections;
	int iFlags;
	int iUserFlags;
	unsigned char ubExtra[12];
	char szCommandString[128];
};

class Resources
{
private:
	static vector<Texture*> m_Textures;

private:
	static Texture* HasTexture(const char* filename);

public:
	Resources();
	~Resources();
	static Texture* LoadTexture(const char* filename);
	static Texture* LoadDTX(const char* filename);
	static void Release();

};

