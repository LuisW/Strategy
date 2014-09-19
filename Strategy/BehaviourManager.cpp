#include "Behaviours.h"

BehaviourManager::BehaviourManager()
{
	behaviours.resize(BT_Unused);

	addBehaviour<Idle_Scan>(BT_Idle);
	addBehaviour<Idle_Roam>(BT_Idle);
	addBehaviour<RangeSetup_CirclePlayer>(BT_RangedSetup);
}

IBehaviour* BehaviourManager::getNewInstanceOf(BehaviourID ID, glm::vec3& location)
{
	return behaviours[ID.getType()][ID.getIndex()].mkNew(location);
}