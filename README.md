instalar ncurses
sudo apt-get install libncurses5-dev libncursesw5-dev
compilar
gcc pong_terminal.c -o pong -lncurses
ejecutar
./pong
