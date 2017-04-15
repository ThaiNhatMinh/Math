#include "..\iostream.h"

bool LTBFile::LoadSkeleton(FILE * pFile, SkeNode* pParent)
{
	SkeNode* p = new SkeNode;
	if(pParent!=NULL)	p->m_ParentIndex = pParent->m_Index;
	else p->m_ParentIndex = -1;
	uint16 len;
	fread((void*)&len, sizeof(unsigned short), 1, pFile);
	char str[100];
	fread(str, len, 1, pFile);
	str[len] = '\0';

	p->m_Name = str;

	uint8 m_Flags;
	uint16 index;
	fread(&index, sizeof(uint16), 1, pFile);
	fread(&m_Flags, sizeof(uint8), 1, pFile);
	p->m_Index = index;
	float* m = p->m_GlobalTransform.ToFloatPtr();


	for (int i = 0; i<16; i++) fread(&m[i], sizeof(float), 1, pFile);
	Vec3 v;
	v.set(p->m_GlobalTransform[3], p->m_GlobalTransform[7], p->m_GlobalTransform[11]);
	p->m_GlobalTransform = p->m_GlobalTransform.Transpose();
	p->m_GlobalTransform[3] = p->m_GlobalTransform[7] = p->m_GlobalTransform[11] = 0.0f;
	p->m_GlobalTransform.Translate(v);

	p->m_InvBindPose = p->m_GlobalTransform.Inverse();
	uint32 numChild;
	fread(&numChild, sizeof(uint32), 1, pFile);

	m_pNodeLists.push_back(p);
	for (size_t i = 0; i < numChild; i++) LoadSkeleton(pFile, p);

	return true;
}

LTBFile::LTBFile()
{
}

LTBFile::LTBFile(const char * filename)
{
	if (!Load(filename)) Log::Message(Log::LOG_ERROR, "Can't load file: " + string(filename));
}

LTBFile::~LTBFile()
{
	for (GLuint i = 0; i < m_pNodeLists.size(); i++)
		delete m_pNodeLists[i];
	for (size_t i = 0; i < m_pMeshs.size(); i++)
		if (m_pMeshs[i]!=NULL) delete m_pMeshs[i];
}

