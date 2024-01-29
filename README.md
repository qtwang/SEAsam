# SEAsam

This repository contains code to sample time series efficiently from a large-scale collection, such that the sampled series can cover the series shapes in the collection.

## usage

Install as a Python module:

``` bash
python setup.py build_py_mod 
pip install .
```

If you need a shared library, run:

``` bash
python setup.py build_lib 
```

An example usage Jupyter notebook is provided at [example/seasam_example.ipynb](https://github.com/qtwang/SEAsam/blob/main/example/seasam_example.ipynb)

## Citation

``` bibtex
@inproceedings{conf/kdd/WangP21,
  author       = {Qitong Wang and
                  Themis Palpanas},
  title        = {Deep Learning Embeddings for Data Series Similarity Search},
  booktitle    = {{KDD}},
  pages        = {1708--1716},
  year         = {2021},
}
```

If you find SEAsam sampling useful, please make sure to cite also the original SEAnet paper (where SEAsam is proposed): [[bibtex]](https://dblp2.uni-trier.de/rec/conf/kdd/WangP21.bib?param=1)
