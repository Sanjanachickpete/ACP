#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define CANVAS_WIDTH 60
#define CANVAS_HEIGHT 20
#define MAX_OBJECTS 100
#define BACKGROUND_CHAR '_'
#define DRAW_CHAR '*'

typedef enum {
    OBJ_NONE = 0,
    OBJ_RECTANGLE,
    OBJ_CIRCLE,
    OBJ_LINE,
    OBJ_TRIANGLE,
} ObjectType;

typedef struct {
    int id;
    ObjectType type;
    int x1, y1;
    int x2, y2;
    int x3, y3;
    int radius;
    char drawChar;
    int active;
} GraphicObject;

char canvas[CANVAS_HEIGHT][CANVAS_WIDTH];
GraphicObject objects[MAX_OBJECTS];
int objectCount = 0;
int nextId = 1;

void initCanvas(void) {
    for (int y = 0; y < CANVAS_HEIGHT; ++y) {
        for (int x = 0; x < CANVAS_WIDTH; ++x) {
            canvas[y][x] = BACKGROUND_CHAR;
        }
    }
}

void clearCanvas(void) {
    initCanvas();
}

int isInsideCanvas(int x, int y) {
    return x >= 0 && x < CANVAS_WIDTH && y >= 0 && y < CANVAS_HEIGHT;
}

void setPixel(int x, int y, char ch) {
    if (isInsideCanvas(x, y)) {
        canvas[y][x] = ch;
    }
}

void drawLineOnCanvas(int x0, int y0, int x1, int y1) {
    int dx = abs(x1 - x0);
    int sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0);
    int sy = y0 < y1 ? 1 : -1;
    int err = dx + dy;

    while (1) {
        setPixel(x0, y0, DRAW_CHAR);
        if (x0 == x1 && y0 == y1) break;
        int e2 = 2 * err;
        if (e2 >= dy) {
            err += dy;
            x0 += sx;
        }
        if (e2 <= dx) {
            err += dx;
            y0 += sy;
        }
    }
}

void drawRectangleOnCanvas(int x1, int y1, int x2, int y2) {
    int left = x1 < x2 ? x1 : x2;
    int right = x1 < x2 ? x2 : x1;
    int top = y1 < y2 ? y1 : y2;
    int bottom = y1 < y2 ? y2 : y1;

    for (int x = left; x <= right; ++x) {
        setPixel(x, top, DRAW_CHAR);
        setPixel(x, bottom, DRAW_CHAR);
    }
    for (int y = top; y <= bottom; ++y) {
        setPixel(left, y, DRAW_CHAR);
        setPixel(right, y, DRAW_CHAR);
    }
}

void drawCircleOnCanvas(int centerX, int centerY, int radius) {
    if (radius < 0) return;
    int x = radius;
    int y = 0;
    int err = 1 - radius;

    while (x >= y) {
        setPixel(centerX + x, centerY + y, DRAW_CHAR);
        setPixel(centerX + y, centerY + x, DRAW_CHAR);
        setPixel(centerX - y, centerY + x, DRAW_CHAR);
        setPixel(centerX - x, centerY + y, DRAW_CHAR);
        setPixel(centerX - x, centerY - y, DRAW_CHAR);
        setPixel(centerX - y, centerY - x, DRAW_CHAR);
        setPixel(centerX + y, centerY - x, DRAW_CHAR);
        setPixel(centerX + x, centerY - y, DRAW_CHAR);

        y++;
        if (err <= 0) {
            err += 2 * y + 1;
        } else {
            x--;
            err += 2 * (y - x) + 1;
        }
    }
}

void drawTriangleOnCanvas(int x1, int y1, int x2, int y2, int x3, int y3) {
    drawLineOnCanvas(x1, y1, x2, y2);
    drawLineOnCanvas(x2, y2, x3, y3);
    drawLineOnCanvas(x3, y3, x1, y1);
}

