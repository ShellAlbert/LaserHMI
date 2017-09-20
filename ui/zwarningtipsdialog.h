#ifndef ZWARNINGTIPSDIALOG_H
#define ZWARNINGTIPSDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QToolButton>
#include <QVBoxLayout>
#include <QFrame>
class ZWarningTipsDialog : public QDialog
{
    Q_OBJECT
public:
    ZWarningTipsDialog(QString title,QString tipsMsg);
    ~ZWarningTipsDialog();
private:
    QLabel *m_llTitle;
    QLabel *m_llTipsMsg;
    QToolButton *m_tbOkay;
    QFrame *m_frm;
    QVBoxLayout *m_vLayoutFrm;
    QVBoxLayout *m_vLayout;
};

#endif // ZWARNINGTIPSDIALOG_H
