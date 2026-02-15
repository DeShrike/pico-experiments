# Pico SDK with Docker

## Install docker

On Debian based systems, see www.docker.com for others

```console
sudo apt-get install docker-ce
or
sudo apt install docker-ce docker-ce-cli containerd.io docker-buildx-plugin docker-compose-plugin
```

See https://docs.docker.com/engine/install/debian/ for more instructions.

## First setup

```console
sudo groupadd docker
sudo usermod -aG docker $USER

cd $HOME
mkdir Pico
cd Pico

docker run -d --rm -it --name picosdk --mount type=bind,source=${PWD},target=/home/dev lukstep/raspberry-pi-pico-sdk:latest
```

On the host, place all pico projects in a subfolder of the Pico folder you just created.

## Next time, after a reboot

```console
docker container ls -a
docker start picosdk
```

## Connect to the running container

```console
docker exec -it -w /home/dev picosdk /bin/bash
. init.sh
```

## Compile a program from inside the running container

```console
cd Blink
mkdir build
cd build
cmake ..
make
```

the folder Blink/build now contains a .uf2 file.
Copy that file to the device.
