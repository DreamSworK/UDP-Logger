using System.Text;
using System.Net.Sockets;

namespace UdpSend
{
    static class Program
    {
        static void Main(string[] args)
        {
            using (UdpClient udpClient = new UdpClient("127.0.0.1", 49152))
            {
                for (int i = 0; i < 10; ++i) {
                    string message = "Message " + i;
                    byte[] dgram = Encoding.ASCII.GetBytes(message);
                    udpClient.Send(dgram, dgram.Length);
                }
            }
        }
    }
}