void renderObjects(void) {
    clearCanvas();
    for (int i = 0; i < MAX_OBJECTS; ++i) {
        if (!objects[i].active) continue;
        GraphicObject *obj = &objects[i];
        switch (obj->type) {
            case OBJ_RECTANGLE:
                drawRectangleOnCanvas(obj->x1, obj->y1, obj->x2, obj->y2);
                break;
            case OBJ_CIRCLE:
                drawCircleOnCanvas(obj->x1, obj->y1, obj->radius);
                break;
            case OBJ_LINE:
                drawLineOnCanvas(obj->x1, obj->y1, obj->x2, obj->y2);
                break;
            case OBJ_TRIANGLE:
                drawTriangleOnCanvas(obj->x1, obj->y1, obj->x2, obj->y2, obj->x3, obj->y3);
                break;
            default:
                break;
        }
    }
}

void displayCanvas(void) {
    renderObjects();
    printf("\n");
    for (int y = 0; y < CANVAS_HEIGHT; ++y) {
        for (int x = 0; x < CANVAS_WIDTH; ++x) {
            putchar(canvas[y][x]);
        }
        putchar('\n');
    }
    printf("\n");
}

const char *getTypeName(ObjectType type) {
    switch (type) {
        case OBJ_RECTANGLE: return "Rectangle";
        case OBJ_CIRCLE: return "Circle";
        case OBJ_LINE: return "Line";
        case OBJ_TRIANGLE: return "Triangle";
        default: return "Unknown";
    }
}

int findObjectIndexById(int id) {
    for (int i = 0; i < MAX_OBJECTS; ++i) {
        if (objects[i].active && objects[i].id == id) {
            return i;
        }
    }
    return -1;
}

void listObjects(void) {
    printf("Current objects:\n");
    printf("%-4s %-10s %-20s\n", "ID", "Type", "Parameters");
    for (int i = 0; i < MAX_OBJECTS; ++i) {
        if (!objects[i].active) continue;
        GraphicObject *obj = &objects[i];
        switch (obj->type) {
            case OBJ_RECTANGLE:
                printf("%-4d %-10s (%d,%d)-(%d,%d)\n", obj->id, getTypeName(obj->type), obj->x1, obj->y1, obj->x2, obj->y2);
                break;
            case OBJ_CIRCLE:
                printf("%-4d %-10s center=(%d,%d) r=%d\n", obj->id, getTypeName(obj->type), obj->x1, obj->y1, obj->radius);
                break;
            case OBJ_LINE:
                printf("%-4d %-10s (%d,%d)-(%d,%d)\n", obj->id, getTypeName(obj->type), obj->x1, obj->y1, obj->x2, obj->y2);
                break;
            case OBJ_TRIANGLE:
                printf("%-4d %-10s (%d,%d)-(%d,%d)-(%d,%d)\n", obj->id, getTypeName(obj->type), obj->x1, obj->y1, obj->x2, obj->y2, obj->x3, obj->y3);
                break;
            default:
                break;
        }
    }
    printf("\n");
}

void addObject(void) {
    if (objectCount >= MAX_OBJECTS) {
        printf("Object limit reached. Delete an object before adding a new one.\n\n");
        return;
    }

    printf("Select object type to add:\n");
    printf("1) Rectangle\n");
    printf("2) Circle\n");
    printf("3) Line\n");
    printf("4) Triangle\n");
    printf("Choice: ");
    int choice;
    if (scanf("%d", &choice) != 1) {
        while (getchar() != '\n');
        printf("Invalid input.\n\n");
        return;
    }

    GraphicObject newObj = {0};
    newObj.id = nextId++;
    newObj.drawChar = DRAW_CHAR;
    newObj.active = 1;

    switch (choice) {
        case 1:
            newObj.type = OBJ_RECTANGLE;
            printf("Enter top-left x y and bottom-right x y: ");
            scanf("%d %d %d %d", &newObj.x1, &newObj.y1, &newObj.x2, &newObj.y2);
            break;
        case 2:
            newObj.type = OBJ_CIRCLE;
            printf("Enter center x y and radius: ");
            scanf("%d %d %d", &newObj.x1, &newObj.y1, &newObj.radius);
            break;
        case 3:
            newObj.type = OBJ_LINE;
            printf("Enter start x y and end x y: ");
            scanf("%d %d %d %d", &newObj.x1, &newObj.y1, &newObj.x2, &newObj.y2);
            break;
        case 4:
            newObj.type = OBJ_TRIANGLE;
            printf("Enter first point x y, second point x y, third point x y: ");
            scanf("%d %d %d %d %d %d", &newObj.x1, &newObj.y1, &newObj.x2, &newObj.y2, &newObj.x3, &newObj.y3);
            break;
        default:
            printf("Invalid object type.\n\n");
            return;
    }

    for (int i = 0; i < MAX_OBJECTS; ++i) {
        if (!objects[i].active) {
            objects[i] = newObj;
            ++objectCount;
            printf("Added object with ID %d.\n\n", newObj.id);
            return;
        }
    }
}

