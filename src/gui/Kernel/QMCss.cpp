#include "QMCss.h"

#include <QRegularExpression>

#include <private/qcssparser_p.h>

#include <QMCore/QMBatch.h>
#include <QMCore/QMNamespace.h>

#include "QPixelSize.h"

namespace QMCss {

    static QStringList extractFunctionToStringList(const QString &str, bool *ok) {
        int leftParen = str.indexOf('(');
        int rightParen = str.lastIndexOf(')');
        QStringList res;
        if (leftParen > 0 && rightParen > leftParen) {
            if (ok) {
                *ok = true;
            }
            res = QStringList({str.mid(0, leftParen), str.mid(leftParen + 1, rightParen - leftParen - 1)});
        } else {
            if (ok) {
                *ok = false;
            }
        }
        return res;
    }

    /*!
        Parse a list of size values separated by comma.
     */
    QList<int> parseSizeValueList(const QString &s) {
        const auto &valueList = parseStringValueList(s);
        QList<int> res;
        for (const auto &i : valueList) {
            QString s = i.simplified();

            // Remove trailing "px"
            if (s.endsWith("px", Qt::CaseInsensitive)) {
                s.chop(2);
            }

            bool isNum;
            auto num = s.toInt(&isNum);
            if (isNum) {
                res.push_back(num);
            } else {
                res.push_back(0);
            }
        }
        return res;
    }

    /*!
        Parse a list of size values using floating point precision separated by comma.
     */
    QList<double> parseSizeFValueList(const QString &s) {
        const auto &valueList = parseStringValueList(s);
        QList<double> res;
        for (const auto &i : valueList) {
            QString s = i.simplified();

            // Remove trailing "px"
            if (s.endsWith("px", Qt::CaseInsensitive)) {
                s.chop(2);
            }

            bool isNum;
            auto num = s.toDouble(&isNum);
            if (isNum) {
                res.push_back(num);
            } else {
                res.push_back(0);
            }
        }
        return res;
    }

    /*!
        Parse a list of expressions separated by comma.

        Support sublists, comments, and quotes in expressions.
     */
    QStringList parseStringValueList(const QString &s) {
        QStringList res;
        int level = 0;
        QString word;

        bool isCommented = false;
        bool isQuoted = false;
        bool isSingleQuoted = false;
        for (int i = 0; i < s.size(); ++i) {
            const QChar &ch = s.at(i);
            if (!isCommented) {
                if (!isSingleQuoted && ch == '\"') {
                    isQuoted = !isQuoted;
                    word.append(ch);
                    continue;
                } else if (!isQuoted && ch == '\'') {
                    isSingleQuoted = !isSingleQuoted;
                    word.append(ch);
                    continue;
                }
            }

            // The comments may appear in the quoted text, parse after we know the char is not quote sign
            if (!isQuoted && !isSingleQuoted && i < s.size() - 1) {
                QChar nxt = s.at(i + 1);
                if (ch == '/' && nxt == '*') {
                    isCommented = true;
                    i++;
                    continue;
                } else if (ch == '*' && nxt == '/') {
                    isCommented = false;
                    i++;
                    continue;
                }
            }

            if (!isCommented && !isQuoted && !isSingleQuoted) {
                if (level == 0 && ch == ',') {
                    res.append(word.trimmed());
                    word.clear();
                } else {
                    if (ch == '(') {
                        level++;
                    } else if (ch == ')') {
                        level--;
                    }
                    word.append(ch);
                }
            }
        }

        word = word.trimmed();
        if (!word.isEmpty()) {
            res.append(word);
        }

        return res;
    }

    /*!
        Parse boolean value from text, case insensitive.

        The valid string can be \c true or \c false.
     */
    bool parseBoolean(const QString &s, bool *ok) {
        // TRUE
        if (s.compare("true", Qt::CaseInsensitive) == 0) {
            if (ok)
                *ok = true;
            return true;
        }

        // FALSE
        if (s.compare("false", Qt::CaseInsensitive) == 0) {
            if (ok)
                *ok = true;
            return true;
        }

        if (ok)
            *ok = false;
        return false;
    }

