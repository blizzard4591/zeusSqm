#ifndef ZEUSOPS_EXCEPTIONS_UNSUPPORTEDVERSIONEXCEPTION_H_
#define ZEUSOPS_EXCEPTIONS_UNSUPPORTEDVERSIONEXCEPTION_H_

#include "exceptions/BaseException.h"
#include "exceptions/ExceptionMacros.h"

ZEUSOPS_NEW_EXCEPTION(UnsupportedVersionExceptionImpl)
#define UnsupportedVersionException() UnsupportedVersionExceptionImpl(__FILE__, __LINE__)

#endif /* ZEUSOPS_EXCEPTIONS_UNSUPPORTEDVERSIONEXCEPTION_H_ */
