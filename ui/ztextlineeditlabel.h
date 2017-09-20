#ifndef ZTEXTLINEEDITLABEL_H
#define ZTEXTLINEEDITLABEL_H

#include <QFrame>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QLabel>
class ZTextLineEditLabel : public QFrame
{
    Q_OBJECT
public:
    ZTextLineEditLabel(QString tipMsg,QString data,Qt::Orientation orien=Qt::Horizontal);
    ZTextLineEditLabel(QString tipMsg,QString data1,QString data2,QPixmap img,Qt::Orientation orien=Qt::Horizontal);
    ~ZTextLineEditLabel();

    void ZSetTips(QString tipMsg);
    void ZSetLineEdit(QString data);
    void ZSetLineEdit2(QString data);
    void ZSetImg(QPixmap img);

    QString ZGetLineEdit();
    QString ZGetLineEdit2();

    void ZSetBackgroundColor2Error();
    void ZSetBackgroundColor2Normal();
protected:
    bool eventFilter(QObject *watched, QEvent *event);
signals:
    void ZSigClicked();
private:
    QLabel *m_llTips;
    QLineEdit *m_leData;
    QLineEdit *m_leData2;
    QLabel *m_llImg;
    QHBoxLayout *m_hLayout;
    QVBoxLayout *m_vLayout;
    Qt::Orientation m_orien;
};

#endif // ZTEXTLINEEDITLABEL_H
