#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct matrixEntry {
  int height;
  int width;
  int type;
  int x1;
  int x2;
  int y1;
  int y2;
  int **matrix;
  struct colored_matrix **matrixc;
};

struct colored_matrix {
  int red;
  int blue;
  int yellow;
};
// incarca o poza alb negru de tip text
struct matrixEntry *readp(char *file_name) {
  FILE *file;
  char s[10];
  struct matrixEntry *entry;
  entry = malloc(sizeof(struct matrixEntry));
  int size, i, j;
  file = fopen(file_name, "rt");
  fgets(s, 10, file);
  fscanf(file, "%d", &entry->width);
  fscanf(file, "%d", &entry->height);
  fscanf(file, "%d", &size);
  entry->matrix = malloc(sizeof(int *) * entry->height);
  for (i = 0; i < entry->height; i++) {
    entry->matrix[i] = malloc(sizeof(int) * entry->width);
  }
  for (i = 0; i < entry->height; i++) {
    for (j = 0; j < entry->width; j++) {
      fscanf(file, "%d", &entry->matrix[i][j]);
    }
  }
  entry->x1 = 0;
  entry->x2 = entry->width;
  entry->y1 = 0;
  entry->y2 = entry->height;
  fclose(file);
  return entry;
}
// incarca o poza alb negru de tip bin
struct matrixEntry *readpb(char *file_name) {
  FILE *file;
  char s[10];
  struct matrixEntry *entry;
  entry = malloc(sizeof(struct matrixEntry));
  int size, i, j;
  file = fopen(file_name, "rb");
  fgets(s, 10, file);
  fscanf(file, "%d", &entry->width);
  fscanf(file, "%d", &entry->height);
  fscanf(file, "%d", &size);
  entry->matrix = calloc(entry->height, sizeof(int *));
  for (i = 0; i < entry->height; i++) {
    entry->matrix[i] = calloc(entry->width, sizeof(int *));
  }
  for (i = 0; i < entry->height; i++) {
    for (j = 0; j < entry->width; j++) {
      fread(&entry->matrix[i][j], 1, 1, file);
    }
  }
  fclose(file);
  // printf("%d %d %d\r\n", entry->height, entry->width, entry->matrix[0][0]);
  entry->x1 = 0;
  entry->x2 = entry->width;
  entry->y1 = 0;
  entry->y2 = entry->height;
  return entry;
}
// incarca o poza color de tip text
struct matrixEntry *readpc(char *file_name) {
  FILE *file;
  char s[10];
  struct matrixEntry *entry;
  entry = malloc(sizeof(struct matrixEntry));
  int size, i, j;
  file = fopen(file_name, "rt");
  fgets(s, 10, file);
  fscanf(file, "%d", &entry->width);
  fscanf(file, "%d", &entry->height);
  fscanf(file, "%d", &size);
  entry->matrixc = calloc(sizeof(int *), entry->height);
  for (i = 0; i < entry->height; i++) {
    entry->matrixc[i] = malloc(sizeof(struct colored_matrix) * entry->width);
  }
  for (i = 0; i < entry->height; i++) {
    for (j = 0; j < entry->width; j++) {
      fscanf(file, "%d", &entry->matrixc[i][j].red);
      fscanf(file, "%d", &entry->matrixc[i][j].blue);
      fscanf(file, "%d", &entry->matrixc[i][j].yellow);
    }
  }
  entry->x1 = 0;
  entry->x2 = entry->width;
  entry->y1 = 0;
  entry->y2 = entry->height;
  fclose(file);
  return entry;
}
// incarca o poza color de tip bin
struct matrixEntry *readpcb(char *file_name) {
  FILE *file;
  char s[10];
  struct matrixEntry *entry;
  entry = calloc(1, sizeof(struct matrixEntry));
  int size, i, j;
  file = fopen(file_name, "rb");
  fgets(s, 10, file);
  fscanf(file, "%d", &entry->width);
  fscanf(file, "%d", &entry->height);
  fscanf(file, "%d", &size);
  entry->matrixc = calloc(sizeof(int *), entry->height);
  for (i = 0; i < entry->height; i++) {
    entry->matrixc[i] = calloc(entry->width, sizeof(struct colored_matrix));
  }
  for (i = 0; i < entry->height; i++) {
    for (j = 0; j < entry->width; j++) {
      fread(&entry->matrixc[i][j].red, 1, 1, file);
      fread(&entry->matrixc[i][j].blue, 1, 1, file);
      fread(&entry->matrixc[i][j].yellow, 1, 1, file);
    }
  }
  entry->x1 = 0;
  entry->x2 = entry->width;
  entry->y1 = 0;
  entry->y2 = entry->height;
  fclose(file);
  return entry;
}
// incarca poza conform caracteristicilor ei
struct matrixEntry *read_file(char *file_name, int *type) {
  FILE *file;
  file = fopen(file_name, "r");
  char imag_type[10];
  fgets(imag_type, 10, file);
  fclose(file);
  struct matrixEntry *entry;
  if (strncmp(imag_type, "P1", 2) == 0) {
    *type = 1;
  } else if (strncmp(imag_type, "P2", 2) == 0) {
    entry = readp(file_name);
    *type = 2;
    entry->type = 2;
  } else if (strncmp(imag_type, "P3", 2) == 0) {
    entry = readpc(file_name);
    *type = 3;
    entry->type = 3;
  } else if (strncmp(imag_type, "P4", 2) == 0) {
    *type = 4;
  } else if (strncmp(imag_type, "P5", 2) == 0) {
    entry = readpb(file_name);
    *type = 5;
    entry->type = 5;
  } else if (strncmp(imag_type, "P6", 2) == 0) {
    entry = readpcb(file_name);
    *type = 6;
    entry->type = 6;
  }
  printf("Loaded %s\r\n", file_name);
  return entry;
}
// verifica daca parametrii primiti de select sunt in ordinea potrivita
void checkselect(struct matrixEntry *entry, int a, int b, int c, int d) {
  if (b < a) {
    int x = a;
    a = b;
    b = x;
  }
  if (d < c) {
    int x = c;
    c = d;
    d = x;
  }
  entry->x1 = a;
  entry->x2 = b;
  entry->y1 = c;
  entry->y2 = d;
  printf("Selected %d %d %d %d\r\n", entry->x1, entry->y1, entry->x2,
         entry->y2);
}
// verifica daca parametrii primiti sunt numere
int number(char *p) {
  for (unsigned long long i = 0; i < strlen(p); i++) {
    if (isdigit(p[i]) == 0) {
      return 0;
    }
  }
  return 1;
}
// verifica daca parametrii primiti x1 si x2 partin matricei
int check_coordinatesx(int x, struct matrixEntry *entry) {
  if (x < 0 || x > entry->height) {
    return 0;
  } else {
    return 1;
  }
}
// verifica daca parametrii primiti y1 y2 apartin matricei
int check_coordinatesy(int x, struct matrixEntry *entry) {
  if (x < 0 || x > entry->width) {
    return 0;
  } else {
    return 1;
  }
}
// selecteaza o portiune din poza
void selectxy(char *p, struct matrixEntry *entry) {
  if (p == NULL || number(p) == 0 || check_coordinatesx(atoi(p), entry) == 0) {
    printf("Invalid set of coordinates\r\n");
  } else {
    int a = atoi(p);
    p = strtok(NULL, " ");
    if (p == NULL || number(p) == 0 ||
        check_coordinatesx(atoi(p), entry) == 0) {
      printf("Invalid set of coordinates\r\n");
    } else {
      int b = atoi(p);
      p = strtok(NULL, " ");
      if (p == NULL || number(p) == 0 ||
          check_coordinatesy(atoi(p), entry) == 0) {
        printf("Invalid set of coordinates\r\n");
      } else {
        int c = atoi(p);
        p = strtok(NULL, " ");
        if (p == NULL || number(p) == 0 ||
            check_coordinatesy(atoi(p), entry) == 0) {
          printf("Invalid set of coordinates\r\n");
        } else {
          int d = atoi(p);
          checkselect(entry, a, c, b, d);
        }
      }
    }
  }
}
// selecteaza toata poza
void selectall(struct matrixEntry *entry) {
  entry->x1 = 0;
  entry->x2 = entry->width;
  entry->y1 = 0;
  entry->y2 = entry->height;
  printf("Selected ALL\r\n");
}
// verifica daca parametrul y primit de histograma este putere a lui 2
int exponensial2(int x) {
  int i, p = 1, v[9];
  for (i = 0; i < 9; i++) {
    v[i] = p;
    p *= 2;
  }
  for (i = 0; i < 9; i++) {
    if (x == v[i]) {
      return 1;
    }
  }
  return 0;
}

