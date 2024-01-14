#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "VertexFormat.h"
#include "BBox.h"
#include "BooleanArray.h"

#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>

enum {POSITION = 0, NORMAL = 1, TEX_COORD = 2, BONE_IDX = 3, BONE_WEIGHT = 4};

struct Bone {
	uint id; aiMatrix4x4 transform;
	inline Bone() : id(0){}
	inline Bone(uint i) : id(i){}
	inline Bone(uint i, const aiMatrix4x4& t) : id(i), transform(t){}
};

class BoneData {
public:
	std::unordered_map<std::string, Bone> bones;
};

struct MeshSubset {
	aiString name; int start; int end;
	inline MeshSubset(const aiString& n, int st, int e) : name(n), start(st), end(e){}
};

std::vector<MeshSubset> meshes;

void getVertexCount(const aiScene* scene, const aiNode* node, int& vcount, int& icount, BoneData& bones){
	for(uint i=0; i<node->mNumMeshes; i++){
		uint mesh_id = node->mMeshes[i]; const aiMesh* mesh = scene->mMeshes[mesh_id];
		if(mesh->mPrimitiveTypes != aiPrimitiveType_TRIANGLE || !mesh->HasPositions() || !mesh->HasFaces()) continue;
		meshes.push_back(MeshSubset(mesh->mName, icount, icount+mesh->mNumFaces*3)); vcount += mesh->mNumVertices; icount += mesh->mNumFaces*3;
	} for(uint i=0; i<node->mNumChildren; i++) getVertexCount(scene, node->mChildren[i], vcount, icount, bones);
}

float4 mul(const aiMatrix4x4& transform, const float4& p){
	return float4::make(transform.a1*p.x+transform.a2*p.y+transform.a3*p.z+transform.a4*p.w,
		transform.b1*p.x+transform.b2*p.y+transform.b3*p.z+transform.b4*p.w,
		transform.c1*p.x+transform.c2*p.y+transform.c3*p.z+transform.c4*p.w,
		transform.d1*p.x+transform.d2*p.y+transform.d3*p.z+transform.d4*p.w);
}
float3 mul(const aiMatrix3x3& transform, const float3& p){
	return float3::make(transform.a1*p.x+transform.a2*p.y+transform.a3*p.z,
		transform.b1*p.x+transform.b2*p.y+transform.b3*p.z,
		transform.c1*p.x+transform.c2*p.y+transform.c3*p.z);
}

uint getNodeBone(BoneData& bones, int& index, const std::string& name, const aiMatrix4x4& transform){
	std::unordered_map<std::string,Bone>::const_iterator i = bones.bones.find(name);
	if(i == bones.bones.end()){
		std::cout << "Bone: " << name.c_str() << " = " << index << std::endl;
		aiMatrix4x4 t = transform; t.Inverse();
		uint bidx = index; index++; bones.bones[name] = Bone(bidx, t); return bidx;
	} else return i->second.id;
}
void traceMatrix(const aiMatrix4x4& mat){
	std::cout << "MAT4:" << mat.a1 << "," << mat.a2 << "," << mat.a3 << "," << mat.a4 << std::endl <<
		mat.b1 << "," << mat.b2 << "," << mat.b3 << "," << mat.b4 << std::endl <<
		mat.c1 << "," << mat.c2 << "," << mat.c3 << "," << mat.c4 << std::endl <<
		mat.d1 << "," << mat.d2 << "," << mat.d3 << "," << mat.d4 << std::endl;
}

aiMatrix4x4 getAbsolute(const aiNode* n){
	aiMatrix4x4 m = n->mTransformation; while(n->mParent != NULL){n = n->mParent; m = n->mTransformation*m;} return m;
}

