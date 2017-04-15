#pragma once

#include "..\iostream.h"


class Terrain: public SceneNode
{
private:
	Shader		m_Shader;
	GLuint		m_ModelUniform;
	GLuint		m_ViewUniform;
	GLuint		m_ProjUniform;
	GLuint		m_lightPosUniform;
	GLuint		m_eyePosUniform;
	GLuint		m_numPointlightUniform;
	GLuint		m_numSpotlightUniform;
	Texture*	m_pBlendTex;
	Texture*	m_pRockTex;
	Mesh*		m_pTerrainMesh;
	float*		m_pData;
	float       mHeightScale;
	float       mHeightOffset;
	int			mRows;
	int			mCols;

	float		m_ScaleTex;
protected:
	void	GenTriGrid(int numVertRows, int numVertCols, float dx, float dz, const Vec3& center, std::vector<Vec3>& verts,	std::vector<unsigned int>& indices);
	void	loadRAW(int m, int n,		const string& filename,		float heightScale, float heightOffset);
	bool	inBounds(int i, int j);
	float	sampleHeight3x3(int i, int j);
	void	filter3x3();
	Vec3	CalculateNormal(int i, int j);
public:
			Terrain();
			~Terrain();

	void	Init(const string filename, float hscale, float hoffset);

	void	OnUpdate(Scene*,float dt);
	void	PreRender(Scene*);
	void	Render(Scene*);
	void	PostRender(Scene*);
	Shader	GetShader() {return m_Shader;};
	float	HetHeight(int i, int j);
};