CC = g++

FLAGS = -std=c++11 -Wall -Wextra -Weffc++

INCLUDES = -I./
DEFINES = -DDEBUG -DUNIX

BIN = bin

EXEC_NAME = Server

all: $(BIN)/$(EXEC_NAME)

$(BIN)/$(EXEC_NAME): net/main.o net/server_socket.o net/tcp_socket.o net/socket.o net/http_parser.o net/resource.o
	$(CC) $(FLAGS) $(DEFINES) $(INCLUDES) $^ -o $@
	
net/%.o: net/%.cpp
	$(CC) $(FLAGS) $(DEFINES) $(INCLUDES) -c $< -o $@
	
clean:
	rm -rf net/*.o *~ $(BIN)/$(EXEC_NAME)
