#ifndef UDATAFILTER_H
#define UDATAFILTER_H

/**************** 数据过滤器 ****************/

#include <QByteArrry>
#include <QString>

enum uDataFilterMode {
    startsWith = 1,
    endsWith,
    contains,
    notStartsWith,
    notEndsWith,
    notContains
};

template <typename T = QByteArray>
bool uDataFilter(
    const T&        content,
    const T&        subContent,
    uDataFilterMode type) {
    switch (type) {
        case startsWith: return content.startsWith(subContent);
        case endsWith: return content.endsWith(subContent);
        case contains: return content.contains(subContent);
        case notStartsWith: return !content.startsWith(subContent);
        case notEndsWith: return !content.contains(subContent);
        case notContains: return !content.contains(subContent);
    }
    return false;
}

#endif  // UDATAFILTER_H
