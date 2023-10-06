#include <stdio.h>
#include <malloc.h>
#include <stdbool.h>

#define max_verticies 2000
#define min_verticies_and_edges 0
#define WHITE 0
#define GREY 1
#define BLACK 2

int cnt = 0;

enum Errors {
    NO_ERRORS,
    BAD_NUMBER_OF_VERTICIES,
    BAD_NUMBER_OF_EDGES,
    BAD_VERTEX,
    BAD_NUMBER_OF_LINES,
    IMPOSSIBLE_TO_SORT
};

enum Errors CheckVerticiesAndEdges(int* vertices, int* edges) {
    int max_edges = *vertices * (*vertices + 1) / 2;
    if (*vertices > max_verticies || *vertices < min_verticies_and_edges) {
        return BAD_NUMBER_OF_VERTICIES;
    }
    if (*edges < min_verticies_and_edges || *edges > max_edges) {
        return BAD_NUMBER_OF_EDGES;
    }
    return NO_ERRORS;
}

enum Errors FillingGraph(bool* graph, int* number_of_vertices, int* number_of_edges) {
    int src, dst;
    for (int i = 0; i < *number_of_edges; i++) {
        if (scanf("%d %d", &src, &dst) != 2) {
            return BAD_NUMBER_OF_LINES;
        }
        if (src < 0 || src > *number_of_vertices || dst < 0 || dst > *number_of_vertices) {
            return BAD_VERTEX;
        }
        graph[(src-1)*(*number_of_vertices)+dst-1] = 1;
    }
    return NO_ERRORS;
}

enum Errors RecursiveSort(int number_of_vertices, bool* graph, short int* colors, int* sort, int start) {
    colors[start] = GREY;
    enum Errors error = NO_ERRORS;
    for (int i = 0; i < number_of_vertices; i++) {
        if (colors[i] == BLACK || graph[start*number_of_vertices+i] == 0) {
            continue;
        }
        if (colors[i] == GREY) {
            error = IMPOSSIBLE_TO_SORT;
            break;
        }
        colors[i] = GREY;
        error = RecursiveSort(number_of_vertices, graph, colors, sort, i);
    }
    sort[cnt] = start;
    cnt++;
    colors[start] = BLACK;
    return error;
}

enum Errors TopSort(int number_of_vertices, bool* graph) {
    enum Errors error =  NO_ERRORS;
    short int* colors = (short int*) malloc (number_of_vertices * sizeof(short int));
    int* sort = (int*) malloc (number_of_vertices * sizeof(int));
    for (int i = 0; i < number_of_vertices; i++) {
        colors[i] = WHITE;
    }
    for (int i = 0; i < number_of_vertices; i++) {
        if (error != NO_ERRORS) {
            break;
        }
        if (colors[i] == BLACK) {
            continue;
        }
        error = RecursiveSort(number_of_vertices, graph, colors, sort, i);
    }
    if (error == NO_ERRORS) {
        for (int i = number_of_vertices-1; i >= 0; i--) {
            printf("%d ", sort[i]+1);
        }
    }
    free(colors);
    free(sort);
    return error;
}

int main() {
    enum Errors error = NO_ERRORS;
    int number_of_vertices = 0;
    int number_of_edges = 0;
    if(scanf("%d\n%d", &number_of_vertices, &number_of_edges) != 2) {
        printf("bad number of lines");
        return 0;
    }
    error = CheckVerticiesAndEdges(&number_of_vertices, &number_of_edges);
    bool* graph = (bool*) calloc(number_of_vertices * number_of_vertices, sizeof(bool));
    if (error == NO_ERRORS) {
        error = FillingGraph(graph, &number_of_vertices, &number_of_edges);
    }
    if (error == NO_ERRORS) {
        error = TopSort(number_of_vertices, graph);
    }
    free(graph);
    switch (error) {
        case BAD_NUMBER_OF_VERTICIES: printf("bad number of vertices"); return 0;
        case BAD_NUMBER_OF_EDGES: printf("bad number of edges"); return 0;
        case BAD_NUMBER_OF_LINES: printf("bad number of lines"); return 0;
        case BAD_VERTEX: printf("bad vertex"); return 0;
        case IMPOSSIBLE_TO_SORT: printf("impossible to sort"); return 0;
        default: return 0;
    }
    return 0;
}
