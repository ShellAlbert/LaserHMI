#ifndef ZCHILLERLOGUI_H
#define ZCHILLERLOGUI_H

#include "base/zbaseui.h"
#include "ui/zframe.h"
#include <QLabel>
#include <QComboBox>
#include <QToolButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTreeWidget>
#include <QProgressBar>
class ZChillerLogUI : public ZBaseUI
{
    Q_OBJECT
public:
    ZChillerLogUI();
    ~ZChillerLogUI();

    void ZDoInitial();
    void ZUnInitial();

    void ZFetchLogs();
private slots:
    void ZSloFilter();
    void ZSloExport();
    void ZSloDelete();
private:

    QToolButton *m_tbFilter;
    QToolButton *m_tbExport;
    QToolButton *m_tbDelete;
    QToolButton *m_tbPrevious;
    QToolButton *m_tbNext;
    QComboBox *m_cbPages;
    QHBoxLayout *m_hLayoutFilter;
    QTreeWidget *m_treeWidget;
    QVBoxLayout *m_vLayoutLog;
    ZFrame *m_frmLog;

    QLabel *m_llStorage;
    QProgressBar *m_progressBar;
    QHBoxLayout *m_hLayoutStorage;
    ZFrame *m_frmStorage;

    QGridLayout *m_gLayoutMain;
};

#endif // ZCHILLERLOGUI_Hl
