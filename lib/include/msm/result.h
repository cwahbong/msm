#ifndef MSM__ERROR_H
#define MSM__ERROR_H

#include <stdexcept>
#include <type_traits>
#include <utility>

namespace msm {

enum class Error {
    OK,
    GENERAL,
    BAD_PARAMETER,
    NOT_SUPPORTED,
};

struct Void final {};
constexpr Void VOID = Void();

template <typename T, typename = std::enable_if<!std::is_same<std::remove_reference_t<T>, msm::Error>::value>>
class Result {
public:
    using ValueType = std::remove_reference_t<T>;
    using ErrorType = msm::Error;

    static_assert(!std::is_reference<T>::value, "Reference type T is prohibited.");

    template <typename = std::enable_if<std::is_copy_constructible<ValueType>::value>>
    Result(const ValueType & value) noexcept(std::is_nothrow_constructible<ValueType>::value): _isError(false), _value(value) {}

    template <typename = std::enable_if<std::is_move_constructible<ValueType>::value>>
    Result(ValueType && value) noexcept(std::is_nothrow_move_constructible<ValueType>::value): _isError(false), _value(std::move(value)) {}

    template <typename U, typename = std::enable_if<std::is_convertible<U, ValueType>::value>>
    Result(U && other) noexcept: _isError(false), _value(std::move(other)) {}

    Result(const ErrorType & error) noexcept: _isError(true), _error(error) {}

    Result(ErrorType && error) noexcept: _isError(true), _error(std::move(error)) {}

    template <typename U, typename = std::enable_if<std::is_convertible<U, ValueType>::value && std::is_copy_assignable<ValueType>::value>>
    Result(const Result<U> & other) noexcept(std::is_nothrow_copy_assignable<ValueType>::value): _isError(other.IsError()) {
        if (_isError) {
            _error = other.GetError();
        } else {
            _value = other.GetValue();
        }
    }

    template <typename U, typename = std::enable_if<std::is_convertible<U, ValueType>::value && std::is_move_assignable<ValueType>::value>>
    Result(Result<U> && other) noexcept(std::is_nothrow_move_assignable<ValueType>::value): _isError(other.IsError()) {
        if (_isError) {
            _error = other.GetError();
        } else {
            _value = other.GetValue();
        }
    }

    ~Result() {
        if (_isError) {
            _error.~ErrorType();
        } else {
            _value.~ValueType();
        }
    }

    bool IsError() const noexcept {
        return _isError;
    }

    ErrorType GetError() const noexcept {
        if (_isError) {
            return _error;
        } else {
            return ErrorType::OK;
        }
    }

    const ValueType & GetValue() const {
        if (!_isError) {
            return _value;
        } else {
            throw std::logic_error("bad value to get");
        }
    }

    ValueType && GetValue() {
        if (!_isError) {
            return std::move(_value);
        } else {
            throw std::logic_error("bad value to get");
        }
    }

private:
    bool _isError;
    union {
        ErrorType _error;
        ValueType _value;
    };
};

// Result<void> is prohibited, use Result<Void> instead.
template <> class Result<void>;

} // namespace msm

#define VALUE_OR_RETURN(result) \
    ({ \
        if (result.IsError()) { \
            return result.GetError(); \
        } \
        result.GetValue(); \
    })

#endif //MSM__ERROR_H
