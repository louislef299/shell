# syntax=docker/dockerfile:1.7
FROM alpine:latest

RUN apk add --no-cache build-base

WORKDIR /app

COPY . .

ENV GCC=gcc
RUN make

CMD ["/app/lsh", "store", "2000"]