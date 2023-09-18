#include "QMGuiDecoratorV2.h"
#include "QMGuiDecoratorV2_p.h"

QMGuiDecoratorV2Private::QMGuiDecoratorV2Private() {
}

QMGuiDecoratorV2Private::~QMGuiDecoratorV2Private() {
}

void QMGuiDecoratorV2Private::init() {
}

/*!
    \class QMGuiDecoratorV2
    \brief The QMGuiDecoratorV2 class is a derived class from QMCoreDecoratorV2 with nothing to
    extend.
*/

/*!
    Constructor.
*/
QMGuiDecoratorV2::QMGuiDecoratorV2(QObject *parent)
    : QMGuiDecoratorV2(*new QMGuiDecoratorV2Private(), parent) {
}

/*!
    Destructor.
*/
QMGuiDecoratorV2::~QMGuiDecoratorV2() {
}

/*!
    \internal
*/
QMGuiDecoratorV2::QMGuiDecoratorV2(QMGuiDecoratorV2Private &d, QObject *parent)
    : QMCoreDecoratorV2(d, parent) {
    d.init();
}