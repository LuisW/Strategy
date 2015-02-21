#pragma once

#include "Transform.h"
#include <stack>
#include <stdexcept>

const glm::vec3 CreatureFront(-1.0f, 0.0f, 0.0f);

glm::quat AISafeSlerpStep(const glm::quat& x, const glm::quat& y, float maxStep, bool& finished);
glm::quat AIFindQuatToTarget(const glm::vec3& target, Transform& trans);
glm::quat AIFindQuatToTarget(const glm::vec3& target, Transform& trans, const glm::vec3& _front);
glm::quat AIFindQuatToTarget(const glm::vec3& target, const glm::vec3& worldFront);

float AIStepTowardsTarget(const glm::vec3& target, float dist, Transform& trans);
void AIStepInDirection(const glm::vec3& dir, float dist, Transform& trans);

glm::vec3 transformToLocalSpace(glm::vec3 pos, Transform& trans);

class AIRotator
{
private:
	glm::quat start;
	glm::quat currRot;
	float progress;
	bool done;

public:
	AIRotator();
	void setTarget(const glm::vec3& target, Transform& trans, const glm::vec3& front = CreatureFront);
	bool isDone();
	void step(float angle, Transform& trans);
};

class AIArea
{
private:
	glm::vec3 pyramid[4];
	glm::vec2 radius;
	glm::vec3 target;
	float targetRange;

public:
	AIArea(glm::dvec2 _radius, glm::dvec2 _phi, glm::dvec2 _theta) : radius(_radius), target(0.0f)
	{
		if (_radius.x > _radius.y || _phi.x > _phi.y || _theta.x > _theta.y)
		{
			throw std::invalid_argument("One of the following requirements doesn´t hold: r.x < r.y; phi.x < phi.y; theta.x < theta.y");
		}

		if (_radius.y < 0.0f || _phi.x < -(2.0 * M_PI + M_Err) || _theta.x < -M_Err || _theta.y >(M_PI + M_Err) || _phi.y >(2.0 * M_PI + M_Err))
		{
			throw std::invalid_argument("One of the following requirements doesn´t hold: r >= 0; 0 <= phi <= 2Pi; 0 <= theta <= Pi");
		}

		glm::vec3 vecs[4];
		glm::dvec2 sinphi = glm::sin(_phi);
		glm::dvec2 sintheta = glm::sin(_theta);
		glm::dvec2 cosphi = glm::cos(_phi);
		glm::dvec2 costheta = glm::cos(_theta);

		vecs[0] = glm::vec3(sintheta.x*cosphi.x, costheta.x, sintheta.x*sinphi.x);
		vecs[1] = glm::vec3(sintheta.x*cosphi.y, costheta.x, sintheta.x*sinphi.y);
		vecs[2] = glm::vec3(sintheta.y*cosphi.y, costheta.y, sintheta.y*sinphi.y);
		vecs[3] = glm::vec3(sintheta.y*cosphi.x, costheta.y, sintheta.y*sinphi.x);

		for (int n = 0; n < 4; n++)
		{
			pyramid[n] = glm::cross(vecs[n], vecs[(n + 1) % 4]);
			target -= pyramid[n];
		}

		if (glm::dot(target, target) < M_Err)
		{
			target = CreatureFront;
		}
		else
		{
			target = glm::normalize(target);
		}

		double range;

		if (_radius.y < INFINITY)
		{
			double sin_theta = glm::sin(_theta.y + _theta.x);
			double sin_theta2 = glm::sin((_theta.y - _theta.x) * 0.5);
			double sin_phi = glm::sin((_phi.y - _phi.x) * 0.5);

			double r13 = radius.x*radius.x*radius.x;
			double r23 = radius.y*radius.y*radius.y;

			range = 3.0*(r23*_radius.y - r13*_radius.x)* sin_phi *(_theta.y - _theta.x + sin_theta);
			range /= 8.0*(r23 - r13) * (_phi.y - _phi.x) * sin_theta2;

			if (range < _radius.x)
			{
				range = (_radius.x + _radius.y) * 0.5;
			}
		}
		else
		{
			range = _radius.x + 1.0f;
		}

		targetRange = (float)range;

		radius *= radius;
	}

	bool pointInside(const glm::vec3& p) const
	{
		for (int n = 0; n < 4; n++)
		{
			if (glm::dot(pyramid[n], p) > 1e-06f)
				return false; 
		}

		float d2 = glm::dot(p, p);

		if (d2 < radius.x || d2 > radius.y)
		{
			return false;
		}

		return true;
	}
	
	inline const glm::vec3& getTargetingDir() const
	{
		return target;
	}

	inline float getTargetingRange() const
	{
		return targetRange;
	}
};

const AIArea FullAOE = AIArea(glm::dvec2(0.0, INFINITY), glm::dvec2(0.0, 2.0*M_PI), glm::dvec2(0.0, M_PI));

struct AIPlanElement
{
	unsigned int Ability;
	bool running;

	AIPlanElement(unsigned int _ability, bool _running) : Ability(_ability), running(_running)
	{}
};

typedef std::stack<AIPlanElement> AIPlan;

enum AIAbilityState { AAS_Running, AAS_Finished, AAS_Aborted, AAS_Precast };