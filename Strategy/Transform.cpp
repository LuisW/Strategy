#include "Transform.h"

Transform::Transform() : m_update(false), m_position(0.0f),m_rotation(),m_scale(1.0f),m_matrix(1.0f)
{
}

Transform::Transform(const glm::vec3& Pos, const glm::quat& Rot, const glm::vec3& Scl) : m_update(false), m_position(Pos),m_rotation(Rot),m_scale(Scl)
{
	makeMat();
}

void Transform::makeMat() const
{
	m_matrix = glm::mat4(glm::toMat3(m_rotation));
	m_matrix[0] *= m_scale.x;
	m_matrix[1] *= m_scale.y;
	m_matrix[2] *= m_scale.z;
	m_matrix[3] = glm::vec4(m_position, 1.0f);
}

void Transform::makePRS()
{
	glm::mat4 M = m_matrix;
	m_scale = glm::vec3(glm::length(m_matrix[0]),glm::length(m_matrix[1]),glm::length(m_matrix[2]));
	M[0] /= m_scale.x;
	M[1] /= m_scale.y;
	M[2] /= m_scale.z;
	m_rotation = glm::quat_cast(M);
	m_position = glm::vec3(M[3]);
}