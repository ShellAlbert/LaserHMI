#include "zbaseui.h"
#include <QEvent>
#include <QDebug>
ZBaseUI::ZBaseUI()
{

}
ZBaseUI::~ZBaseUI()
{

}

void ZBaseUI::ZSloGetDeviceStatus(void)
{

}

ZLabelLineEditCheckBox::ZLabelLineEditCheckBox(QString labelText,QString lineEditText,QString checkBoxText,bool checked,bool checkedVisible)
{
    this->setObjectName("ZLabelFocusLineEditCheckBox");
    this->m_ll=new QLabel;
    this->m_ll->setObjectName("ZLabelFocusLineEditCheckBox_Label");
    this->m_ll->setText(labelText);
    this->m_ll->setAlignment(Qt::AlignCenter);

    this->m_le=new QLineEdit;
    this->m_le->setText(lineEditText);
    this->m_le->setAlignment(Qt::AlignCenter);
    this->m_le->installEventFilter(this);

    this->m_cb=new QCheckBox;
    this->m_cb->setText(checkBoxText);
    this->m_cb->setChecked(checked);
    this->m_cb->setVisible(checkedVisible);

    this->m_hLayout=new QHBoxLayout;
    this->m_hLayout->addWidget(this->m_ll);
    this->m_hLayout->addWidget(this->m_le);
    this->m_hLayout->addWidget(this->m_cb);
    this->setLayout(this->m_hLayout);

}
ZLabelLineEditCheckBox::~ZLabelLineEditCheckBox()
{
    delete this->m_ll;
    delete this->m_le;
    delete this->m_cb;
    delete this->m_hLayout;
}
QString ZLabelLineEditCheckBox::ZGetLineEditText()
{
    return this->m_le->text().trimmed();
}
void ZLabelLineEditCheckBox::ZSetLineEditText(QString data)
{
    this->m_le->setText(data);
}
bool ZLabelLineEditCheckBox::ZIsChecked()
{
    return this->m_cb->isChecked();
}
bool ZLabelLineEditCheckBox::eventFilter(QObject *watched, QEvent *event)
{
    if(watched==this->m_le)
    {
        if(event->type()==QEvent::MouseButtonPress)
        {
            emit this->ZSigLineEditClicked();
            return true;
        }
    }
    return QFrame::eventFilter(watched,event);
}
