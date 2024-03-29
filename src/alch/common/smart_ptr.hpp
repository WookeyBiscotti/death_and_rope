#pragma once

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <utility>

namespace al {

namespace detail {

struct RefCountSharedData {
	unsigned int shared;
	unsigned int weak;
};

template<class T>
struct PtrData {
	PtrData() = default;
	template<class TT>
	PtrData(const PtrData<TT>& o): d(o.d), p(o.p) {
		static_assert(std::is_same_v<T, TT> || std::is_base_of_v<T, TT>);
	}

	RefCountSharedData* d;
	T* p;
};

template<class T>
void destroyAndReleaseMemory(PtrData<T>& pd) {
	pd.p->~T();
	std::free(pd.d);
}

template<class T>
void callDestructor(PtrData<T>& pd) {
	pd.p->~T();
}

template<class T>
void releaseMemoty(PtrData<T>& pd) {
	std::free(pd.d);
}

template<class T>
struct RefCountSharedDataAnd {
	RefCountSharedData contersBlock;
	T objStorage;
};

} // namespace detail

template<class TT>
class EnableSharedFromThis;

template<class T>
class SharedPtr {
	template<class TT>
	friend class SharedPtr;

	template<class TT>
	friend class WeakPtr;

	template<class TT>
	friend class EnableSharedFromThis;

  public:
	template<class... Args>
	static SharedPtr make(Args&&... args) noexcept {
		SharedPtr sp;
		auto p = std::launder<detail::RefCountSharedDataAnd<T>>(reinterpret_cast<detail::RefCountSharedDataAnd<T>*>(
		    std::aligned_alloc(alignof(detail::RefCountSharedDataAnd<T>), sizeof(detail::RefCountSharedDataAnd<T>))));
		sp._pd.d = &p->contersBlock;
		sp._pd.d->shared = 1;
		sp._pd.d->weak = 0;
		sp._pd.p = &p->objStorage;

		processEnableIf(&sp, &p->objStorage);

		new (sp._pd.p) T(std::forward<Args>(args)...);
		return sp;
	}

  private:
	template<class TT>
	static void processEnableIf(SharedPtr* sp, EnableSharedFromThis<TT>* p) {
		p->_pd = sp->_pd;
	}

	static void processEnableIf(...) {}

  public:
	template<class TT>
	SharedPtr<TT> cast() const {
		auto ptr = SharedPtr<TT>();
		ptr._pd.d = _pd.d;
		ptr._pd.p = static_cast<TT*>(_pd.p);
		if (_pd.d && _pd.d->shared != 0) {
			_pd.d->shared++;
		}

		return ptr;
	}

	SharedPtr(): _pd{} {}
	SharedPtr(std::nullptr_t): SharedPtr() {}
	SharedPtr(detail::PtrData<T> o): _pd(o) {}

	template<class TT>
	SharedPtr(const SharedPtr<TT>& other) noexcept: _pd(other._pd) {
		static_assert(std::is_same_v<T, TT> || std::is_base_of_v<T, TT>);
		if (_pd.d && _pd.d->shared != 0) {
			_pd.d->shared++;
		}
	}

	template<class TT>
	SharedPtr(SharedPtr<TT>&& other) noexcept: _pd(other._pd) {
		static_assert(std::is_same_v<T, TT> || std::is_base_of_v<T, TT>);
		other._pd = {};
	}

	template<class TT>
	auto& operator=(const SharedPtr<TT>& other) noexcept {
		static_assert(std::is_same_v<T, TT> || std::is_base_of_v<T, TT>);

		if (this->_pd.d == other._pd.d) {
			return *this;
		}

		reset();

		_pd = other._pd;

		if (_pd.d) {
			_pd.d->shared++;
		}

		return *this;
	}

	template<class TT>
	auto& operator=(SharedPtr<TT>&& other) noexcept {
		static_assert(std::is_same_v<T, TT> || std::is_base_of_v<T, TT>);

		if (this->_pd.d == other._pd.d) {
			return *this;
		}

		reset();

		_pd = other._pd;
		other._pd = {};

		return *this;
	}

	void reset() {
		assert(!_pd.d || _pd.d->shared > 0);
		if (_pd.d && --_pd.d->shared == 0) {
			if (_pd.d->weak == 0) {
				destroyAndReleaseMemory(_pd);
			} else {
				_pd.p->~T();
			}
		}
		_pd = {};
	}

	~SharedPtr() { reset(); }

	operator bool() const { return _pd.d; }

	T& operator*() { return *_pd.p; }

	// T* operator->() { return _pd.p; }
	T* operator->() const { return _pd.p; }

	T* get() const { return _pd.p; }

	unsigned int useCount() const { return _pd.d ? _pd.d->shared : 0; }

	bool unique() const { return useCount() <= 1; }

