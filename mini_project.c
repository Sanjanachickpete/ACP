#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define ROWS 25
#define COLS 80
#define MAX_OBJECTS 50

typedef enum {
    OBJ_CIRCLE,
    OBJ_RECTANGLE,
    OBJ_LINE,
    OBJ_TRIANGLE
} ObjectType;

typedef struct {
    ObjectType type;
    int x1, y1;
    int x2, y2;
    int x3, y3;
    int radius;
    int id;
} GraphicObject;

char canvas[ROWS][COLS];
GraphicObject objects[MAX_OBJECTS];
int objectCount = 0;
int nextId = 1;

void clearCanvas(void) {
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            canvas[r][c] = '_';
        }
    }
}

void printCanvas(void) {
    printf("\n");
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            putchar(canvas[r][c]);
        }
        putchar('\n');
    }
    printf("\n");
}

int inBounds(int x, int y) {
    return x >= 0 && x < COLS && y >= 0 && y < ROWS;
}

void setPixel(int x, int y) {
    if (inBounds(x, y)) {
        canvas[y][x] = '*';
    }
}

void drawLine(int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = x1 < x2 ? 1 : -1;
    int sy = y1 < y2 ? 1 : -1;
    int err = dx - dy;

    while (1) {
        setPixel(x1, y1);
        if (x1 == x2 && y1 == y2) break;
        int e2 = err * 2;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}

void drawRectangle(int x, int y, int width, int height) {
    int x2 = x + width - 1;
    int y2 = y + height - 1;
    for (int cx = x; cx <= x2; cx++) {
        setPixel(cx, y);
        setPixel(cx, y2);
    }
    for (int cy = y; cy <= y2; cy++) {
        setPixel(x, cy);
        setPixel(x2, cy);
    }
}

void drawCircle(int cx, int cy, int radius) {
    if (radius <= 0) return;
    int x = radius;
    int y = 0;
    int err = 0;

    while (x >= y) {
        setPixel(cx + x, cy + y);
        setPixel(cx + y, cy + x);
        setPixel(cx - y, cy + x);
        setPixel(cx - x, cy + y);
        setPixel(cx - x, cy - y);
        setPixel(cx - y, cy - x);
        setPixel(cx + y, cy - x);
        setPixel(cx + x, cy - y);

        y++;
        if (err <= 0) {
            err += 2 * y + 1;
        }
        if (err > 0) {
            x--;
            err -= 2 * x + 1;
        }
    }
}

void drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3) {
    drawLine(x1, y1, x2, y2);
    drawLine(x2, y2, x3, y3);
    drawLine(x3, y3, x1, y1);
}

void renderObjects(void) {
    clearCanvas();
    for (int i = 0; i < objectCount; i++) {
        GraphicObject *obj = &objects[i];
        switch (obj->type) {
            case OBJ_CIRCLE:
                drawCircle(obj->x1, obj->y1, obj->radius);
                break;
            case OBJ_RECTANGLE:
                drawRectangle(obj->x1, obj->y1, obj->x2, obj->y2);
                break;
            case OBJ_LINE:
                drawLine(obj->x1, obj->y1, obj->x2, obj->y2);
                break;
            case OBJ_TRIANGLE:
                drawTriangle(obj->x1, obj->y1, obj->x2, obj->y2, obj->x3, obj->y3);
                break;
        }
    }
}

void listObjects(void) {
    if (objectCount == 0) {
        printf("No objects in the picture.\n");
        return;
    }
    printf("Current objects:\n");
    for (int i = 0; i < objectCount; i++) {
        GraphicObject *obj = &objects[i];
        printf("[%d] ", obj->id);
        switch (obj->type) {
            case OBJ_CIRCLE:
                printf("Circle center=(%d,%d) r=%d\n", obj->x1, obj->y1, obj->radius);
                break;
            case OBJ_RECTANGLE:
                printf("Rectangle top-left=(%d,%d) w=%d h=%d\n", obj->x1, obj->y1, obj->x2, obj->y2);
                break;
            case OBJ_LINE:
                printf("Line from=(%d,%d) to=(%d,%d)\n", obj->x1, obj->y1, obj->x2, obj->y2);
                break;
            case OBJ_TRIANGLE:
                printf("Triangle (%d,%d), (%d,%d), (%d,%d)\n", obj->x1, obj->y1, obj->x2, obj->y2, obj->x3, obj->y3);
                break;
        }
    }
}

