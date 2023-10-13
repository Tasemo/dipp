FROM ubuntu:latest
ENV LANG=C.UTF-8
ARG DEBIAN_FRONTEND=noninteractive
EXPOSE 12345/tcp 22/tcp
VOLUME /data
RUN rm -f /etc/apt/apt.conf.d/docker-clean; echo 'Binary::apt::APT::Keep-Downloaded-Packages "true";' > /etc/apt/apt.conf.d/keep-cache
RUN \
    --mount=type=cache,sharing=locked,target=/var/cache/apt \
    --mount=type=cache,sharing=locked,target=/var/lib/apt \
    apt-get update && apt-get install -y --no-install-recommends \
    git cmake make g++ libcurl4-openssl-dev libuv1-dev zlib1g-dev \
    libjpeg-turbo8-dev openssh-server ca-certificates
RUN useradd -m cluster
RUN mkdir -p /home/cluster/.ssh
RUN touch /home/cluster/.ssh/authorized_keys
RUN chmod 600 /home/cluster/.ssh/authorized_keys
RUN chown -R cluster:cluster /home/cluster
WORKDIR /app
COPY . .
RUN \
    --mount=type=cache,sharing=locked,target=build \
    CXX=g++ cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=Off
RUN \
    --mount=type=cache,sharing=locked,target=build \
    cmake --build build --config Release --parallel $(nproc)
ENTRYPOINT echo "$0" > /home/cluster/.ssh/authorized_keys && service ssh start && /bin/bash
