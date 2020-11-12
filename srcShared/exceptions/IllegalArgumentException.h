#ifndef ZEUSOPS_EXCEPTIONS_ILLEGALARGUMENTEXCEPTION_H_
#define ZEUSOPS_EXCEPTIONS_ILLEGALARGUMENTEXCEPTION_H_

#include "exceptions/BaseException.h"
#include "exceptions/ExceptionMacros.h"

ZEUSOPS_NEW_EXCEPTION(IllegalArgumentExceptionImpl)
#define IllegalArgumentException() IllegalArgumentExceptionImpl(__FILE__, __LINE__)

#endif /* ZEUSOPS_EXCEPTIONS_ILLEGALARGUMENTEXCEPTION_H_ */
