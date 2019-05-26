#pragma once
// 
// From: https://bartoszmilewski.com/2014/04/21/getting-lazy-with-c/
//

#include <functional>

template<class T>
class Susp
{
	// thunk
	static T const& thunkForce(Susp* susp) {
		return susp->setMemo();
	}
	// thunk
	static T const& thunkGet(Susp* susp) {
		return susp->getMemo();
	}
	T const& getMemo() {
		return _memo;
	}
	T const& setMemo() {
		_memo = _f();
		_thunk = &thunkGet;
		return getMemo();
	}
public:
	explicit Susp(std::function<T()> f)
		: _f(f), _thunk(&thunkForce), _memo(T())
	{}
	T const& get() {
		return _thunk(this);
	}
private:
	T const& (*_thunk)(Susp*);
	mutable T   _memo;

	std::function<T()> _f;
};

template<class T> class Cell;

template<class T>
class Stream
{
private:
	std::shared_ptr <Susp<Cell<T>>> _lazyCell;
public:
	Stream() {}
	Stream(std::function<Cell<T>()> f)
		: _lazyCell(std::make_shared<Susp<Cell<T>>>(f))
	{}
	Stream(Stream&& stm)
		: _lazyCell(std::move(stm._lazyCell))
	{}
	Stream(const Stream& stm)
		: _lazyCell(stm._lazyCell)
	{}
	Stream& operator=(Stream&& stm)
	{
		_lazyCell = std::move(stm._lazyCell);
		return *this;
	}
	bool isEmpty() const
	{
		return !_lazyCell;
	}
	T get() const
	{
		return _lazyCell->get().val();
	}
	Stream<T> pop_front() const
	{
		return _lazyCell->get().pop_front();
	}

	Stream<T> take(int n) const {
		if (n == 0 || isEmpty())
			return Stream();
		auto cell = _lazyCell;
		return Stream([cell, n]()
			{
				auto v = cell->get().val();
				auto t = cell->get().pop_front();
				return Cell<T>(v, t.take(n - 1));
			});
	}
	
	//template<class F>
	//void forEach(Stream<T> strm, F f)
	//{
	//	while (!strm.isEmpty())
	//	{
	//		f(strm.get());
	//		strm = strm.pop_front();
	//	}
	//}

	//Stream<T> mjoin(Stream<Stream<T>> stm)
	//{
	//	while (!stm.isEmpty() && stm.get().isEmpty())
	//	{
	//		stm = stm.pop_front();
	//	}
	//	if (stm.isEmpty()) return Stream<T>();
	//	return Stream<T>([stm]()
	//		{
	//			Stream<T> hd = stm.get();
	//			return Cell<T>(hd.get()
	//				, concat(hd.pop_front(), mjoin(stm.pop_front())));
	//		});
	//}

	//template<class F>
	//auto mbind(Stream<T> stm, F f) -> decltype(f(stm.get()))
	//{
	//	return mjoin(fmap(stm, f));
	//}

	//template<class F>
	//auto fmapv(Stream<T> stm, F f)->Stream<decltype(f())>
	//{
	//	using U = decltype(f());
	//	static_assert(std::is_convertible<F, std::function<U()>>::value,
	//		"fmapv requires a function type U()");

	//	if (stm.isEmpty()) return Stream<U>();
	//	return Stream<U>([stm, f]()
	//		{
	//			return Cell<U>(f(), fmapv(stm.pop_front(), f));
	//		});
	//}

	//template<class F>
	//auto mthen(Stream<T> stm, F f) -> decltype(f())
	//{
	//	return mjoin(fmapv(stm, f));
	//}

	//template<class T>
	//Stream<T> mreturn(T v)
	//{
	//	return Stream<T>([v]() {
	//		return Cell<T>(v);
	//		});
	//}

};

template<class T>
class Cell
{
public:
	Cell() {} // need default constructor for memoization
	Cell(T v, Stream<T> const& tail)
		: _v(v), _tail(tail)
	{}
	explicit Cell(T v) : _v(v) {}
	T val() const {
		return _v;
	}
	Stream<T> pop_front() const {
		return _tail;
	}

private:
	T _v;
	Stream<T> _tail;
};


template<class T, class F>
void forEach(Stream<T> strm, F f)
{
	while (!strm.isEmpty())
	{
		f(strm.get());
		strm = strm.pop_front();
	}
}

template<class T, class F>
auto fmap(Stream<T> stm, F f)->Stream<decltype(f(stm.get()))>
{
	using U = decltype(f(stm.get()));
	static_assert(std::is_convertible<F, std::function<U(T)>>::value,
		"fmap requires a function type U(T)");
	if (stm.isEmpty()) return Stream<U>();
	return Stream<U>([stm, f]()
		{
			return Cell<U>(f(stm.get()), fmap(stm.pop_front(), f));
		});
}

//template<class T>
//Stream<T> mjoin(Stream<Stream<T>> stm)
//{
//	while (!stm.isEmpty() && stm.get().isEmpty())
//	{
//		stm = stm.pop_front();
//	}
//	if (stm.isEmpty()) return Stream<T>();
//	return Stream<T>([stm]()
//		{
//			Stream<T> hd = stm.get();
//			return Cell<T>(hd.get()
//				, concat(hd.pop_front(), mjoin(stm.pop_front())));
//		});
//}

//template<class T, class F>
//auto mbind(Stream<T> stm, F f) -> decltype(f(stm.get()))
//{
//	return mjoin(fmap(stm, f));
//}

//template<class T, class F>
//auto fmapv(Stream<T> stm, F f)->Stream<decltype(f())>
//{
//	using U = decltype(f());
//	static_assert(std::is_convertible<F, std::function<U()>>::value,
//		"fmapv requires a function type U()");
//
//	if (stm.isEmpty()) return Stream<U>();
//	return Stream<U>([stm, f]()
//		{
//			return Cell<U>(f(), fmapv(stm.pop_front(), f));
//		});
//}
//
//template<class T, class F>
//auto mthen(Stream<T> stm, F f) -> decltype(f())
//{
//	return mjoin(fmapv(stm, f));
//}
//
//template<class T>
//Stream<T> mreturn(T v)
//{
//	return Stream<T>([v]() {
//		return Cell<T>(v);
//		});
//}

