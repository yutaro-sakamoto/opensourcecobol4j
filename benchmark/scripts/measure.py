#!/usr/bin/env python3
"""1コマンドの実行時間・最大RSSを計測するラッパー。

使い方: measure.py <出力jsonファイル> <コマンド...>
wall時間(秒)、user/sys CPU時間(秒)、子プロセスの最大RSS(KB)をJSONで出力する。
GNU time が無い環境でも動くよう resource.getrusage(RUSAGE_CHILDREN) を使う。

注意: RUSAGE_CHILDREN の ru_maxrss は「このプロセスが起動した全子プロセスの
歴代最大RSS」を返すため、このスクリプトは1回の計測ごとに新しいプロセスとして
起動すること (run_macro.sh はそのように呼び出している)。1プロセス内で複数回
計測すると前回の最大値が引き継がれてしまう。
"""

import json
import resource
import subprocess
import sys
import time


def main():
    out_path = sys.argv[1]
    cmd = sys.argv[2:]
    before = resource.getrusage(resource.RUSAGE_CHILDREN)
    t0 = time.monotonic()
    proc = subprocess.run(cmd)
    wall = time.monotonic() - t0
    after = resource.getrusage(resource.RUSAGE_CHILDREN)
    result = {
        "cmd": " ".join(cmd),
        "wall_s": round(wall, 3),
        "user_s": round(after.ru_utime - before.ru_utime, 3),
        "sys_s": round(after.ru_stime - before.ru_stime, 3),
        # ru_maxrss は Linux では KB 単位 (このプロセスツリーでの最大値)
        "maxrss_kb": after.ru_maxrss,
        "exit": proc.returncode,
    }
    with open(out_path, "w") as f:
        json.dump(result, f)
    print(json.dumps(result))
    sys.exit(proc.returncode)


if __name__ == "__main__":
    main()
