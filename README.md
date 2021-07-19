# cpp_learing
---
包含以下内容：
### 1. effective stl
### 2. json.hpp的使用例程
### 3. 设计模式
### 4. c++ 多线程编程

---
## protobuf3 下载安装、卸载与使用教程
[总的教程](https://github.com/protocolbuffers/protobuf/tree/master/src)
### 1. 下载
[protobuf3 的下载地址](https://github.com/protocolbuffers/protobuf/releases/latest
) 
由于我所有的版本我都要需要，所以要下载第一个 probobuf-all...
下载后直接解压，如果没有进行解压则下载相应的解压软件。

### 2.安装

[可借鉴的教程](https://blog.csdn.net/tropicofcancer9/article/details/96306797?utm_medium=distribute.pc_relevant.none-task-blog-2%7Edefault%7EBlogCommendFromMachineLearnPai2%7Edefault-3.control&depth_1-utm_source=distribute.pc_relevant.none-task-blog-2%7Edefault%7EBlogCommendFromMachineLearnPai2%7Edefault-3.control)
以下的每一步都是必须要做的,如果没有权限则加上sudo 
而有些教程上的 ./autogen.sh　这部是用另外一个方法进行获取的资源，目的也是生成　 configure.sh　文件。

~~~bash
./configure
make #　漫长的等待。
sudo make check #　又是漫长的等待,如果不加sudo 可能会失败，后续也能进行安装，但有些功能就可能会缺失。所以这步还是要做的！！！
sudo make install #　将依赖装入到/usr　路径下
sudo ldconfig # 必做项，刷新共享库的缓存区。不然无法进行 protoc --version　指令。
~~~
验证是否安装成功的方法。

~~~bash
protoc --version #　可以显示我所装的protoc的版本号。

which -a protoc # 搜索的只是可执行文件所在的地方，如果不加-a, 则输出第一个找到的可执行文件的位置。
# 输出结果：　/usr/local/bin/protoc　　默认安装到了 /usr/local/bin下面，这是可执行文件的位置所在。

whereis protoc #　输出所有相关的结果：　protoc: /usr/bin/protoc /usr/local/bin/protoc /usr/share/man/man1/protoc.1.gz


cmake --help-module FindProtobuf #　可以终端看到CmakeLists.txt应该要有的内容。

~~~

### 3. 卸载
3.1 失败的方法
~~~bash
sudo apt-get remove protobuf-compiler
sudo apt-get remove libprotobuf-dev　# **这步有重大风险，可能会把ros 搞崩！！！**
~~~
因为只有通过　 apt-get install 才能用　apt-get remove　进行卸载，而由之前的教程可以看出是通过　make install 进行的。所以要用以下方法进行卸载。

~~~bash
cd <到你make install 的文件夹下>
make uninstall　#　这个时候应该就可以，再用　whereis protoc 验证下即可，正常只有ubuntu　自还的protoc　版本了。
~~~

### 4. 使用教程
#### 4.1 cpp　使用教程
[1.　基础教程](https://www.cnblogs.com/DswCnblog/p/6700660.html)
[2. 编译教程](https://blog.csdn.net/m0_37542524/article/details/94905775?utm_medium=distribute.wap_relevant.none-task-blog-baidujs_title-0)

~~~cpp
// game.proto

syntax = "proto3";
package pt;
//option optimize_for = LIFE_RUNTIME;

message req_login
{
    string username = 1;
    string password = 2;

}

message obj_user_info
{
    string nickname = 1; //　昵称
    string icon = 2; //　头像
    int64 coin = 3; //　金币
    string location = 4; //　所属地
}

//　游戏数据统计
message obj_user_game_record
{
    string time = 1;
    int32 kill = 2; // 杀死别人的人头数
    int32 dead = 3; //　死亡数。
    int32 assist = 4; //　助攻数
}

message rsp_login
{
    enum LIFE_RUNTIME{
        SUCCESS = 0; 
        ACCOUNT_NULL = 1; //　账号不存在
        ACCOUNT_LOCK = 2; // 账号锁定
        PASSWORD_ERROR = 3; //　密码错误
        ERROR = 10;
    }

    int32 ret = 1;
    obj_user_info  user_info = 2;
    repeated obj_user_game_record record = 3; //　repeated 关键字是数组的意思。
}
~~~

~~~cpp
// main.cpp
#include <iostream>
#include <string>
#include "game.pb.h"

int main()
{
    pt::rsp_login rsp{};
    rsp.set_ret(pt::rsp_login_LIFE_RUNTIME_SUCCESS);
    auto user_info = rsp.mutable_user_info();　//　返回一个指针，指向user_info
    user_info->set_nickname("dsw"); //　set_"name"() 写　的方式
    user_info->set_icon("345DS55GF34D774S");
    user_info->set_coin(2000);
    user_info->set_location("zh");


    for (int i = 0; i < 5; i++) {
        auto record = rsp.add_record();　//　record　是repeated数组，先使用add 实例化。
        record->set_time("2017/4/13 12:22:11");　// 接下来再set
        record->set_kill(i * 4);
        record->set_dead(i * 2);
        record->set_assist(i * 5);
                
    }

    std::string buff{};
    rsp.SerializeToString(&buff); //　可能是因为数据结构比较大
    //使用　& 的方法进行使用。 proto 序列化为string。

    //------------------解析----------------------
    pt::rsp_login rsp2{};
    if (!rsp2.ParseFromString(buff)) {　//　从string　反序列化为　proto　数据类型。
        std::cout << "parse error\n";
    }
    
    auto temp_user_info = rsp2.user_info();
    std::cout << "nickname:" << temp_user_info.nickname() << std::endl;
    std::cout << "coin:" << temp_user_info.coin() << std::endl;
    for (int m = 0; m < rsp2.record_size(); m++) {
        auto temp_record = rsp2.record(m);  //　record是数组，读取数组每m个位置用()
        // 来读取。
        std::cout << "time:" << temp_record.time() << " kill:" << temp_record.kill() << " dead:" << temp_record.dead() << " assist:" << temp_record.assist() << std::endl;
    }
}
~~~

~~~cmake
cmake_minimum_required(VERSION 3.0.2)
project(protoc_tutorial)

add_compile_options(-std=c++11)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
)

set(proto_dir ${PROJECT_SOURCE_DIR}/proto)
file(GLOB  proto_files ${PROJECT_SOURCE_DIR}/proto/game.proto

 )
message(STATUS "Proto Source Dir: ${proto_dir}")
message(STATUS "Proto Source Files: ${proto_files}")

#Find required protobuf package
find_package(Protobuf REQUIRED)
if(PROTOBUF_FOUND)
    message(STATUS "protobuf library found")
else()
    message(FATAL_ERROR "protobuf library is needed but cant not be found")
endif()

set(PROTO_SRCS ${PROJECT_SOURCE_DIR}/proto/)
set(PROTO_HDRS ${PROJECT_SOURCE_DIR}/proto/)
message(STATUS "proto src ${PROTO_SRCS}")
include_directories(${PROTOBUF_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
PROTOBUF_GENERATE_CPP(PROTO_SRCS PROTO_HDRS ${proto_files})

include_directories(
   ＃include
  ${catkin_INCLUDE_DIRS}
)

add_executable(cppTest src/main.cpp ${PROTO_SRCS} ${PROTO_HDRS})
target_link_libraries(cppTest ${PROTOBUF_LIBRARIES})
~~~


## python　调用 C++ .SO　文件
~~~cpp
#include <stdio.h>
 
typedef struct TestDLL_
{
	int a;
	//char *b;
} testdll;
 
int test(testdll t)
{
	t.a=t.a+t.a;
	printf("%d\n",t.a);
	return t.a;
}

extern "C" {
	int Test (testdll t) {
		int result = test(t);
		return result;
	}
}
~~~

~~~python
from ctypes import  *

ll = cdll.LoadLibrary('./libmy.so')

class pyStruct(Structure):
    _fields_ = [
        ("a", c_int)
    ]
data = pyStruct()
data.a = 1

result = ll.Test(data)
print(result)
~~~
