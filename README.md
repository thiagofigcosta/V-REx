# V-REx
Vulnerabilities' Risk of Exploitation

Neural network with parts of the code from: 
>- https://github.com/keroro824/HashingDeepLearning 
>- https://github.com/kapilt/mongoqueue
>- https://github.com/sarthakpati/cnpy 
>- https://github.com/madler/zlib

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


## Running with Docker compose
```
docker-compose up
```
or in background
```
docker-compose up -d
```

### Cleanup docker compose volumes
```
sudo rm -rf docker_volumes
```

## Build docker images

### Data crawler
```
./update_Pytho\{N\}.sh # update Pytho{\}
docker build -t data-crawler:v1.0.0 data-crawler/  # build image
docker rmi -f $(docker images -f "dangling=true" -q) # cleanup <none> images
```

### Front end
```
./update_Pytho\{N\}.sh # update Pytho{\}
docker build -t front-end:v1.0.0 front-end/  # build image
docker rmi -f $(docker images -f "dangling=true" -q) # cleanup <none> images
```

### Core
```
docker build -t core:v1.0.0 core/  # build image
docker rmi -f $(docker images -f "dangling=true" -q) # cleanup <none> images
```

## Running docker images

### Data crawler
```
docker run data-crawler:v1.0.0
```

### Front end
```
docker run front-end:v1.0.0
```

### Core
```
docker run core:v1.0.0
```


## Web Interfaces:
>- Mongo-Express: http://localhost:8081/