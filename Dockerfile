FROM gcc:latest
WORKDIR /app
COPY twc.c /app/
ENV CFLAGS=-Oz -std=c17
RUN gcc -o twc twc.c
CMD ["./twc"]
