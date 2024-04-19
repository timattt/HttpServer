FROM gcc:latest

RUN apt-get update
RUN apt-get -y install cmake
RUN apt-get -y install gdb
RUN apt-get -y install tree

WORKDIR /app

COPY ./src/     /app/src/
COPY ./webroot/ /app/webroot/

RUN mkdir build   && \
    cd build      && \
    cmake ../src  && \
    make          && \
    chmod +x /app/build/WebServer

ENTRYPOINT ["/bin/sh", "-c" ,"tree && mkdir -p /app/artifacts && cp /app/build/WebServer /app/artifacts/WebServer && cd artifacts && ./WebServer"]