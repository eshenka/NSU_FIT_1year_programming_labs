#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

#define min 0
#define max_number_of_points 100000

typedef struct Point {
    double x;
    double y;
} Point;

int BadNumberOfPoints(int n) {
    return n < min || n > max_number_of_points;
}

void ScanPoints(int number_of_points, Point* points, int* error) {
    double x, y;
    for (int i = 0; i < number_of_points; i++) {
        if (scanf("%lf %lf", &x, &y) != 2) {
            *error = 1;
            break;
        }
        points[i] = (Point) {x, y};
    }
}

int ComparePoints(Point* i, Point* j) {
    return i->x > j->x || (i->x == j->x && i->y > j->y);
}

int Rotation(Point p1, Point p2, Point p3) {
    return (p2.x - p1.x) * (p3.y - p1.y) - (p2.y - p1.y) * (p3.x - p1.x) <= 0;
}

void AndrewAlgo(int number_of_points, Point* points) {
    Point* hull = (Point*) malloc (sizeof(Point) * (number_of_points + 1));
    int size = 0;
    for (int i = 0; i < number_of_points; i++) {
        while (size >= 2 && (Rotation(hull[size - 2], hull[size - 1], points[i]))) {
            size--;
        }
        hull[size++] = points[i];
    }
    for (int i = number_of_points - 2, t = size + 1; i >= 0; i--) {
        while (size >= t && (Rotation(hull[size - 2], hull[size - 1], points[i]))) {
            size--;
        }
        hull[size++] = points[i];
    }
    for (int i = 0; i < size - 1; i++) {
        printf("%.0f %.0f\n", hull[i].x, hull[i].y);
    }
    free(hull);
}

int main() {
    int number_of_points;
    if (!scanf("%d", &number_of_points)) {
        return 0;
    }
    if (BadNumberOfPoints(number_of_points)) {
        printf("bad number of points");
        return 0;
    }

    Point* points = (Point*) malloc (sizeof(Point) * number_of_points);
    int bad_number_of_lines = 0;
    ScanPoints(number_of_points, points, &bad_number_of_lines);
    if (bad_number_of_lines) {
        printf("bad number of lines");
        free(points);
        return 0;
    }
    qsort(points, number_of_points, sizeof(Point), (int (*)(const void*, const void*)) ComparePoints);

    if (number_of_points < 3) {
        for (int i = 0; i < number_of_points; i++) {
            printf("%.0f %.0f\n", points[i].x, points[i].y);
        }
    } else {
        AndrewAlgo(number_of_points, points);
    }

    free(points);
    return 0;
}
