#include "Transform.h"

Transform::Transform() : update(false), Position(0.0f),Rotation(),Scale(1.0f),Matrix(1.0f)
{
}

Transform::Transform(glm::vec3& Pos, glm::quat& Rot, glm::vec3& Scl) : update(false), Position(Pos),Rotation(Rot),Scale(Scl)
{
	makeMat();
}

void Transform::makeMat()
{
	Matrix = glm::scale(glm::translate(glm::mat4(1.0f),Position) * glm::toMat4(Rotation),Scale);
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