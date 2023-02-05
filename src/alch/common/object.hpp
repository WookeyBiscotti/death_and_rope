#pragma once

#include <alch/common/type_id.hpp>
//
#include <type_name/type_name.hpp>
//
#include <string_view>
#include <type_traits>

namespace al {

class VarOArchive;
class VarIArchive;

class Object {
  public:
	virtual void save(VarOArchive& archive) const = 0;
	virtual void load(VarIArchive& archive) = 0;

	virtual std::string_view objName() const = 0;
	virtual type_id_t objType() const = 0;
};

#define AL_OBJECT_REGISTER()                                      \
	std::string_view objName() const override {                   \
		return type_name_v<std::remove_cvref_t<decltype(*this)>>; \
	}                                                             \
	type_id_t objType() const override {                          \
		return TypeId<std::remove_cvref_t<decltype(*this)>>();    \
	}

} // namespace al
