all:
	gcc -o km crypto.c connection.c km.c -lcrypto -lssl -pthread
	gcc -o a crypto.c connection.c a.c -lcrypto -lssl -pthread
	gcc -o b crypto.c connection.c b.c -lcrypto -lssl -pthread