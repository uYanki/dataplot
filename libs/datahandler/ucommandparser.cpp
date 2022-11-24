#include "ucommandparser.h"

bool uCommandParser::bind(const QByteArray& funcName, HANDLER_TYPE handler) {
    if (handler == Q_NULLPTR || mHandlers.value(funcName) != Q_NULLPTR) {
        return false;
    } else {
        mHandlers.insert(funcName, handler);
        // qDebug() << mHandlers.keys();
        return true;
    }
}

bool uCommandParser::parse(const QByteArray& in) {
    p = (char*)(in.constData());

    skipWhiteSpace();

    /////////////// scan function name ///////////////
    if (scanFuncName()) {
        QByteArray funcName = QByteArray::fromRawData(s, len);

        if (!mHandlers.contains(funcName)) {
            qDebug() << "error: " << funcName << "is not bound";  // 函数名未绑定
            return false;
        }

        QMap<QByteArray, QVector<QByteArray>> funcParms;
        QVector<QByteArray>*                  pParmValues = Q_NULLPTR;

        while (true) {
            bool b = skipWhiteSpace();

            if (*p == '\0') {
                /////////////// end of command ///////////////
                // qDebug() << "parse sucessfully";
                // qDebug() << funcName << funcParms;
                // Q_FOREACH (auto key, funcParms.keys())
                //     qDebug() << key << *(funcParms.value(key));
                mHandlers.value(funcName)(funcName, funcParms);  // call handler
                return true;
            } else if (!b) {
                /////////////// error format ///////////////
                qDebug() << p;
                qDebug() << "error format: no whitespace after function name、param name or param value";
                return false;
            }

            switch (*p) {
                /////////////// scan param name ///////////////
                case '@': {
                    if (scanParmName()) {
                        pParmValues = &funcParms.insert(QByteArray::fromRawData(s, len), *new QVector<QByteArray>).value();
                        break;
                    } else {
                        qDebug("error: '%c' can't be the beginning of a param name", *p);
                        return false;
                    }
                }
                /////////////// scan param value ///////////////
                default: {
                    if ((pParmValues != Q_NULLPTR) && scanParmValue()) {
                        pParmValues->append(QByteArray::fromRawData(s, len));
                        break;
                    } else {
                        qDebug("error: '%c' can't be the beginning of a param value", *p);
                        return false;
                    }
                }
            }
        }
    }
    qDebug("error: '%c' can't be the beginning of a function name", *p);
    return false;
}

bool uCommandParser::skipWhiteSpace() {
    s = p;
    while (isspace(*p)) ++p;
    return p > s;
}

bool uCommandParser::scanFuncName() {
    if (isalpha(*p)) {
        s = p++;
        while (*p && (isalpha(*p) || isdigit(*p) || *p == '_' || *p == '.')) ++p;
        len = p - s;
        return true;
    }
    return false;
}
bool uCommandParser::scanParmName() {
    if (isalpha(*++p)) {
        s = p++;
        while (*p && (isalpha(*p) || isdigit(*p) || *p == '_')) ++p;
        len = p - s;
        return true;
    }
    return false;
}
bool uCommandParser::scanParmValue() {
    if (*p == '"') {
        s = ++p;
        while (*p) {
            if (*p++ == '"') {
                len = p - s - 1;
                return true;
            }
        }
        qDebug() << "error: can't find another \"";
        return false;
    } else {
        s = p++;
        while (*p && !isspace(*p)) { ++p; }
        len = p - s;
        return true;
    }
}

int getMiddleBytes(const QByteArray& _in, QByteArray& _out, const QByteArray& left, const QByteArray& right, int pos_start) {
    int pos_left;

    if (left.isEmpty()) {
        pos_left = 0;
    } else {
        pos_left = _in.indexOf(left, pos_start);
        if (pos_left == -1) return -1;
        pos_left += left.size();
    }

    int pos_right;

    if (right.isEmpty()) {
        pos_right = _in.size();
    } else {
        pos_right = _in.indexOf(right, pos_left);
        if (pos_right == -1) return -1;
    }

    _out = QByteArray::fromRawData(_in.constData() + pos_left, pos_right - pos_left);

    return pos_right + right.size();
}

bool convertBytesToDoubleVector(
    const QByteArray& _in,
    QVector<double>&  _out,
    const QByteArray& separator) {
    if (_in.isEmpty()) return false;

    int          start = 0, cursor;
    QVector<int> pos;  // vector of separator position
    pos.append(0);
    while ((cursor = _in.indexOf(separator, start)) != -1) {
        pos.append(cursor);
        pos.append(start = cursor + separator.size());
    }
    pos.append(_in.size());

    bool isOK;
    _out.clear();
    for (int i = 0; i < pos.size(); i += 2) {
        auto buf = QByteArray::fromRawData(_in.constData() + pos[i], pos[i + 1] - pos[i]);
        _out.append(buf.toDouble(&isOK));
        if (!isOK) return false;
    }

    // qDebug() << _out;
    return true;
}
