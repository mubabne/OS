# OS Lab 03 - Chapter 3: Processes

Оюутан: Ч.Мөнхбаяр  
Код: B242270045

Энэ хавтсанд Chapter 3-ын Practice Exercises 3.1-3.7-ийн монгол тайлан, Programming Problems 3.18-3.27-ийн эх код, мөн 1 минутын бичлэгийн ярианы төлөвлөгөө байна.

## Файлууд

- `OS_Lab03_Chapter3_Report.pdf` - асуулт, хариултын тайлан
- `programs/` - C болон Java бодлогуудын эх код
- `VIDEO_SCRIPT_MN.md` - өөрийн дуу хоолойгоор бичлэг хийх товч төлөвлөгөө

## Linux дээр шалгах

```bash
cd programs
make
./zombie
./time_pipe ls
./time_shm ls
./pid_manager
./collatz_fork 35
./collatz_shm 35
./reverse_case_pipes "Hi There"
./filecopy input.txt copy.txt
```

Java серверүүд:

```bash
javac QuoteServer.java HaikuServer.java EchoServer.java
java QuoteServer
java HaikuServer
java EchoServer
```

`fork()`, POSIX shared memory зэрэг нь Linux/POSIX API тул Windows дээр шууд ажиллахгүй. Linux эсвэл WSL2 дээр ажиллуулна.

