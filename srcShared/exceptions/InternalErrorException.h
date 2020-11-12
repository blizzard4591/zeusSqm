#ifndef ZEUSOPS_EXCEPTIONS_INTERNALERROREXCEPTION_H_
#define ZEUSOPS_EXCEPTIONS_INTERNALERROREXCEPTION_H_

#include "exceptions/BaseException.h"
#include "exceptions/ExceptionMacros.h"

ZEUSOPS_NEW_EXCEPTION(InternalErrorExceptionImpl)
#define InternalErrorException() InternalErrorExceptionImpl(__FILE__, __LINE__)
 
#endif
