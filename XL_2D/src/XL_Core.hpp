#ifndef XL_CORE_HPP_
#define XL_CORE_HPP_

#include "glad/glad.h"
#include "glm/glm.hpp"
#include <memory>
#include <string>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <filesystem>
#include <iostream>
#include <fstream>
#include <cstdint>
#include <array>
#include <vector>

#include "entt.hpp"

#define _NAMESPACE_BEGIN   namespace XL{
#define _NAMESPACE_END     }

#ifdef XL_BUILD_DLL
	#define XL_API __declspec(dllexport)
#else
	#define XL_API __declspec(dllimport)
#endif

_NAMESPACE_BEGIN
static constexpr size_t MaxBatchVertices = 131072;
static void XL_uSleep(int uSec)
{
	auto start = std::chrono::system_clock::now();
	while (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - start).count() < uSec);
}

static void XL_mSleep(int mSec)
{
	XL_uSleep(mSec * 1000);
}

template<typename T>
size_t GetVertexSize(const T& v)
{
	return v.GetVertexSize();
}

template<typename T>
void EnableVertexAttribPointer(const T& v)
{
	v.EnableVertexAttribPointer();
}

enum class VertexType
{
	None = 0,
	BatchQuadVertex,
	BatchCircleVertex,
	BatchLineVertex
};
_NAMESPACE_END
#endif//XL_CORE_HPP_
