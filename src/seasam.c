/*
 *  SEAsam -- Shape-based sampling for large time series collections.
 *
 *  This code is open-sourced at https://github.com/qtwang/SEAsam.
 *  Supplemental materials can be found at
 *  https://qtwang.github.io/kdd21-seanet.
 *
 *  If you find this code useful in your work, please consider citing:
 *  Qitong Wang, Themis Palpanas: Deep Learning Embeddings for Data Series
 *  Similarity Search. KDD 2021: 1708-1716.
 *  BibTeX record can be found at
 *  https://dblp.org/rec/conf/kdd/WangP21.html?view=bibtex.
 *
 *
 *  Copyright (C) 2021 - Qitong Wang <qitong.wang@etu.u-paris.fr> and Themis
 *  Palpanas <themis@mi.parisdescartes.fr>, Université Paris Cité, France.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the Mozilla Public License Version 2.0 as published by
 *  the Mozilla Foundation. If a copy of the MPL was not distributed with this
 *  file, you can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "seasam.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "breakpoints.h"

int compare_breakpoints(const void *a, const void *b) {
  float *c = (float *)b - 1;
  if (*(float *)a > *(float *)c && *(float *)a <= *(float *)b) {
    return 0;
  } else if (*(float *)a <= *(float *)c) {
    return -1;
  } else {
    return 1;
  }
}

int sequence2sax(float *sequence, unsigned char *sax, float *paa,
                 int len_segment, int size_segments, int size_alphabeta,
                 int cardinality) {
  for (int i = 0; i < size_segments; ++i) {
    paa[i] = 0;

    for (int j = 0; j < len_segment; ++j) {
      paa[i] += sequence[(i * len_segment) + j];
    }
    paa[i] /= len_segment;
  }

  int offset = (size_alphabeta - 1) * (size_alphabeta - 2) / 2;
  for (int i = 0; i < size_segments; ++i) {
    sax[i] = 0;
    float *breakpoint =
        (float *)bsearch(&paa[i], &sax_breakpoints[offset], size_alphabeta - 1,
                         sizeof(float), compare_breakpoints);
    if (breakpoint != NULL) {
      sax[i] = (int)(breakpoint - &sax_breakpoints[offset]);
    } else if (paa[i] > 0) {
      sax[i] = size_alphabeta - 1;
    }
  }

  return 1;
}

void invSax(unsigned char *sax, unsigned char *invsax, int segments,
            int cardinality) {
  int i, j;
  unsigned long long sax_tmp;

  for (i = 0; i < segments; ++i) {
    invsax[i] = 0;
  }

  int segi = 0, invj = cardinality - 1;

  for (i = cardinality - 1; i >= 0; --i) {
    for (j = 0; j < segments; ++j) {
      sax_tmp = sax[j];
      sax_tmp = sax_tmp >> i;

      invsax[segi] |= (sax_tmp % 2) << invj;

      invj--;
      if (invj == -1) {
        ++segi;
        invj = cardinality - 1;
      }
    }
  }

  for (i = 0; i < segments; ++i) {
    sax[i] = invsax[i];
  }
}

static int GLOBAL_PAA_SEGMENTS;
int compare_invsax(const void *a1, const void *b1) {
  SAX *a = (SAX *)a1;
  SAX *b = (SAX *)b1;

  for (int i = 0; i < GLOBAL_PAA_SEGMENTS; i++) {
    if (a->values[i] > b->values[i]) {
      return 1;
    } else if (a->values[i] < b->values[i]) {
      return -1;
    }
  }
  return 0;
}

int seasam_sample(char *path_database, long size_database,
                  char *path_sorted_indices, char *path_train_indices,
                  int size_train, char *path_val_indices, int size_val,
                  int len_sequence, int sax_cardinality, int paa_segments) {
  GLOBAL_PAA_SEGMENTS = paa_segments;
  int len_segment = len_sequence / paa_segments;
  int size_alphabeta = (int)pow(2, sax_cardinality);

  FILE *fp_database;
  if ((fp_database = fopen(path_database, "r")) == NULL) {
    // FILE *f = fopen("../log/sampling.log", "w");
    // fprintf(f, "database path = %s", path_database);
    // fclose(f);
    return -2;
  }

  FILE *fp_sorted_indices = fopen(path_sorted_indices, "wb");
  FILE *fp_train_indices = fopen(path_train_indices, "wb");
  FILE *fp_val_indices = fopen(path_val_indices, "wb");

  fseek(fp_database, 0L, SEEK_END);
  int size_database_available =
      (unsigned long long)ftell(fp_database) / (sizeof(float) * len_sequence);
  fseek(fp_database, 0L, SEEK_SET);
  if (size_database_available < size_database) {
    return -6;
  }

  float *sequence_cache = (float *)malloc(sizeof(float) * len_sequence);
  if (sequence_cache == NULL) {
    return -7;
  }

  float *paa_cache = (float *)malloc(sizeof(float) * paa_segments);
  if (paa_cache == NULL) {
    free(sequence_cache);
    return -8;
  }

  unsigned char *invsax_cache =
      (unsigned char *)malloc(sizeof(unsigned char) * paa_segments);
  if (invsax_cache == NULL) {
    free(paa_cache);
    free(sequence_cache);
    return -9;
  }

  SAX *saxs = (SAX *)malloc(sizeof(SAX) * size_database);
  if (saxs == NULL) {
    free(invsax_cache);
    free(paa_cache);
    free(sequence_cache);
    return -10;
  }

  for (int i = 0; i < size_database; ++i) {
    saxs[i].values =
        (unsigned char *)malloc(sizeof(unsigned char) * paa_segments);

    if (saxs[i].values == NULL) {
      for (int j = 0; j < i; ++i) {
        free(saxs[j].values);
      }

      free(saxs);
      free(invsax_cache);
      free(paa_cache);
      free(sequence_cache);
      return -11;
    }
  }

  for (long i = 0; i < size_database; ++i) {
    fread(sequence_cache, sizeof(float), len_sequence, fp_database);
    saxs[i].index = i;

    if (sequence2sax(sequence_cache, saxs[i].values, paa_cache, len_segment,
                     paa_segments, size_alphabeta, sax_cardinality) == 1) {
      invSax(saxs[i].values, invsax_cache, paa_segments, sax_cardinality);
    } else {
      for (int j = 0; j < size_database; ++j) {
        free(saxs[j].values);
      }

      free(saxs);
      free(invsax_cache);
      free(paa_cache);
      free(sequence_cache);
      return -12;
    }
  }

  qsort((void *)saxs, (size_t)size_database, sizeof(SAX), compare_invsax);

  if (fp_sorted_indices != NULL) {
    for (int i = 0; i < size_database; ++i) {
      fwrite(&(saxs[i].index), sizeof(long), 1, fp_sorted_indices);
    }
  }

  if (fp_train_indices != NULL) {
    int step = size_database / size_train;
    for (int i = 0; i < size_train; ++i) {
      fwrite(&(saxs[i * step].index), sizeof(long), 1, fp_train_indices);
    }
  }

  if (fp_val_indices != NULL) {
    int step = size_database / size_train;
    int offset = step / 3;
    step = size_database / size_val;

    for (int i = 0; i < size_val; ++i) {
      fwrite(&(saxs[offset + i * step].index), sizeof(long), 1, fp_val_indices);
    }
  }

  for (int j = 0; j < size_database; ++j) {
    free(saxs[j].values);
  }

  free(saxs);
  free(invsax_cache);
  free(paa_cache);
  free(sequence_cache);

  if (fp_sorted_indices != NULL) {
    fclose(fp_sorted_indices);
    fp_sorted_indices = NULL;
  }
  fclose(fp_database);

  if (fp_train_indices != NULL) {
    fclose(fp_train_indices);
  }

  if (fp_val_indices != NULL) {
    fclose(fp_val_indices);
  }

  return 0;
}