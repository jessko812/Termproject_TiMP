.PHONY: all debug release clean

RELEASE_FLAGS = -O2 -Wall -DNDEBUG -std=c++17
DEBUG_FLAGS   = -g -O0 -Wall -std=c++17
RELEASE_EXEC  = client
DEBUG_EXEC    = client-dbg
SOURCES       = main.cpp interface.cpp file_handler.cpp network_manager.cpp linux_authenticator.cpp

all: debug release

debug: $(DEBUG_EXEC)

$(DEBUG_EXEC): $(SOURCES)
	g++ $(DEBUG_FLAGS) $(SOURCES) -o $(DEBUG_EXEC) -lpthread

release: $(RELEASE_EXEC)

$(RELEASE_EXEC): $(SOURCES)
	g++ $(RELEASE_FLAGS) $(SOURCES) -o $(RELEASE_EXEC) -lpthread

clean:
	rm -f $(RELEASE_EXEC) $(DEBUG_EXEC)
