#include <iostream>

#include "json.hpp"
#include "ros/ros.h"
using namespace std;
using json = nlohmann::json;


/**
 * @brief 利用cpp的流读取操作，将json文件读入并简单的存取。
 * json读取后的操作，其用法与vector类似，指定你要进行的内容进行遍历；开辟出新new
 * 的内存进行存储；
 * */
#include <fstream>
namespace read_file_space
{
    void test()
    {
        string json_path;
        json_path = "/home/leiyubiao/json_test_ws/src/json1/cfg/test.json";
        ifstream i(json_path);
        json j;
        i >> j;
        //cout<<j<<endl;
        int size = j.size();
        cout << "size = " << size << endl;
        int size1 = j["agents"].size();
        for (int i = 0; i < size1; i++)
        {
            //cout<<i<<endl;
            auto p = j["agents"][i];
            cout << p["vehicle_param"][0] << endl;
        }
    }
} // namespace read_file_space

int main(int argc, char **argv)
{
    read_file_space::test();
    return 0;
}
