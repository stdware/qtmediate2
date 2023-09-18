#ifndef ICONCOLORIMPL_H
#define ICONCOLORIMPL_H

#include <QPen>
#include <QPushButton>
#include <QToolButton>

#include <QMNamespace.h>

namespace IconColorImpl {

    void correctIconStateAndColor(QIcon &icon, QM::ClickState state, const QString &salt,
                                  const std::function<QString()> &getColor);

    void getTextColor(QPen &pen, const QSize &size, const std::function<void(QPainter *)> &paint);

    QString defaultSalt(QObject *obj);

    QM::ClickState getButtonClickState(QAbstractButton *button);

};

#endif // ICONCOLORIMPL_H
