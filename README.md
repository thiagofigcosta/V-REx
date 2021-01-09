# V-REx
Vulnerabilities' Risk of Exploitation

Neural network with parts of the code from: 
>- https://github.com/keroro824/HashingDeepLearning 
>- https://github.com/kapilt/mongoqueue
>- https://github.com/sarthakpati/cnpy 
>- https://github.com/madler/zlib
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
docker build -t core:v1.0.0 core/  # build image
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

## Cleanup docker compose volumes
```
sudo rm -rf docker_volumes
```

## Web Interfaces:
>- Mongo-Express: http://localhost:8081/
>- Portainer: http://localhost:9000/