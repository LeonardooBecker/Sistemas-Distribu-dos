all: tempo

tempo: tempo.o smpl.o rand.o cisj.o
	$(LINK.c) -o $@ -Bstatic tempo.o smpl.o rand.o cisj.o -lm

cisj.o: cisj.c cisj.h
	$(COMPILE.c) -g cisj.c

smpl.o: smpl.c smpl.h
	$(COMPILE.c)  -g smpl.c

tempo.o: tempo.c smpl.h
	$(COMPILE.c) -g  tempo.c

rand.o: rand.c
	$(COMPILE.c) -g rand.c

clean:
	$(RM) *.o tempo relat saida
