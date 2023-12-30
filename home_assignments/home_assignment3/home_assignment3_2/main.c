#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_LINE_LENGTH 255

struct University {
    char name[256];
    int foundationYear;
    char website[256];
    double latitude;
    double longitude;
    unsigned distance; 
};

unsigned haversine(double lat1, double lon1, double lat2, double lon2) {
    lat1 = lat1 * M_PI / 180.0;
    lon1 = lon1 * M_PI / 180.0;
    lat2 = lat2 * M_PI / 180.0;
    lon2 = lon2 * M_PI / 180.0;

    double dlon = lon2 - lon1;
    double dlat = lat2 - lat1;
    double a = (sin(dlat / 2.0) * sin(dlat / 2.0)) +
               cos(lat1) * cos(lat2) * (sin(dlon / 2.0) * sin(dlon / 2.0));
    double c = 2.0 * asin(sqrt(a));
    double r = 6371.0;

    return (unsigned)floor(c * r);
}

// Функція порівняння для qsort
int compareUniversities(const void *a, const void *b) {
    const struct University *uniA = (const struct University *)a;
    const struct University *uniB = (const struct University *)b;

    if (uniA->distance != uniB->distance) {
        return uniA->distance - uniB->distance;
    } else {
        return strcmp(uniA->name, uniB->name);
    }
}

int main() {
    struct University *universities = NULL;
    int numUniversities = 0;

    char buffer[MAX_LINE_LENGTH];
    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        char *token;
        token = strtok(buffer, ";");
        strcpy(universities[numUniversities].name, token);

        token = strtok(NULL, ";");
        universities[numUniversities].foundationYear = atoi(token);

        token = strtok(NULL, ";");
        strcpy(universities[numUniversities].website, token);

        token = strtok(NULL, ";");
        universities[numUniversities].latitude = atof(token);

        token = strtok(NULL, ";");
        universities[numUniversities].longitude = atof(token);

        numUniversities++;
        universities = realloc(universities, numUniversities * sizeof(struct University));
    }

    for (int i = 0; i < numUniversities; ++i) {
        universities[i].distance = haversine(46.9714764, 32.01839468478296,
        universities[i].latitude, universities[i].longitude);
    }

    qsort(universities, numUniversities, sizeof(struct University), compareUniversities);

    for (int i = 0; i < numUniversities; ++i) {
        printf("%s;%u;%.7f;%.15f\n", universities[i].name, universities[i].distance,
               universities[i].latitude, universities[i].longitude);
    }

    free(universities);

    return 0;
}
