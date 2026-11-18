FROM ubuntu:24.04 AS builder
RUN apt-get update && apt-get install -y --no-install-recommends cmake g++ make \
    && rm -rf /var/lib/apt/lists/*
WORKDIR /src
COPY CMakeLists.txt .
COPY include/ include/
COPY src/ src/
RUN cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=OFF \
    && cmake --build build --parallel

FROM ubuntu:24.04
WORKDIR /app
COPY --from=builder /src/build/matrix_calculator .
CMD ["./matrix_calculator"]