bool loadMesh(const aiScene* scene, int mesh_id, int& index, const aiString& name, const aiMatrix4x4& transform, VertexBuffer& vertices, IndexBuffer& indices, int& voff, int& ioff, BBox3D<double>& bounds, BoneData& bones){
	const aiMesh* mesh = scene->mMeshes[mesh_id];
	if(mesh->mPrimitiveTypes != aiPrimitiveType_TRIANGLE || !mesh->HasPositions() || !mesh->HasFaces()) return false;
	aiMatrix3x3 normalMat = aiMatrix3x3(transform); normalMat.Inverse(); normalMat.Transpose();
	bool hasNormals = mesh->HasNormals(), hasBones = mesh->HasBones(), hasTexCoords = mesh->HasTextureCoords(0);
	for(unsigned int i=0; i<mesh->mNumVertices; i++){
		aiVector3D v = mesh->mVertices[i]; float4 pos = float4::make(v.x, v.y, v.z, 1);
		float4 bpos = mul(transform, pos); bounds += double3::make(bpos.x, bpos.y, bpos.z);
		vertices.set(voff+i, POSITION, float4::make(bpos.x, bpos.y, bpos.z, 1));
		if(hasNormals){
			v = mesh->mNormals[i]; float3 norm = float3::make(v.x, v.y, v.z);
			norm = mul(normalMat, norm); normalize_m(norm);
			vertices.set(voff+i, NORMAL, float4::make(norm.x, norm.y, norm.z, 1));
		} if(hasTexCoords){
			v = mesh->mTextureCoords[0][i]; vertices.set(voff+i, TEX_COORD, float4::make(v.x, v.y, v.z, 1));
		}
	} uint nFaces = mesh->mNumFaces;
	for(unsigned int f=0; f<nFaces; f++){
		const aiFace& face = mesh->mFaces[f];
		for(int i=0; i<3; i++) indices.set(ioff+f*3+i, face.mIndices[i]+voff);
	} if(scene->HasAnimations()){
		if(hasBones){
			unsigned int numBones = mesh->mNumBones;
			 for(unsigned int b=0; b<numBones; b++){
				const aiBone* bone = mesh->mBones[b];
				std::unordered_map<std::string,Bone>::const_iterator i = bones.bones.find(bone->mName.C_Str()); unsigned int bidx;
				if(i == bones.bones.end()){
					aiMatrix4x4 t = transform; t.Inverse();
					bidx = index; index++; bones.bones[bone->mName.C_Str()] = Bone(bidx, bone->mOffsetMatrix*t);
					std::cout << "Bone: " << bone->mName.C_Str() << " = " << bidx << std::endl;
				} else bidx = i->second.id;
				for(unsigned int w=0; w<bone->mNumWeights; w++){
					const aiVertexWeight& vw = bone->mWeights[w];
					float4 idx = vertices.get(voff+vw.mVertexId, BONE_IDX);
					float4 wt = vertices.get(voff+vw.mVertexId, BONE_WEIGHT);
					uchar minidx=4;
					for(uchar c=0; c<4; c++) if(wt[c] == 0 || idx[c] == bidx){minidx = c; break;}
					if(minidx >= 4) continue;
					idx[minidx] = (float)bidx; wt[minidx] = vw.mWeight;
					vertices.set(voff+vw.mVertexId, BONE_IDX, idx);
					vertices.set(voff+vw.mVertexId, BONE_WEIGHT, wt);
				}
			} for(unsigned int i=0; i<mesh->mNumVertices; i++){
				float4 wt = vertices.get(voff+i, BONE_WEIGHT);
				if(wt.x == 0){
					uint default_bone = getNodeBone(bones, index, std::string(name.C_Str())+"_auto", transform);
					wt.x = 1; vertices.set(voff+i, BONE_WEIGHT, wt);
					vertices.set(voff+i, BONE_IDX, float4::make((float)default_bone,0,0,0));
					vertices.set(voff+i, BONE_WEIGHT, float4::make(1,0,0,0));
				} else {
					wt /= (wt.x+wt.y+wt.z+wt.w); vertices.set(voff+i, BONE_WEIGHT, wt);
				}
			}
		} else {
			uint default_bone = getNodeBone(bones, index, std::string(name.C_Str()) + "_auto", transform);
			for(unsigned int i=0; i<mesh->mNumVertices; i++){
				vertices.set(voff+i, BONE_IDX, float4::make((float)default_bone,0,0,0));
				vertices.set(voff+i, BONE_WEIGHT, float4::make(1,0,0,0));
			}
		}
	} voff += mesh->mNumVertices; ioff += nFaces*3; return true;
}

