FROM debian

RUN apt-get update && apt-get install -y build-essential valgrind

