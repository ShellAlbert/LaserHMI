#ifndef ZLOGFILTERDIALOG_H
#define ZLOGFILTERDIALOG_H
#include "base/zdatetimechooser.h"
#include "ui/zframe.h"
#include "ui/ztextcombobox.h"
#include <QGridLayout>
#include <QDialog>
#include <QToolButton>
#include <QLabel>
#include <QVBoxLayout>
class ZLogFilterDialog : public QDialog
{
    Q_OBJECT
public:
    ZLogFilterDialog(QString title);
    ~ZLogFilterDialog();
private:
    QLabel *m_llTitle;
    ZTextComboBox *m_cbType;
    ZDateTimeChooser *m_beginTime;
    ZDateTimeChooser *m_endTime;
    QToolButton *m_tbOkay;
    QToolButton *m_tbCancel;
    QHBoxLayout *m_hLayout;
    QVBoxLayout *m_vLayout;
};

#endif // ZLOGFILTERDIALOG_H
