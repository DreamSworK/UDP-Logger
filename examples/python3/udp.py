import socket

def send(ip: str, port: int, message: str):
	sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
	address = (ip, port)
	sock.sendto(message.encode(), address)
	sock.close()