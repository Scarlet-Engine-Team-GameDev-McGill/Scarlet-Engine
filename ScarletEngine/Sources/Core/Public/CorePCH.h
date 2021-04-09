#pragma once

/** This is where large headers which we don't expect to change often should go. They will be precompiled on compatible systems */

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

// STL
#include <cstdint>
#include <memory>
#include <vector>
#include <array>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <optional>
#include <mutex>
#include <type_traits>
#include <string>
#include <string_view>
#include <sstream>
#include <functional>
#include <tuple>
#include <map>
#include <compare>
#include <filesystem>
#include <concepts>
#include <ranges>
#include <deque>
#include <queue>


// thirdparty
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Tracy.hpp>
