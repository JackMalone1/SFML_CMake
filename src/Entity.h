#pragma once
#include "Globals.h"
#include "entt.hpp"

#include <memory>
#include <string>
#include <LuaBridge.h>

class Entity
{
private:
	entt::registry& _registry;
	entt::entity entity;
	std::string name;
	std::unique_ptr<luabridge::LuaRef> updateFunc;
public:
	Entity() : _registry(registry), updateFunc(nullptr)
	{
		entity = _registry.create();
	}

	~Entity()
	{
		_registry.destroy(entity);
	}

	template<typename... Components>
	void addComponents(Components&&... components)
	{
		(_registry.template emplace<Components>(entity, std::forward<Components>(components)), ...);
	}

	template<typename Component>
	void addComponent(Component&& component)
	{
		_registry.template emplace<Component>(entity, component);
	}

	template<typename Component>
	bool hasComponent()
	{
		return _registry.any_of<Component>(entity);
	}

	template<typename... Components>
	bool hasAnyOfComponents()
	{
		return _registry.any_of<Components...>(entity);
	}

	template<typename... Components>
	bool hasAllComponents()
	{
		return _registry.all_of<Components...>(entity);
	}

	template<typename Component>
	Component& getComponent()
	{
		return _registry.get<Component>(entity);
	}

	template<typename Component>
	void removeComponent()
	{
		_registry.remove<Component>(entity);
	}

	void loadScript(lua_State* L, const std::string& scriptFilename,
		const std::string& tableName)
	{
		using namespace luabridge;
		if (luaL_dofile(L, scriptFilename.c_str()) == 0)
		{
			LuaRef table = getGlobal(L, tableName.c_str());
			if (table.isTable())
			{
				if (table["name"].isString())
				{
					name = table["name"].cast<std::string>();
				}
				else
				{
					name = "Null";
				}

				if (table["update"].isFunction())
				{
					updateFunc = std::make_unique<LuaRef>(table["update"]);
				}
			}
			else
			{
				std::cout << "Error, can't open script" << std::endl;
			}
		}
	}

	void update()
	{
		if (updateFunc) {
			try {
				(*updateFunc)(this);
			}
			catch (luabridge::LuaException const& e) {
				std::cout << "LuaException: " << e.what() << std::endl;
			}
		}
	}
};