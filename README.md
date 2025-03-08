# Socket_programming

Project: Client-Server Socket Communication (C++)
1. Client Class:

	- The client takes the names of bank customers (each customer is represented as an object).
		For each customer, the name and the amount are input via cin.
2. Server Class:

	- The server listens for incoming requests and stores the customer data in a vector.

	Account Types:

	- Users have both a saving account and a checking account (with restrictions on withdrawing all funds in a single transaction).

	Data Structure:

	- Two objects are created from the Client class.

	- Users are represented by an Account class, where each account holds the user’s name, ID, and balance.

	Operations:

	- The system allows transferring money between users. It checks the validity of the transaction using a database and responds to the client accordingly.
