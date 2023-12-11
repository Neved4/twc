FROM gcc:latest
WORKDIR /app
COPY twc.c /app/
ENV CFLAGS=-O2
RUN gcc -o twc twc.c
CMD ["./twc"]
