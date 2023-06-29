/**
 * @file main.c 
 * @author adaskin
 * @brief this simulates the movement of a hunter and animal game in 2D site grid
 * @version 0.1
 * @date 2023-05-03
 *
 * @copyright Copyright (c) 2023
 *
 */
#include <stdio.h>
#include <stdlib.h>

typedef enum { BEAR, BIRD, PANDA} AnimalType;

typedef enum { ALIVE, DEAD } AnimalStatus;

typedef struct {
    int x;
    int y;
} Location;

typedef enum { FEEDING, NESTING, WINTERING } SiteType;

typedef struct {
    /** animal can be DEAD or ALIVE*/
    AnimalStatus status;
    /** animal type, bear, bird, panda*/
    AnimalType type;
    /** its location in 2D site grid*/
    Location location;
} Animal;

/*example usage*/
Animal bird, bear, panda;

/** type of Hunter*/
typedef struct {
    /** points indicate the number of animals, a hunter killed*/
    int points;
    /** its location in the site grid*/
    Location location;
} Hunter;

/** type of a site (a cell in the grid)*/
typedef struct {
    /** array of pointers to the hunters located at this site*/
    Hunter **hunters;
    /** the number of hunters at this site*/
    int nhunters;
    /** array of pointers to the animals located at this site*/
    Animal **animals;
    /** the number of animals at this site*/
    int nanimals;
    /** the type of site*/
    SiteType type;
} Site;

/** 2D site grid*/
typedef struct {
    /** number of rows, length at the x-coordinate*/
    int xlength;
    /** number of columns, length at the y-coordinate*/
    int ylength;
    /** the 2d site array*/
    Site **sites;
} Grid;

/* initial grid, empty*/
Grid grid = {0, 0, NULL};

/**
 * @brief initialize grid with random site types
 * @param xlength
 * @param ylength
 * @return Grid
 */
Grid initgrid(int xlength, int ylength) {
    grid.xlength = xlength;
    grid.ylength = ylength;
	
    grid.sites = (Site **)malloc(sizeof(Site *) * xlength);
    for (int i = 0; i < xlength; i++) {
        grid.sites[i] = (Site *)malloc(sizeof(Site) * ylength);
        for (int j = 0; j < ylength; j++) {
            grid.sites[i][j].animals = NULL;
            grid.sites[i][j].hunters = NULL;
            grid.sites[i][j].nhunters = 0;
            grid.sites[i][j].nanimals = 0;
            double r = rand() / (double)RAND_MAX;
            SiteType st;
            if (r < 0.33)
                st = WINTERING;
            else if (r < 0.66)
                st = FEEDING;
            else
                st = NESTING;
            grid.sites[i][j].type = st;
        }
    }
	
    return grid;
}

/**
 * @brief
 *
 */
void deletegrid() {
    for (int i = 0; i < grid.xlength; i++) {
        free(grid.sites[i]);
    }
	
    free(grid.sites);
	
    grid.sites = NULL;
    grid.xlength = -1;
    grid.ylength = -1;
}

/**
 * @brief prints the number animals and hunters in each site
 * of a given grid
 * @param grid
 */
void printgrid() {
    for (int i = 0; i < grid.xlength; i++) {
        for (int j = 0; j < grid.ylength; j++) {
            Site *site = &grid.sites[i][j];
            int count[3] = {0}; /* do not forget to initialize*/
            for (int a = 0; a < site->nanimals; a++) {
                Animal *animal = site->animals[a];
                count[animal->type]++;
            }
			
            printf("|%d-{%d, %d, %d}{%d}|", site->type, count[0], count[1],
                   count[2], site->nhunters);
        }
        printf("\n");
    }
}

/**
 * @brief prints the info of a given site
 *
 */
void printsite(Site *site) {
    int count[3] = {0}; /* do not forget to initialize*/
    for (int a = 0; a < site->nanimals; a++) {
        Animal *animal = site->animals[a];
        count[animal->type]++;
    }
    printf("|%d-{%d,%d,%d}{%d}|", site->type, count[0], count[1], count[2],
           site->nhunters);
}

/*
============================================================= 
TODO: you need to complete following three functions 
DO NOT CHANGE ANY OF THE FUNCTION NAME OR TYPES
============================================================= 
*/

/**
 * @brief  it moves a given hunter or animal
 * randomly in the grid
 * @param args is an animalarray
 * @return void*
 */
void *simulateanimal(void *args) {
    /*TODO: complete this function:
        get animal type,
        check if animal dies,
        then chose random direction and move
        make sure to update site.animals
        then sleep
    */