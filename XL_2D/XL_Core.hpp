#ifndef __XL_CORE_HPP__
#define __XL_CORE_HPP__

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

#define _NAMESPACE_BEGIN   namespace XL{
#define _NAMESPACE_END     }

#ifdef XL_BUILD_DLL
	#define XL_API __declspec(dllexport)
#else
	#define XL_API __declspec(dllimport)
#endif

#endif//__XL_CORE_HPP__
