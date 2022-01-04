a.out: main.cc map.h monster.h hero.h
	g++ main.cc -lcurl -lncurses

clean:
	rm a.out core *.o