    /*!
        Parse color value from text in CSS format.
     */
    QColor parseColor(const QString &s) {
        if (s.isEmpty()) {
            return Qt::transparent;
        }

        // Use the feature of QCssParser
        QCss::Declaration dec;
        QCss::Value val;

        bool ok;
        QStringList valueList = extractFunctionToStringList(s, &ok);
        if (ok) {
            val.type = QCss::Value::Function;
            val.variant = valueList;
        } else {
            val.type = QCss::Value::String;
            val.variant = s;
        }

        dec.d->values.push_back(val);
        return dec.colorValue();
    }

    /*!
        Returns the CSS format of color.
     */
    QString colorName(const QColor &color) {
        if (color.alpha() == 255) {
            return color.name();
        }
        return QString("rgba(%1, %2, %3, %4)")
            .arg(color.red())   //
            .arg(color.green()) //
            .arg(color.blue())  //
            .arg(color.alpha());
    }

    /*!
        Returns the index of equal sign if the text is a assign expression matching <tt>key=value</tt>

        The key must consist of letters, numbers, <tt>_</tt> or <tt>-</tt>
     */
    int indexOfEqSign(const QString &s) {
        for (int i = 0; i < s.size(); ++i) {
            const auto &ch = s.at(i);
            if (ch == '=')
                return i;
            if (!ch.isLetterOrNumber() && ch != '_' && ch != '-')
                return -1;
        }
        return -1;
    }

    /*!
        \enum FallbackOption
        \brief Fallback strategy of parsing argument list.

        \var FallbackOption::FO_Value
        \brief Use the specified string as the fallback

        \var FallbackOption::FO_Reference
        \brief Use the argument which the specified string refers to as the fallback
    */

    /*!
        Returns a mapping table from names to arguments parsed from the text.

        \param s                Text that is a list of expressions separated by comma
        \param expectedKeys     Expected argument name in order of positional argument
        \param fallbacks        The fallback of arguments if not found
    */
    QHash<QString, QString> parseArgList(const QString &s, const QStringList &expectedKeys,
                                         const QHash<QString, QPair<QString, FallbackOption>> &fallbacks) {
        if (expectedKeys.isEmpty())
            return {};

        QHash<QString, QString> res;
        auto valueList = parseStringValueList(s);
        bool isKeywordArg = false;
        for (int i = 0; i < valueList.size(); ++i) {
            auto item = valueList.at(i).trimmed();
            int eq = indexOfEqSign(item);
            if (eq < 0) {
                // Positional argument
                if (isKeywordArg || valueList.size() <= i) {
                    // Not allowed
                    return {};
                }
                res.insert(expectedKeys.at(i), QMBatch::strRemoveSideQuote(item.trimmed()));
                continue;
            }

            // Keyword argument
            isKeywordArg = true;
            res.insert(item.left(eq).trimmed(), QMBatch::strRemoveSideQuote(item.mid(eq + 1).trimmed()));
        }

        if (!fallbacks.isEmpty()) {
            // Handle fallbacks
            for (const auto &key : expectedKeys) {
                if (res.contains(key))
                    continue;

                auto it = fallbacks.find(key);
                if (it == fallbacks.end())
                    continue;

                const auto &fallback = it.value();
                if (fallback.second == FO_Value) {
                    // Use value
                    res.insert(key, fallback.first);
                } else {
                    // Use reference
                    res.insert(key, res.value(fallback.first));
                }
            }
        }
        return res;
    }

    /*!
        Returns values parsed from the expression string which is in the following format.

        Format: <tt>up=A, over=B, down=C, disabled=D, up2=E, over2=F, down2=G, disabled2=H</tt>.
    */
    bool parseButtonStateList(const QString &s, QString arr[], bool resolveFallback) {

        QHash<QString, QPair<QString, FallbackOption>> fallbacks;
        if (resolveFallback) {
            fallbacks = {
                {"over",      {"up", FO_Reference}   },
                {"down",      {"over", FO_Reference} },
                {"disabled",  {"up", FO_Reference}   },
                {"up2",       {"up", FO_Reference}   },
                {"over2",     {"up2", FO_Reference}  },
                {"down2",     {"over2", FO_Reference}},
                {"disabled2", {"up2", FO_Reference}  },
            };
        }

        auto res0 = parseArgList(s,
                                 {
                                     "up",
                                     "over",
                                     "down",
                                     "disabled",
                                     "up2",
                                     "over2",
                                     "down2",
                                     "disabled2",
                                 },
                                 fallbacks);
        if (res0.isEmpty())
            return false;

        arr[QM::ButtonNormal] = res0.value("up");
        arr[QM::ButtonHover] = res0.value("over");
        arr[QM::ButtonPressed] = res0.value("down");
        arr[QM::ButtonDisabled] = res0.value("disabled");
        arr[QM::ButtonNormalChecked] = res0.value("up2");
        arr[QM::ButtonHoverChecked] = res0.value("over2");
        arr[QM::ButtonPressedChecked] = res0.value("down2");
        arr[QM::ButtonDisabledChecked] = res0.value("disabled2");

        return true;
    }

}

