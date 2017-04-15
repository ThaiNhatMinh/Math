#include "..\iostream.h"


mat4 ToMatrix(Vec3 v)
{
	Quat ort;

	float sp, sy, sr;
	float cp, cy, cr;
	Math::SinCos(sp, cp, (v.x*0.5f));
	Math::SinCos(sy, cy, (v.y*0.5f));
	Math::SinCos(sr, cr, (v.z * 0.5f));

	ort.set(cr*cp*cy + sp*sy*sr, cr*sp*cy + sr*cp*sy, -cr*sp*sy + sy*cp*cy, -sr*sp*cy + cr*cp*sy);

	return ort.ToMatrix();

}
bool SMDAnimation::Load(const char * pFilename,int numnode)
{
	FILE* pFile = fopen(pFilename, "rt");
	if (!pFile)
	{
		Log::Message(Log::LOG_ERROR, "Can't open file: " + string(pFilename));
		return 0;
	}

	char buffer[512];
	int pad;
	fscanf(pFile, "%s %d", buffer, &pad);
	fscanf(pFile, "%s\n", buffer);
	for (int i = 0; i < numnode; i++)
		fscanf(pFile, "%d %s %d\n", &pad, buffer, &pad);
	fscanf(pFile, "%s\n", buffer); // end
	fscanf(pFile, "%s\n", buffer); // skeleton

	while (!feof(pFile))
	{
		int time=-1;
		//Vec3 pos, ort;
		Frame frame;
		FrameBone frameBone;
		fscanf(pFile, "%s %d\n", buffer, &time);
		if (time == -1) break;
		frame.time = time;
		for (int i = 0; i < numnode; i++)
		{
			fscanf(pFile, "%d %f %f %f %f %f %f\n", &frameBone.BoneID, &frameBone.Pos.x, &frameBone.Pos.z, &frameBone.Pos.y, &frameBone.Ort.x, &frameBone.Ort.z, &frameBone.Ort.y);
			frame.Frames.push_back(frameBone);
		}

		m_FrameList.push_back(frame);
	}

	fclose(pFile);
	return 1;
}

void SMDAnimation::GetFrame(int frame, vector<Bone>& bone, vector<Skeleton>& ske)
{
	static bool ch = 0;
	for (size_t i = 0; i < bone.size(); i++)
	{
		Skeleton node;
		node.BoneID = bone[i].iID;
		node.ParentBoneID = bone[i].iParentID;
		mat4 transf = ToMatrix(m_FrameList[frame].Frames[i].Ort);
		transf = transf.Inverse();
		transf.Translate(m_FrameList[frame].Frames[i].Pos);
		if (node.ParentBoneID != -1)
			node.Transform = transf * ske[node.ParentBoneID].Transform;
		else node.Transform = transf;
		
		
		ske.push_back(node);
	}
}