void generateMesh(const aiScene* scene, const aiNode* node, int& index, const aiMatrix4x4& transform, VertexBuffer& vertices, IndexBuffer& indices, int& voff, int& ioff, BBox3D<double>& bounds, BoneData& bones){
	aiMatrix4x4 mat = transform*node->mTransformation;
	std::cout << "Node: " << node->mName.C_Str() << ", Children: " << node->mNumChildren << ", Meshes: " << node->mNumMeshes << std::endl;
	for(uint i=0; i<node->mNumMeshes; i++){
		loadMesh(scene, node->mMeshes[i], index, node->mName, mat, vertices, indices, voff, ioff, bounds, bones);
	} for(uint i=0; i<node->mNumChildren; i++) generateMesh(scene, node->mChildren[i], index, mat, vertices, indices, voff, ioff, bounds, bones);

}

const aiNode* loadTree(std::vector<std::pair<const aiNode*, int> >& nodes, const aiNode* node, int cur, int& index, std::unordered_map<std::string, int>& node_map, const BoneData& bones){
	int len = node->mNumChildren; int childIdx = index; index += len; const aiNode* ret = NULL;
	if(node->mNumMeshes == 0 && node_map.find(node->mName.C_Str()) == node_map.end()) node_map[node->mName.C_Str()] = cur;
	if(nodes.size() <= cur) nodes.resize(cur+1); nodes[cur] = std::make_pair(node, childIdx);
	for(uint i=0; i<len; i++){const aiNode* r = loadTree(nodes, node->mChildren[i], childIdx+i, index, node_map, bones); if(ret == NULL) ret = r;} return ret;
}

