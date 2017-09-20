#-------------------------------------------------
#
# Project created by QtCreator 2017-04-25T13:34:47
#
#-------------------------------------------------

QT       += core gui sql network serialport charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LaserHMI
TEMPLATE = app

SOURCES += main.cpp\
        base/zbaseobj.cpp \
    base/zbaseui.cpp \
    ui/zbarui.cpp \
    ui/ztopbar.cpp \
    ui/zbottombar.cpp \
    ui/znavigateui.cpp \
    ui/zmainui.cpp \
    ui/zaboutui.cpp \
    ui/zmainctlui.cpp \
    ui/zlaserui.cpp \
    ui/zmdlctlui.cpp \
    ui/zframe.cpp \
    ui/ztextimglabel.cpp \
    ui/zlinelabel.cpp \
    ui/zloginui.cpp \
    ui/zchillerui.cpp \
    ui/ztextlineeditlabel.cpp \
    ui/log/zoperatelogui.cpp \
    base/zdatetimechooser.cpp \
    ui/log/zlaserlogui.cpp \
    ui/log/zchillerlogui.cpp \
    ui/zlogui.cpp \
    ui/log/zlogmanager.cpp \
    db/zbasedb.cpp \
    db/zsqlitedb.cpp \
    gbl/hmi_const.cpp \
    ui/zloadui.cpp \
    ringbuffer/zringbuffer.cpp \
    ci/zbaseci.cpp \
    ci/zlitronicsci.cpp \
    ci/crc16.cpp \
    ui/zvslider.cpp \
    ui/zcurveui.cpp \
    ui/zsummaryui.cpp \
    ui/ztextcombobox.cpp \
    ui/log/zlogfilterdialog.cpp \
    ui/zdbgtestui.cpp \
    ui/zcheckbox.cpp \
    ui/zspinbox.cpp \
    ui/zshutdwndia.cpp \
    ui/zsyssetupui.cpp \
    base/zpiechart.cpp \
    ui/zdigkbd.cpp \
    ui/zfullkbd.cpp \
    dump/zdbgserver.cpp \
    ui/zpwrprecisionsetdialog.cpp \
    ui/zdatetimesetdia.cpp \
    ui/zupdatewizarddialog.cpp \
    ui/zlaserrunhoursui.cpp \
    ui/znetworkui.cpp \
    ui/zwaitingdialog.cpp \
    ui/zcolortemptuningdia.cpp \
    ui/zprogressbar.cpp \
    ui/zcircleindicator.cpp \
    ui/zverticalindicator.cpp \
    ui/zwarningtipsdialog.cpp

HEADERS  += base/zbaseobj.h \
    base/zbaseui.h \
    ui/zbarui.h \
    ui/ztopbar.h \
    ui/zbottombar.h \
    ui/znavigateui.h \
    ui/zmainui.h \
    ui/zaboutui.h \
    gbl/hmi_const.h \
    ui/zmainctlui.h \
    ui/zlaserui.h \
    ui/zmdlctlui.h \
    ui/zframe.h\
    ui/ztextimglabel.h \
    ui/zlinelabel.h \
    ui/zloginui.h \
    ui/zchillerui.h \
    ui/ztextlineeditlabel.h \
    ui/log/zoperatelogui.h \
    base/zdatetimechooser.h \
    ui/log/zlaserlogui.h \
    ui/log/zchillerlogui.h \
    ui/zlogui.h \
    ui/log/zlogmanager.h \
    db/zbasedb.h \
    db/zsqlitedb.h \
    ui/zloadui.h \
    ringbuffer/zringbuffer.h \
    ci/zbaseci.h \
    ci/zlitronicsci.h \
    ci/crc16.h \
    ui/zvslider.h \
    ui/zcurveui.h \
    ui/zsummaryui.h \
    ui/ztextcombobox.h \
    ui/log/zlogfilterdialog.h \
    ui/zdbgtestui.h \
    ui/zcheckbox.h \
    ui/zspinbox.h \
    ui/zshutdwndia.h \
    ui/zsyssetupui.h \
    base/zpiechart.h \
    ui/zdigkbd.h \
    ui/zfullkbd.h \
    dump/zdbgserver.h \
    ui/zpwrprecisionsetdialog.h \
    ui/zdatetimesetdia.h \
    ui/zupdatewizarddialog.h \
    ui/zlaserrunhoursui.h \
    ui/znetworkui.h \
    ui/zwaitingdialog.h \
    ui/zcolortemptuningdia.h \
    ui/zprogressbar.h \
    ui/zcircleindicator.h \
    ui/zverticalindicator.h \
    ui/zwarningtipsdialog.h

RESOURCES += \
    hmires.qrc

DISTFILES += \
    skin/defaultSkin \
    test_command

INCLUDEPATH += /home/shell.albert/CAMC/6QWeb/qrcode-master/libqr
LIBS += -L/home/shell.albert/CAMC/6QWeb/qrcode-master/build4x64 -lqr
target.path = /HMI
INSTALLS += target
