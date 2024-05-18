#pragma once

#ifndef ESTL_USE_EXCEPTIONS
    #define ESTL_USE_EXCEPTIONS 1
    // Alternatives 0 : Don't use execptions
#endif

#if (ESTL_USE_EXCEPTIONS == 1)
    #define ESTL_THROW(exception_type, message) throw exception_type(message)
#else
    #define ESTL_THROW(exception_type, message)
#endif