void writeByte(std::ofstream& file, char f){file.write(&f, 1);}
void writeShort(std::ofstream& file, short f){
	file.write(reinterpret_cast<const char *>(&f), 2);
}
void writeInt(std::ofstream& file, int f){
	file.write(reinterpret_cast<const char *>(&f), 4);
}
void writeFloat(std::ofstream& file, float f){
	file.write(reinterpret_cast<const char *>(&f), 4);
}
void writeUTF(std::ofstream& file, const aiString& s){
	ushort len = s.length; writeShort(file, len); file.write(s.C_Str(), len);
}
bool equalsFuzzy(const float3& a, const float3& b, float d) {return abs(a.x-b.x)<d && abs(a.y-b.y)<d && abs(a.z-b.z)<d;}
bool equalsFuzzy(const aiQuaternion& a, const aiQuaternion& b, float d) {return abs(a.x-b.x)<d && abs(a.y-b.y)<d && abs(a.z-b.z)<d && abs(a.w-b.w)<d;}
void writeVectorArray(std::ofstream& file, aiVectorKey* keys, uint count){
	std::vector<uint> ar; 
	for(uint i=0; i<count; i++){
		const aiVectorKey& k = keys[i];
		if(i > 0 && i < count-1){
			float3 from = float3::make(keys[i-1].mValue.x, keys[i-1].mValue.y, keys[i-1].mValue.z);
			float3 to = float3::make(keys[i+1].mValue.x, keys[i+1].mValue.y, keys[i+1].mValue.z);
			if(equalsFuzzy(interp(from, to, (k.mTime-keys[i-1].mTime)/(keys[i+1].mTime-keys[i-1].mTime)), float3::make(k.mValue.x, k.mValue.y, k.mValue.z), 0.00001)) continue;
		} else if(i == count-1 && i > 0 && equalsFuzzy(float3::make(keys[i-1].mValue.x, keys[i-1].mValue.y, keys[i-1].mValue.z), float3::make(k.mValue.x, k.mValue.y, k.mValue.z), 0.00001)) continue;
		ar.push_back(i);
	} writeInt(file, ar.size()*4); for(uint i=0; i<ar.size(); i++){
		const aiVectorKey& k = keys[ar[i]]; writeFloat(file, k.mTime); writeFloat(file, k.mValue.x); writeFloat(file, k.mValue.y); writeFloat(file, k.mValue.z);
	}
}
void writeQuatArray(std::ofstream& file, aiQuatKey* keys, uint count){
	std::vector<uint> ar;
	for(uint i=0; i<count; i++){
		const aiQuatKey& k = keys[i];
		if(i > 0 && i < count-1){
			const aiQuatKey& prev = keys[ar[ar.size()-1]];
			const aiQuaternion& from = prev.mValue;
			const aiQuaternion& to = keys[i+1].mValue;
			aiQuaternion est; from.Interpolate(est, from, to, (k.mTime-prev.mTime)/(keys[i+1].mTime-prev.mTime));
			if(equalsFuzzy(est, k.mValue, 0.00001)) continue;
		} else if(i == count-1 && i > 0 && equalsFuzzy(keys[ar[ar.size()-1]].mValue, k.mValue, 0.00001)) continue;
		ar.push_back(i);
	} writeInt(file, ar.size()*5); for(uint i=0; i<ar.size(); i++){
		const aiQuatKey& k = keys[ar[i]]; writeFloat(file, k.mTime); writeFloat(file, k.mValue.w);
		writeFloat(file, k.mValue.x); writeFloat(file, k.mValue.y); writeFloat(file, k.mValue.z);
	}
}

bool NO_SCALE = false; bool WRITE_MESHES = false;
void loadAnimation(std::ofstream& file, const aiScene* scene, const aiAnimation* anim, const std::unordered_map<std::string, int>& node_map){
	writeUTF(file, anim->mName); std::cout << "Animation: " << anim->mName.C_Str() << std::endl;
	writeFloat(file, anim->mDuration); writeInt(file, anim->mNumChannels);
	for(uint i=0; i<anim->mNumChannels; i++){
		const aiNodeAnim* n = anim->mChannels[i];
		std::unordered_map<std::string,int>::const_iterator it = node_map.find(n->mNodeName.C_Str());
		if(it == node_map.end()) continue; writeShort(file, it->second);
		writeVectorArray(file, n->mPositionKeys, n->mNumPositionKeys);
		writeQuatArray(file, n->mRotationKeys, n->mNumRotationKeys);
		if(NO_SCALE){
			writeInt(file, 4); writeFloat(file, 0); writeFloat(file, 1); writeFloat(file, 1); writeFloat(file, 1);
		} else writeVectorArray(file, n->mScalingKeys, n->mNumScalingKeys);
	}
}