void histogram(char *p, struct matrixEntry *entry) {
  p = strtok(NULL, " ");
  if (p == NULL || number(p) == 0) {
    printf("Invalid set of parameters\r\n");
    return;
  }
  int stars = atoi(p);
  p = strtok(NULL, " ");
  if (p == NULL || number(p) == 0 || exponensial2(atoi(p)) == 0) {
    printf("Invalid set of parameters\r\n");
    return;
  }
  int bins = atoi(p);
  long long *v;
  int group = 256 / bins;
  v = calloc(256, sizeof(long long));
  int i, j;
  for (i = 0; i < entry->height; i++) {
    for (j = 0; j < entry->width; j++) {
      v[entry->matrix[i][j] / group]++;
    }
  }
  int max = -100;
  for (i = 0; i < 256; i++)
    if (v[i] > max) max = v[i];
  int *no_stars;
  no_stars = malloc(256 * sizeof(int));
  for (i = 0; i < bins; i++) {
    no_stars[i] = round(((double)v[i] / max) * stars);
  }
  for (i = 0; i < bins; i++) {
    printf("%d\t|\t", no_stars[i]);
    for (j = 0; j < no_stars[i]; j++) {
      printf("*");
    }
    printf("\n");
  }
  free(v);
  free(no_stars);
}
// verifica daca fisierul exista
int checkfile(char *file_name) {
  FILE *file;
  file = fopen(file_name, "r");
  if (file == NULL) {
    printf("Failed to load %s\r\n", file_name);
    return 0;
  }
  fclose(file);
  return 1;
}
// verifica daca a fost citit un fisier
int file_in_storage(int *type) {
  if (*type == 0) {
    printf("No image loaded\r\n");
    return 0;
  }
  return 1;
}
// salveaza pozele care primesc parametrul ascii
void save(char *p, struct matrixEntry *entry) {
  FILE *file;
  file = fopen(p, "wt");
  switch (entry->type) {
    case 2:
      fprintf(file, "P2\n");
      fprintf(file, "%d %d\r\n", entry->width, entry->height);
      fprintf(file, "%d\n", 255);
      for (int i = 0; i < entry->height; i++) {
        for (int j = 0; j < entry->width; j++) {
          fprintf(file, "%d ", entry->matrix[i][j]);
        }
        fprintf(file, "\n");
      }
      break;
    case 3:
      fprintf(file, "P3\n");
      fprintf(file, "%d %d\r\n", entry->width, entry->height);
      fprintf(file, "%d\n", 255);
      for (int i = 0; i < entry->height; i++) {
        for (int j = 0; j < entry->width; j++) {
          fprintf(file, "%d ", entry->matrixc[i][j].red);
          fprintf(file, "%d ", entry->matrixc[i][j].blue);
          fprintf(file, "%d ", entry->matrixc[i][j].yellow);
        }
        fprintf(file, "\n");
      }
      break;
    case 5:
      fprintf(file, "P2\n");
      fprintf(file, "%d %d\r\n", entry->width, entry->height);
      fprintf(file, "%d\n", 255);
      for (int i = 0; i < entry->height; i++) {
        for (int j = 0; j < entry->width; j++) {
          fprintf(file, "%d ", entry->matrix[i][j]);
        }
        fprintf(file, "\n");
      }
      break;
    case 6:
      fprintf(file, "P3\n");
      fprintf(file, "%d %d\r\n", entry->width, entry->height);
      fprintf(file, "%d\n", 255);
      for (int i = 0; i < entry->height; i++) {
        for (int j = 0; j < entry->width; j++) {
          fprintf(file, "%d ", entry->matrixc[i][j].red);
          fprintf(file, "%d ", entry->matrixc[i][j].blue);
          fprintf(file, "%d ", entry->matrixc[i][j].yellow);
        }
        fprintf(file, "\n");
      }
      break;
  }
  fclose(file);
  printf("Saved %s\r\n", p);
}
// dealoca si iese din program
void free_exit(struct matrixEntry *entry, int *type) {
  int i;
  if (*type == 2 || *type == 5) {
    for (i = 0; i < entry->height; i++) {
      free(entry->matrix[i]);
    }
    free(entry->matrix);
  } else {
    for (i = 0; i < entry->height; i++) {
      free(entry->matrixc[i]);
    }
    free(entry->matrixc);
  }
  free(entry);
}
// salveaza pozele care nu primesc parametrul ascii
void savebin(char *p, struct matrixEntry *entry) {
  FILE *file;
  file = fopen(p, "wb");
  switch (entry->type) {
    case 2:
      fprintf(file, "P5\n");
      fprintf(file, "%d %d\r\n", entry->width, entry->height);
      fprintf(file, "%d\n", 255);
      for (int i = 0; i < entry->height; i++) {
        for (int j = 0; j < entry->width; j++) {
          fwrite(&entry->matrix[i][j], 1, 1, file);
        }
      }
      break;
    case 3:
      fprintf(file, "P6\n");
      fprintf(file, "%d %d\r\n", entry->width, entry->height);
      fprintf(file, "%d\n", 255);
      for (int i = 0; i < entry->height; i++) {
        for (int j = 0; j < entry->width; j++) {
          fwrite(&entry->matrixc[i][j].red, 1, 1, file);
          fwrite(&entry->matrixc[i][j].blue, 1, 1, file);
          fwrite(&entry->matrixc[i][j].yellow, 1, 1, file);
        }
      }
      break;
    case 5:
      fprintf(file, "P5\n");
      fprintf(file, "%d %d\r\n", entry->width, entry->height);
      fprintf(file, "%d\n", 255);
      for (int i = 0; i < entry->height; i++) {
        for (int j = 0; j < entry->width; j++) {
          fwrite(&entry->matrix[i][j], 1, 1, file);
        }
      }
      break;
    case 6:
      fprintf(file, "P6\n");
      fprintf(file, "%d %d\r\n", entry->width, entry->height);
      fprintf(file, "%d\n", 255);
      for (int i = 0; i < entry->height; i++) {
        for (int j = 0; j < entry->width; j++) {
          fwrite(&entry->matrixc[i][j].red, 1, 1, file);
          fwrite(&entry->matrixc[i][j].blue, 1, 1, file);
          fwrite(&entry->matrixc[i][j].yellow, 1, 1, file);
        }
      }
      break;
  }
  printf("Saved %s\r\n", p);
}
// verifica daca poza este alb negru
int check_greyscale(int *type) {
  if (*type == 2 || *type == 5) {
    return 1;
  }
  printf("Black and white image needed\r\n");
  return 0;
}
// se asira ca valoare pizelului este intre 0 255
int clamp(int x) {
  // printf("%d\r\n", x);
  if (x < 0) {
    return 0;
  } else if (x > 255) {
    return 255;
  }
  return x;
}

