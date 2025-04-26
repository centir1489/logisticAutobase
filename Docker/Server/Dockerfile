FROM ubuntu:22.04 as builder

RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    cmake \
    qtbase5-dev \
    qttools5-dev \
    libpq-dev \
    qtbase5-dev-tools \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /autobase
COPY . .

RUN mkdir -p build && \
    cd build && \
    cmake .. && \
    cmake --build . --target Server -- -j$(nproc)

FROM ubuntu:22.04

RUN apt-get update && apt-get install -y --no-install-recommends \
    libqt5core5a \
    libqt5gui5 \
    libqt5widgets5 \
    libqt5network5 \
    libqt5sql5 \
    libqt5sql5-psql \
    libpq5 \
    openssl \
    && rm -rf /var/lib/apt/lists/*

ARG UID=100001
ARG GID=100001
RUN groupadd -g $GID tester && \
    useradd -u $UID -g tester -m tester && \
    mkdir -p /autobase/config && \
    chown tester:tester /autobase/config && \
    ln -snf /usr/share/zoneinfo/${TZ:-Europe/Moscow} /etc/localtime && \
    echo ${TZ:-Europe/Moscow} > /etc/timezone

COPY --from=builder --chown=tester:tester /autobase/build/Server /autobase/server

ENV TZ=Europe/Moscow
ENV QT_DEBUG_PLUGINS=1
ENV QT_PLUGIN_PATH=/usr/lib/x86_64-linux-gnu/qt5/plugins
ENV QML2_IMPORT_PATH=/usr/lib/x86_64-linux-gnu/qt5/qml

USER tester
WORKDIR /autobase

CMD ["/autobase/server"]
