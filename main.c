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
#include <pthread.h>
#include <unistd.h>

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

    srand(time(NULL));
    Animal* animal = (Animal*)args;

    Site* currentSite = &grid.sites[animal->location.x][animal->location.y];

    if(animal == &bird)
        animal->type = BIRD;
    else if(animal == &bear)
        animal->type = BEAR;
    else if(animal == &panda)
        animal->type = PANDA;

    while (1) {

        if(currentSite->type == NESTING){
            // Check if animal dies
            if (animal->status = DEAD) 
                break;
            else{
                animal->status = ALIVE;
                
                Animal* newAnimal = (Animal*)malloc(sizeof(Animal));
                newAnimal->status = ALIVE;
                newAnimal->type = animal->type;
                newAnimal->location = animal->location;

                // Add the new animal to the nesting site
                currentSite->animals = (Animal**)realloc(currentSite->animals, sizeof(Animal*) * (currentSite->nanimals + 1));
                currentSite->animals[currentSite->nanimals] = newAnimal;
                currentSite->nanimals++;
            
                // Choose a random direction (up, down, left, or right)
                int dx, dy;
                int direction = rand() % 4;
                switch (direction) {
                    case 0: // Up
                        dx = -1;
                        dy = 0;
                        break;
                    case 1: // Down
                        dx = 1;
                        dy = 0;
                        break;
                    case 2: // Left
                        dx = 0;
                        dy = -1;
                        break;
                    case 3: // Right
                        dx = 0;
                        dy = 1;
                        break;
                }

                // Move the animal to a neighboring random location
                int newX = animal->location.x + dx;
                int newY = animal->location.y + dy;

                // Update the grid with the new animal location
                
                Site* oldSite = &grid.sites[animal->location.x][animal->location.y];
                Site* newSite = &grid.sites[newX][newY];

                // Remove the animal from the old site
                for (int i = 0; i < oldSite->nanimals; i++) {
                    if (oldSite->animals[i] == animal) {
                        oldSite->animals[i] = NULL;
                        break;
                    }
                }
                oldSite->nanimals--;

                // Add the animal to the new site
                newSite->animals = (Animal**)realloc(newSite->animals, sizeof(Animal*) * (newSite->nanimals + 1));
                newSite->animals[newSite->nanimals] = animal;
                newSite->nanimals++;

                // Update the animal's location
                animal->location.x = newX;
                animal->location.y = newY;
            }
        }

        else if(currentSite->type == WINTERING){
            // Check if animal dies
            double r = rand() / (double)RAND_MAX;
            if (r <= 0.5) {
                animal->status = DEAD;
                break;
            }
            else{
                animal->status = ALIVE;
                
                Animal* newAnimal = (Animal*)malloc(sizeof(Animal));
                newAnimal->status = ALIVE;
                newAnimal->type = animal->type;
                newAnimal->location = animal->location;

                // Add the new animal to the nesting site
                currentSite->animals = (Animal**)realloc(currentSite->animals, sizeof(Animal*) * (currentSite->nanimals + 1));
                currentSite->animals[currentSite->nanimals] = newAnimal;
                currentSite->nanimals++;
            
                // Choose a random direction (up, down, left, or right)
                int dx, dy;
                int direction = rand() % 4;
                switch (direction) {
                    case 0: // Up
                        dx = -1;
                        dy = 0;
                        break;
                    case 1: // Down
                        dx = 1;
                        dy = 0;
                        break;
                    case 2: // Left
                        dx = 0;
                        dy = -1;
                        break;
                    case 3: // Right
                        dx = 0;
                        dy = 1;
                        break;
                }

                // Move the animal to a neighboring random location
                int newX = animal->location.x + dx;
                int newY = animal->location.y + dy;

                // Update the grid with the new animal location
                
                Site* oldSite = &grid.sites[animal->location.x][animal->location.y];
                Site* newSite = &grid.sites[newX][newY];

                // Remove the animal from the old site
                for (int i = 0; i < oldSite->nanimals; i++) {
                    if (oldSite->animals[i] == animal) {
                        oldSite->animals[i] = NULL;
                        break;
                    }
                }
                oldSite->nanimals--;

                // Add the animal to the new site
                newSite->animals = (Animal**)realloc(newSite->animals, sizeof(Animal*) * (newSite->nanimals + 1));
                newSite->animals[newSite->nanimals] = animal;
                newSite->nanimals++;

                // Update the animal's location
                animal->location.x = newX;
                animal->location.y = newY;
            }
        }

        else if (currentSite->type == FEEDING){
            double r = rand() / (double)RAND_MAX;
            if (r <= 0.8) {
                animal->status = ALIVE;
                break;
            }
            else{
                animal->status = ALIVE;
                
                Animal* newAnimal = (Animal*)malloc(sizeof(Animal));
                newAnimal->status = ALIVE;
                newAnimal->type = animal->type;
                newAnimal->location = animal->location;

                // Add the new animal to the nesting site
                currentSite->animals = (Animal**)realloc(currentSite->animals, sizeof(Animal*) * (currentSite->nanimals + 1));
                currentSite->animals[currentSite->nanimals] = newAnimal;
                currentSite->nanimals++;
            
                // Choose a random direction (up, down, left, or right)
                int dx, dy;
                int direction = rand() % 4;
                switch (direction) {
                    case 0: // Up
                        dx = -1;
                        dy = 0;
                        break;
                    case 1: // Down
                        dx = 1;
                        dy = 0;
                        break;
                    case 2: // Left
                        dx = 0;
                        dy = -1;
                        break;
                    case 3: // Right
                        dx = 0;
                        dy = 1;
                        break;
                }

                // Move the animal to a neighboring random location
                int newX = animal->location.x + dx;
                int newY = animal->location.y + dy;

                // Update the grid with the new animal location
                
                Site* oldSite = &grid.sites[animal->location.x][animal->location.y];
                Site* newSite = &grid.sites[newX][newY];

                // Remove the animal from the old site
                for (int i = 0; i < oldSite->nanimals; i++) {
                    if (oldSite->animals[i] == animal) {
                        oldSite->animals[i] = NULL;
                        break;
                    }
                }
                oldSite->nanimals--;

                // Add the animal to the new site
                newSite->animals = (Animal**)realloc(newSite->animals, sizeof(Animal*) * (newSite->nanimals + 1));
                newSite->animals[newSite->nanimals] = animal;
                newSite->nanimals++;

                // Update the animal's location
                animal->location.x = newX;
                animal->location.y = newY; 
            }
        }
            // Sleep for 1 millisecond
            usleep(1000);
    }
    return NULL;
}

