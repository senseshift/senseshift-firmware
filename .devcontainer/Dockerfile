FROM mcr.microsoft.com/vscode/devcontainers/base:ubuntu

ARG CLANG_VERSION=16

RUN apt-get update \
    && apt-get -qqy install \
    lsb-release \
    wget \
    software-properties-common \
    gnupg \
    bc \
    sudo \
    build-essential \
    ca-certificates \
    clang \
    curl \
    gcc \
    git \
    python3 \
    python3-dev \
    python3-pip \
    python3-venv \
    python3-distutils \
    python3-setuptools \
    srecord \
    udev \
    xz-utils \
    && wget --quiet https://apt.llvm.org/llvm.sh \
    && chmod +x llvm.sh \
    && ./llvm.sh $CLANG_VERSION && rm -rf llvm.sh \
    && apt-get --no-install-recommends -y install clang-format-$CLANG_VERSION clang-tidy-$CLANG_VERSION \
    && ln -s /usr/bin/clang-format-$CLANG_VERSION /usr/bin/clang-format \
    && ln -s /usr/bin/clang-tidy-$CLANG_VERSION /usr/bin/clang-tidy \
    && ln -s /usr/bin/python3 /usr/bin/python \
    && apt-get autoremove -y \
    && apt-get clean -y \
    && rm -rf /var/lib/apt/lists/*

RUN curl -fLo /etc/udev/rules.d/99-platformio-udev.rules --create-dirs https://raw.githubusercontent.com/platformio/platformio-core/develop/platformio/assets/system/99-platformio-udev.rules

USER $USERNAME

# RUN wget https://raw.githubusercontent.com/platformio/platformio-core-installer/master/get-platformio.py -O get-platformio.py \
#     && python3 get-platformio.py \
#     && rm -rf get-platformio.py

RUN python -m pip install --upgrade pip \
    && pip install --upgrade platformio

RUN sudo wget -O /usr/local/bin/wokwi-cli https://github.com/wokwi/wokwi-cli/releases/latest/download/wokwi-cli-linuxstatic-x64 \
    && sudo chmod +x /usr/local/bin/wokwi-cli
