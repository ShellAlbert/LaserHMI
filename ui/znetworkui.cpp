#include "znetworkui.h"
#include <string.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <QDebug>
#include <QFile>
#include <ui/zwaitingdialog.h>
int getLocalInfo(QStringList &nicList)
{
    int fd;
    int interfaceNum = 0;
    struct ifreq buf[16];
    struct ifconf ifc;
    struct ifreq ifrcopy;
    char mac[16] = {0};
    char ip[32] = {0};
    char broadAddr[32] = {0};
    char subnetMask[32] = {0};

    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket");
        close(fd);
        return -1;
    }

    ifc.ifc_len = sizeof(buf);
    ifc.ifc_buf = (caddr_t)buf;
    if (!ioctl(fd, SIOCGIFCONF, (char *)&ifc))
    {
        interfaceNum = ifc.ifc_len / sizeof(struct ifreq);
        printf("interface num = %d\n", interfaceNum);
        while (interfaceNum-- > 0)
        {
            QString nicDev;
            printf("ndevice name: %s\n", buf[interfaceNum].ifr_name);
            //ignore the interface that not up or not runing
            ifrcopy = buf[interfaceNum];
            if (ioctl(fd, SIOCGIFFLAGS, &ifrcopy))
            {
                printf("ioctl: %s [%s:%d]\n", strerror(errno), __FILE__, __LINE__);
                close(fd);
                return -1;
            }
            nicDev+="dev="+QString(buf[interfaceNum].ifr_name)+",";

            //get the mac of this interface
            if (!ioctl(fd, SIOCGIFHWADDR, (char *)(&buf[interfaceNum])))
            {
                memset(mac, 0, sizeof(mac));
                snprintf(mac, sizeof(mac), "%02x%02x%02x%02x%02x%02x",
                         (unsigned char)buf[interfaceNum].ifr_hwaddr.sa_data[0],
                        (unsigned char)buf[interfaceNum].ifr_hwaddr.sa_data[1],
                        (unsigned char)buf[interfaceNum].ifr_hwaddr.sa_data[2],

                        (unsigned char)buf[interfaceNum].ifr_hwaddr.sa_data[3],
                        (unsigned char)buf[interfaceNum].ifr_hwaddr.sa_data[4],
                        (unsigned char)buf[interfaceNum].ifr_hwaddr.sa_data[5]);
                printf("device mac: %s\n", mac);
            }else
            {
                printf("ioctl: %s [%s:%d]\n", strerror(errno), __FILE__, __LINE__);
                close(fd);
                return -1;
            }
            nicDev+="mac="+QString(mac)+",";

            //get the IP of this interface
            if (!ioctl(fd, SIOCGIFADDR, (char *)&buf[interfaceNum]))
            {
                snprintf(ip, sizeof(ip), "%s",
                         (char *)inet_ntoa(((struct sockaddr_in *)&(buf[interfaceNum].ifr_addr))->sin_addr));
                printf("device ip: %s\n", ip);
            }else
            {
                printf("ioctl: %s [%s:%d]\n", strerror(errno), __FILE__, __LINE__);
                close(fd);
                return -1;
            }
            nicDev+="ip="+QString(ip)+",";

            //get the broad address of this interface
            if (!ioctl(fd, SIOCGIFBRDADDR, &buf[interfaceNum]))
            {
                snprintf(broadAddr, sizeof(broadAddr), "%s",
                         (char *)inet_ntoa(((struct sockaddr_in *)&(buf[interfaceNum].ifr_broadaddr))->sin_addr));
                printf("device broadAddr: %s\n", broadAddr);
            }else
            {
                printf("ioctl: %s [%s:%d]\n", strerror(errno), __FILE__, __LINE__);
                close(fd);
                return -1;
            }
            nicDev+="broad="+QString(broadAddr)+",";

            //get the subnet mask of this interface
            if (!ioctl(fd, SIOCGIFNETMASK, &buf[interfaceNum]))
            {
                snprintf(subnetMask, sizeof(subnetMask), "%s",
                         (char *)inet_ntoa(((struct sockaddr_in *)&(buf[interfaceNum].ifr_netmask))->sin_addr));
                printf("device subnetMask: %s\n", subnetMask);
            }else
            {
                printf("ioctl: %s [%s:%d]\n", strerror(errno), __FILE__, __LINE__);
                close(fd);
                return -1;
            }
            nicDev+="mask="+QString(subnetMask)+",";
            nicList.append(nicDev);
        }
    }else
    {
        printf("ioctl: %s [%s:%d]\n", strerror(errno), __FILE__, __LINE__);
        close(fd);
        return -1;
    }
    close(fd);
    return 0;
}
ZDHCPThread::ZDHCPThread()
{

}
void ZDHCPThread::run()
{
    bool bSuccess=true;
    //close firewall.
    FILE *fpFw=popen("/sbin/SuSEfirewall2 off","r");
    pclose(fpFw);

    //dhcp.
    FILE *fp=popen("rm /tmp/dhcp.ret ; /sbin/dhclient -4 eth0 ; ifconfig eth0|grep 'inet addr'|awk '{print $2}'>/tmp/dhcp.ret","r");
    pclose(fp);
    QFile dhcpFile("/tmp/dhcp.ret");
    if(dhcpFile.open(QIODevice::ReadOnly))
    {
        QString dhcpRetCode=dhcpFile.readAll();
        dhcpFile.close();
        if(dhcpRetCode.isEmpty())
        {
            bSuccess=false;
        }
    }else{
        bSuccess=false;
    }

    if(bSuccess)
    {
        emit this->ZSigThreadDone(0,"");
    }else{
        emit this->ZSigThreadDone(-1,tr("无法获取网络参数!"));
    }
}
ZNetworkUI::ZNetworkUI()
{

}
ZNetworkUI::~ZNetworkUI()
{

}
void ZNetworkUI::ZDoInitial()
{
    this->m_mac=new ZLabelLineEditCheckBox(tr("MAC地址"),"00-00-00-00-00-00","",true,false);
    this->m_ip=new ZLabelLineEditCheckBox(tr("IP地址"),"000.000.000.000","",true,false);
    this->m_mask=new ZLabelLineEditCheckBox(tr("子网掩码"),"000.000.000.000","",true,false);
    this->m_broad=new ZLabelLineEditCheckBox(tr("广播地址"),"000.000.000.000","",true,false);
    this->m_gw=new ZLabelLineEditCheckBox(tr("默认网关"),"000.000.000.000","",true,false);
    this->m_dns=new ZLabelLineEditCheckBox(tr("DNS地址"),"000.000.000.000","",true,false);


    this->m_gLayout=new QGridLayout;
    this->m_gLayout->addWidget(this->m_mac,0,0,1,3);
    this->m_gLayout->addWidget(this->m_ip,1,0,1,1);
    this->m_gLayout->addWidget(this->m_mask,1,1,1,1);
    this->m_gLayout->addWidget(this->m_broad,1,2,1,1);

    this->m_gLayout->addWidget(this->m_gw,2,0,1,3);
    this->m_gLayout->addWidget(this->m_dns,3,0,1,3);

    this->m_tbDHCP=new QToolButton;
    this->m_tbDHCP->setText(tr("DHCP"));
    this->m_tbRefresh=new QToolButton;
    this->m_tbRefresh->setText(tr("刷新"));
    this->m_tbSetup=new QToolButton;
    this->m_tbSetup->setText(tr("设置"));
    this->m_hLayout=new QHBoxLayout;
    this->m_hLayout->addStretch(1);
    this->m_hLayout->addWidget(this->m_tbRefresh);
    this->m_hLayout->addWidget(this->m_tbDHCP);
    this->m_hLayout->addWidget(this->m_tbSetup);
    this->m_vLayout=new QVBoxLayout;
    this->m_vLayout->addLayout(this->m_gLayout);
    this->m_vLayout->addLayout(this->m_hLayout);
    this->m_vLayout->setContentsMargins(4,4,4,4);
    this->setLayout(this->m_vLayout);

    this->m_tbDHCP->setObjectName("ZNetworkUIButton");
    this->m_tbRefresh->setObjectName("ZNetworkUIButton");
    this->m_tbSetup->setObjectName("ZNetworkUIButton");
    connect(this->m_tbDHCP,SIGNAL(clicked(bool)),this,SLOT(ZSloDHCP()));
    connect(this->m_tbRefresh,SIGNAL(clicked(bool)),this,SLOT(ZSloRefresh()));
    connect(this->m_tbSetup,SIGNAL(clicked(bool)),this,SLOT(ZSloSetup()));

    this->ZSloRefresh();
}
void ZNetworkUI::ZUnInitial()
{
    delete this->m_mac;
    delete this->m_ip;
    delete this->m_mask;
    delete this->m_broad;
    delete this->m_gw;
    delete this->m_dns;
    delete this->m_gLayout;
    delete this->m_tbDHCP;
    delete this->m_tbRefresh;
    delete this->m_tbSetup;
    delete this->m_hLayout;
    delete this->m_vLayout;
}
void ZNetworkUI::ZSloDHCP()
{
    ZWaitingDialog waitDia(tr("正在自动获取IP地址..."));
    ZDHCPThread thread;
    connect(&thread,SIGNAL(ZSigThreadDone(qint32,QString)),&waitDia,SLOT(ZSloClose(qint32,QString)));
    thread.start();
    waitDia.exec();
    thread.wait();
    this->ZSloRefresh();
    return;
}
void ZNetworkUI::ZSloRefresh()
{
    QStringList nicDevList;
    if(getLocalInfo(nicDevList)<0)
    {
        return;
    }
    qDebug()<<nicDevList;
    for(qint32 i=0;i<nicDevList.size();i++)
    {
        if(nicDevList.at(i).startsWith("dev=lo"))
        {
            continue;//bypass lo.
        }
        QStringList nicDevInfoList=nicDevList.at(i).split(",");
        for(qint32 j=0;j<nicDevInfoList.size();j++)
        {
            QString keyVal=nicDevInfoList.at(j);
            QStringList keyValList=keyVal.split("=");
            if(keyValList.size()!=2)
            {
                continue;
            }
            if(keyValList.at(0)=="mac")
            {
                this->m_mac->ZSetLineEditText(keyValList.at(1));
            }else if(keyValList.at(0)=="ip")
            {
                this->m_ip->ZSetLineEditText(keyValList.at(1));
            }else if(keyValList.at(0)=="broad")
            {
                this->m_broad->ZSetLineEditText(keyValList.at(1));
            }else if(keyValList.at(0)=="mask")
            {
                this->m_mask->ZSetLineEditText(keyValList.at(1));
            }
        }
    }
    //gateway.
    FILE *fp1=popen("route -n | grep eth0|grep UG | awk '{print $2}'>/tmp/net.gw","r");
    pclose(fp1);
    QFile gwFile("/tmp/net.gw");
    if(gwFile.open(QIODevice::ReadOnly))
    {
        QString gwData=gwFile.readAll();
        gwFile.close();

        QStringList gwList=gwData.split("\n");
        QString gwAll;
        for(qint32 i=0;i<gwList.size();i++)
        {
            gwAll+=gwList.at(i)+" ";
        }
        this->m_gw->ZSetLineEditText(gwAll);
    }
    //dns.
    FILE *fp2=popen("cat /etc/resolv.conf | grep '^nameserver'| awk '{print $2}' >/tmp/net.dns","r");
    pclose(fp2);
    QFile dnsFile("/tmp/net.dns");
    if(dnsFile.open(QIODevice::ReadOnly))
    {
        QString dnsData=dnsFile.readAll();
        dnsFile.close();
        QStringList dnsList=dnsData.split("\n");
        QString dnsAll;
        for(qint32 i=0;i<dnsList.size();i++)
        {
            dnsAll+=dnsList.at(i)+" ";
        }
        this->m_dns->ZSetLineEditText(dnsAll);
    }
}
void ZNetworkUI::ZSloSetup()
{
    FILE *fp=popen("ifconfig eth0 down;ifconfig eth0 192.168.1.8;ifconfig eth0 up","r");
    pclose(fp);
    this->ZSloRefresh();
}