bool LTBFile::Load(const char * pFileName)
{
	
	unsigned short str_len;

	FILE* pFile = fopen(pFileName, "rb");
	if (!pFile)
	{
		Log::Message(Log::LOG_ERROR, "Can't load file: " + string(pFileName));
		return false;
	}

	LTB_Header Header;
	unsigned int FileVersion;
	fread((void*)&Header, sizeof(LTB_Header), 1, pFile);
	fread((void*)&FileVersion, sizeof(unsigned int), 1, pFile);

	fread(&m_nKeyFrames, sizeof(uint32), 1, pFile);
	fread(&m_nParentAnims, sizeof(uint32), 1, pFile);
	fread(&m_nNodes, sizeof(uint32), 1, pFile);
	fread(&m_nPieces, sizeof(uint32), 1, pFile);
	fread(&m_nChildModels, sizeof(uint32), 1, pFile);
	fread(&m_nTris, sizeof(uint32), 1, pFile);
	fread(&m_nVerts, sizeof(uint32), 1, pFile);
	fread(&m_nVertexWeights, sizeof(uint32), 1, pFile);
	fread(&m_nLODs, sizeof(uint32), 1, pFile);
	fread(&m_nSockets, sizeof(uint32), 1, pFile);
	fread(&m_nWeightSets, sizeof(uint32), 1, pFile);
	fread(&m_nStrings, sizeof(uint32), 1, pFile);
	fread(&m_StringLengths, sizeof(uint32), 1, pFile);
	fread(&m_VertAnimDataSize, sizeof(uint32), 1, pFile);
	fread(&m_nAnimData, sizeof(uint32), 1, pFile);

	fread((void*)&str_len, sizeof(unsigned short), 1, pFile);
	char* str = new char[str_len];
	fread(str, str_len, 1, pFile);
	delete[] str;

	fread(&m_GlobalRadius, sizeof(float), 1, pFile);
	fread(&m_iNumEnabledOBBs, sizeof(uint32), 1, pFile);
	uint32 numPieces;
	fread(&numPieces, sizeof(uint32), 1, pFile);


	for (uint32 iPieceCnt = 0; iPieceCnt < numPieces; iPieceCnt++)
	{
		LTBMesh* pmesh = new LTBMesh;
		std::vector<Vertex>& vertex = pmesh->m_vVertex;
		std::vector<unsigned int>& Index = pmesh->m_vIndices;

		fread((void*)&str_len, sizeof(unsigned short), 1, pFile);
		char* str = new char[str_len];
		fread(str, str_len, 1, pFile);
		str[str_len] = '\0';
		//printf("%d %s\n",len,str);

		// name of mesh
		pmesh->m_Name = str;

		uint32 numLOD;
		float LODDists;
		fread(&numLOD, sizeof(uint32), 1, pFile);
		//printf("numLOD: %d\n", numLOD);
		for (uint32 dist_cnt = 0; dist_cnt < numLOD; dist_cnt++)
		{
			fread(&LODDists, sizeof(float), 1, pFile);
			//printf("LODDists: %f\n", LODDists);
		}

		// write out min/max lod offset values.
		uint32 minlodoff, maxlodoff;
		fread(&minlodoff, sizeof(uint32), 1, pFile);
		//("minlodoff: %d\n", minlodoff);
		fread(&maxlodoff, sizeof(uint32), 1, pFile);
		//printf("maxlodoff: %d\n", maxlodoff);
		for (uint32 iLODCnt = 0; iLODCnt < numLOD; iLODCnt++)
		{

			uint32 itmp_RS_use_index = 0;
			uint32 m_nNumTextures;
			int32 m_iTextures[4], m_iRenderStyle;
			uint8 m_nRenderPriority;
			fread(&m_nNumTextures, sizeof(uint32), 1, pFile);
			fread(&m_iTextures, sizeof(int32) * 4, 1, pFile);
			fread(&m_iRenderStyle, sizeof(int32), 1, pFile);
			fread(&m_nRenderPriority, sizeof(uint8), 1, pFile);

			uint PieceType;
			fread(&PieceType, sizeof(uint32), 1, pFile);
			//see if this is a null mesh
			if (PieceType == eNullMesh) fread(&PieceType, sizeof(uint32), 1, pFile);
			else
			{
				uint32 iMaxBonesPerTri, iMaxBonesPerVert;
				uint32 iRendObjectSize = 0;
				uint32 iVertCount;
				uint32 iPolyCount;
				uint32 StreamData[4];
				switch (PieceType)
				{
				case eRigidMesh:
					uint32 Bone;
					printf("  PieceType: RigidMesh\n");
					fread(&iRendObjectSize, sizeof(uint32), 1, pFile);
					fread(&iVertCount, sizeof(uint32), 1, pFile);
					fread(&iPolyCount, sizeof(uint32), 1, pFile);
					fread(&iMaxBonesPerTri, sizeof(uint32), 1, pFile);
					fread(&iMaxBonesPerVert, sizeof(uint32), 1, pFile);
					fread(StreamData, sizeof(uint32) * 4, 1, pFile);
					fread(&Bone, sizeof(uint32), 1, pFile);
					for (uint32 iStream = 0; iStream < 4; ++iStream)
						for (uint32 i = 0; i < iVertCount; ++i)
						{
							Vertex ver;
							if (StreamData[iStream] & VERTDATATYPE_POSITION)
							{
								fread(&ver.Pos.x, sizeof(float), 1, pFile);
								fread(&ver.Pos.y, sizeof(float), 1, pFile);
								fread(&ver.Pos.z, sizeof(float), 1, pFile);
								ver.Weights[0] = (Weight(Bone, 1.0f));
								//ver.Weights[1] = (Weight(255, 0.0f));
								//ver.Weights[2] = (Weight(255, 0.0f));
								//ver.Weights[3] = (Weight(255, 0.0f));
								vertex.push_back(ver);
							}
							if (StreamData[iStream] & VERTDATATYPE_NORMAL)
							{
								fread(&vertex[i].Normal.x, sizeof(float), 1, pFile);
								fread(&vertex[i].Normal.y, sizeof(float), 1, pFile);
								fread(&vertex[i].Normal.z, sizeof(float), 1, pFile);
							}
							if (StreamData[iStream] & VERTDATATYPE_COLOR)
							{
								uint32 a;
								fread(&a, sizeof(uint32), 1, pFile);
							}
							if (StreamData[iStream] & VERTDATATYPE_UVSETS_1)
							{
								Vec2 uv;
								fread(&uv.x, sizeof(float), 1, pFile);
								fread(&uv.y, sizeof(float), 1, pFile);
								vertex[i].UV = uv;
							}
							if (StreamData[iStream] & VERTDATATYPE_UVSETS_2)
							{
								float temp;
								fread(&temp, sizeof(float), 1, pFile);
								fread(&temp, sizeof(float), 1, pFile);
							}
							if (StreamData[iStream] & VERTDATATYPE_UVSETS_3)
							{
								float temp2;
								fread(&temp2, sizeof(float), 1, pFile);
								fread(&temp2, sizeof(float), 1, pFile);
							}
							if (StreamData[iStream] & VERTDATATYPE_UVSETS_4)
							{
								float temp3;
								fread(&temp3, sizeof(float), 1, pFile);
								fread(&temp3, sizeof(float), 1, pFile);
							}

							if (StreamData[iStream] & VERTDATATYPE_BASISVECTORS)
							{
								float temp4[6];
								fread(&temp4, sizeof(float) * 6, 1, pFile);
							}
						}
					//vector<uint16> IndexList;
					// Write out pIndexList...
					for (uint32 i = 0; i<iPolyCount * 3; ++i)
					{
						uint16 index;
						fread(&index, sizeof(uint16), 1, pFile);
						//IndexList.push_back(index);
						Index.push_back(index);
					}

					break;
				case eVAMesh:
					printf("  PieceType: VertexAnimatedMesh\n");
					break;
				case eSkelMesh:



					bool bReIndexBones;

					bool UseMatrixPalettes;
					uint32 iMinBone = 256; uint32 iMaxBone = 0;

					fread(&iRendObjectSize, sizeof(uint32), 1, pFile);
					fread(&iVertCount, sizeof(uint32), 1, pFile);
					fread(&iPolyCount, sizeof(uint32), 1, pFile);
					fread(&iMaxBonesPerTri, sizeof(uint32), 1, pFile);
					fread(&iMaxBonesPerVert, sizeof(uint32), 1, pFile);
					fread(&bReIndexBones, sizeof(bool), 1, pFile);
					fread(StreamData, sizeof(uint32) * 4, 1, pFile);
					fread(&UseMatrixPalettes, sizeof(bool), 1, pFile);

					vector<VSTREAM_XYZ_B0> OutVerts_B0;
					vector<VSTREAM_XYZ_B1> OutVerts_B1;
					vector<VSTREAM_XYZ_B2> OutVerts_B2;
					vector<VSTREAM_XYZ_B3> OutVerts_B3;
					vector<VSTREAM_THE_REST> OutVerts_TheRest;

					for (uint32 iStream = 0; iStream < 4; ++iStream)
					{
						for (uint32 i = 0; i<iVertCount; ++i)
						{
							Vertex ver;
							if (StreamData[iStream] & VERTDATATYPE_POSITION)
							{
								switch (iMaxBonesPerTri) {
								case 1:
									VSTREAM_XYZ_B0 b0;
									fread(&b0, sizeof(VSTREAM_XYZ_B0), 1, pFile);
									break;
								case 2:
									VSTREAM_XYZ_B1 tb1;
									fread(&tb1, sizeof(VSTREAM_XYZ_B1), 1, pFile);
									ver.Pos = Vec3(tb1.x, tb1.y, tb1.z);
									ver.Weights[0] = (Weight(255, tb1.blend1));
									ver.Weights[1] = (Weight(255, 1.0f - tb1.blend1));
									vertex.push_back(ver);
									break;
								case 3:
									VSTREAM_XYZ_B2 tx;
									fread(&tx, sizeof(tx), 1, pFile);
									ver.Pos = Vec3(tx.x, tx.y, tx.z);
									ver.Weights[0] = (Weight(255, tx.blend1));
									ver.Weights[1] = (Weight(255, tx.blend2));
									ver.Weights[2] = (Weight(255, 1.0f - tx.blend1 - tx.blend2));
									vertex.push_back(ver);
									break;
								case 4:
									VSTREAM_XYZ_B3 t;
									fread(&t, sizeof(VSTREAM_XYZ_B3), 1, pFile);
									ver.Pos = Vec3(t.x, t.y, t.z);
									ver.Weights[0] = (Weight(255, t.blend1));
									ver.Weights[1] = (Weight(255, t.blend2));
									ver.Weights[2] = (Weight(255, t.blend3));
									ver.Weights[3] = (Weight(255, 1.0f - t.blend3 - t.blend2 - t.blend1));
									vertex.push_back(ver);
									break;
								}
							}

							if (StreamData[iStream] & VERTDATATYPE_NORMAL)
							{
								VSTREAM_THE_REST t;
								fread(&t.nx, sizeof(float), 1, pFile);
								fread(&t.ny, sizeof(float), 1, pFile);
								fread(&t.nz, sizeof(float), 1, pFile);
								vertex[i].Normal = Vec3(t.nx, t.ny, t.nz);
							}
							if (StreamData[iStream] & VERTDATATYPE_COLOR)
							{
								uint32 tem;
								fread(&tem, sizeof(uint32), 1, pFile);
							}
							if (StreamData[iStream] & VERTDATATYPE_UVSETS_1)
							{
								Vec2 uv;
								fread(&uv, sizeof(Vec2), 1, pFile);
								vertex[i].UV = uv;
							}
							if (StreamData[iStream] & VERTDATATYPE_UVSETS_2)
							{
								fread(&OutVerts_TheRest[i].u2, sizeof(float), 1, pFile);
								fread(&OutVerts_TheRest[i].v2, sizeof(float), 1, pFile);
							}
							if (StreamData[iStream] & VERTDATATYPE_UVSETS_3)
							{
								fread(&OutVerts_TheRest[i].u3, sizeof(float), 1, pFile);
								fread(&OutVerts_TheRest[i].v3, sizeof(float), 1, pFile);
							}
							if (StreamData[iStream] & VERTDATATYPE_UVSETS_4)
							{
								fread(&OutVerts_TheRest[i].u4, sizeof(float), 1, pFile);
								fread(&OutVerts_TheRest[i].v4, sizeof(float), 1, pFile);
							}

							if (StreamData[iStream] & VERTDATATYPE_BASISVECTORS)
							{
								fread(&OutVerts_TheRest[i].S.x, sizeof(float), 1, pFile);
								fread(&OutVerts_TheRest[i].S.y, sizeof(float), 1, pFile);
								fread(&OutVerts_TheRest[i].S.z, sizeof(float), 1, pFile);
								fread(&OutVerts_TheRest[i].T.x, sizeof(float), 1, pFile);
								fread(&OutVerts_TheRest[i].T.y, sizeof(float), 1, pFile);
								fread(&OutVerts_TheRest[i].T.z, sizeof(float), 1, pFile);
							}
						}
					}


					// Write out pIndexList...
					for (uint32 i = 0; i<iPolyCount * 3; ++i)
					{
						uint16 index;
						fread(&index, sizeof(uint16), 1, pFile);
						Index.push_back(index);
					}

					uint BoneComboListSize;
					fread(&BoneComboListSize, sizeof(uint32), 1, pFile);
					//printf("BoneComboListSize: %d\n", BoneComboListSize);
					for (uint32 i = 0; i<BoneComboListSize; i++)
					{
						uint16 m_BoneIndex_Start, m_BoneIndex_End;
						uint8 m_BoneList[4];

						fread(&m_BoneIndex_Start, sizeof(uint16), 1, pFile);
						fread(&m_BoneIndex_End, sizeof(uint16), 1, pFile);


						fread(&m_BoneList[0], sizeof(uint8) * 4, 1, pFile);

						for (uint16 k = m_BoneIndex_Start; k<m_BoneIndex_Start + m_BoneIndex_End; k++)
						{
							unsigned int wid;
							for (wid = 0; wid <4 ; wid++) vertex[k].Weights[wid].Bone = m_BoneList[wid];
								
							
						}

						uint32 m_iIndexIndex;
						fread(&m_iIndexIndex, sizeof(uint32), 1, pFile);
						//printf("m_iIndexIndex: %d\n",m_iIndexIndex);
					}

					break;

				}

			}



			uint8 m_UsedNodeListSize;
			fread(&m_UsedNodeListSize, sizeof(uint8), 1, pFile);
			vector<uint32>			m_UsedNodeList;
			for (uint32 iUsedNodeCnt = 0; iUsedNodeCnt <m_UsedNodeListSize; iUsedNodeCnt++)
			{
				uint8 t;
				fread(&t, sizeof(uint8), 1, pFile);
				//printf("m_UsedNodeList[%d]: %d\n", iUsedNodeCnt, t);
				m_UsedNodeList.push_back(t);
			}
		}

		string file = pFileName;
		size_t t1 = file.find_last_of("\\/");
		size_t t2 = file.size() - t1;
		string texFile = file.substr(t1, t2 - 4) + ".DTX";
		string TexPathFile = (file.substr(0, t1)) + texFile;
		//pmesh->m_pTexture = Resources::LoadDTX(TexPathFile.c_str());
		m_pMeshs.push_back(pmesh);
	}
	
	
	
	//m_pSkeleton = new SkeletonNode;
	//m_pSkeleton->m_pParent = NULL;
	//m_pSkeleton->LoadSkeleton(pFile);
	LoadSkeleton(pFile, NULL);

	uint32 valid_ws_indexsize;
	fread(&valid_ws_indexsize, sizeof(uint32), 1, pFile);
	for (uint16 i = 0; i < valid_ws_indexsize; i++)
	{
		uint32 k;
		uint16 len;
		fread((void*)&len, sizeof(unsigned short), 1, pFile);
		char str[100];
		fread(str, len, 1, pFile);
		str[len] = '\0';
		//printf("%s\n", str);
		uint32 m_WeightsGetSize;
		fread(&m_WeightsGetSize, sizeof(uint32), 1, pFile);
		for (k = 0; k < m_WeightsGetSize; k++)
		{
			float t;
			fread(&t, sizeof(float), 1, pFile);
			//printf("m_Weights[%d]: %.3f\n",k,t);
		}
	}

	uint32 NumChildModels;
	fread(&NumChildModels, sizeof(uint32), 1, pFile);

	for (size_t i = 1; i <NumChildModels; ++i)
	{
		fread((void*)&str_len, sizeof(unsigned short), 1, pFile);
		char str[100];
		fread(str, str_len, 1, pFile);
		str[str_len] = '\0';
		//printf("%d %s\n", str_len, str);
	}

	// Save animations.
	uint32 nAnimDataSize = 0;
	m_Animation.LoadAnimation(pFile,m_pNodeLists);
	/*
	uint32 CalcNumParentAnims;

	fread(&CalcNumParentAnims, sizeof(uint32), 1, pFile);
	//printf("CalcNumParentAnims: %d\n", CalcNumParentAnims);
	for (uint32 i = 0; i < CalcNumParentAnims; i++)
	{
		float D[3];
		fread(&D, sizeof(float) * 3, 1, pFile);
		//printf("Dimension: %.2f, %.2f, %.2f\n",D[0],D[1],D[2]);

		// Read name of animation
		fread((void*)&str_len, sizeof(unsigned short), 1, pFile);
		char str[100];
		fread(str, str_len, 1, pFile);
		str[str_len] = '\0';

		
		fread(&m_CompressionType, sizeof(uint32), 1, pFile);
		fread(&m_InterpolationMS, sizeof(uint32), 1, pFile);

		//printf("Anim Name: %s\n",str);
		//printf("m_CompressionType: %d\n",m_CompressionType);
		//printf("m_InterpolationMS: %d\n",m_InterpolationMS);

		uint32 m_KeyFramesGetSize;
		fread(&m_KeyFramesGetSize, sizeof(uint32), 1, pFile);
		std::vector<AnimKeyFrame> KeyFrame;
		for (uint16 j = 0; j<m_KeyFramesGetSize; j++)
		{
			AnimKeyFrame key;
			fread(&key.m_Time, sizeof(uint32), 1, pFile);
			fread((void*)&str_len, sizeof(unsigned short), 1, pFile);
			char str1[100];
			fread(str1, str_len, 1, pFile);
			str1[str_len] = '\0';

			key.m_pString = str1;
			KeyFrame.push_back(key);
		}
		m_pSkeleton->LoadAnimation(pFile, str, KeyFrame, m_CompressionType);
		//printf("Finish animation: %s\n",str);
	}
	*/

	uint32 NumSockets;
	fread(&NumSockets, sizeof(uint32), 1, pFile);
	for (uint32 i = 0; i < NumSockets; i++)
	{
		uint32 m_iNode;
		fread(&m_iNode, sizeof(uint32), 1, pFile);
		fread((void*)&str_len, sizeof(unsigned short), 1, pFile);
		char str1[100];
		fread(str1, str_len, 1, pFile);
		str1[str_len] = '\0';
		Quat rot;
		fread(&rot, sizeof(float) * 4, 1, pFile);
		Vec3 pos;
		fread(&pos, sizeof(float) * 3, 1, pFile);
		Vec3 scale;
		fread(&scale, sizeof(float) * 3, 1, pFile);
		//printf("SocketName: %s \n", str1);
	}
	fclose(pFile);
	return 1;
	
}


void LTBFile::SetupMesh(Shader* p)
{
	for (size_t i = 0; i < m_pMeshs.size(); i++)
	{
		m_pMeshs[i]->Topology = GL_TRIANGLES;
		m_pMeshs[i]->Finalize(p);
	}
}
