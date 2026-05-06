FROM python:3.14-slim-bookworm

WORKDIR /myapp

RUN apt-get update && apt-get -y upgrade

RUN apt-get update && apt-get install -y \
        x11-apps \
        python3-tk

COPY ./requirements.txt /myapp/

RUN pip3 install --upgrade pip && pip3 install -r /myapp/requirements.txt
