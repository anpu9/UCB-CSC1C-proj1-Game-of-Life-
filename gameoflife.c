/************************************************************************
**
** NAME:        gameoflife.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Justin Yokota - Starter Code
**				Yuyang Tian
**
**
** DATE:        2023-10-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

int dx[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
int dy[8] = {1, 0, -1, 1, -1, 1, 0, -1};

int ring(int n, int m) {
    return (n + m) % m;
}

//Determines what color the cell at the given row/col should be. This function allocates space for a new Color.
//Note that you will need to read the eight neighbors of the cell in question. The grid "wraps", so we treat the top row as adjacent to the bottom row
//and the left column as adjacent to the right column.
Color *evaluateOneCell(Image *image, int row, int col, uint32_t rule)
{
	//YOUR CODE HERE
    Color *next = (Color *)malloc(sizeof (Color));
    int isAliveR, isAliveG, isAliveB;
    int aliveNeighourR = 0, aliveNeighourG = 0, aliveNeighourB = 0;
    int idxR, idxG, idxB;
    //determine itself is alive or dead
    Color **img = image->image;
    img += (col + row*(image->cols));
    isAliveR = ((*img)->R == 255);
    isAliveG = ((*img)->G == 255);
    isAliveB = ((*img)->B == 255);

    //need to consider 8 neighours, count the number of lifes
    //consider the R,G,B channel respectively
    Color * neighbour;
    for (int i = 0; i < 8; ++i) {
        int newRow = ring(row+ dx[i],image->rows);
        int newCol = ring(col+dy[i],image->cols);
        neighbour = *(image->image + newRow * image->cols + newCol);
        aliveNeighourR += (neighbour->R == 255);
        aliveNeighourG += (neighbour->G == 255);
        aliveNeighourB += (neighbour->B == 255);
    }
    //compute the position for aliveneighbour
    idxR = 9*isAliveR+aliveNeighourR;
    idxG = 9*isAliveG+aliveNeighourG;
    idxB = 9*isAliveB+aliveNeighourB;
    //change the color according to rule
    if((1<<idxR) & rule) {
        next->R = 255;
    } else {
        next->R = 0;
    }
    if((1<<idxG) & rule) {
        next->G = 255;
    } else {
        next->G = 0;
    }
    if((1<<idxB) & rule) {
        next->B = 255;
    } else {
        next->B = 0;
    }
    return next;
}

//The main body of Life; given an image and a rule, computes one iteration of the Game of Life.
//You should be able to copy most of this from steganography.c
Image *life(Image *image, uint32_t rule)
{
	//YOUR CODE HERE
    Image *newimg = (Image *)malloc(sizeof(Image));
    newimg->cols = image->cols;
    newimg->rows = image->rows;
    newimg->image = (Color**)malloc(sizeof(Color*) * (image->rows) * (image->cols));
    Color** p = newimg->image;
    for (int i = 0; i < newimg->rows; i++)
        for (int j = 0; j < newimg->cols; j++) {
            *p = evaluateOneCell(image, i, j, rule);
            p++;
        }
    return newimg;
}

/*
Loads a .ppm from a file, computes the next iteration of the game of life, then prints to stdout the new image.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a .ppm.
argv[2] should contain a hexadecimal number (such as 0x1808). Note that this will be a string.
You may find the function strtol useful for this conversion.
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!

You may find it useful to copy the code from steganography.c, to start.
*/
int main(int argc, char **argv)
{
	//YOUR CODE HERE
    if (argc != 3) {
        printf("usage: ./gameOfLife filename rule\nfilename is an ASCII PPM file (type P3) with maximum value 255.\nrule is a hex number beginning with 0x; Life is 0x1808.");
        return 1;
    }
    Image *img = readData(argv[1]);
    uint32_t rule = strtol(argv[2], NULL, 16);
    Image *nextImg = life(img, rule);
    writeData(nextImg);
    freeImage(img);
    freeImage(nextImg);
    return 0;
}