/**
 * @brief simulates the moving of a hunter
 *
 * @param args
 * @return void*
 */
void *simulatehunter(void *args) {
    /*TODO: get random position and move,
    then  kill all the animals in this site,
    increase count,
    then sleep*/
    Hunter* hunter = (Hunter*)args;

    while (1) {
        // Get the hunter's current location
        int x = hunter->location.x;
        int y = hunter->location.y;

        // Kill all animals in the current site
        Site* site = &grid.sites[x][y];
        site->nanimals = 0;
        free(site->animals);
        site->animals = NULL;

        hunter->points++;

        // Generate random coordinates for the next move
        int dx, dy;
        do {
            dx = (rand() % 3) - 1; 
            dy = (rand() % 3) - 1;
        } while (x + dx < 0 || x + dx >= grid.xlength || y + dy < 0 || y + dy >= grid.ylength);

        // Update the hunter's location
        hunter->location.x += dx;
        hunter->location.y += dy;

        // Sleep for 1 millisecond
        usleep(1000);
    }
    return NULL;
}

/**
 * the main function for the simulation
 */
int main(int argc, char *argv[]) {
    /*TODO argv[1] is the number of hunters*/
    if(argc < 2){
        printf("Please type number of hunters like './main 2'\n2 is the number of hunters\n");
        return 1;
    }
    int nHunters = atoi(argv[1]); /* number of hunters for the begin*/

    printf("nHunters: %d\n", nHunters);

    initgrid(5, 5);

    /*TODO: init threads for each animal, and hunters, 
        the program should end after 1 second
     */

    /** Creating threads for each animal
     * and get animal type as arg ((void*)BIRD etc.)
    */

    pthread_t birdThread, bearThread, pandaThread;
    pthread_create(&birdThread, NULL, simulateanimal, &bird);
    pthread_create(&bearThread, NULL, simulateanimal, &bear);
    pthread_create(&pandaThread, NULL, simulateanimal, &panda);

    // Create threads for each hunter
    pthread_t hunterThreads[nHunters];
    for (int i = 0; i < nHunters; i++) {
        Hunter* hunter = (Hunter*)malloc(sizeof(Hunter));
        hunter->points = 0;

        pthread_create(&hunterThreads[i], NULL, simulatehunter, hunter);
    }

    sleep(1);

    printgrid();
    deletegrid();

    return 0;
}