void addCircle(void) {
    if (objectCount >= MAX_OBJECTS) {
        printf("Object limit reached.\n");
        return;
    }
    GraphicObject obj;
    obj.type = OBJ_CIRCLE;
    obj.id = nextId++;
    printf("Enter center x y: ");
    scanf("%d %d", &obj.x1, &obj.y1);
    printf("Enter radius: ");
    scanf("%d", &obj.radius);
    objects[objectCount++] = obj;
}

void addRectangle(void) {
    if (objectCount >= MAX_OBJECTS) {
        printf("Object limit reached.\n");
        return;
    }
    GraphicObject obj;
    obj.type = OBJ_RECTANGLE;
    obj.id = nextId++;
    printf("Enter top-left x y: ");
    scanf("%d %d", &obj.x1, &obj.y1);
    printf("Enter width height: ");
    scanf("%d %d", &obj.x2, &obj.y2);
    objects[objectCount++] = obj;
}

void addLine(void) {
    if (objectCount >= MAX_OBJECTS) {
        printf("Object limit reached.\n");
        return;
    }
    GraphicObject obj;
    obj.type = OBJ_LINE;
    obj.id = nextId++;
    printf("Enter start x y: ");
    scanf("%d %d", &obj.x1, &obj.y1);
    printf("Enter end x y: ");
    scanf("%d %d", &obj.x2, &obj.y2);
    objects[objectCount++] = obj;
}

void addTriangle(void) {
    if (objectCount >= MAX_OBJECTS) {
        printf("Object limit reached.\n");
        return;
    }
    GraphicObject obj;
    obj.type = OBJ_TRIANGLE;
    obj.id = nextId++;
    printf("Enter first point x y: ");
    scanf("%d %d", &obj.x1, &obj.y1);
    printf("Enter second point x y: ");
    scanf("%d %d", &obj.x2, &obj.y2);
    printf("Enter third point x y: ");
    scanf("%d %d", &obj.x3, &obj.y3);
    objects[objectCount++] = obj;
}

void deleteObject(void) {
    if (objectCount == 0) {
        printf("No objects to delete.\n");
        return;
    }
    listObjects();
    int id;
    printf("Enter object id to delete: ");
    scanf("%d", &id);
    int index = -1;
    for (int i = 0; i < objectCount; i++) {
        if (objects[i].id == id) {
            index = i;
            break;
        }
    }
    if (index == -1) {
        printf("Object id %d not found.\n", id);
        return;
    }
    for (int i = index; i < objectCount - 1; i++) {
        objects[i] = objects[i + 1];
    }
    objectCount--;
    printf("Object %d deleted.\n", id);
}

void addObjectMenu(void) {
    int choice;
    printf("Add object:\n");
    printf("1. Circle\n");
    printf("2. Rectangle\n");
    printf("3. Line\n");
    printf("4. Triangle\n");
    printf("Enter choice: ");
    scanf("%d", &choice);
    switch (choice) {
        case 1: addCircle(); break;
        case 2: addRectangle(); break;
        case 3: addLine(); break;
        case 4: addTriangle(); break;
        default: printf("Invalid shape choice.\n"); break;
    }
}

void showMenu(void) {
    printf("2D Graphics Editor\n");
    printf("-------------------\n");
    printf("1. Add object\n");
    printf("2. Delete object\n");
    printf("3. Display picture\n");
    printf("4. List objects\n");
    printf("5. Exit\n");
    printf("Enter choice: ");
}

int main(void) {
    clearCanvas();
    int choice;
    do {
        showMenu();
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Exiting.\n");
            break;
        }
        switch (choice) {
            case 1:
                addObjectMenu();
                renderObjects();
                break;
            case 2:
                deleteObject();
                renderObjects();
                break;
            case 3:
                printCanvas();
                break;
            case 4:
                listObjects();
                break;
            case 5:
                printf("Goodbye.\n");
                break;
            default:
                printf("Invalid menu option.\n");
                break;
        }
    } while (choice != 5);

    return 0;
}
