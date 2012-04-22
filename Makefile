exe: clean main game boat water timer menu button user bullet enemy pup
	g++ -o ld23 ld23.o Game.o Boat.o Water.o Timer.o start.o Button.o User.o Bullet.o Pup.o Enemy.o -lSDL -lSDL_image -lGL -lGLU
	rm -rf *.o
main: src/ld23.cpp
	g++ -c -Wall src/ld23.cpp
game: src/classes/Game.cpp
	g++ -c -Wall src/classes/Game.cpp
boat: src/classes/Boat.cpp
	g++ -c -Wall src/classes/Boat.cpp
water: src/classes/Water.cpp
	g++ -c -Wall src/classes/Water.cpp
timer: src/classes/utils/Timer.cpp
	g++ -c -Wall src/classes/utils/Timer.cpp
menu: src/menu/start.cpp
	g++ -c -Wall src/menu/start.cpp
button: src/classes/utils/Button.cpp
	g++ -c -Wall src/classes/utils/Button.cpp
user: src/classes/User.cpp
	g++ -c -Wall src/classes/User.cpp
bullet: src/classes/Bullet.cpp
	g++ -c -Wall src/classes/Bullet.cpp
enemy: src/classes/Enemy.cpp
	g++ -c -Wall src/classes/Enemy.cpp
pup: src/classes/Pup.cpp
	g++ -c -Wall src/classes/Pup.cpp
clean:
	rm -rf *.o
