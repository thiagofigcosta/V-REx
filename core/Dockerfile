FROM ubuntu:18.04 AS builder
RUN apt-get update
RUN apt-get install -y --no-install-recommends gcc g++ build-essential cmake

WORKDIR /code
COPY src/ .
RUN cd SLIDE && mkdir bin && cd bin && cmake .. && make
# RUN ls -R | awk '/:$/&&f{s=$0;f=0}/:$/&&!f{sub(/:$/,"");s=$0;f=1;next}NF&&f{ print s"/"$0 }' # show folder structure

FROM ubuntu:18.04
RUN apt-get update
RUN apt-get install -y libgomp1

WORKDIR /SLIDE
COPY --from=builder /code/SLIDE/bin ./bin/
COPY dataset ./dataset/
COPY src/SLIDE/Config_amz.csv .
RUN chmod +x ./bin/runme
CMD ["./bin/runme","Config_amz.csv"]