[json使用教程](https://blog.csdn.net/fengxinlinux/article/details/71037244)
---
### 1. 简易方法：
- 到[github](https://github.com/leiyubiao/json)上找到jsoncpp中，单独下载single_include/include文件中json.hpp文件，具体的方法点击raw的右键，选择“下载链接为”选项，单独输出文件。

- 然后在头文件中include即可，并加入using json = nlohmann::json;
- 对文件的读操作：
    - 要include<fstream>,进行流读取的操作；
    - 后续将要json的路径改成用ros进行读取相对的路径；//TODO: