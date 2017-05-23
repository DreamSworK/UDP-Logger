#include <QtNetwork>

int main(int argc, char *argv[])
{
    QUdpSocket udpSocket;
    for (int i = 0; i < 10; ++i) {
        QByteArray datagram = "Message " + QByteArray::number(i);
        udpSocket.writeDatagram(datagram.data(), datagram.size(), QHostAddress::LocalHost, 49152);
    }
    return 0;
}