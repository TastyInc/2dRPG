#include "ECS.hpp"

void Entity::addGroup(Group mGroup) {
	groupBitset[mGroup] = true;
	manager.AddToGroup(this, mGroup);

}

void Entity::addSubGroup(subGroup mSubGroup) {
	subGroupBitset[mSubGroup] = true;
	manager.AddToSubGroup(this, mSubGroup);
}