void writeMat4(std::ofstream& file, const aiMatrix4x4& mat){
	float* ar = (float*)(&mat); for(int i=0; i<16; i++) writeFloat(file, ar[i]);
}
void loadScene(std::ofstream& file, const aiScene* scene){
	int vcount = 0, icount = 0, voff = 0, ioff = 0; BoneData bones;
	getVertexCount(scene, scene->mRootNode, vcount, icount, bones);
	VertexFormat format; format.addAttribute<float, 3, false>();
	format.addAttribute<float, 3, false>(); format.addAttribute<float, 2, false>();
	short nAnim = scene->HasAnimations()?(short)scene->mNumAnimations:0;
	if(nAnim > 0){format.addAttribute<float, 4, false>(); format.addAttribute<float, 4, false>();}
	VertexBuffer vertices(&format, vcount); IndexFormat iformat(vcount); IndexBuffer indices(&iformat, icount);
	int index = 0; BBox3D<double> bounds; aiMatrix4x4 identity(1,0,0,0,0,0,-1,0,0,1,0,0,0,0,0,1);
	generateMesh(scene, scene->mRootNode, index, identity, vertices, indices, voff, ioff, bounds, bones);

	writeInt(file, vcount); writeInt(file, icount); writeShort(file, nAnim);
	file.write(reinterpret_cast<const char *>(vertices.getBytes()), vertices.getSize());
	file.write(reinterpret_cast<const char *>(indices.getBytes()), indices.getSize());
	writeFloat(file, bounds.botLeft.x); writeFloat(file, bounds.botLeft.y); writeFloat(file, bounds.botLeft.z);
	writeFloat(file, bounds.topRight.x); writeFloat(file, bounds.topRight.y); writeFloat(file, bounds.topRight.z);

	std::cout << "Bounds: [" << bounds.botLeft.x << "," << bounds.botLeft.y << "," << bounds.botLeft.z  << "] - [" << bounds.topRight.x << "," << bounds.topRight.y << "," << bounds.topRight.z << "]" << std::endl;

	if(nAnim > 0){
		std::vector<std::pair<const aiNode*, int> > nodes; std::unordered_map<std::string, int> node_map;
		int index = 1; const aiNode* n = loadTree(nodes, scene->mRootNode, 0, index, node_map, bones);
		for(int i=0; i<nAnim; i++) loadAnimation(file, scene, scene->mAnimations[i], node_map);
		int len = nodes.size(); writeShort(file, len); for(int j=0; j<len; j++){
			std::pair<const aiNode*, int>& p = nodes[j]; const aiNode* node = p.first; writeByte(file, node->mNumChildren);
			if(node->mNumChildren > 0) writeShort(file, p.second);
			if(j == 0) writeMat4(file, identity*node->mTransformation); else writeMat4(file, node->mTransformation);
			std::unordered_map<std::string,Bone>::const_iterator i = bones.bones.find(node->mNumMeshes == 0?node->mName.C_Str():(std::string(node->mName.C_Str())+"_auto"));
			if(i != bones.bones.end()){
				writeShort(file, i->second.id); writeMat4(file, i->second.transform);
			} else writeShort(file, -1);
		}
	} if(WRITE_MESHES){
		int nMesh = meshes.size(); writeShort(file, nMesh); for(int i=0; i<nMesh; i++){
			const MeshSubset& m = meshes[i]; writeUTF(file, m.name); writeInt(file, m.start); writeInt(file, m.end);
		}
	}
}

int main(int argc, char *argv[]){
	if(argc > 3 && strcmp(argv[argc-1], "-noscale") == 0){argc--; NO_SCALE = true;}
	if(argc > 3 && strcmp(argv[argc-1], "-writemeshes") == 0){argc--; WRITE_MESHES = true;}
	if(argc != 3){
		std::cout << "Usage: CreateWOBJ in.fbx out.wobj [-writemeshes]" << std::endl; return -1;
	} aiLogStream stream = aiGetPredefinedLogStream(aiDefaultLogStream_STDOUT,NULL);
    aiAttachLogStream(&stream); char* in = argv[1]; char* out = argv[2];
	int flags = aiProcessPreset_TargetRealtime_Quality|aiProcess_OptimizeGraph|aiProcess_MakeLeftHanded|aiProcess_FlipUVs;
	flags &= ~aiProcess_SplitLargeMeshes;
	if(!WRITE_MESHES) flags |= aiProcess_OptimizeMeshes;
	const aiScene* scene = aiImportFile(in,flags);
	if(scene){
		std::ofstream file(out, std::ios::out | std::ios::binary | std::ios::trunc);
		if(!file.is_open()){
			std::cout << "Error: Could not write " << out << std::endl; return -1;
		} loadScene(file, scene); file.close();
		aiReleaseImport(scene);
	} return 0;
}