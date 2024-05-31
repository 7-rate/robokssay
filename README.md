「踏めば助かるのに...」でおなじみのロボカスに何でも言わせることが出来ます。

## 使い方
```bash
gcc main.c -o robokassay
./robokassay "セリフ"
```

### Dockerを使って実行する場合
以下のようにビルドします。
```bash
docker build . -t robokassay
```

実行は以下のようにします。
```bash
docker run robokssay robokssay
```

## Result Example
![result](docs/image.png)
