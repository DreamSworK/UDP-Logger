import socket

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
address = ('127.0.0.1', 49152)
for i in range(10):
	sock.sendto('Message ' + str(i), address)
sock.close()