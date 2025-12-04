FROM almalinux:9

SHELL ["/bin/bash", "-o", "pipefail", "-c"]

# classpath settings
ENV CLASSPATH=:/usr/lib/opensourcecobol4j/libcobj.jar

# hadolint ignore=DL3041
RUN dnf update -y \
    && dnf install -y --setopt=install_weak_deps=False \
        epel-release \
    && dnf install -y --setopt=install_weak_deps=False \
        gcc \
        make \
        bison \
        flex \
        automake \
        autoconf \
        diffutils \
        gettext \
        java-21-openjdk-devel \
        git-clang-format \
        cppcheck \
        libtool \
        gettext-devel \
        unzip \
    && dnf clean all \
    && rm -rf /var/cache/dnf

RUN echo 'export CLASSPATH=:/usr/lib/opensourcecobol4j/libcobj.jar' >> ~/.bashrc
