FROM ubuntu:16.04
MAINTAINER Benedikt Wolters <benedikt.wolters@rwth-aachen.de>


RUN apt-get update -y && apt-get install -y \
  git \
  build-essential \
  libboost-all-dev \ 
  ca-certificates \ 
  doxygen \
  cmake \
  --no-install-recommends --force-yes && rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*  && \
  apt-get clean autoclean && \
  rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*
  

RUN git clone https://github.com/geneial/geneial.git geneial
RUN cd geneial && mkdir build && cd build && cmake -D CMAKE_BUILD_TYPE=Release ../src && make && make install
