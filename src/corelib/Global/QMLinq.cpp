#include "QMLinq.h"

/*!
    \namespace QMLinq
    \brief QtMediate collection utility (C# style) namespace.
*/

namespace QMLinq {

    /*!
        \fn Array<T> Where(const Array<T> &list, Selector selector)

        \brief Returns a new collection containing all elements matching the predicate.
    */

    /*!
        \fn Array2<T> Where(const Array1<T> &list, Selector selector)

        \brief Returns a new collection containing all elements matching the predicate.
    */

    /*!
        \fn Array<V> Select(const Array<T> &list, Mapper mapper)

        \brief Returns a new collection of the mapped values of all elements.
    */

    /*!
        \fn Array2<V> Select(const Array1<T> &list, Mapper mapper)

        \brief Returns a new collection of the mapped values of all elements.
    */

    /*!
        \fn bool Any(const Array<T> &list, Validator validator)

        \brief Returns true if any of the element matches the predicate, otherwise returns false.
    */

    /*!
        \fn bool All(const Array<T> &list, Validator validator)

        \brief Returns true if all of the elements match the predicate, otherwise returns false.
    */

    /*!
        \fn void ForEach(const Array<T> &list, Func func)

        \brief Call \c func for each of the element.
    */

}