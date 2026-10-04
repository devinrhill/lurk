// Devin Hill 2026

#pragma once

namespace lvk::util {

struct NullOpt_t {};
inline constexpr NullOpt_t NullOpt{};

template<class T>
class Option {
public:
	Option():
		Option(NullOpt)
	{
	}

	Option(NullOpt_t):
		_value{},
		_hasValue{false}
	{
	}

	Option(T value):
		_value{value}
	{
		_hasValue = true;
	}

	Option(const Option& other) {
		_value = other._value;
		_hasValue = other._hasValue;
	}
	
	bool hasValue() const {
		return _hasValue;
	}

	T valueOr() const {
		return _hasValue ? _value : (T)0;
	}

	Option& operator=(T value) {
		_value = value;
		_hasValue = true;

		return *this;
	}

	Option& operator=(NullOpt_t) {
		_value = (T)0;
		_hasValue = false;

		return *this;
	}

protected:
	T _value;
	bool _hasValue;
};

} // namespace lvk::util
