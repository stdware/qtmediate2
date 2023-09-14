#ifndef QMWIDGETTOOLSGLOBAL_H
#define QMWIDGETTOOLSGLOBAL_H

#include <QtGlobal>

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

#endif // QMWIDGETTOOLSGLOBAL_H
