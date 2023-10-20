To get started, you can download a pre-build docker image from the GitHub Registry. You can also tag it, to give it a shorter name.

```shell
docker pull ghcr.io/tasemo/dipp:latest
docker tag ghcr.io/tasemo/dipp:latest dipp:latest
```

If you want, you can also build the docker image yourself with the following command. It will be built in Release mode with all optimizations enabled.

```shell
docker build -t dipp:latest .
```

You can then deploy the image to an environment of your choice. You can also run the containers locally if you want to test the cluster setup. To be able to run this program on a cluster, each container needs to be accessable by SSH. To allow for that, you have to specify a ssh public key when starting the container. If needed, you can create one with ssh-keygen.

```shell
ssh-keygen -t rsa -b 4096
```

To start a container, you have to mount a persistent volume to the internal /data directory. This example starts a container named "node", uses a local /data/docker directory for persistance and reads the ssh public key from the default location.

```shell
docker run -ditP --rm -v /data/docker:/data --name node1 dipp "$(cat ~/.ssh/id_rsa.pub)"
```

You can start as many containers as you like. You also need the IP address of each container, which you can get with the following command. They start at 172.17.0.2 by default.

```shell
docker inspect -f '{{.NetworkSettings.IPAddress}}' node1
```

To control the behaviour of the application (e.g. the amount of workers per host), you have to use environment variables. A default configuration with an explanation for all values is [provided](configure.default.sh). Copy the file, make your changes and remember to source the file every time you make a new change. Note, that all environment variables are also passed to each node. Make sure to set the "THRILL_NET" setting to the correct value (most likely tcp).

```shell
cp configure.default.sh configure.sh
source configure.sh
```

Once the (local) cluster is set up, you can run the program on it by calling the ssh deploy script provided by thrill. You have to specify a list of hosts (-h), the directory where the program is found (-C), a SSH username (-u) and finally the program name and all necessary arguments.

```shell
./bin/deploy/ssh/invoke.sh -h "172.17.0.2:12345 172.17.0.3:12345" -C "/app/bin" -u "cluster" main
```
