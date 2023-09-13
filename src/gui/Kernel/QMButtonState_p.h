#ifndef QMBUTTONSTATE_P_H
#define QMBUTTONSTATE_P_H

//
//  W A R N I N G !!!
//  -----------------
//
// This file is not part of the QtMediate API. It is used purely as an
// implementation detail. This header file may change from version to
// version without notice, or may even be removed.
//

#include <QMCore/QMNamespace.h>
#include <QMGui/QMGuiGlobal.h>

class QMGUI_EXPORT QMButtonStates {
public:
    QMButtonStates();

    void syncState(QM::ButtonState state);
    void syncStates(const QList<QM::ButtonState> &states);

    QM::ButtonState state(QM::ButtonState state) const;

public:
    void setState(QM::ButtonState state); // Without sync
    void syncInternal();

private:
    int m_arr[8];
};

template <class T>
class QMButtonAttributes {
public:
    QMButtonAttributes();

public:
    T value(QM::ButtonState state = QM::ButtonNormal) const;
    void setValue(const T &value, QM::ButtonState state = QM::ButtonNormal);
    void setValues(const QList<T> &values);
    void setValues(const QList<QPair<QM::ButtonState, T>> &values);

    void setValueAll(const T &value);

private:
    T m_values[8];
    QMButtonStates m_state;
};

template <class T>
QMButtonAttributes<T>::QMButtonAttributes() {
}

template <class T>
T QMButtonAttributes<T>::value(QM::ButtonState state) const {
    return m_values[m_state.state(state)];
}

template <class T>
void QMButtonAttributes<T>::setValue(const T &value, QM::ButtonState state) {
    m_values[state] = value;
    m_state.syncState(state);
}

template <class T>
void QMButtonAttributes<T>::setValues(const QList<T> &values) {
    int sz = qMin(values.size(), 8);
    for (int i = 0; i < sz; ++i) {
        m_values[i] = values[i];
        m_state.setState(static_cast<QM::ButtonState>(i));
    }
    m_state.syncInternal();
}

template <class T>
void QMButtonAttributes<T>::setValues(const QList<QPair<QM::ButtonState, T>> &values) {
    for (const QPair<QM::ButtonState, T> &pair : values) {
        m_values[pair.first] = pair.second;
        m_state.setState(static_cast<QM::ButtonState>(pair.first));
    }
    m_state.syncInternal();
}

template <class T>
void QMButtonAttributes<T>::setValueAll(const T &value) {
    for (int i = 0; i < 8; ++i) {
        m_values[i] = value;
    }
    m_state = {};
}

#endif // QMBUTTONSTATE_P_H
