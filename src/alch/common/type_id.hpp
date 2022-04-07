#pragma once

// inline int TYPE_ID_SEQ = 0;

// template<typename T>
// inline constexpr int TypeId = TYPE_ID_SEQ++;

// template<typename T>
// size_t TypeId() {
// 	static int x;
// 	return (size_t)(&x);
// }

namespace detail {

/**
 * Template class that hold the declaration of the id.
 *
 * We use the pointer of this id as type id.
 */
template<typename T>
struct type_id_ptr {
	// Having a static data member will ensure (I hope) that it has only one address for the whole program.
	// Furthermore, the static data member having different types will ensure (I hope) it won't get optimized.
	static const T* const id;
};

/**
 * Definition of the id.
 */
template<typename T>
const T* const type_id_ptr<T>::id = nullptr;

} // namespace detail

/**
 * The type of a type id.
 */
using type_id_t = const void*;

/**
 * The function that returns the type id.
 *
 * It uses the pointer to the static data member of a class template to achieve this.
 * Altough the value is not predictible, it's stable (I hope).
 */
template<typename T>
constexpr auto TypeId() noexcept -> type_id_t {
	return &detail::type_id_ptr<T>::id;
}
