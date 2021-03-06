# vface-server-cpp
A vface-server C++ implementation.
See [vface-server](https://github.com/c910335/vface-server)
and [vface-web](https://github.com/c910335/vface-web)
for more detail.

## Dependencies
- dlib (will be install when compiling)
- OpenCV
- [websocketd](https://github.com/joewalnes/websocketd) (as websocket server)

## Compile
### Linux and macOS
```
$ mkdir build && cd build && cmake .. && make
```

If you have CPU which supports AVX instructions then run
this to have faster running speed:
```
$ mkdir build && cd build && cmake .. -DUSE_AVX_INSTRUCTIONS=1 && make
```

### Windows
I have not tested on Windows, so PR about this part is welcome :)

## Usage
```
$ websocketd --port=5566 ./vface_server_cpp
```

Then run [vface-web](https://github.com/c910335/vface-web).

## TO-DO
- [ ] use other C++ websocket libraries
- [ ] refactor the code

## Special Thanks
The source code of tracker is adopted from [GazeTracking](https://github.com/antoinelame/GazeTracking)
and rewritten in C++.
