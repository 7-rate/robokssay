FROM ubuntu:24.04

RUN apt -y update && apt -y upgrade
RUN apt install -y build-essential

RUN mkdir /src
COPY main.c /src

WORKDIR /src
RUN gcc main.c -o robokssay
RUN cp robokssay /usr/bin

CMD robokssay
