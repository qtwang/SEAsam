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

#include "seasam.h"

#ifdef PYTHON_MODULE

#include <Python.h>

static PyObject *seasam_sample_wrapper(PyObject *self, PyObject *args) {
  char *path_database, *path_sorted_indices, *path_train_indices,
      *path_val_indices;
  long size_database;
  int size_train, size_val, len_sequence, sax_cardinality, paa_segments;

  if (!PyArg_ParseTuple(args, "slzsiziiii", &path_database, &size_database,
                        &path_sorted_indices, &path_train_indices, &size_train,
                        &path_val_indices, &size_val, &len_sequence,
                        &sax_cardinality, &paa_segments)) {
    return NULL;
  }

  int return_code =
      seasam_sample(path_database, size_database, path_sorted_indices,
                    path_train_indices, size_train, path_val_indices, size_val,
                    len_sequence, sax_cardinality, paa_segments);

  return Py_BuildValue("i", return_code);
}

static PyMethodDef seasam_methods[] = {
    {"sample", seasam_sample_wrapper, METH_VARARGS,
     "Shape-based sampling for large time series collections"},
    {NULL, NULL, 0, NULL} /* Sentinel */
};

static struct PyModuleDef seasam_module = {
    PyModuleDef_HEAD_INIT, "seasam_c_ext", /* name of module */
    NULL, /* module documentation, may be NULL */
    -1,   /* size of per-interpreter state of the module,
             or -1 if the module keeps state in global variables. */
    seasam_methods};

PyMODINIT_FUNC PyInit_seasam_c_ext(void) {
  return PyModule_Create(&seasam_module);
}

#endif  // PYTHON_MODULE
