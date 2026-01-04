#ifndef __HF_ENTITY_HPP__
#define __HF_ENTITY_HPP__

#include "XL_Core.hpp"
#include "entt.hpp"

_NAMESPACE_BEGIN
class Scene;

class Entity
{
public:
	Entity() = default;
	Entity(entt::entity handle, Scene* scene);
	Entity(const Entity& other) = default;

	operator bool() const { return m_EntityHandle != entt::null; }

	uint32_t toInt() const { return static_cast<uint32_t>(m_EntityHandle); }

	bool operator==(const Entity& other) {
		return (*this).m_EntityHandle == other.m_EntityHandle && (*this).m_Scene == other.m_Scene;
	}

	bool operator!=(const Entity& other) {
		return !operator==(other);
	}

	entt::entity GetHandleID() const { return m_EntityHandle; }

	bool isValid() const { return m_EntityHandle != entt::null; }

	Scene* GetScene() const { return m_Scene; }
private:
	entt::entity	m_EntityHandle{ entt::null };
	Scene*		m_Scene{ nullptr };
};

_NAMESPACE_END
#endif //__HF_ENTITY_HPP__