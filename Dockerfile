FROM debian:9
WORKDIR /code

RUN apt-get update && apt-get upgrade -y
RUN apt-get install -y gcc g++ make valgrind tcpdump wget git curl man vim cmake
