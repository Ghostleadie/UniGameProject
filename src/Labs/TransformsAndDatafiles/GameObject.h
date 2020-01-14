#pragma once
#include "Component.h"
#include <unordered_map>
#include <typeindex>

class GameObject
{
public:
	std::string name;
	std::string getName() {
		return name;
	}
	const char* navname;
	const char* getnavname() {
		return navname;
	}

	int Fleets;
	std::string listinfo;
	std::string team;
	std::string getTeam() {
		return team;
	}
	int getFleets() {
		return Fleets;
	}

	template <typename T>
	T* getComponent()
	{

		auto iter = m_components.find(typeid(T));

		if(iter != std::end(m_components))
		{
			// if found dynamic cast the component pointer and return it
			return dynamic_cast<T*>(iter->second);
		}
		
		// return null if we don't have a component of that type
		return nullptr;
		
	}

	template <typename T>
	void addComponent(T* comp)
	{
		// add the component to unoreder map with hash of its typeid
		m_components[typeid(T)] = comp;
	}
	
private:
	std::unordered_map<std::type_index, Component*> m_components;
};
