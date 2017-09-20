#include "zfullkbd.h"
#include <QKeyEvent>
#include <QApplication>
#include <gbl/hmi_const.h>
ZFullKbd::ZFullKbd(QString title)
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    //default is chinese mode.
    this->m_bChMode=true;

    this->m_llTitle=new QLabel;
    this->m_llTitle->setObjectName("ZFullKbd_Title");
    this->m_llTitle->setText(title);
    this->m_tbOkay=new QToolButton;
    this->m_tbOkay->setText(tr("确认"));
    this->m_tbOkay->setObjectName("ZFullKbdOkayCancel");
    connect(this->m_tbOkay,SIGNAL(clicked(bool)),this,SLOT(accept()));
    this->m_tbCancel=new QToolButton;
    this->m_tbCancel->setText(tr("取消"));
    this->m_tbCancel->setObjectName("ZFullKbdOkayCancel");
    connect(this->m_tbCancel,SIGNAL(clicked(bool)),this,SLOT(reject()));

    this->m_le=new QLineEdit;
    this->m_le->setObjectName("ZFullKbdLineEdit");
    this->m_llInput=new QLabel;
    this->m_llInput->setText("");
    for(qint32 i=0;i<2;i++)
    {
        this->m_tbTurnPage[i]=new QToolButton;
        this->m_tbTurnPage[i]->setObjectName("ZFullKbdTurnPage");
        this->m_tbTurnPage[i]->setIconSize(QSize(32,24));
        this->m_tbTurnPage[i]->setToolButtonStyle(Qt::ToolButtonIconOnly);
        switch(i)
        {
        case 0:
            this->m_tbTurnPage[i]->setIcon(QIcon(":/Kbd/images/Keyboard/previous.png"));
            break;
        case 1:
            this->m_tbTurnPage[i]->setIcon(QIcon(":/Kbd/images/Keyboard/next.png"));
            break;
        default:
            break;
        }
    }
    for(qint32 i=0;i<6;i++)
    {
        this->m_tbChSelect[i]=new QToolButton;
        this->m_tbChSelect[i]->setText(QString("%1.").arg(i+1));
        this->m_tbChSelect[i]->setObjectName("ZFullKbdChSelect");
        connect(this->m_tbChSelect[i],SIGNAL(clicked(bool)),this,SLOT(ZSloChSelectButtonClicked()));
    }
    //A-Z.
    for(qint32 i=0;i<26;i++)
    {
        this->m_tbA2Z[i]=new QToolButton;
        this->m_tbA2Z[i]->setObjectName("ZFullKbdA2Z");
        this->m_tbA2Z[i]->setText(QString("%1").arg(QChar(65+i)));
        connect(this->m_tbA2Z[i],SIGNAL(clicked(bool)),this,SLOT(ZSloA2ZButtonClicked()));
    }
    for(qint32 i=0;i<9;i++)
    {
        this->m_tbFun[i]=new QToolButton;
        switch(i)
        {
        case 0:
            this->m_tbFun[i]->setText(tr("En"));
            this->m_tbFun[i]->setObjectName("ZFullKbdEnCh");
            break;
        case 1:
            this->m_tbFun[i]->setText(tr("Ctrl"));
            this->m_tbFun[i]->setObjectName("ZFullKbdCtrl");
            break;
        case 2:
            this->m_tbFun[i]->setText(tr("Shift"));
            this->m_tbFun[i]->setObjectName("ZFullKbdShift");
            break;
        case 3:
            this->m_tbFun[i]->setText(tr("CapsLock"));
            this->m_tbFun[i]->setObjectName("ZFullKbdCapsLock");
            break;
        case 4:
            this->m_tbFun[i]->setText(tr("Space"));
            this->m_tbFun[i]->setObjectName("ZFullKbdSpace");
            break;
        case 5:
            this->m_tbFun[i]->setText(tr("123"));
            this->m_tbFun[i]->setObjectName("ZFullKbd123");
            break;
        case 6:
            this->m_tbFun[i]->setText(tr("!@#?"));
            this->m_tbFun[i]->setObjectName("ZFullKbdSymbol");
            break;
        case 7:
            this->m_tbFun[i]->setText(tr("Enter"));
            this->m_tbFun[i]->setObjectName("ZFullKbdEnter");
            break;
        case 8:
            this->m_tbFun[i]->setText(tr("<--"));
            this->m_tbFun[i]->setObjectName("ZFullKbdBackspace");
            break;
        default:
            break;
        }
        connect(this->m_tbFun[i],SIGNAL(clicked(bool)),this,SLOT(ZSloFunButtonClicked()));
    }
    //layout.
    for(qint32 i=0;i<7;i++)
    {
        this->m_hLayout[i]=new QHBoxLayout;
        switch(i)
        {
        case 0:
            this->m_hLayout[i]->addWidget(this->m_llTitle);
            this->m_hLayout[i]->addWidget(this->m_le);
            break;
        case 1:
            this->m_hLayout[i]->addWidget(this->m_llInput,1,Qt::AlignLeft);
            break;
        case 2:
            this->m_hLayout[i]->addWidget(this->m_tbTurnPage[0]);
            for(qint32 j=0;j<6;j++)
            {
                this->m_hLayout[i]->addWidget(this->m_tbChSelect[j]);
            }
            this->m_hLayout[i]->addWidget(this->m_tbTurnPage[1]);
            this->m_hLayout[i]->addWidget(this->m_tbOkay);
            this->m_hLayout[i]->addWidget(this->m_tbCancel);
            break;
        case 3:
            this->m_hLayout[i]->addWidget(this->m_tbFun[0]);//En.
            this->m_hLayout[i]->addWidget(this->m_tbA2Z[(qint32)QChar('Q'-'A').toLatin1()]);
            this->m_hLayout[i]->addWidget(this->m_tbA2Z[(qint32)QChar('W'-'A').toLatin1()]);
            this->m_hLayout[i]->addWidget(this->m_tbA2Z[(qint32)QChar('E'-'A').toLatin1()]);
            this->m_hLayout[i]->addWidget(this->m_tbA2Z[(qint32)QChar('R'-'A').toLatin1()]);
            this->m_hLayout[i]->addWidget(this->m_tbA2Z[(qint32)QChar('T'-'A').toLatin1()]);
            this->m_hLayout[i]->addWidget(this->m_tbA2Z[(qint32)QChar('Y'-'A').toLatin1()]);
            this->m_hLayout[i]->addWidget(this->m_tbA2Z[(qint32)QChar('U'-'A').toLatin1()]);
            this->m_hLayout[i]->addWidget(this->m_tbA2Z[(qint32)QChar('I'-'A').toLatin1()]);
            this->m_hLayout[i]->addWidget(this->m_tbA2Z[(qint32)QChar('O'-'A').toLatin1()]);
            this->m_hLayout[i]->addWidget(this->m_tbA2Z[(qint32)QChar('P'-'A').toLatin1()]);
            break;
        case 4:
            this->m_hLayout[i]->addWidget(this->m_tbFun[1]);//Ctrl.
            this->m_hLayout[i]->addWidget(this->m_tbA2Z[(qint32)QChar('A'-'A').toLatin1()]);
            this->m_hLayout[i]->addWidget(this->m_tbA2Z[(qint32)QChar('S'-'A').toLatin1()]);
            this->m_hLayout[i]->addWidget(this->m_tbA2Z[(qint32)QChar('D'-'A').toLatin1()]);
            this->m_hLayout[i]->addWidget(this->m_tbA2Z[(qint32)QChar('F'-'A').toLatin1()]);
            this->m_hLayout[i]->addWidget(this->m_tbA2Z[(qint32)QChar('G'-'A').toLatin1()]);
            this->m_hLayout[i]->addWidget(this->m_tbA2Z[(qint32)QChar('H'-'A').toLatin1()]);
            this->m_hLayout[i]->addWidget(this->m_tbA2Z[(qint32)QChar('J'-'A').toLatin1()]);
            this->m_hLayout[i]->addWidget(this->m_tbA2Z[(qint32)QChar('K'-'A').toLatin1()]);
            this->m_hLayout[i]->addWidget(this->m_tbA2Z[(qint32)QChar('L'-'A').toLatin1()]);
            this->m_hLayout[i]->addWidget(this->m_tbFun[8]);//<--.
            break;
        case 5:
            this->m_hLayout[i]->addWidget(this->m_tbFun[2]);//Shift.
            this->m_hLayout[i]->addWidget(this->m_tbA2Z[(qint32)QChar('Z'-'A').toLatin1()]);
            this->m_hLayout[i]->addWidget(this->m_tbA2Z[(qint32)QChar('X'-'A').toLatin1()]);
            this->m_hLayout[i]->addWidget(this->m_tbA2Z[(qint32)QChar('C'-'A').toLatin1()]);
            this->m_hLayout[i]->addWidget(this->m_tbA2Z[(qint32)QChar('V'-'A').toLatin1()]);
            this->m_hLayout[i]->addWidget(this->m_tbA2Z[(qint32)QChar('B'-'A').toLatin1()]);
            this->m_hLayout[i]->addWidget(this->m_tbA2Z[(qint32)QChar('N'-'A').toLatin1()]);
            this->m_hLayout[i]->addWidget(this->m_tbA2Z[(qint32)QChar('M'-'A').toLatin1()]);
            this->m_hLayout[i]->addWidget(this->m_tbFun[7]);//Enter.
            break;
        case 6:
            this->m_hLayout[i]->addWidget(this->m_tbFun[3]);//CapsLock.
            this->m_hLayout[i]->addWidget(this->m_tbFun[4]);//Space.
            this->m_hLayout[i]->addWidget(this->m_tbFun[5]);//123.
            this->m_hLayout[i]->addWidget(this->m_tbFun[6]);//!@#?.
            break;
        }
    }
    this->m_frm=new QFrame;
    this->m_frm->setObjectName("ZFullKbd");
    this->m_vLayoutFrm=new QVBoxLayout;
    for(qint32 i=0;i<7;i++)
    {
        this->m_vLayoutFrm->addLayout(this->m_hLayout[i]);
    }
    this->m_frm->setLayout(this->m_vLayoutFrm);

    this->m_vLayout=new QVBoxLayout;
    this->m_vLayout->setContentsMargins(0,0,0,0);
    this->m_vLayout->addWidget(this->m_frm);
    this->setLayout(this->m_vLayout);
}
ZFullKbd::~ZFullKbd()
{
    delete this->m_llTitle;
    delete this->m_tbOkay;
    delete this->m_tbCancel;
    delete this->m_le;
    delete this->m_llInput;
    for(qint32 i=0;i<2;i++)
    {
        delete this->m_tbTurnPage[i];
    }
    for(qint32 i=0;i<6;i++)
    {
        delete this->m_tbChSelect[i];
    }
    for(qint32 i=0;i<26;i++)
    {
        delete this->m_tbA2Z[i];
    }
    for(qint32 i=0;i<9;i++)
    {
        delete this->m_tbFun[i];
    }
    for(qint32 i=0;i<7;i++)
    {
        delete this->m_hLayout[i];
    }
    delete this->m_vLayoutFrm;
    delete this->m_frm;
    delete this->m_vLayout;
}
QString ZFullKbd::ZGetText()
{
    return this->m_le->text().trimmed();
}
void ZFullKbd::ZSetText(QString text)
{
    this->m_le->setText(text);
}
void ZFullKbd::ZSloA2ZButtonClicked()
{
    QToolButton *src=qobject_cast<QToolButton*>(this->sender());
    if(src)
    {
        if(this->m_bChMode)//chinese mode.
        {
            if(this->m_llInput->text().length()<=10)
            {
                this->m_llInput->setText(this->m_llInput->text()+src->text());
                //search the database to find the specified chinese.
                QStringList hanziList;
                if(!gDB.ZGetPinYinData(this->m_llInput->text().toLower(),hanziList))
                {
                    for(qint32 i=0;i<hanziList.size();i++)
                    {
                        if(i<6)
                        {
                            this->m_tbChSelect[i]->setText(hanziList.at(i));
                        }
                    }
                }
            }
        }else{ //english mode.
            int keyCode=this->ZGetKeyCode(src->text().toUpper());
            QKeyEvent *keyEvent1=new QKeyEvent(QEvent::KeyPress,keyCode,Qt::NoModifier,src->text());
            QApplication::postEvent(QApplication::focusObject(),keyEvent1);
            QKeyEvent *keyEvent2=new QKeyEvent(QEvent::KeyRelease,keyCode,Qt::NoModifier,src->text());
            QApplication::postEvent(QApplication::focusObject(),keyEvent2);
        }
    }
}
void ZFullKbd::ZSloChSelectButtonClicked()
{
    QToolButton *src=qobject_cast<QToolButton*>(this->sender());
    if(src)
    {
        this->m_le->setText(this->m_le->text()+src->text());
        this->m_llInput->setText("");
    }
}
void ZFullKbd::ZSloFunButtonClicked()
{
    QToolButton *src=qobject_cast<QToolButton*>(this->sender());
    if(src)
    {
        if(src->text()=="CapsLock")
        {

        }else if(src->text()=="<--")
        {
            QKeyEvent *keyEvent1=new QKeyEvent(QEvent::KeyPress,Qt::Key_Backspace,Qt::NoModifier);
            QApplication::postEvent(QApplication::focusObject(),keyEvent1);
            QKeyEvent *keyEvent2=new QKeyEvent(QEvent::KeyRelease,Qt::Key_Backspace,Qt::NoModifier);
            QApplication::postEvent(QApplication::focusObject(),keyEvent2);

            QString inputChar=this->m_llInput->text();
            this->m_llInput->setText(inputChar.left(inputChar.length()-1));
        }else if(src->text()=="Enter")
        {
            QKeyEvent *keyEvent1=new QKeyEvent(QEvent::KeyPress,Qt::Key_Enter,Qt::NoModifier);
            QApplication::postEvent(QApplication::focusObject(),keyEvent1);
            QKeyEvent *keyEvent2=new QKeyEvent(QEvent::KeyRelease,Qt::Key_Enter,Qt::NoModifier);
            QApplication::postEvent(QApplication::focusObject(),keyEvent2);
        }
    }
}
qint32 ZFullKbd::ZGetKeyCode(QString keyChar)
{
    qint32 code;
    if(keyChar=="A")
    {
        code=Qt::Key_A;
    }else if(keyChar=="B")
    {
        code=Qt::Key_B;
    }else if(keyChar=="C")
    {
        code=Qt::Key_C;
    }else if(keyChar=="D")
    {
        code=Qt::Key_D;
    }else if(keyChar=="E")
    {
        code=Qt::Key_E;
    }else if(keyChar=="F")
    {
        code=Qt::Key_F;
    }else if(keyChar=="G")
    {
        code=Qt::Key_G;
    }else if(keyChar=="H")
    {
        code=Qt::Key_H;
    }else if(keyChar=="I")
    {
        code=Qt::Key_I;
    }else if(keyChar=="J")
    {
        code=Qt::Key_J;
    }else if(keyChar=="K")
    {
        code=Qt::Key_K;
    }else if(keyChar=="L")
    {
        code=Qt::Key_L;
    }else if(keyChar=="M")
    {
        code=Qt::Key_M;
    }else if(keyChar=="N")
    {
        code=Qt::Key_N;
    }else if(keyChar=="O")
    {
        code=Qt::Key_O;
    }else if(keyChar=="P")
    {
        code=Qt::Key_P;
    }else if(keyChar=="Q")
    {
        code=Qt::Key_Q;
    }else if(keyChar=="R")
    {
        code=Qt::Key_R;
    }else if(keyChar=="S")
    {
        code=Qt::Key_S;
    }else if(keyChar=="T")
    {
        code=Qt::Key_T;
    }else if(keyChar=="U")
    {
        code=Qt::Key_U;
    }else if(keyChar=="V")
    {
        code=Qt::Key_V;
    }else if(keyChar=="W")
    {
        code=Qt::Key_W;
    }else if(keyChar=="X")
    {
        code=Qt::Key_X;
    }else if(keyChar=="Y")
    {
        code=Qt::Key_Y;
    }else if(keyChar=="Z")
    {
        code=Qt::Key_Z;
    }
    return code;
}
