# Conway's Game of Life

## Description
This is a simple implementation of Conway's Game of Life in C++ using the [gtkmm-3.0](https://gtkmm.org/en/index.html) library. The game follows the basic principle that cells on a 2D grid can either be alive or dead, and advance to the next generation based on its neighbors' statuses. This implementation was developed on Ubuntu 22.04.3LTS.

## Installation
### Dependencies
* [gtkmm-3.0](https://gtkmm.org/en/index.html)
* [Make](https://www.gnu.org/software/make/)
* [g++](https://gcc.gnu.org/)

### Instructions
1. Clone the repository to your local machine in a directory of your choice
```
git clone https://github.com/michaelzhan1/game-of-life.git
```
2. Navigate to the repository
```
cd game-of-life
```
3. Compile the program
```
make
```
4. Run the program
```
bin/main
```