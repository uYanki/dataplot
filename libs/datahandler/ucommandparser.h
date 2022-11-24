#ifndef UCOMMANDPARSER_H
#define UCOMMANDPARSER_H

/**************** 指令解析器 ****************/

#include <QDebug>

#include <QByteArray>
#include <QMap>
#include <QVector>

// command example: funcName @parmName1 parmValue @parmName2 parmValu1e "parmValue2" ...
// - funcName: 函数名需以字母开头且位于指令头, 字母 a-z A-Z, 数字 0-9, 下划线 _, 点 .
// - parmName: 参数名需以@开头, 字母 a-z A-Z，数字 0-9，下划线 _
// - parmValues: 参数值需跟在参数名后，多个参数值则以空格进行隔开，若要包含空格则使用双引号括起来

#define HANDLER_TYPE std::function<void(const QByteArray& funcName, const QMap<QByteArray /*parmName*/, QVector<QByteArray> /*parmValues*/>& params)>

#define HANDLER_FROM_CLASS(_class, _func, _obj) std::bind(&_class::_func, _obj, std::placeholders::_1, std::placeholders::_2)  // 绑定类成员函数

// #define bytes(content,start,end) QByteArray::fromRawData(content.constData() + start, end - start)

class uCommandParser {
public:
    uCommandParser() {}
    ~uCommandParser() {}

    bool bind(const QByteArray& funcName, HANDLER_TYPE handler);
    bool parse(const QByteArray& in);

private:
    QMap<QByteArray, HANDLER_TYPE> mHandlers;

    char *s, *p;
    int   len;

    bool skipWhiteSpace();
    bool scanFuncName();
    bool scanParmName();
    bool scanParmValue();
};

int getMiddleBytes(
    const QByteArray& _in,
    QByteArray&       _out,
    const QByteArray& left,
    const QByteArray& right,
    int               pos_start = 0);

bool convertBytesToDoubleVector(
    const QByteArray& _in,
    QVector<double>&  _out,
    const QByteArray& separator = QByteArray(","));

#endif  // UCOMMANDPARSER_H
