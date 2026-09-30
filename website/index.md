---
layout: home

hero:
  name: Hagane COBOL
  text: COBOL to Java compiler
  tagline: Translate COBOL programs into Java programs and run them on the JVM.
  actions:
    - theme: brand
      text: Linux
      link: /guide/linux
    - theme: alt
      text: Windows
      link: /guide/windows
    - theme: alt
      text: Docker
      link: /guide/docker
    - theme: alt
      text: API Reference (Javadoc)
      link: /javadoc/libcobj/index.html
      target: _self

features:
  - icon: 🐧
    title: Linux
    details: Build from the release tarball on Ubuntu or AlmaLinux and install cobj and libcobj.jar.
    link: /guide/linux
    linkText: Install on Linux
  - icon: 🪟
    title: Windows
    details: Build cobj.exe with Visual Studio and libcobj.jar with Gradle.
    link: /guide/windows
    linkText: Install on Windows
  - icon: 🐳
    title: Docker
    details: Pull a ready-to-use image from GitHub Container Registry and try it in seconds.
    link: /guide/docker
    linkText: Run with Docker
  - icon: 📚
    title: API Reference
    details: Javadoc of libcobj.jar, the runtime library used by the generated Java programs.
    link: /javadoc/libcobj/index.html
    target: _self
    linkText: Open Javadoc
---

## Hello World

```bash
# Translate HELLO.cbl into HELLO.java and compile it to HELLO.class
cobj HELLO.cbl

# Run it
java HELLO
```

See [Usage](./guide/usage) for more.

## Formerly opensource COBOL 4J

Hagane COBOL was formerly developed as a fork of [opensource COBOL 4J](https://github.com/opensourcecobol/opensourcecobol4j).
Version 1.0.0 is based on opensource COBOL 4J 2.1.0. The command names (`cobj`, `cobj-idx`, `cobj-api`), the runtime library
`libcobj.jar`, and the install paths are unchanged.
