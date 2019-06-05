# pod_plugin
Add-ons for Protocol Buffers, it change proto message to c struct 


## Clone the repository (including submodules)

```sh
 $ git clone https://github.com/snow1313113/pod_plugin.git
 $ cd pod_plugin
 $ git submodule update --init
 ```

## Build from source

```sh
 $ mkdir build
 $ cd build
 $ cmake ../
 $ make
 ```

## Use the plugin

 protoc --proto_path=*your path* --pod_out=*output path* --plugin=protoc-gen-pod=pod_plugin *proto file*

## Build gtest case

```sh
 $ cd pod_plugin/test
 $ mkdir build
 $ cd build
 $ cmake ../
 $ make
 ```

