#include "XL_Scene.hpp"
#include "XL_Components.hpp"

_NAMESPACE_BEGIN
Scene::Scene() {
}

Entity Scene::CreateEntity(int index)
{
	auto ent = m_Registry.create();
	
	return { ent, this };
}

void Scene::DestroyEntity(Entity entity, bool destroy /*= true*/)
{
	/*if (HasComponent<>(this, entity.GetHandleID()))
	{
		RemoveComponent<>(this, entity.GetHandleID());
	}*/

	if(destroy)
		m_Registry.destroy(entity.GetHandleID());
}
_NAMESPACE_END