/*!
    \class QMCssType
    \brief Static class managing the bidirectional mapping of type id and name for parsing QVariant value from string.
*/

static const char *colorFuncNames[] = {
    "rgb",
    "rgba",
    "hsv",
    "hsvl",
};

using MetaTypeHash1 = QHash<int, std::string>;
using MetaTypeHash2 = QHash<std::string, int>;

Q_GLOBAL_STATIC(MetaTypeHash1, m_types1); // Index: id   -> name
Q_GLOBAL_STATIC(MetaTypeHash2, m_types2); // Index: name -> id

/*!
    Register an id-name pair to the global map, returns true if the operation succeeds.

    The \c id and \c name is cannot be the same as any of the previously registered types.
*/
bool QMCssType::registerMetaTypeName(int id, const std::string &name) {
    if (m_types1->contains(id) || m_types2->contains(name))
        return false;
    m_types1->insert(id, name);
    m_types2->insert(name, id);
    return true;
}

/*!
    Unregister an id-name pair to the global map, searched by id, returns true if the operation succeeds.
*/
bool QMCssType::unregisterMetaTypeName(int id) {
    auto it = m_types1->find(id);
    if (it == m_types1->end())
        return false;
    m_types1->erase(it);
    return true;
}

/*!
    Unregister an id-name pair to the global map, searched by name, returns true if the operation succeeds.
*/
bool QMCssType::unregisterMetaTypeName(const std::string &name) {
    auto it = m_types2->find(name);
    if (it == m_types2->end())
        return false;
    m_types2->erase(it);
    return true;
}

/*!
    Returns the type name of a registered type id, null if not found.
*/
std::string QMCssType::metaTypeName(int id) {
    auto it = m_types1->find(id);
    if (it == m_types1->end())
        return nullptr;
    return it->data();
}

/*!
    Returns the type id of a registered type name, null if not found.
*/
int QMCssType::metaTypeId(const std::string &name) {
    auto it = m_types2->find(name);
    if (it == m_types2->end())
        return -1;
    return it.value();
}

/*!
    Parse variant from text, support QtMediate style variants.
 */
QVariant QMCssType::parse(const QString &s) {
    bool ok;
    QStringList valueList = QMCss::extractFunctionToStringList(s, &ok);
    if (ok) {
        // format: func(a, b, ...)
        const auto &func = valueList.front();
        int id = metaTypeId(func.toStdString().data());
        if (id >= 0) {
            QVariant var(valueList);
            if (var.convert(id)) {
                return var;
            } else {
                return s;
            }
        }

        // format: rgba(xxx)
        // Assume QColor
        for (const auto &item : qAsConst(colorFuncNames)) {
            if (func == item) {
                return QMCss::parseColor(s);
            }
        }
        return s;
    }

    // format: #XXXXXX
    if (s.startsWith("#")) {
        return QMCss::parseColor(s);
    }

    if (s.endsWith("px", Qt::CaseInsensitive)) {
        // format: Apx Bpx
        // Assume QSize
        if (s.contains(' ')) {
            QRegularExpression regex(R"((\d+)px?\s+(\d+)px?)");
            QRegularExpressionMatch match = regex.match(s);

            QSize size;
            if (match.hasMatch()) {
                QString widthString = match.captured(1);
                QString heightString = match.captured(2);
                int width = widthString.toInt();
                int height = heightString.toInt();
                size = QSize(width, height);
            }
            return size;
        }

        // format: Apx
        // Assume QPixelSize
        {
            QVariant var;
            var.setValue(QPixelSize::fromString(s));
            return var;
        }
    }

    // Assume number
    {
        bool ok2;
        double val = s.toDouble(&ok2);
        if (ok2) {
            return val;
        }
        return s;
    }
}