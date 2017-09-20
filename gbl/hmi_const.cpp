#include "gbl/hmi_const.h"
ZSQLiteDB gDB;
ZGblHelp g_GblHelp;
quint8 ZMapColorTemp2Hex(QString colorTemp)
{
    quint8 colorTempHex;
    if(colorTemp=="D27")
    {
        colorTempHex=0x1B;
    }else if(colorTemp=="D30")
    {
        colorTempHex=0x1E;
    }else if(colorTemp=="D35")
    {
        colorTempHex=0x23;
    }
    else if(colorTemp=="D40")
    {
        colorTempHex=0x28;
    }
    else if(colorTemp=="D63")
    {
        colorTempHex=0x3F;
    }
    else if(colorTemp=="D65")
    {
        colorTempHex=0x41;
    }
    else if(colorTemp=="D80")
    {
        colorTempHex=0x50;
    }
    else if(colorTemp=="D93")
    {
        colorTempHex=0x5D;
    }else if(colorTemp=="D95")
    {
        colorTempHex=0x5F;
    }else if(colorTemp=="DCI")
    {
        colorTempHex=0x66;
    }else{
        //unknow color temp.
        colorTempHex=0xff;
    }
    return colorTempHex;
}
QString ZMapColorTemp2String(quint8 colorTemp)
{
    QString colorTempStr;
    switch(colorTemp)
    {
    case 0x1B:
        colorTempStr="D27";
        break;
    case 0x1E:
        colorTempStr="D30";
        break;
    case 0x23:
        colorTempStr="D35";
        break;
    case 0x28:
        colorTempStr="D40";
        break;
    case 0x3F:
        colorTempStr="D63";
        break;
    case 0x41:
        colorTempStr="D65";
        break;
    case 0x50:
        colorTempStr="D80";
        break;
    case 0x5D:
        colorTempStr="D93";
        break;
    case 0x5F:
        colorTempStr="D95";
        break;
    case 0x66:
        colorTempStr="DCI";
        break;
    default:
        colorTempStr="ERR";
        break;
    }
    return colorTempStr;
}

void ZGblHelp::beep()
{
    if(g_GblHelp.m_bBeep)
    {
        FILE *fp=popen("/usr/bin/echo -e '\a' >/dev/tty10","r");
        if(fp)
        {
            pclose(fp);
        }
    }
}
