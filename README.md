# MuInstantMessage

- [运行方法](#运行方法)

## 运行方法
### GateServer
该子项目是CMake生成项目，需要自己通过CMake生成，并通过编译器编译即可，还需要将该子项目根目录的config.ini移动到编译的目录。
作者使用的是最新的VS2022环境进行编译，包含jsoncpp、boost、grpc、redis plus plus等C++相关库，可通过vcpkg进行安装并管理。

### VarfiyServer
该子项目用到的了Bun.js，主要用于提供验证服务，运行的话仅仅需要添加一些包，然后运行下面这行命令即可。
``` powershell
bun run server.ts
```

### muchat
该子项目用到的Qt版本是5.15.2，构建项目用的是CMake，所以说打开整个项目只需要通过Qt Creator打开CMakeLists.txt即可。
