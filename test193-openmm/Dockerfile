FROM alpine:3.5

ENV OPENMM_VERSION 7.1.1

RUN apk add --no-cache libstdc++ fftw python3                                       \
 && apk add --no-cache --virtual .build-deps curl build-base cmake doxygen graphviz \
                                 swig fftw-dev python3-dev                          \
 && curl -LO https://github.com/pandegroup/openmm/archive/${OPENMM_VERSION}.tar.gz  \
 && tar xzf ${OPENMM_VERSION}.tar.gz                                                \
 && cd openmm-${OPENMM_VERSION}                                                     \
 && cmake . -DOPENMM_INSTALL_PREFIX=/usr/local                                      \
 && make install PythonInstall                                                      \
 && cd ..                                                                           \
 && rm -rf openmm-${OPENMM_VERSION}                                                 \
 && rm ${OPENMM_VERSION}.tar.gz                                                     \
 && apk del --purge .build-deps

CMD sh
