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
