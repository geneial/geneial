FROM ubuntu:24.04 AS build
RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential cmake ninja-build libboost-test-dev ca-certificates \
    && rm -rf /var/lib/apt/lists/*
WORKDIR /geneial
COPY . .
RUN cmake --preset release && cmake --build --preset release --parallel 2 \
    && ctest --preset release --output-on-failure

FROM ubuntu:24.04
RUN apt-get update && apt-get install -y --no-install-recommends libstdc++6 \
    && rm -rf /var/lib/apt/lists/*
COPY --from=build /geneial/output/release/src/demo/triangle_art /usr/local/bin/triangle_art
COPY --from=build /geneial/src/demo/showcase/assets/mona_lisa.ppm /geneial/src/demo/showcase/assets/mona_lisa.ppm
WORKDIR /work
ENTRYPOINT ["triangle_art"]
