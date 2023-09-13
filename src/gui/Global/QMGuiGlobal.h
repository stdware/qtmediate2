#ifndef QMGUIGLOBAL_H
#define QMGUIGLOBAL_H

#include <QtGlobal>

// clang-format off

#ifndef QMGUI_EXPORT
#  ifdef QMGUI_STATIC
#    define QMGUI_EXPORT
#  else
#    ifdef QMGUI_LIBRARY
#      define QMGUI_EXPORT Q_DECL_EXPORT
#    else
#      define QMGUI_EXPORT Q_DECL_IMPORT
#    endif
#  endif
#endif

// clang-format on

#endif // QMGUIGLOBAL_H
