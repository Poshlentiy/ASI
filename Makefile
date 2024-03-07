all: client server
		

client: createfolder
	g++ client.cpp -o app/client

server: createfolder
	g++ server.cpp -o app/server

createfolder:
	mkdir -p app 

clean:
	rm -R app || true

.PHONY: all client server clean
