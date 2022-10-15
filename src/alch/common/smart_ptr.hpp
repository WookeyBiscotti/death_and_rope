#pragma once

#include <cassert>
#include <utility>

namespace al {

struct RefCountSharedData {
	unsigned int shared;
	char data;
};

template<class T>
struct RefCountSharedDataT {
	unsigned int shared;
	char data[sizeof(T)];
};

template<class T>
class SharedPtr {
	template<class TT>
	friend class SharedPtr;

  public:
	template<class... Args>
	static SharedPtr make(Args&&... args) noexcept {
		SharedPtr sp;
		auto p = new RefCountSharedDataT<T>();
		sp._pointer = new (p->data) T(std::forward<Args>(args)...);
		sp._data = reinterpret_cast<RefCountSharedData*>(p);

		p->shared = 1;

		return sp;
	}

	SharedPtr(): _data(nullptr) {}

	template<class TT>
	SharedPtr(const SharedPtr<TT>& other) noexcept: _data(other._data) {
		static_assert(std::is_same_v<T, TT> || std::is_base_of_v<T, TT>);
		if (_data) {
			_data->shared++;
			_pointer = other._pointer;
		}
	}

	template<class TT>
	SharedPtr(SharedPtr<TT>&& other) noexcept: _data(other._data) {
		static_assert(std::is_same_v<T, TT> || std::is_base_of_v<T, TT>);
		other._data = nullptr;
		_pointer = other._pointer;
	}

	auto& operator=(const SharedPtr& other) noexcept {
		if (this->_data == other._data) {
			return *this;
		}

		if (_data && --_data->shared == 0) {
			releaseData();
		}

		_data = other._data;
		_pointer = other._pointer;

		if (_data) {
			_data->shared++;
		}

		return *this;
	}

	template<class TT>
	auto& operator=(SharedPtr<TT>&& other) noexcept {
		static_assert(std::is_same_v<T, TT> || std::is_base_of_v<T, TT>);

		_data = other._data;
		other._data = nullptr;
		_pointer = other._pointer;

		return *this;
	}

	~SharedPtr() {
		if (_data && --_data->shared == 0) {
			releaseData();
		}
	}

	operator bool() const { return _data; }

	T& operator*() { return *_pointer; }

	T* operator->() { return _pointer; }

	T* get() { return _pointer; }

	unsigned int useCount() const { return _data ? _data->shared : 0; }

  private:
	void releaseData() {
		_pointer->~T();
		delete _data;
	}

  private:
	RefCountSharedData* _data;
	T* _pointer;
};

} // namespace al

#include "alch/common/test_framework.hpp"

INLINE_TESTER(SharedPtr, Constructors, {
	IT_IS_FALSE(al::SharedPtr<int>());
	IT_IS_TRUE(al::SharedPtr<int>::make());

	IT_IS_TRUE(*al::SharedPtr<int>::make(77) == 77);

	{
		auto sp1 = al::SharedPtr<int>::make(77);
		al::SharedPtr<int> sp2(sp1);

		IT_IS_TRUE(*sp2 == 77);
		IT_IS_TRUE(sp2.get() == sp1.get());
	}

	{
		auto sp1 = al::SharedPtr<int>::make(77);
		al::SharedPtr<int> sp2;
		sp2 = sp1;

		IT_IS_TRUE(*sp2 == 77);
		IT_IS_TRUE(sp2.get() == sp1.get());
	}

	{
		auto sp1 = al::SharedPtr<int>::make(77);
		al::SharedPtr<int> sp2(std::move(sp1));

		IT_IS_TRUE(sp2);
		IT_IS_FALSE(sp1);
	}

	{
		auto sp1 = al::SharedPtr<int>::make(77);
		al::SharedPtr<int> sp2;
		sp2 = std::move(sp1);

		IT_IS_TRUE(sp2);
		IT_IS_FALSE(sp1);
	}
});

INLINE_TESTER(SharedPtr, BaseToDerived, {
	struct Base {
		int i;
	};
	struct Derived: Base {
		float f;
	};

	auto spd = al::SharedPtr<Derived>::make();
	spd->f = 321;
	spd->i = 777;

	al::SharedPtr<Base> spb;
	spb = spd;
	IT_IS_TRUE(spb->i == 777);
	spb->i = 666;
	IT_IS_TRUE(spb->i == 666);
	IT_IS_TRUE(spd->i == 666);

	IT_IS_TRUE(spd->f == 321);
});

INLINE_TESTER(SharedPtr, useCount, {
	al::SharedPtr<int> sp;
	IT_IS_TRUE(sp.useCount() == 0);

	sp = al::SharedPtr<int>::make();
	IT_IS_TRUE(sp.useCount() == 1);

	{
		al::SharedPtr<int> spArr[10];

		for (auto& s : spArr) {
			s = sp;
		}

		IT_IS_TRUE(sp.useCount() == 11);
	}

	IT_IS_TRUE(sp.useCount() == 1);
});