void equalize(struct matrixEntry *entry) {
  int i, j, k;
  long long H[256], Area, sum;
  for (i = 0; i < 256; i++) {
    H[i] = 0;
  }
  for (i = 0; i < entry->height; i++) {
    for (j = 0; j < entry->width; j++) {
      H[entry->matrix[i][j]]++;
    }
  }
  Area = entry->height * entry->width;
  for (i = 0; i < entry->height; i++) {
    for (j = 0; j < entry->width; j++) {
      sum = 0;
      for (k = 0; k < entry->matrix[i][j]; k++) {
        sum += H[k];
      }
      entry->matrix[i][j] = clamp(round((255.0 / Area) * sum));
    }
  }
  printf("Equalization done\r\n");
}

void crop(struct matrixEntry *entry) {
  struct matrixEntry *entry1;
  entry1 = malloc(sizeof(struct matrixEntry));
  entry1->type = entry->type;
  entry1->width = entry->x2 - entry->x1;
  entry1->height = entry->y2 - entry->y1;
  entry1->x1 = 0;
  entry1->x2 = entry1->width;
  entry1->y1 = 0;
  entry1->y2 = entry1->height;
  int i, j;
  if (entry->type == 2 || entry->type == 5) {
    entry1->matrix = malloc(sizeof(int *) * entry1->height);
    for (i = 0; i < entry1->height; i++) {
      entry1->matrix[i] = malloc(entry1->width * sizeof(int));
    }
    for (i = 0; i < entry1->height; i++) {
      for (j = 0; j < entry1->width; j++) {
        entry1->matrix[i][j] = entry->matrix[entry->x1 + i][entry->y1 + j];
      }
    }
    for (i = 0; i < entry->height; i++) {
      free(entry->matrix[i]);
    }
    free(entry->matrix);
  } else if (entry->type == 3 || entry->type == 6) {
    entry1->matrixc = calloc(entry1->height, sizeof(int *));
    for (i = 0; i < entry1->height; i++) {
      entry1->matrixc[i] =
          malloc(sizeof(struct colored_matrix) * entry1->width);
    }
    for (i = 0; i < entry1->height; i++) {
      for (j = 0; j < entry1->width; j++) {
        entry1->matrixc[i][j].blue =
            entry->matrixc[entry->x1 + i][entry->y1 + j].blue;
        entry1->matrixc[i][j].red =
            entry->matrixc[entry->x1 + i][entry->y1 + j].red;
        entry1->matrixc[i][j].yellow =
            entry->matrixc[entry->x1 + i][entry->y1 + j].yellow;
      }
    }
    for (i = 0; i < entry->height; i++) {
      free(entry->matrixc[i]);
    }
    free(entry->matrixc);
  }
  *entry = *entry1;
  free(entry1);
  printf("Image cropped\r\n");
}
// verifica daca matricea este colorata
int check_colored_matrix(int *type) {
  if (*type == 3 || *type == 6) {
    return 1;
  }
  printf("Easy, Charlie Chaplin");
  return 0;
}
// face kernelul in functie de paramentru
void kernel(double k[][3], char *p) {
  if (strncmp(p, "EDGE", 4) == 0) {
    k[0][0] = -1;
    k[0][1] = -1;
    k[0][2] = -1;
    k[1][0] = -1;
    k[1][1] = 8;
    k[1][2] = -1;
    k[2][0] = -1;
    k[2][1] = -1;
    k[2][2] = -1;
  } else if (strncmp(p, "SHARPEN", 7) == 0) {
    k[0][0] = 0;
    k[0][1] = -1;
    k[0][2] = 0;
    k[1][0] = -1;
    k[1][1] = 5;
    k[1][2] = -1;
    k[2][0] = 0;
    k[2][1] = -1;
    k[2][2] = 0;
  } else if (strncmp(p, "BLUR", 4) == 0) {
    k[0][0] = 1 / 9;
    k[0][1] = 1 / 9;
    k[0][2] = 1 / 9;
    k[1][0] = 1 / 9;
    k[1][1] = 1 / 9;
    k[1][2] = 1 / 9;
    k[2][0] = 1 / 9;
    k[2][1] = 1 / 9;
    k[2][2] = 1 / 9;
  } else if (strncmp(p, "GAUSSIAN_BLUR", 13) == 0) {
    k[0][0] = 1 / 16;
    k[0][1] = 2 / 16;
    k[0][2] = 1 / 16;
    k[1][0] = 2 / 16;
    k[1][1] = 4 / 16;
    k[1][2] = 2 / 16;
    k[2][0] = 1 / 16;
    k[2][1] = 2 / 16;
    k[2][2] = 1 / 16;
  }
}
// realizeza efectul cerut
void effect(struct matrixEntry *entry, double k[][3], int x1, int x2, int y1,
            int y2) {
  struct colored_matrix **matrixc;
  matrixc = malloc(sizeof(int *) * entry->height);
  int i, j;
  for (i = 0; i < entry->height; i++)
    matrixc[i] = malloc(sizeof(struct colored_matrix) * entry->width);
  for (i = 0; i < entry->height; i++) {
    for (j = 0; j < entry->width; j++) {
      matrixc[i][j].red = entry->matrixc[i][j].red;
      matrixc[i][j].blue = entry->matrixc[i][j].blue;
      matrixc[i][j].yellow = entry->matrixc[i][j].yellow;
    }
  }
  double sum;
  for (i = y1; i < y2; i++) {
    for (j = x1; j < x2; j++) {
      sum = 0;
      sum += k[0][0] * (double)matrixc[i - 1][j - 1].red;
      sum += k[0][1] * (double)matrixc[i - 1][j].red;
      sum += k[0][2] * (double)matrixc[i - 1][j + 1].red;
      sum += k[1][0] * (double)matrixc[i][j - 1].red;
      sum += k[1][1] * (double)matrixc[i][j].red;
      sum += k[1][2] * (double)matrixc[i][j + 1].red;
      sum += k[2][0] * (double)matrixc[i + 1][j - 1].red;
      sum += k[2][1] * (double)matrixc[i + 1][j].red;
      sum += k[2][2] * (double)matrixc[i + 1][j + 1].red;
      entry->matrixc[i][j].red = clamp(round(sum));
      sum = 0;
      sum += k[0][0] * (double)matrixc[i - 1][j - 1].blue;
      sum += k[0][1] * (double)matrixc[i - 1][j].blue;
      sum += k[0][2] * (double)matrixc[i - 1][j + 1].blue;
      sum += k[1][0] * (double)matrixc[i][j - 1].blue;
      sum += k[1][1] * (double)matrixc[i][j].blue;
      sum += k[1][2] * (double)matrixc[i][j + 1].blue;
      sum += k[2][0] * (double)matrixc[i + 1][j - 1].blue;
      sum += k[2][1] * (double)matrixc[i + 1][j].blue;
      sum += k[2][2] * (double)matrixc[i + 1][j + 1].blue;
      entry->matrixc[i][j].blue = clamp(round(sum));
      sum = 0;
      sum += k[0][0] * (double)matrixc[i - 1][j - 1].yellow;
      sum += k[0][1] * (double)matrixc[i - 1][j].yellow;
      sum += k[0][2] * (double)matrixc[i - 1][j + 1].yellow;
      sum += k[1][0] * (double)matrixc[i][j - 1].yellow;
      sum += k[1][1] * (double)matrixc[i][j].yellow;
      sum += k[1][2] * (double)matrixc[i][j + 1].yellow;
      sum += k[2][0] * (double)matrixc[i + 1][j - 1].yellow;
      sum += k[2][1] * (double)matrixc[i + 1][j].yellow;
      sum += k[2][2] * (double)matrixc[i + 1][j + 1].yellow;
      entry->matrixc[i][j].yellow = clamp(round(sum));
    }
  }
  for (i = 0; i < entry->height; i++) {
    free(matrixc[i]);
  }
  free(matrixc);
}
// se asigura ca efectul pt apply nu se aplica si pe margini
void selection(struct matrixEntry *entry, int *x1, int *x2, int *y1, int *y2) {
  if (entry->x1 == 0) {
    *x1 = 1;
  } else {
    *x1 = entry->x1;
  }
  if (entry->y1 == 0) {
    *y1 = 1;
  } else {
    *y1 = entry->y1;
  }
  if (entry->x2 == entry->width) {
    *x2 = entry->width - 1;
  } else {
    *x2 = entry->x2;
  }
  if (entry->y2 == entry->height) {
    *y2 = entry->height - 1;
  } else {
    *y2 = entry->y2;
  }
}

