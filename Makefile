#
# 
# Makefile  for CS4815 lab 
#
# Copyright (C) 2008 Jacek Rosik <jacek.rosik@ul.ie>
# 


LIBS=-lglut -lGLU -lGL
SOURCES=   camsys.cc


CC=gcc
CFLAGS=-g -Wall -I /home/pgstud/cs4815/eigen-3.3.7

CXX=g++
CXXFLAGS=$(CFLAGS)

LD=g++
# LDFLAGS=-L /usr/lib/x86_64-linux-gnu


OBJECTS=${SOURCES:.cc=.o}


.PHONY: clean
.SUFFIXES: .o .c .cpp .cc


all: camsys

clean:
	$(RM) $(OBJECTS)
	$(RM) camsys

.c.o:
	$(CC) -c $(CFLAGS) -o $@ $<

.cpp.o:
	$(CXX) -c $(CXXFLAGS) -o $@ $<

.cc.o:
	$(CXX) -c $(CXXFLAGS) -o $@ $<

camsys: $(OBJECTS)
	$(LD) $^ $(LDFLAGS) $(LIBS) -o $@

