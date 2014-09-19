#include "Behaviours.h"
#include "ei/glm/gtc/quaternion.hpp"

glm::quat AISafeSlerpStep(const glm::quat& x, const glm::quat& y, float maxStep, bool& finished)
{
	float phi = glm::dot(x, y);

	phi = glm::clamp(phi, -1.0f, 1.0f);
	phi = glm::acos(phi);

	if (phi <= maxStep)
	{
		finished = true;
		return y;
	}

	finished = false;

	return (glm::sin(phi - maxStep) * x + glm::sin(maxStep) * y) / glm::sin(phi);
}

glm::quat AIFindQuatToTarget(const glm::vec3& target, Transform& trans)
{
	glm::vec3 front = trans.getRot() * CreatureFront;
	glm::vec3 a = glm::cross(front, target);
	float w = 1.0f + glm::dot(front, target);

	if (w < 1e-06f)
	{
		return glm::angleAxis(glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	return glm::normalize(glm::quat(w, a));
}

float AIStepTowardsTarget(const glm::vec3& target, float dist, Transform& trans)
{
	glm::vec3 distv = target - trans.getPos();
	float fullDist = glm::length(distv);

	if (dist >= fullDist || fullDist == 0.0f)
	{
		trans.setPos(target);
		return 0.0f;
	}
	else
	{
		trans.setPos(trans.getPos() + dist * distv / fullDist);
		return fullDist - dist;
	}
}

void AIStepInDirection(const glm::vec3& dir, float dist, Transform& trans)
{
	trans.setPos(trans.getPos() + dist * dir);
}

#pragma region AIRotator

AIRotator::AIRotator() : done(false), currRot(), progress(0.0f), start()
{
}

void AIRotator::setTarget(glm::vec3& target, Transform& trans)
{
	currRot = AIFindQuatToTarget(target, trans);

	start = trans.getRot();
	progress = 0.0f;
	done = false;
}

bool AIRotator::isDone()
{
	return done;
}

void AIRotator::step(float angle, Transform& trans)
{
	if (isDone())
		return;

	progress += angle;
	trans.setRot(start * AISafeSlerpStep(glm::quat(), currRot, progress, done));
}

#pragma endregion
