#ifndef ZEUSOPS_EXCEPTIONS_CONFIGNOTFOUNDEXCEPTION_H_
#define ZEUSOPS_EXCEPTIONS_CONFIGNOTFOUNDEXCEPTION_H_

#include "exceptions/BaseException.h"
#include "exceptions/ExceptionMacros.h"

ZEUSOPS_NEW_EXCEPTION(ConfigNotFoundExceptionImpl)
#define ConfigNotFoundException() ConfigNotFoundExceptionImpl(__FILE__, __LINE__)

#endif
