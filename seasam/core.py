'''
SEAsam -- Shape-based sampling for large time series collections.

This code is open-sourced at https://github.com/qtwang/SEAsam.
Supplemental materials can be found at https://qtwang.github.io/kdd21-seanet.

If you find this code useful in your work, please consider citing:
Qitong Wang, Themis Palpanas: Deep Learning Embeddings for Data Series
Similarity Search. KDD 2021: 1708-1716.
BibTeX record can be found at
https://dblp.org/rec/conf/kdd/WangP21.html?view=bibtex.


Copyright (C) 2021 - Qitong Wang <qitong.wang@etu.u-paris.fr> and Themis
Palpanas <themis@mi.parisdescartes.fr>, Université Paris Cité, France.

This program is free software; you can redistribute it and/or modify
it under the terms of the Mozilla Public License Version 2.0 as published by
the Mozilla Foundation. If a copy of the MPL was not distributed with this
file, you can obtain one at https://mozilla.org/MPL/2.0/.
'''

from seasam_c_ext import sample as c_sample

import numpy as np


# int seasam(char *path_database, long size_database, char *path_sorted_indices,
#            char *path_train_indices, int size_train, char *path_val_indices,
#            int size_val, int len_sequence, int sax_cardinality,
#            int paa_segments) {

def seasam_sample(path_database: str,
                  size_database: np.int64,
                  len_sequence: np.int32,
                  path_train_indices: str,
                  size_train: np.int64,
                  path_val_indices: str = None,
                  size_val: np.int32 = -1,
                  path_sorted_indices: str = None,
                  sax_cardinality: np.int32 = 8,
                  paa_segments: np.int32 = 16):
    assert (path_val_indices is None) == (size_val <= 0)

    sample_output = c_sample(path_database, size_database, path_sorted_indices,
                             path_train_indices, size_train, path_val_indices,
                             size_val, len_sequence, sax_cardinality, paa_segments)

    return sample_output
