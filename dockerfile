FROM ubuntu:22.04

RUN apt-get update && \
    apt-get install -y clang make && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY . .

RUN make all

EXPOSE 8080

CMD ["bin/proda"]