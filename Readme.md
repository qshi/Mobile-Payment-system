#Mobile Payment system

This project is a brief payment system based on client-server architecture. We simulated the SSL protocol with DH key exchange algorithm. We also used AES encryption algorithm during the communication between client and server. To improve the secureity of a payment system, use added the email confirmation in this project. When a masquerader pick up some one's bank card, he still need to have a emailed confirmation code to accomplish the payment. Thus the system is safer than that only need card information to make a payment.

##How it works
The software is based on a client-server architecture and UI design. First, open the server, and then the client. The client will show a log in window, you can enter your username and password to log in, or also a valid email address to sign up. When logged in seccuseffuly, you will see the main window, where you can make a payment. When you enter your cardnumber, security number and card holder's name, youcan make a paymet. if you are using this system in a usually logged in IP, the server will only check your card information, otherwise, the server will send you an email that contians a randomly generated confirmation number, you enter the confirmation number and the server check it.If it's OK, your payment will success, else this transaction will be denied.

###Futher improvement:
There's some unconvenient with this system. Once you mistakenly enter a wrong information, you have to restart the client software to try again. This should be fixed at the server end.

###Extensions: 
1.In this project, we only simulated the SSL protocol, may be we could use a real SSL library, such as OpenSSL.
2.We just implement the customer part, we could do more to implement the merchant part or even a bank server in this system.
3.Maybe we could add more fancy functions, such as it you enter the passwork wrong 3 times, your account will be blocked, and there may be a smart algorithm to save usually used IP.
4. Also, the UI can be designed more beautiful.

##Set up
Our system is based on windows system, so it has to be run on a window platform.
To run the server, just copy the floder "serversoftware" to your computer and run the "server.exe".
To run the client, copy the floder "clientsoftware" and run the "server.exe" is OK.

##Test
We tested the function of signing up, logging in, making a payment, email confiration. All test case pass through.
