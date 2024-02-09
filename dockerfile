FROM ubuntu:22.04
#FROM myoung34/github-runner:2.311.0-ubuntu-jammy

WORKDIR /root


RUN apt-get update && \
      apt-get -y install sudo

RUN apt-get update && apt-get install -y \
  software-properties-common \
  build-essential \
  cmake \
  git \
  libssl-dev \
  wget \
  python3 \
  vim \
  libgmp-dev


#RUN useradd -m docker && echo "docker:docker" | chpasswd && adduser docker sudo

RUN mkdir /home/mnt

#CMD ["/bin/bash"]

#USER docker
ENTRYPOINT ["tail"]
CMD ["-f", "/dev/null"]