void apply(struct matrixEntry *entry, char *p) {
  p = strtok(NULL, " ");
  if (strncmp(p, "EDGE", 4) != 0 && strncmp(p, "SHARPEN", 7) != 0 &&
      strncmp(p, "BLUR", 4) != 0 && strncmp(p, "GAUSSIAN_BLUR", 13)) {
    printf("APPLY parameter invalid");
    return;
  }
  int x1, x2, y1, y2;
  selection(entry, &x1, &x2, &y1, &y2);
  double k[3][3];
  kernel(k, p);
  effect(entry, k, x1, x2, y1, y2);
  printf("APPLY %s done\r\n", p);
}

int main() {
  char command[100], *p, *file_name, *p1;
  int *type;
  struct matrixEntry *entry;
  type = malloc(sizeof(int));
  *type = 0;
  p = malloc(100 * sizeof(char));
  p1 = p;
  while (1) {
    fgets(command, sizeof(char) * 100, stdin);
    if (command[strlen(command) - 1] == '\n') {
      command[strlen(command) - 1] = '\0';
    }
    p = strtok(command, " ");
    if (strcmp(command, "LOAD") == 0) {
      if (*type != 0) {
        free_exit(entry, type);
      }
      p = strtok(NULL, " ");
      if (checkfile(p)) {
        entry = read_file(p, type);
        *type = entry->type;
      }
    } else if (strncmp(command, "SELECT", 6) == 0) {
      p = strtok(NULL, " ");
      if (strncmp(p, "ALL", 3) == 0) {
        if (file_in_storage(type)) {
          selectall(entry);
        }
      } else {
        if (file_in_storage(type)) {
          selectxy(p, entry);
        }
      }
    } else if (strncmp(command, "HISTOGRAM", 9) == 0) {
      if (file_in_storage(type) && check_greyscale(type)) {
        histogram(p, entry);
      }
    } else if (strncmp(command, "EXIT", 4) == 0) {
      if (type != 0) {
        free_exit(entry, type);
        free(type);
      }
      break;
    } else if (strncmp(command, "SAVE", 4) == 0) {
      if (file_in_storage(type)) {
        p = strtok(NULL, " ");
        file_name = p;
        p = strtok(NULL, " ");
        if (p != NULL) {
          if (strncmp(p, "ascii", 5) == 0) {
            save(file_name, entry);
          }
        } else if (p == NULL) {
          savebin(file_name, entry);
        }
      }
    } else if (strncmp(command, "EQUALIZE", 8) == 0) {
      if (file_in_storage(type) && check_greyscale(type)) {
        equalize(entry);
      }
    } else if (strncmp(command, "CROP", 4) == 0) {
      if (file_in_storage(type)) {
        crop(entry);
      }
    } else if (strncmp(command, "APPLY", 5) == 0) {
      if (file_in_storage(type) && check_colored_matrix(type)) {
        apply(entry, p);
      }
    }
  }
  free(p1);
  return 0;
}
