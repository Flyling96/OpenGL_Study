#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor; 
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aNormal;
layout (location = 4) in vec3 aTangent;
layout (location = 5) in vec3 aBitangent;
layout (location = 6) in ivec4 aBoneIDs;
layout (location = 7) in vec4 aBoneWeight;

out vec3 FragPos;
out vec2 TexCoord;
out vec3 Normal;
out vec3 Tangent;
out vec3 Bitangent;


const int MAX_BONES = 100;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 gBones[MAX_BONES];

void main()
{

	mat4 BoneTransform = gBones[aBoneIDs[0]] * aBoneWeight[0];
    BoneTransform     += gBones[aBoneIDs[1]] * aBoneWeight[1];
    BoneTransform     += gBones[aBoneIDs[2]] * aBoneWeight[2];
    BoneTransform     += gBones[aBoneIDs[3]] * aBoneWeight[3];

    gl_Position = projection * view * model *  BoneTransform * vec4(aPos, 1.0);

	Normal =  vec3(transpose(inverse(model)) * BoneTransform * vec4(aNormal,1.0));
    TexCoord = aTexCoord;    
	Tangent =  mat3(transpose(inverse(model))) *aTangent;
	Bitangent =  mat3(transpose(inverse(model))) *aBitangent;
	FragPos = vec3(model * BoneTransform * vec4(aPos, 1.0));
}