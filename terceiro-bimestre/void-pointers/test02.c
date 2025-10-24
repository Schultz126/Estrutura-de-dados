#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME_LENGTH 32

// Define the function pointer type for the print callback
typedef void (*PrintCallback)(const void* element);

typedef struct {
    char name[NAME_LENGTH];
    int age;
} Person;

typedef struct {
    float x;
    float y;
} Point;

// A generic function that iterates through an array of unknown type
void printStuff(const void* array, size_t num_elements, size_t element_size, PrintCallback printer) {
    for (size_t i = 0; i < num_elements; i++) {
        // Calculate the address of the current element
        const void* current_element = (const char*)array + i * element_size;
        // Call the specific print function for this element
        printer(current_element);
    }
}

// Specific print function for the Person type
void printPerson(const void* element) {
    const Person* p = (const Person*)element;
    printf("  Name: %s, Age: %d\n", p->name, p->age);
}

// Specific print function for the Point type
void printPoint(const void* element) {
    const Point* pt = (const Point*)element;
    printf("  Point: (%.2f, %.2f)\n", pt->x, pt->y);
}

int main(void) {
    // Array of Persons
    Person people[2];
    strcpy(people[0].name, "Rafael");
    people[0].age = 19;
    strcpy(people[1].name, "Joao");
    people[1].age = 18;

    printf("Printing Persons:\n");
    printStuff(people, 2, sizeof(Person), printPerson);

    printf("\n");

    // Array of Points
    Point points[3] = {{1.0, 2.0}, {3.5, 4.5}, {5.0, 6.0}};

    printf("Printing Points:\n");
    printStuff(points, 3, sizeof(Point), printPoint);

    return 0;
}