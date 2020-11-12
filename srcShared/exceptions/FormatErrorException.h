#ifndef ZEUSOPS_EXCEPTIONS_FORMATERROREXCEPTION_H_
#define ZEUSOPS_EXCEPTIONS_FORMATERROREXCEPTION_H_

#include "exceptions/BaseException.h"
#include "exceptions/ExceptionMacros.h"

ZEUSOPS_NEW_EXCEPTION(FormatErrorExceptionImpl)
#define FormatErrorException() FormatErrorExceptionImpl(__FILE__, __LINE__)

#endif
