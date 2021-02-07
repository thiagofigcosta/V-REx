# V-REx
Vulnerabilities' Risk of Exploitation

Neural network with parts of the code from: 
>- https://github.com/keroro824/HashingDeepLearning 
>- https://github.com/jl2922/hps
>- https://github.com/kapilt/mongoqueue
>- https://github.com/mongodb/mongo-cxx-driver
>- https://github.com/mongodb/mongo-c-driver
>- https://github.com/mnmlstc/core

## To run commands

Access the front-end container and use one of the following commands:

>- bash
>- front
>- frontend
>- front-end

Type `h` + enter for help

### Example:
```
docker exec -it $(docker container ls | grep front-end | cut -f 1 -d' ') front
```

## To follow logs

To follow logs type one of the commands below
### Data Crawler
```
docker logs --follow $(docker container ls | grep data-crawler | cut -f 1 -d' ')
```
### Data Processor 
```
docker logs --follow $(docker container ls | grep data-processor | cut -f 1 -d' ')
```

## Passwords location
Inside .env file

## Requirements
>- docker
>- docker-compose

## Images required
>- mongo-express:0.54.0
>- mongo:4.0.20
>- python:3.8
>- python:3.8-slim


## Running with Docker compose (recommended)
```
docker-compose build
docker-compose up
```
or in background
```
docker-compose build
docker-compose up -d
```
or in background with replicas
```
docker-compose up -d --scale data-crawler=2
```
or in background without some modules
```
docker-compose up -d --scale data-crawler=0 --scale data-processor=0
```

To Stop run:
```
docker-compose stop
```

## Running without Docker compose

### Build docker images

#### Data Crawler
```
./update_Pytho\{N\}.sh # update Pytho{\}
docker build -t data-crawler:v1.0.0 data-crawler/  # build image
docker rmi -f $(docker images -f "dangling=true" -q) # cleanup <none> images
```

#### Data Processor
```
./update_Pytho\{N\}.sh # update Pytho{\}
docker build -t data-processor:v1.0.0 data-processor/  # build image
docker rmi -f $(docker images -f "dangling=true" -q) # cleanup <none> images
```

#### Front end
```
./update_Pytho\{N\}.sh # update Pytho{\}
docker build -t front-end:v1.0.0 front-end/  # build image
docker rmi -f $(docker images -f "dangling=true" -q) # cleanup <none> images
```

#### Core
```
docker build -f core/BaseImage.dockerfile -t ubuntu_2004_vrex_core_build core/ # build base image
docker build -f core/Dockerfile -t core:v1.0.0 core/ # build image
docker rmi -f $(docker images -f "dangling=true" -q) # cleanup <none> images
```

### Running docker images

#### Data crawler
```
docker run data-crawler:v1.0.0
```

#### Front end
```
docker run front-end:v1.0.0
```

#### Core
```
docker run core:v1.0.0
```

To debug with gdb use:
```
docker run --cap-add=SYS_PTRACE --security-opt seccomp=unconfined -it core:v1.0.0 args --debug
```

To profile with valgrind use:
```
docker run --cap-add=SYS_PTRACE --security-opt seccomp=unconfined core:v1.0.0 args --profiling
```

To limit memoru usage use:
```
docker run --memory="25G" core:v1.0.0
```

## Cleanup docker compose volumes
```
sudo rm -rf docker_volumes
```

## Cleanup RAM memory cache if needed
```
sudo su
free -mh && sync && echo 3 > /proc/sys/vm/drop_caches && free -mh
```

## Web Interfaces:
>- Mongo-Express: http://localhost:8081/
>- Portainer: http://localhost:9000/