# Run with Docker

Docker images with Hagane COBOL installed are published on
[GitHub Container Registry](https://github.com/yutaro-sakamoto/Hagane-COBOL/pkgs/container/hagane-cobol).

| Image | Source code encoding |
| -- | -- |
| `ghcr.io/yutaro-sakamoto/hagane-cobol:1.0.0` | Shift_JIS |
| `ghcr.io/yutaro-sakamoto/hagane-cobol:1.0.0-utf8` | UTF-8 |

## Try the sample program

```bash
docker run -it --rm ghcr.io/yutaro-sakamoto/hagane-cobol:1.0.0
```

In the container:

```bash
cd /root/cobol_sample
cobj HELLO.cbl
java HELLO
# HELLO WORLD!
```

## Compile your own programs

Mount the current directory into the container and compile the programs in it:

```bash
docker run --rm -v "$PWD":/work -w /work ghcr.io/yutaro-sakamoto/hagane-cobol:1.0.0 \
  bash -c 'cobj HELLO.cbl && java HELLO'
```

On Windows PowerShell, use `${PWD}` instead of `"$PWD"`.

::: info
The container runs as root, so on a Linux host the generated `.java` and `.class` files are owned by root.
:::
