/*
 *  SEAsam -- Shape-based sampling for large time series collections.
 *
 *  This code is open-sourced at https://github.com/qtwang/SEAsam.
 *  Supplemental materials can be found at
 * https://qtwang.github.io/kdd21-seanet.
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

typedef struct {
  unsigned char *values;
  long index;
} SAX;

int seasam_sample(char *path_database, long size_database,
                  char *path_sorted_indices, char *path_train_indices,
                  int size_train, char *path_val_indices, int size_val,
                  int len_sequence, int sax_cardinality, int paa_segments);