void deleteObject(void) {
    listObjects();
    printf("Enter object ID to delete: ");
    int id;
    if (scanf("%d", &id) != 1) {
        while (getchar() != '\n');
        printf("Invalid input.\n\n");
        return;
    }

    int index = findObjectIndexById(id);
    if (index < 0) {
        printf("Object ID %d not found.\n\n", id);
        return;
    }
    objects[index].active = 0;
    --objectCount;
    printf("Deleted object ID %d.\n\n", id);
}

void modifyObject(void) {
    listObjects();
    printf("Enter object ID to modify: ");
    int id;
    if (scanf("%d", &id) != 1) {
        while (getchar() != '\n');
        printf("Invalid input.\n\n");
        return;
    }
    int index = findObjectIndexById(id);
    if (index < 0) {
        printf("Object ID %d not found.\n\n", id);
        return;
    }

    GraphicObject *obj = &objects[index];
    printf("Modifying object ID %d (%s)\n", obj->id, getTypeName(obj->type));
    switch (obj->type) {
        case OBJ_RECTANGLE:
            printf("Enter new top-left x y and bottom-right x y: ");
            scanf("%d %d %d %d", &obj->x1, &obj->y1, &obj->x2, &obj->y2);
            break;
        case OBJ_CIRCLE:
            printf("Enter new center x y and radius: ");
            scanf("%d %d %d", &obj->x1, &obj->y1, &obj->radius);
            break;
        case OBJ_LINE:
            printf("Enter new start x y and end x y: ");
            scanf("%d %d %d %d", &obj->x1, &obj->y1, &obj->x2, &obj->y2);
            break;
        case OBJ_TRIANGLE:
            printf("Enter new first point x y, second point x y, third point x y: ");
            scanf("%d %d %d %d %d %d", &obj->x1, &obj->y1, &obj->x2, &obj->y2, &obj->x3, &obj->y3);
            break;
        default:
            printf("Unknown object type.\n");
            return;
    }
    printf("Object ID %d updated.\n\n", obj->id);
}

void showMenu(void) {
    printf("2D Graphics Editor\n");
    printf("Canvas size: %d x %d\n", CANVAS_WIDTH, CANVAS_HEIGHT);
    printf("1) Display picture\n");
    printf("2) Add object\n");
    printf("3) Delete object\n");
    printf("4) Modify object\n");
    printf("5) List objects\n");
    printf("6) Clear all objects\n");
    printf("0) Exit\n");
    printf("Choice: ");
}

void clearAllObjects(void) {
    for (int i = 0; i < MAX_OBJECTS; ++i) {
        objects[i].active = 0;
    }
    objectCount = 0;
    printf("All objects removed from picture.\n\n");
}

int main(void) {
    initCanvas();
    memset(objects, 0, sizeof(objects));

    while (1) {
        showMenu();
        int option;
        if (scanf("%d", &option) != 1) {
            while (getchar() != '\n');
            printf("Invalid choice. Please enter a number.\n\n");
            continue;
        }
        switch (option) {
            case 1:
                displayCanvas();
                break;
            case 2:
                addObject();
                break;
            case 3:
                deleteObject();
                break;
            case 4:
                modifyObject();
                break;
            case 5:
                listObjects();
                break;
            case 6:
                clearAllObjects();
                break;
            case 0:
                printf("Exiting editor.\n");
                return 0;
            default:
                printf("Invalid choice. Please choose between 0 and 6.\n\n");
                break;
        }
    }

    return 0;
}
