QT       += core gui serialport network printsupport bluetooth

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    3rdparty/QCustomPlot/qcustomplot.cpp \
    area_transfer.cpp \
    bits_tool.cpp \
    formlayout.cpp \
    libs/datahandler/uabstractdatachannel.cpp \
    libs/datahandler/uabstractdatatransmitter.cpp \
    libs/datahandler/ucommandparser.cpp \
    libs/dataplotter/datagraph.cpp \
    libs/dataplotter/dataplot.cpp \
    libs/dataplotter/fft4.cpp \
    libs/qdocktabwidget/qdockpopup.cpp \
    libs/qdocktabwidget/qdocktabbar.cpp \
    libs/qdocktabwidget/qdocktabwidget.cpp \
    libs/getsysteminfo/devicewatcher.cpp \
    libs/getsysteminfo/getlocalip.cpp \
    libs/widgetutils/input.cpp \
    libs/widgetutils/qmenuplus.cpp \
    libs/widgetutils/savefile.cpp \
    libs/widgetutils/setwidgetsprop.cpp \
    main.cpp \
    mainwindow.cpp \
    page_cmdformat.cpp \
    page_datafilter.cpp \
    page_subrecv.cpp \
    src/channels/chanblclient.cpp \
    src/channels/chanserialport.cpp \
    src/channels/chantcpclient.cpp \
    src/channels/chantcpserver.cpp \
    src/channels/datatransmitter.cpp \
    treeitem_datafilter.cpp

HEADERS += \
    3rdparty/QCustomPlot/qcustomplot.h \
    area_transfer.h \
    bits_tool.h \
    formlayout.h \
    libs/datahandler/uabstractdatachannel.h \
    libs/datahandler/uabstractdatatransmitter.h \
    libs/datahandler/ucommandparser.h \
    libs/datahandler/udatafilter.h \
    libs/dataplotter/datagraph.h \
    libs/dataplotter/dataplot.h \
    libs/dataplotter/fft4.h \
    libs/qdocktabwidget/qdockpopup.h \
    libs/qdocktabwidget/qdocktabbar.h \
    libs/qdocktabwidget/qdocktabwidget.h \
    libs/getsysteminfo/devicewatcher.h \
    libs/getsysteminfo/getlocalip.h \
    libs/widgetutils/input.h \
    libs/widgetutils/qmenuplus.h \
    libs/widgetutils/savefile.h \
    libs/widgetutils/setwidgetsprop.h \
    mainwindow.h \
    page_cmdformat.h \
    page_datafilter.h \
    page_subrecv.h \
    src/channels/chanblclient.h \
    src/channels/chanserialport.h \
    src/channels/chantcpclient.h \
    src/channels/chantcpserver.h \
    src/channels/datatransmitter.h \
    treeitem_datafilter.h

FORMS += \
    area_transfer.ui \
    mainwindow.ui \
    page_cmdformat.ui \
    page_datafilter.ui \
    page_subrecv.ui \
    src/channels/chanblclient.ui \
    src/channels/chanserialport.ui \
    src/channels/chantcpclient.ui \
    src/channels/chantcpserver.ui \
    src/channels/datatransmitter.ui

INCLUDEPATH += \
    ./src/channels \
    ./libs/datahandler \
    ./libs/dataplotter \
    ./libs/qdocktabwidget \
    ./libs/getsysteminfo \
    ./libs/widgetutils \
    ./3rdparty/QCustomPlot

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    libs/datahandler/.vscode/settings.json
