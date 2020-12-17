FROM python:3.8 AS builder

COPY requirements.txt .

RUN pip install --user -r requirements.txt

FROM python:3.8-slim
WORKDIR /code

COPY --from=builder /root/.local /root/.local
COPY Pytho{N}.py .
COPY entrypoint.sh .
COPY frontstarter.sh .
COPY src/ .

RUN chmod +x entrypoint.sh
RUN chmod +x frontstarter.sh

RUN ln -s frontstarter.sh /bin/front-end
RUN ln -s frontstarter.sh /bin/frontend
RUN ln -s frontstarter.sh /bin/front

ENV PATH=/root/.local/bin:$PATH

RUN cp /bin/bash /bin/bash_bkp
RUN rm -f /bin/bash && ln -s entrypoint.sh /bin/bash
# RUN rm -f /bin/sh && ln -s entrypoint.sh /bin/sh # still testing

CMD ["./entrypoint.sh"]