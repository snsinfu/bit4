#!/bin/bash
set -eux -o pipefail

curl -fsSL \
    -o /root/install-miniconda.sh \
    https://repo.anaconda.com/miniconda/Miniconda3-latest-Linux-x86_64.sh

bash /root/install-miniconda.sh \
    -b \
    -p /opt/miniconda3

cat > /root/jupyter.yml << EOF
name: jupyter

channels:
  - conda-forge

dependencies:
  - python=3.6
  - node
  - biopython
  - gensim
  - jupyter_nbextensions_configurator
  - jupyterlab
  - keras
  - matplotlib
  - numpy
  - pandas
  - scikit-image
  - scikit-learn
  - scipy
  - statsmodels
  - tensorflow
EOF

/opt/miniconda3/bin/conda env create -q --force -f /root/jupyter.yml
