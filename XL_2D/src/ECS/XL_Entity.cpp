#include "XL_Entity.hpp"
#include "XL_Scene.hpp"

_NAMESPACE_BEGIN
Entity::Entity(entt::entity handle, Scene* scene)
	: m_EntityHandle(handle), m_Scene(scene)
{
}
_NAMESPACE_END