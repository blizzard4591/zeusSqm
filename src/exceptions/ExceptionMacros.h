#ifndef ZEUSOPS_EXCEPTIONS_EXCEPTIONMACROS_H_
#define ZEUSOPS_EXCEPTIONS_EXCEPTIONMACROS_H_

/*!
 * Macro to generate descendant exception classes. As all classes are nearly the same, this makes changing common
 * features much easier.
 */
#define ZEUSOPS_NEW_EXCEPTION(exception_name) namespace zeusops { \
namespace exceptions { \
class exception_name : public BaseException { \
public: \
exception_name(char const* file, int line) : BaseException(file, line) { \
} \
exception_name(char const* file, int line, char const* cstr) : BaseException(file, line, cstr) { \
} \
exception_name(exception_name const& cp) : BaseException(cp) { \
} \
virtual ~exception_name() throw() { \
} \
template<typename T> \
exception_name& operator<<(T const& var) { \
	this->stream << var; \
	return *this; \
} \
virtual const char* name() const NOEXCEPT override { \
	return #exception_name; \
} \
}; \
} \
}

#endif /* ZEUSOPS_EXCEPTIONS_EXCEPTIONMACROS_H_ */
