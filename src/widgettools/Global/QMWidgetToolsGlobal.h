#ifndef QMWIDGETTOOLSGLOBAL_H
#define QMWIDGETTOOLSGLOBAL_H

#include <QtGlobal>

// clang-format off

#ifndef QMWTOOLS_EXPORT
#  ifdef QMWTOOLSSTATIC
#    define QMWTOOLS_EXPORT
#  else
#    ifdef QMWTOOLS_LIBRARY
#      define QMWTOOLS_EXPORT Q_DECL_EXPORT
#    else
#      define QMWTOOLS_EXPORT Q_DECL_IMPORT
#    endif
#  endif
#endif

// clang-format on

#endif // QMWIDGETTOOLSGLOBAL_H
