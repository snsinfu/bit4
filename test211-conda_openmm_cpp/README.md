Copy etc directory into the conda environment prefix. For miniconda3
installation with Homebrew on macOS:

    conda env create -f environment.yml
    cp -R etc /usr/local/miniconda3/envs/bit4-openmm-experiments
    source activate bit4-openmm-experiments
