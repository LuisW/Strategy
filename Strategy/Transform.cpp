#include "Transform.h"

Transform::Transform() : update(false), Position(0.0f),Rotation(),Scale(1.0f),Matrix(1.0f)
{
}

Transform::Transform(const glm::vec3& Pos, const glm::quat& Rot, const glm::vec3& Scl) : update(false), Position(Pos),Rotation(Rot),Scale(Scl)
{
	makeMat();
}

void Transform::makeMat() const
{
	Matrix = glm::mat4(glm::toMat3(Rotation));
	Matrix[0] *= Scale.x;
	Matrix[1] *= Scale.y;
	Matrix[2] *= Scale.z;
	Matrix[3] = glm::vec4(Position, 1.0f);
}

void Transform::makePRS()
{
	glm::mat4 M = Matrix;
	Scale = glm::vec3(glm::length(Matrix[0]),glm::length(Matrix[1]),glm::length(Matrix[2]));
	M[0] /= Scale.x;
	M[1] /= Scale.y;
	M[2] /= Scale.z;
	Rotation = glm::quat_cast(M);
	Position = glm::vec3(M[3]);
}