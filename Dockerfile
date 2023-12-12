FROM ubuntu:latest AS base
ARG CPU_ARCHITECTURE="native"
ENV LANG=C.UTF-8
EXPOSE 12345/tcp 22/tcp
VOLUME /data
WORKDIR /app
RUN rm -f /etc/apt/apt.conf.d/docker-clean
RUN \
    --mount=type=cache,sharing=locked,target=/var/cache/apt \
    --mount=type=cache,sharing=locked,target=/var/lib/apt \
    DEBIAN_FRONTEND=noninteractive apt-get update && apt-get install -y -o APT::Keep-Downloaded-Packages=true --no-install-recommends \
    libcurl4-openssl-dev ca-certificates libuv1-dev zlib1g-dev libjpeg-turbo8-dev

FROM base AS build
RUN \
    --mount=type=cache,sharing=locked,target=/var/cache/apt \
    --mount=type=cache,sharing=locked,target=/var/lib/apt \
    DEBIAN_FRONTEND=noninteractive apt-get update && apt-get install -y -o APT::Keep-Downloaded-Packages=true --no-install-recommends \
    git cmake make g++
COPY . .
RUN \
    --mount=type=cache,sharing=locked,target=build \
    CXX=g++ cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=Off -DCPU_ARCHITECTURE=${CPU_ARCHITECTURE}
RUN \
    --mount=type=cache,sharing=locked,target=build \
    cmake --build build --config Release --parallel $(nproc)

FROM base AS runtime
RUN useradd -m cluster
RUN mkdir -p /home/cluster/.ssh
RUN touch /home/cluster/.ssh/authorized_keys
RUN chmod 600 /home/cluster/.ssh/authorized_keys
RUN chown -R cluster:cluster /home/cluster
RUN \
    --mount=type=cache,sharing=locked,target=/var/cache/apt \
    --mount=type=cache,sharing=locked,target=/var/lib/apt \
    DEBIAN_FRONTEND=noninteractive apt-get update && apt-get install -y -o APT::Keep-Downloaded-Packages=true --no-install-recommends \
    openssh-server
COPY --from=build /app/bin/main ./bin/main
ENTRYPOINT echo "$0" > /home/cluster/.ssh/authorized_keys && service ssh start && /bin/bash
