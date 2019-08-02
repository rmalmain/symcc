// RUN: %symcc -O2 %s -o %t
// RUN: echo -ne "\x05\x00\x00\x00" | %t | FileCheck %s
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

struct point {
    int x;
    int y;
};

struct line {
    struct point start;
    struct point end;
};

static struct point g_point = {1, 2};
static struct point g_point_array[] = {{1, 2}, {3, 4}, {5, 6}};

int main(int argc, char* argv[]) {
    int x;
    if (read(STDIN_FILENO, &x, sizeof(x)) != sizeof(x)) {
        printf("Failed to read x\n");
        return -1;
    }

    struct point p = {x, 17};

    printf("%s\n", (p.x < 100) ? "yes" : "no");
    // CHECK: Trying to solve
    // CHECK: Found diverging input
    // CHECK: yes

    printf("%s\n", (p.y < 100) ? "yes" : "no");
    // CHECK-NOT: Trying to solve
    // CHECK: yes

    printf("%s\n", (p.x < p.y) ? "yes" : "no");
    // CHECK: Trying to solve
    // CHECK: Found diverging input
    // CHECK: yes

    printf("%s\n", ((p.x < g_point.x) || (p.y < g_point.y)) ? "yes" : "no");
    // CHECK: Trying to solve
    // CHECK: Found diverging input
    // CHECK: no

    printf("%s\n", (g_point_array[1].x < x) ? "yes" : "no");
    // CHECK: Trying to solve
    // CHECK: Found diverging input
    // CHECK: yes

    // Nested structs

    struct line l = {{0, 0}, {5, 5}};

    printf("%s\n", (l.end.x > x) ? "yes" : "no");
    // CHECK: Trying to solve
    // CHECK: Found diverging input
    // CHECK: no

    return 0;
}
