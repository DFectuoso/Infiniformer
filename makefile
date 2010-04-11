all:
	g++ LevelMap.cpp Player.cpp core.cpp main.cpp `allegro-config --libs` -o TEST -arch i686
	rm -rf *o hello
run:
	$(all)
	./TEST

