#ifndef XL_COMPONENTS_H_
#define XL_COMPONENTS_H_

#include "XL_Scene.hpp"

_NAMESPACE_BEGIN

struct QuadComponent
{

};

struct CircleComponent
{

};

struct LineCompomemt
{

};

template<typename T, typename... Args>
T& AddComponent(Scene* m_Scene, entt::entity handle, Args&&... args)
{
	return m_Scene->Registry().emplace<T>(handle, std::forward<Args>(args)...);
}

template<typename T>
T& GetComponent(Scene* m_Scene, entt::entity handle)
{
	return m_Scene->Registry().get<T>(handle);
}

template<typename T>
bool HasComponent(Scene* m_Scene, entt::entity handle)
{
	return m_Scene->Registry().all_of<T>(handle);
}

template<typename T>
void RemoveComponent(Scene* m_Scene, entt::entity handle)
{
	m_Scene->Registry().remove<T>(handle);
}

_NAMESPACE_END
#endif //XL_COMPONENTS_H_