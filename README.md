# UDP Logger
Very simple, minimalistic cross-platform UDP Logger like _**syslog**_ (Unix, Linux, MacOS) or _**DebugView**_ (Windows)

## About:

This is a very simple implementation of a UDP Log server with GUI. The program is written using Qt4, a cross-platform framework. The program listens a UDP port and displays the information that it receives from a client.

> You can use the source code as template for your own logger or a server like application.

## Description:

Sometimes we don’t want to use a file logging system. And we can just send debug messages to a system logger. To do that on a Unix system, we can use the function ```void syslog(int priority, const char *format, ...)``` or do that on Windows, we can use ```void OutputDebugString(LPCTSTR lpOutputString)```. Even though that could be an easy solution, it wouldn’t be portable or cross-platform. Besides, most hi-level programming languages (such as Java) do not contain these native functions.

An alternative would be to use your own UDP Logger and send debug messages to it. This option is more portable because there are functions for sending simple UDP messages in most languages. Sending packets to a UDP port is very fast compared to saving them to a file or sending them to a TCP port because there is no need to establish a connection with the server. An added advantage is that you don’t need to create a file for logging.

Also the function ```OutputDebugString``` has the length limit of _**4092**_ bytes with a NUL byte at the end. And the message length of ```syslog``` must not exceed _**1024**_ bytes. However, if you use your own _**UDP Logger**_ the maximum size of a UDP message is _**65507**_ bytes, that's more than enough.

## Examples:

Some examples how to send debug messages to UDP Logger you can find in [examples](examples) folder.
