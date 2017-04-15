#include "..\iostream.h"
SMDFile::SMDFile()
{
}
SMDFile::~SMDFile()
{
	for (size_t i = 0; i < m_MeshList.size(); i++)
		delete m_MeshList[i];
}

bool SMDFile::Init(const char * pFilename)
{

	FILE* pFile = fopen(pFilename, "rt");
	if (!pFile) {
		printf("ERROR");
		return 0;
	}
	char buffer[512];
	int num;
	fscanf(pFile, "%s %d", buffer, &m_iVersion);
	//printf("%s %d\n", buffer, m_iVersion);
	fscanf(pFile, "%s\n", buffer);
	//printf("%s \n", buffer);
	Bone bone;
	while (1)
	{
		if (fscanf(pFile, "%d %s %d\n", &bone.iID, &bone.Name, &bone.iParentID) != 3) break;
		//printf("%s %d %d\n", bone.Name, bone.iID, bone.iParentID);
		m_BoneList.push_back(bone);
	}
	
	fscanf(pFile, "%s\n", buffer); // end
	fscanf(pFile, "%s\n", buffer); // skeleton
	//printf("%s\n", buffer);
	fscanf(pFile, "%s %d", buffer, &num); // time 0
	Vec3 pos, Ort;
	int id;
	for (size_t i = 0; i < m_BoneList.size(); i++)
	{
		fscanf(pFile, "%d %f %f %f %f %f %f\n", &id, &pos.x, &pos.z, &pos.y, &Ort.x, &Ort.z, &Ort.y);
		if (id == m_BoneList[i].iID)
		{
			mat4 m = ToMatrix(Ort);
			m.Translate(pos);
			m_BoneList[i].Pose = m.Inverse();
			m_BoneList[i].vPos = pos;
			m_BoneList[i].vOrt = Ort;
		}
		else
		{
			printf("ERROR");
			return 0;
		}
	}
	fscanf(pFile, "%s\n", buffer); // end
	fscanf(pFile, "%s\n", buffer); // triangle
	while (!feof(pFile))
	{
		SMDMesh* mesh = new SMDMesh;
		Vec3 pos, normal;
		Vec2 uv;
		WeightSet ws;
		int parentBone, num;
		char material[32] = "0";
		//fscanf(pFile, "%s\n", buffer); // material
		//mesh.m_Material = buffer;
		while (1)
		{
			fscanf(pFile, "%s\n", buffer);
			if (!mesh->m_Material.size())
			{
				mesh->m_Material = buffer;
			}
			else if (mesh->m_Material != string(buffer))
			{
				fpos_t fpos;
				fgetpos(pFile, &fpos);
				fpos -= (strlen(buffer)+4);
				fsetpos(pFile, &fpos);
				break;
			}
			else if (strstr(buffer, "end"))
				break;
			fscanf(pFile, "%d %f %f %f %f %f %f %f %f %d %d %f \n", &parentBone, &pos.x, &pos.z, &pos.y, &normal.x, &normal.z, &normal.y, &uv.x, &uv.y, &num, &ws.iBone, &ws.fWeight);
			mesh->m_vPositions.push_back(pos);
			mesh->m_vNormals.push_back(normal);
			mesh->m_vUVs.push_back(uv);
			mesh->m_Weights.push_back(ws);
			mesh->m_ParentBone.push_back(parentBone);
			fscanf(pFile, "%d %f %f %f %f %f %f %f %f %d %d %f \n", &parentBone, &pos.x, &pos.z, &pos.y, &normal.x, &normal.z, &normal.y, &uv.x, &uv.y, &num, &ws.iBone, &ws.fWeight);
			mesh->m_vPositions.push_back(pos);
			mesh->m_vNormals.push_back(normal);
			mesh->m_vUVs.push_back(uv);
			mesh->m_Weights.push_back(ws);
			mesh->m_ParentBone.push_back(parentBone);
			fscanf(pFile, "%d %f %f %f %f %f %f %f %f %d %d %f \n", &parentBone, &pos.x, &pos.z, &pos.y, &normal.x, &normal.z, &normal.y, &uv.x, &uv.y, &num, &ws.iBone, &ws.fWeight);
			mesh->m_vPositions.push_back(pos);
			mesh->m_vNormals.push_back(normal);
			mesh->m_vUVs.push_back(uv);
			mesh->m_Weights.push_back(ws);
			mesh->m_ParentBone.push_back(parentBone);
		}
		
		if (!strstr(buffer, "end")) m_MeshList.push_back(mesh);

	}


	return false;
}

void SMDFile::SetupMesh()
{
	for (size_t i = 0; i < m_MeshList.size(); i++)
	{
		m_MeshList[i]->Topology = GL_TRIANGLES;
		m_MeshList[i]->Finalize();
	}
}
