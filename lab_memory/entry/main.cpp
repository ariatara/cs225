/**
 * @file main.cpp
 * A simple program that allocates students to rooms.
 */

#include <algorithm>
#include <vector>
#include <iostream>
#include <utility>

#include "../src/fileio.h"
#include "../src/letter.h"
#include "../src/room.h"
#include "../src/allocator.h"

/**
 * Main routine.
 * Creates an Allocator object and allocates the students.
 */
int main()
{
    std::cout << std::endl;

    fileio::init();
    Allocator theAllocator("../students.txt", "../rooms.txt");
    theAllocator.allocate();
    theAllocator.printRooms();
    fileio::halt();

    std::cout << std::endl;
    return 0;
}
