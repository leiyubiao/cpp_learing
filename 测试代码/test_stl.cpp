#include<vector>
#include <list>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include<iostream>
using namespace std;

template<typename T>
void Print(const T data)
{
    cout<<data<<endl;
}

/********************************************************************/
//? 条款30： 确保目标区间足够大
//? 当要插入时，要确保被插入目标的容量 capacity足够大，一个通常的做法是用vector配合reserve，但是reserve只分配了内存，没有
//? 初始化元素，所以要配合back_inserter()来返回要的尾部迭代器。 
/********************************************************************/

namespace item30 // 确保目标区间足够大
{

    int transmogrify(const int x)
    {
        return x+1;
    }

    // 测试程序的全局变量
    vector<int> values{1,2,3};
    vector<int> results{4,5,6};

    void test1() // 将values的内容插入到results后面，（values的值通过了transmogrify的调用）
    {

        results.reserve(results.size() + values.size()); // reserve：开辟括号内大小的内存，但是没有初始化里面的数据。在transform中如果直接用不行。

        /**
         * @brief 将values里面的内容通过transmogrify函数操作用，再插入到results的尾部，通过调用back_inserter;注意拼写，是以er结尾的；
         * @param values.begin(): 要插入内容的头个迭代器，整个是左闭右开的。
         * @param values.end():  迭代器的尾部
         * @param back_inserter(results): 要插入迭代器的第一个位置，调用back_inserter函数后返回results的最后一个元素迭代器+1，
         * @param transmogrify: 对values插入范围元素的操作函数的指针。
         * @return void
         * 
         * 如果把back_inserter(results)改成results.end()则因为前面开辟内存用了reserve但是并没有初始内存，所以造成运行期的错误:输出的测试结果仍然为 4 5 6
         * 是一个迭代器适配器。
         **/
        transform(values.begin(), values.end(), back_inserter(results),transmogrify);
        for_each(results.begin(),results.end(),Print<int>); // FIXME:for_each里面的函数传入函数模板时要指定类型。
        // 结果为 4 5 6 2 3 4
    }

    void test2()
    {
        if (results.size() < values.size())
        {
            results.resize(values.size()); // resize也相当于初始化了
        }
        // results.clear();
        // results.reserve(values.size());


        transform(values.begin(),values.end(),results.begin(),transmogrify);
        //transform(values.begin(), values.end(), front_inserter(results),transmogrify);
        //! front_inserter 不能对vector使用，因为vector没有front_back()
        for_each(results.begin(),results.end(),Print<int>); 
        // 结果为：2 3 4
    }

} // namespace item30

/********************************************************************/
//? 条款31：了解你的排序选择
/**
 * ? 1. 有时候不需要对全局的元素进行排序，只需要对最前面的部分元素排序，而且也还会有两种情况发生
 * ? 一是前面几个元素排序好了，对应partial_sort；二是只需要取出前面最大的几个元素，但是也不关心顺序，对应nth_element
 * FIXME:前面说的这两种算法都是不稳定的。
 * ? 2. 如果要找到某一个特定的元素，比如第20%大（第二大）的元素，则用nth_element，在test2会有。
 * ? 
/********************************************************************/
namespace item31 // 了解你的排序选择
{
    vector<int> values{1,2,3,9,4,20};

    template<typename T>
    bool Compare(const T& lhs, const T& rhs)
    {
        return lhs > rhs;
    }

    /**
     * @brief 只关心部分排序，把前三大的数字排序出来，不关心其他的排序。前三在的数字按顺序输出。partial_sort
     * 如果也不关心前三大元素的排序，则用nth_element，两者的区别还在于中间的数值还差一位数。
     * */
    void test1() 
    {
        
        /**
         * @brief 对values中排序，取出最大的3个数，（也可以是最小的，按照Compare的定义，默认是最小的）。
         * @param 输入三个迭代器，第一个是起始位，第二个是要进行排序的位置，3代表要排序元素的个数，第三个结束的区间。Compare是自定义的迭代器。
         * */
        //partial_sort(values.begin(),values.begin() + 3, values.end(), Compare<int>);

        nth_element(values.begin(),values.begin() + 2, values.end(), Compare<int>);
        for_each(values.begin(), values.end(), Print<int>);
    }

    void test2()
    {
        vector<int>::iterator begin(values.begin()); // 方便表示迭代器的起点
        vector<int>::iterator end(values.end()); // 方便表示迭代器的终点

        //vector<int>::size_type goal_offset = 0.2*values.size(); // 输出质量前20%的元素，因为Compare是从大到小排序的，所以是第20%大的元素。
        vector<int>::size_type goal_offset = 3; // 输出质量第3大的。
        nth_element(begin,begin+goal_offset, end,Compare<int>);
        cout<<"the need value = "<<*(begin + goal_offset); // cout: 9
        //for_each(values.begin(), values.end(), Print<int>);

    }











} // namespace item31




int main(int argc ,char** argv)
{
    item31::test2();
    return 0;
}