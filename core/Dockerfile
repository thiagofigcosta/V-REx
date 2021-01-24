FROM ubuntu_2004_vrex_core_build

RUN mkdir /vrex

COPY res/iris.data /vrex/res/iris.data
COPY src/vrex /vrex/src/

RUN cd /vrex/src/ ; make compileAndLink
RUN chmod +x /vrex/src/bin/vrex

ENV LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib

COPY entrypoint.sh /entrypoint.sh
RUN chmod +x /entrypoint.sh
CMD ["./entrypoint.sh"]