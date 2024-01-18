FROM gcc:latest
WORKDIR /app
COPY twc.c /app/
ENV CFLAGS=-O3 -std=c99
RUN gcc -o twc twc.c
CMD ["./twc"]
