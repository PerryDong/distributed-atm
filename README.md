# distributed-atm
Simulate a distributed ATM banking system. Three banks (A, B, C )are connected with each other over networking connection. The user is able to make corresponding banking transactions via the network remotely once owning an account in any one of these three banks. For instance, a user opened a checking account from bank B, he can check his balance, deposit or withdraw cash from either bank B or C.
ATM system design is as follow:
1.	Bank A is the main server, containing the IP address or hostname of bank B or C.
2.	A user on the bank B wants to check his balance from the ATM on the bank C, bank C will verify his ID by sending request to main server A.
3.	bank A gets the ip address of bank C from the database and sends it back to bank C.
4.	once bank C receives the ip address of bank B, it is able to connect to bank B directly.
5.	Now Bank B and C are able to do normal transaction directly.
Use Socket to implement the bank A, B, C connection
Server side: bank A
Client side: bank B or C
Your program will be tested on three pc in the lab over network connection

