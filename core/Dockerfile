FROM ubuntu:18.04 AS builder
RUN apt-get update -y
RUN apt-get install -y --no-install-recommends gcc g++ build-essential cmake

WORKDIR /code
COPY src/ .
RUN cd SLIDE && mkdir bin && cd bin && cmake .. && make
# RUN ls -R | awk '/:$/&&f{s=$0;f=0}/:$/&&!f{sub(/:$/,"");s=$0;f=1;next}NF&&f{ print s"/"$0 }' # show folder structure

FROM ubuntu:18.04
RUN apt-get update -y
RUN apt-get install -y libgomp1

# to build mongo drivers 
# TODO move to builder
RUN apt-get install -y --no-install-recommends cmake libssl-dev libsasl2-dev g++ python-dev build-essential
RUN mkdir /cpp_libs
COPY src/mongo-c-driver /cpp_libs/mongo-c-driver
RUN mkdir -p /cpp_libs/mongo-c-driver/cmake-build 
RUN cd /cpp_libs/mongo-c-driver/cmake-build ; \
 cmake -D ENABLE_AUTOMATIC_INIT_AND_CLEANUP=OFF .. &&  \
 cmake --build . && \
 cmake --build . --target install

COPY src/mongo-cxx-driver /cpp_libs/mongo-cxx-driver
COPY src/mnmlstc_core /cpp_libs/mnmlstc_core
RUN mkdir -p /cpp_libs/mongo-cxx-driver/build
RUN cd /cpp_libs/mongo-cxx-driver/build ; \
 cmake -D CMAKE_BUILD_TYPE=Release -D CMAKE_INSTALL_PREFIX=/usr/local -D CMAKE_CXX_COMPILER="/usr/bin/g++" .. && \
 cmake --build . --target EP_mnmlstc_core && \
 cmake --build . && \
 cmake --build . --target install

RUN rm -f libmongocxx.so._noabi || ln -s /usr/local/lib/libmogocxx.so.3.4.0 libmongocxx.so._noabi
RUN rm -f /usr/local/lib/libmongocxx.so || ln -s /usr/local/lib/libmongocxx.so._noabi /usr/local/lib/libmongocxx.so

WORKDIR /SLIDE
COPY --from=builder /code/SLIDE/bin ./bin/
COPY res/dataset ./dataset/
COPY src/SLIDE/Config_amz.csv .
RUN chmod +x ./bin/runme
CMD ["./bin/runme","Config_amz.csv"]