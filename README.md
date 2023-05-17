# Mini-RAT

Mini-RAT is a remote access tool designed to remotely monitor and control other
computers through a secure encrypted connection. This project takes heavy
inspiration from ![AsyncRAT](https://github.com/NYAN-x-CAT/AsyncRAT-C-Sharp).

## Installation and Deployment

This project is split into two subprojects: the server (mratd) and client
(mrat). Both the server and reference client requires OpenSSL, but only the
server requires POSIX threads.

The mratd server needs to run a master control server that all clients have
access to, either on the Internet with port-forwarding or on the local network.
The clients need to be running on each machine the administrator needs access to
and configured to connect back to the control server.

## LEGAL DISCLAIMER

I, the creator and all those associated with the development and production of
this program are not responsible for any actions and or damages caused by this
software. You bear the full responsibility of your actions and acknowledge that
this software was created for educational purposes only. This software's
intended purpose is NOT to be used maliciously, or on any system that you do not
have own or have explicit permission to operate and use this program on. By
using this software, you automatically agree to the above.
