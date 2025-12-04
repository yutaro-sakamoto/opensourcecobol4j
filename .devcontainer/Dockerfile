FROM almalinux:9-minimal

SHELL ["/bin/bash", "-o", "pipefail", "-c"]

# classpath settings
ENV CLASSPATH=:/usr/lib/opensourcecobol4j/libcobj.jar

# hadolint ignore=DL3041
RUN microdnf update -y \
    && microdnf install -y --setopt=install_weak_deps=False \
        epel-release \
    && microdnf install -y --setopt=install_weak_deps=False \
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
    && microdnf clean all \
    && rm -rf /var/cache/dnf

RUN echo 'export CLASSPATH=:/usr/lib/opensourcecobol4j/libcobj.jar' >> ~/.bashrc
