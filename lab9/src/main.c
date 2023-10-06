#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <malloc.h>

#define min_number 0
#define max_number_vert 5000
#define NO_WAY UINT_MAX
#define INT_MAX_PLUS (NO_WAY - 1)

typedef enum Errors {
    NO_ERRORS,
    BAD_NUMBER_OF_VERTICES,
    BAD_NUMBER_OF_EDGES,
    BAD_VERTEX,
    BAD_LENGTH,
    BAD_NUMBER_OF_LINES,
} Errors;

void PrintError(Errors error) {
    switch (error) {
    case BAD_NUMBER_OF_VERTICES:
        printf("bad number of vertices");
        return;
    case BAD_NUMBER_OF_EDGES:
        printf("bad number of edges");
        return;
    case BAD_VERTEX:
        printf("bad vertex");
        return;
    case BAD_LENGTH:
        printf("bad length");
        return;
    case BAD_NUMBER_OF_LINES:
        printf("bad number of lines");
        return;
    default:
        return;
    }
}

void CheckError(Errors error) {
    if (error != NO_ERRORS) {
        PrintError(error);
        exit(0);
    }
}

void GetAndCheckVariables(int* number_of_vertices, int* number_of_edges, int* src, int* dst, Errors* error) {
    if (!scanf("%d %d %d %d", number_of_vertices, src, dst, number_of_edges)) {
        *error = BAD_NUMBER_OF_LINES;
        return;
    }
    if (*number_of_vertices < min_number || max_number_vert < *number_of_vertices) {
        *error = BAD_NUMBER_OF_VERTICES;
        return;
    }

    int max_number_edges = *number_of_vertices * (*number_of_vertices - 1) / 2;
    if (*number_of_edges < min_number || max_number_edges < *number_of_edges) {
        *error = BAD_NUMBER_OF_EDGES;
        return;
    }
    if (*src < 1 || *number_of_vertices < *src || *dst < 1 || *number_of_vertices < *dst) {
        *error = BAD_VERTEX;
        return;
    }
    (*src)--;
    (*dst)--;
}


void AddGraph(unsigned int* graph, int number_of_vertices, int src, int dst, unsigned int weight) {
    graph[(src - 1) * number_of_vertices + (dst - 1)] = weight;
}


unsigned int GetGraph(unsigned int* graph, int number_of_vertices, int src, int dst) {
    return graph[src * number_of_vertices + dst];
}

void FillStructures(int number_of_vertices, unsigned int* shortestWayFromS, char* used) {
    for (int i = 0; i < number_of_vertices; i++) {
        shortestWayFromS[i] = NO_WAY;
        used[i] = 0;
    }
}

void ScanEdges(unsigned int* graph, int number_of_vertices, int number_of_edges, Errors *error) {
    for (int i = 0; i < number_of_edges; i++) {
        int src;
        int dst;
        unsigned int weight;

        if (scanf("%d %d %u", &src, &dst, &weight) != 3) {
            free(graph);
            *error = BAD_NUMBER_OF_LINES;
            return;
        }
        if (src < 1 || number_of_vertices < src || dst < 1 || number_of_vertices < dst) {
            free(graph);
            *error = BAD_VERTEX;
            return;
        }
        if (weight < 1 || INT_MAX < weight) {
            free(graph);
            *error = BAD_LENGTH;
            return;
        }

        AddGraph(graph, number_of_vertices, src, dst, weight);
        AddGraph(graph, number_of_vertices, dst, src, weight);
    }
}

int NextShortestWay(int number_of_vertices, unsigned int* graph, unsigned int* shortest_ways, char* used, int* path, int dst) {
    for (int i = 0; i < number_of_vertices; i++) {
        unsigned int cur_length = GetGraph(graph, number_of_vertices, dst, i);

        if (!used[i] && cur_length) {
            long long new_length = (long long) cur_length + shortest_ways[dst];

            if (new_length <= shortest_ways[i]) {
                if (new_length > INT_MAX) {
                    shortest_ways[i] = INT_MAX_PLUS;
                }
                else {
                    shortest_ways[i] = new_length;
                }

                path[i] = dst;
            }
        }
    }

    unsigned int min_length = NO_WAY;
    int min_vertex = 0;
    for (int i = 0; i < number_of_vertices; i++) {
        if (!used[i] && i != dst) {
            if (shortest_ways[i] < min_length) {
                min_length = shortest_ways[i];
                min_vertex = i;
            }
        }
    }

    return min_vertex;
}

void PrintShortestWays(int number_of_vertices, unsigned int* shortest_ways) {
    for (int i = 0; i < number_of_vertices; i++) {
        if (shortest_ways[i] == NO_WAY) {
            printf("oo ");
        }
        else if (shortest_ways[i] == INT_MAX_PLUS) {
            printf("INT_MAX+ ");
        }
        else {
            printf("%u ", shortest_ways[i]);
        }
    }
    printf("\n");
}

void PrintTheWay(unsigned int* shortest_ways, int pathes_to_dst, int* path, int dst) {
    if (shortest_ways[dst] == NO_WAY)
        printf("no path");
    else if (shortest_ways[dst] == INT_MAX_PLUS && pathes_to_dst > 1)
        printf("overflow");
    else {
        while (path[dst] != dst) {
            printf("%d ", dst + 1);
            dst = path[dst];
        }
        printf("%d ", dst + 1);
    }
}

void DijkstraAlgorithm(int number_of_vertices, unsigned int* graph, int src, int dst) {
    unsigned int* shortest_ways = (unsigned int*) malloc (number_of_vertices * sizeof(unsigned int));
    int* path = (int*) malloc (number_of_vertices * sizeof(int));
    char used[number_of_vertices];

    FillStructures(number_of_vertices, shortest_ways, used);

    shortest_ways[src] = 0;
    path[src] = src;

    int next = src;
    for (int i = 0; i < number_of_vertices; i++) {
        used[next] = 1;
        next = NextShortestWay(number_of_vertices, graph, shortest_ways, used, path, next);
    }

    int pathes_to_dst = 0;
    for (int i = 0; i < number_of_vertices; i++) {
        int length = GetGraph(graph, number_of_vertices, dst, i);
        if (shortest_ways[i] != NO_WAY && length) {
            pathes_to_dst++;
        }
    }

    PrintShortestWays(number_of_vertices, shortest_ways);
    PrintTheWay(shortest_ways, pathes_to_dst, path, dst);

    free(shortest_ways);
    free(path);
}

int main() {
    Errors error = NO_ERRORS;

    int number_of_vertices;
    int number_of_edges;
    int src;
    int dst;
    GetAndCheckVariables(&number_of_vertices, &number_of_edges, &src, &dst, &error);
    CheckError(error);

    unsigned int* graph = (unsigned int*) malloc(number_of_vertices * number_of_vertices * sizeof(unsigned int));
    for (int i = 1; i <= number_of_vertices; i++) {
        for (int j = 1; j <= number_of_vertices; j++) {
            AddGraph(graph, number_of_vertices, i, j, 0);
        }
    }

    ScanEdges(graph, number_of_vertices, number_of_edges, &error);
    CheckError(error);

    if (error == NO_ERRORS) {
        DijkstraAlgorithm(number_of_vertices, graph, src, dst);
        free(graph);
    }
    
    return 0;
}
