
INCLUDE = -I. -I/usr/local/include
CPP 	= g++
LIBRARY = -L. -L/usr/local/lib
SYSLIBS = -lPocoFoundation

#core
obj-MooWoo += MooWoo.o
obj-MooWoo += ArgumentsParser.o
obj-MooWoo += ComputeCore.o
obj-MooWoo += AsynCore.o
obj-MooWoo += Message.o
obj-MooWoo += Protocol.o
obj-MooWoo += UniqueServerQueue.o
obj-MooWoo += SocketManager.o
obj-MooWoo += TReactor.o

