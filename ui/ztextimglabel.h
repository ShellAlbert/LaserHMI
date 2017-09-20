#ifndef ZTEXTIMGLABEL_H
#define ZTEXTIMGLABEL_H

#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QPixmap>
class ZTextImgLabel:public QFrame
{
    Q_OBJECT
public:
    ZTextImgLabel(QString text);
    ZTextImgLabel(QString text,QPixmap img);
    ~ZTextImgLabel();

    void ZSetText(QString text);
    void ZSetPixmap(QPixmap img);
signals:
    void ZSigClicked();
protected:
    bool eventFilter(QObject *watched, QEvent *event);
private:
    QLabel *m_llText;
    QLabel *m_llImg;
    QHBoxLayout *m_hLayout;
};

#endif // ZTEXTIMGLABEL_H
