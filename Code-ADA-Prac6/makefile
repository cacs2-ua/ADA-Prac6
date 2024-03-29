#CRISTIAN ANDRÉS CÓRDOBA SILVESTRE DNI: 05988721G

OPTS = -O3 -Wall -Wextra -std=c++17
OPTSDEBUG = -g -Wall -Wextra -std=c++17

all: mcp.cc
	g++ ${OPTS} -o mcp mcp.cc

debug: mcp.cc
	g++ ${OPTSDEBUG} -o build/Debug/mcp mcp.cc


tar: mcp.cc makefile
	tar cvfz 05988721G.tar.gz mcp.cc makefile