	auto& operator=(const SharedPtr& other) { return operator=<T>(other); };
	auto& operator=(SharedPtr&& other) { return operator=<T>(std::move(other)); };

	SharedPtr(const SharedPtr& other) noexcept: _pd(other._pd) {
		if (_pd.d && _pd.d->shared != 0) {
			_pd.d->shared++;
		}
	}
	SharedPtr(SharedPtr&& other) noexcept: _pd(other._pd) { other._pd = {}; }

  private:
	detail::PtrData<T> _pd{};
};

template<class T>
class WeakPtr {
	template<class TT>
	friend class EnableSharedFromThis;

  public:
	~WeakPtr() {
		if (_pd.d && _pd.d->shared == 0 && --_pd.d->weak == 0) {
			releaseMemoty(_pd);
		}
	}

	WeakPtr(): _pd{} {}

	WeakPtr(std::nullptr_t): WeakPtr() {}

	template<class TT>
	WeakPtr(const SharedPtr<TT>& sp) noexcept: _pd(sp._pd) {
		if (_pd.d) {
			_pd.d->weak++;
		}
	}

	template<class TT>
	WeakPtr(const WeakPtr<TT>& wp) noexcept: _pd(wp._pd) {
		if (_pd.d) {
			_pd.d->weak++;
		}
	}

	template<class TT>
	WeakPtr(WeakPtr<TT>&& sp) noexcept: _pd(sp._pd) {
		sp._pd.d = nullptr;
		sp._pd.p = nullptr;
	}

	template<class TT>
	auto& operator=(const WeakPtr<TT>& wp) {
		if (this->_pd.d == wp._pd.d) {
			return *this;
		}

		if (_pd.d && _pd.d->shared == 0 && --_pd.d->weak == 0) {
			releaseMemoty(_pd);
		}

		_pd = wp._pd;

		if (_pd.d) {
			_pd.d->weak++;
		}

		return *this;
	}

	template<class TT>
	auto& operator=(WeakPtr<TT>&& wp) {
		static_assert(std::is_same_v<T, TT> || std::is_base_of_v<T, TT>);

		if (this->_pd.d == wp._pd.d) {
			return *this;
		}

		if (_pd.d && _pd.d->shared == 0 && --_pd.d->weak == 0) {
			releaseMemoty(_pd);
		}

		_pd = wp._pd;
		wp._pd = {};

		return *this;
	}

	auto lock() const {
		if (_pd.d && _pd.d->shared != 0) {
			_pd.d->shared++;
			return SharedPtr<T>(_pd);
		}

		return SharedPtr<T>();
	}

  private:
	detail::PtrData<T> _pd{};
};

template<class T>
class EnableSharedFromThis {
	template<class TT>
	friend class SharedPtr;

  public:
	SharedPtr<T> sharedFromThis() const {
		SharedPtr<T> ptr;
		ptr._pd = _pd;
		_pd.d->shared++;

		return ptr;
	}

	WeakPtr<T> weakFromThis() const {
		WeakPtr<T> ptr;
		ptr._pd = _pd;
		_pd.d->weak++;

		return ptr;
	}

  private:
	detail::PtrData<T> _pd;
};

} // namespace al

#include "alch/common/test_framework.hpp"

ALCH_TEST("SharedPtr Constructors") {
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
}

ALCH_TEST("SharedPtr, BaseToDerived") {
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
}

ALCH_TEST("SharedPtr, useCount") {
	al::SharedPtr<int> sp;
	IT_IS_TRUE(sp.useCount() == 0);
	IT_IS_TRUE(sp.unique());

	sp = al::SharedPtr<int>::make();
	IT_IS_TRUE(sp.useCount() == 1);
	IT_IS_TRUE(sp.unique());

	{
		al::SharedPtr<int> spArr[10];

		for (auto& s : spArr) {
			s = sp;
		}

		IT_IS_FALSE(sp.unique());
		IT_IS_TRUE(sp.useCount() == 11);
	}

	IT_IS_TRUE(sp.useCount() == 1);
}

ALCH_TEST("SharedPtr, Destructor") {
	struct DD {
		DD(int& i): i(i) {}
		int& i;
		~DD() { i = 0; }
	};
	int i = 1;
	{
		auto sp = al::SharedPtr<DD>::make(i);
		{
			al::SharedPtr<DD> spArr[10];

			for (auto& s : spArr) {
				s = sp;
			}
			IT_IS_TRUE(i == 1);
		}
	}

	IT_IS_TRUE(i == 0);
}

ALCH_TEST("SharedPtr, Weak") {
	auto sp = al::SharedPtr<int>::make();
	al::WeakPtr<int> wp(sp);
	{
		al::SharedPtr<int> spArr[10];

		for (auto& s : spArr) {
			s = sp;
		}
	}
	auto sp2 = wp.lock();

	IT_IS_TRUE(sp2.useCount() == 2);
}
