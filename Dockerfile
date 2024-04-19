FROM gcc:latest

RUN apt-get update
RUN apt-get -y install cmake
RUN apt-get -y install gdb
RUN apt-get -y install tree

WORKDIR /app

COPY ./src/     /app/src/
COPY ./webroot/ /app/webroot/

RUN mkdir build                     && \
    cmake -B/app/build -S/app/src   && \
    make -C /app/build              && \
    chmod +x /app/build/WebServer

WORKDIR /app/build
ENTRYPOINT ./WebServer

# if we need to save something to volume
#ENTRYPOINT ["/bin/sh", "-c" ,"tree && mkdir -p /app/artifacts && cp /app/build/WebServer /app/artifacts/WebServer && cd artifacts && ./WebServer"]