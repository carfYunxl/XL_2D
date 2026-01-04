#ifndef __HF_SCENE_HPP__
#define __HF_SCENE_HPP__

#include "XL_Core.hpp"
#include "XL_Entity.hpp"

_NAMESPACE_BEGIN

class Scene
{
public:
	Scene();
	~Scene() {};

	Entity CreateEntity(int index);

	void DestroyEntity(Entity entity,bool destroy = true);

	entt::registry& Registry() { return m_Registry; }

private:
	entt::registry	m_Registry;

	friend class HF_Entity;
};

_NAMESPACE_END
#endif //__HF_SCENE_HPP__