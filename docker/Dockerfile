# Build Hagane COBOL from the source tree in the build context and
# install it into a minimal runtime image.
#
# Build arguments:
#   configure_args : extra arguments passed to ./configure
#                    (e.g. "--enable-utf8" for the UTF-8 build)

# Build stage
FROM almalinux:9-minimal AS builder

ARG configure_args=""

SHELL ["/bin/bash", "-c"]

# install build dependencies
RUN microdnf update -y && \
    microdnf install -y gcc make bison flex m4 automake autoconf diffutils gettext java-11-openjdk-devel tar gzip && \
    microdnf clean all

# build and install Hagane COBOL (EXEC SQL support is included)
COPY . /root/opensourcecobol4j
WORKDIR /root/opensourcecobol4j
RUN ./configure --prefix=/usr/ ${configure_args} && \
    touch cobj/*.m4 && \
    make && \
    make install DESTDIR=/staging

# Runtime stage
FROM almalinux:9-minimal

# link the image published to GitHub Container Registry to this repository
LABEL org.opencontainers.image.source="https://github.com/yutaro-sakamoto/Hagane-COBOL"
LABEL org.opencontainers.image.description="Hagane COBOL, a COBOL compiler that translates COBOL programs to Java programs"
LABEL org.opencontainers.image.licenses="GPL-3.0-or-later AND LGPL-3.0-or-later"

SHELL ["/bin/bash", "-c"]

# install runtime dependencies only (javac is required by cobj)
RUN microdnf update -y && \
    microdnf install -y java-11-openjdk-devel && \
    microdnf clean all && \
    rm -rf /var/cache/microdnf/*

# copy the installed files from the builder stage
COPY --from=builder /staging/usr/ /usr/

# classpath settings
ENV CLASSPATH=:/usr/lib/opensourcecobol4j/libcobj.jar
RUN echo 'export CLASSPATH=:/usr/lib/opensourcecobol4j/libcobj.jar' >> ~/.bashrc

# add sample programs
COPY docker/cobol_sample /root/cobol_sample

WORKDIR /root/

CMD ["/bin/bash"]
