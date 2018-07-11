#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>

//Entity Component System

class Component;
class Entity;
class Manager;

using ComponentID = std::size_t;
using Group = std::size_t;
using subGroup = std::size_t;


inline ComponentID getNewComponentTypeID() {
	static ComponentID lastID = 0u; //unsigned
	return lastID++;
}

template <typename T> inline ComponentID getComponentTypeID() noexcept {
	
	static ComponentID typeID = getNewComponentTypeID();
	return typeID;
}

constexpr std::size_t maxComponents = 32;
constexpr std::size_t maxGroups = 32;
constexpr std::size_t maxSubGroups = 32;

using ComponentBitSet = std::bitset<maxComponents>;
using GroupBitset = std::bitset<maxGroups>;
using SubGroupBitset = std::bitset<maxSubGroups>;

using ComponentArray = std::array<Component*, maxComponents>;

class Component {
public:
	Entity* entity;
	
	virtual void init() {};
	virtual void update() {};
	virtual void draw() {};

	virtual ~Component() {};

};

class Entity {
private:
	Manager& manager;
	bool active = true;
	std::vector<std::unique_ptr<Component>> components;

	ComponentArray componentArray;
	ComponentBitSet componentBitSet;
	GroupBitset groupBitset;
	SubGroupBitset subGroupBitset;

public:
	Entity(Manager& mManager) : manager(mManager) {} //Manager = mManager

	void update() {
		for (auto& c : components) c->update();
	}

	void draw() {
		for (auto& c : components) c->draw();
	};

	bool isActive() const { return active; }

	void destroy() { active = false; }

	bool hasGroup(Group mGroup) {
		return groupBitset[mGroup];
	}

	bool hasSubGroup(subGroup mSubGroup) {
		return subGroupBitset[mSubGroup];
	}

	void addGroup(Group mGroup);

	void addSubGroup(subGroup mSubGroup);

	void delGroup(Group mGroup) {
		groupBitset[mGroup] = false;
	}
	void delSubGroup(subGroup mSubGroup) {
		subGroupBitset[mSubGroup] = false;
	}

	template <typename T> bool hasComponent() const {
		return componentBitSet[getComponentTypeID<T>()];
	}

	template <typename T, typename... TArgs> T& addComponent(TArgs&&... mArgs) {
		T* c(new T(std::forward<TArgs>(mArgs)...));
		c->entity = this;
		std::unique_ptr<Component> uPtr{ c };
		components.emplace_back(std::move(uPtr));

		componentArray[getComponentTypeID<T>()] = c;
		componentBitSet[getComponentTypeID<T>()] = true;

		c->init();
		return *c;
	}

	template<typename T> T& getComponent() const {
		auto ptr(componentArray[getComponentTypeID<T>()]);
		return *static_cast<T*>(ptr);
	}

};

class Manager {
private:
	std::vector<std::unique_ptr<Entity>> entities;
	std::array<std::vector<Entity*>, maxGroups> groupedEntities; //32 Arrays wil have a Vector of a pointer to each entity in the Manager
	std::array<std::vector<Entity*>, maxSubGroups> subgroupedEntities;

public:
	void update() {
		for (auto& e : entities) e->update();
	}

	void draw() {
		for (auto& e : entities) e->draw();
	}

	void refresh() {
		for (auto i(0u); i < maxGroups; i++){
			auto& v(groupedEntities[i]); //reference for every group
			v.erase(
				std::remove_if(std::begin(v), std::end(v),
					[i](Entity* mEntity) {
				return !mEntity->isActive() || !mEntity->hasGroup(i);
			}),
				std::end(v));
		}

		entities.erase(std::remove_if(std::begin(entities), std::end(entities), [](const std::unique_ptr<Entity> &mEntity) {
			return !mEntity->isActive();
		}), std::end(entities));
	}

	void AddToGroup(Entity* mEntity, Group mGroup) {
		groupedEntities[mGroup].emplace_back(mEntity);
	}

	void AddToSubGroup(Entity* mEntity, subGroup mSubGroup) {
		subgroupedEntities[mSubGroup].emplace_back(mEntity);
	}

	std::vector<Entity*>& getGroup(Group mGroup) {
		return groupedEntities[mGroup];
	}

	std::vector<Entity*>& getSubGroup(subGroup mSubGroup) {
		return subgroupedEntities[mSubGroup];
	}

	Entity& addEntity() {
		Entity* e = new Entity(*this);
		std::unique_ptr<Entity> uPtr{ e };
		entities.emplace_back(std::move(uPtr));
		return *e;
	}

};