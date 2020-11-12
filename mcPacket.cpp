#include "mcPacket.h"

McPacket::McPacket() : isReceiving(false)
{

}

McPacket::~McPacket()
{

}

void McPacket::removeHeader(QByteArray &packet)
{
    packet.remove(0, 8);
}

void McPacket::initPacketBuffer()
{
    totalsize = 0;
    protocol = "";
    data = "";
}
void McPacket::extractReadPacket(QByteArray &packet)
{

    extractReadPacketTotalSize(packet);
    extractReadPacketProtocol(packet);
    extractReadPacketData(packet);
}

void McPacket::extractReadPacketTotalSize(QByteArray &packet)
{
    bool ok;
    totalsize = packet.mid(0, sizeof(int)).toHex().toInt(&ok, 16);
    remainsize = totalsize;
    packet.remove(0, sizeof(int));
    debugger->debugMessage("Total Size", totalsize);
    remainsize -= sizeof(int);
}

int McPacket::getTotalSize()
{
    return totalsize;
}

void McPacket::setRemainSize(int size)
{
    this->remainsize = size;
}

int McPacket::getRemainSize()
{
    return remainsize;
}

void McPacket::extractReadPacketProtocol(QByteArray &packet)
{
    if(packet.indexOf(0x1D) == -1)
    {
        protocol = packet;
        packet = "";
        remainsize -= protocol.size();
    }
    else
    {
        protocol = packet.left(packet.indexOf(0x1D));
        packet.remove(0, packet.indexOf(0x1D) + SEPARATOR);
        remainsize -= (protocol.size() + SEPARATOR);
    }
    debugger->debugMessage("protocol", protocol);
}
QString McPacket::getProtocol()
{
    return protocol;
}

void McPacket::extractReadPacketData(QByteArray &packet)
{
    int readsize = (packet.size() < remainsize) ? packet.size() : remainsize;
    data += packet.mid(0, readsize);
//    data += QString::fromLocal8Bit(packet.data(), readsize).toUtf8();
    remainsize -= readsize;
    debugger->debugMessage("data", data);
}
QString McPacket::getData()
{
    return data;
}
void McPacket::setData(QString data)
{
    this->data = data;
}

bool McPacket::isReadCompleted()
{
    return (remainsize == 0);
}
