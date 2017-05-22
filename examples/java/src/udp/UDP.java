package udp;

import java.net.*;

public final class UDP {

    private final static String host = "127.0.0.1";
    private final static int port = 49152;
    private static DatagramSocket socket;
    private static InetAddress address;

    static {
        try {
            socket = new DatagramSocket();
            address = InetAddress.getByName(host);
            Runtime.getRuntime().addShutdownHook(new Thread() {
                public void run() {
                    socket.close();
                }
            });
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static void send(String message) {
        try {
            byte[] bytes = message.getBytes();
            DatagramPacket packet = new DatagramPacket(bytes, bytes.length, address, port);
            socket.send(packet);
        }
        catch (Exception e) {
            e.printStackTrace();
        }
    }
}