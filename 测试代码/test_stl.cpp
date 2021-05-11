#include<vector>
#include <list>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <memory>
#include<iostream>
using namespace std;

template<typename T>
void Print(const T data)
{
    cout<<data<<endl;
}


class Widget
{
     public:
        Widget()=default;
        Widget(const int data):data_(data){}
        ~Widget(){}

        bool isCertified(shared_ptr<Widget> none) const
        {
            if (data_ > 0)
            {
                return true;
            }
            else 
            {
                return false;
            }
            
        }
        
        int get_data() const
        {
            return data_;
        }
    private:
        int data_;
};
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
 * ? 3. sort,partial_sort, nth_element需要随机访问迭代器，所以它们只能用于vector,deque,string,数组。对标准关联容器排序元素没有意义
 *? 因为这样的容器在任何时候保持有序。唯一不能使用的容器是list,但list.sort()提供的稳定排序。
 *?4. 有时不需要鉴别出20个质量最高的widget，而需要鉴别出所有质量等级为1，或2的。使用partion()来返回迭代器。
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
     * 如果也不关心前三大元素的排序，则用nth_element，两者的区别还在于中间的数值还差一位数,nth_element第二个变量是元素的个数，而partial_element是区间长度。
     * */
    void test1() 
    {
        
        /**
         * @brief 对values中排序，取出最大的3个数，（也可以是最小的，按照Compare的定义，默认是最小的）。
         * @param 输入三个迭代器一个比较函数，第一个是起始位，第二个是要进行排序的位置，3代表要排序元素的个数，第三个结束的区间。Compare是自定义的迭代器。
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

    // bool hasAcceptableQuality(const Widget& w)
    // {
    //     // 返回质量等级是否是或更高；
    // }

    // void test3()
    // {
    //     vector<Widget> widgets;
    //     vector<Widget>::iterator good_end;
    //     good_end = partition(widgets.begin(), widgets.end(),hasAcceptableQuality); // 返回所有满足has...的widget移动widgets前端，
    //     // 并返回一个指向第一个不满足的widget的迭代器。
    // }
}

/********************************************************************/
//? 条款32： 如果你真的想删除东西的话就在类似remove的算法后接上erase
//? 1. 关于remove：remove(first_iterator, end_iterator, value) 传入的是一对迭代器，不接收容器，因为
//? 因为它不知道作用于哪个容器。而从容器中除去一个元素的唯一方法是调用这个容器的成员函数，几乎都是erase的某个形式，所以
//? remove无法删除元素。 remove是运用不断覆盖的方法来直到返回一个，即将被覆盖元素的迭代器。不会改变元素顺序。
//? 详细过程，见pdf 145 页。
//
//? 2. list中，remove与erase已经整合到它的成员函数中去了。li.remove(99); 可以直接使用
//? 3. 关联容器中，成员函数中有直接的erase能直接用。
//? 4. 类似的还有remove_if unique（用来从一个区间删除东西，邻近的重复值，而不用访问持有区间元素的容器。）
/********************************************************************/

namespace item32
{
    vector<int> values{1,2,3,99,5,99,7,8,99};
    void test1()
    {
        vector<int> a;
        a.reserve(10);
        for (int i=0;i<10;i++)
        {
           a.push_back(i); // 用了reserve只能用push_back的方式。
           //a[i] = i;
        }
        for_each(a.begin(), a.end(),Print<int>);
    }
    void test2()
    {
        values.erase(remove(values.begin(), values.end(),99), values.end());
        for_each(values.begin(),values.end(),Print<int>);
    }


}// namespace item32


/********************************************************************/
//? 条款33：提防在指针的容器上使用类似remove的算法。
/**
 * ? 1. 如果使用erase 和remove 或者 remove_if，则删除的是vector上存入的指针，但不会使指针使指向的内存消失，
 * 比较好的一个方法是会shared_pt来管理内存。
/********************************************************************/

#include<memory>
namespace item33
{
    /* 以下为伪代码，需要注释掉，不然会出错。*/
   /* void test1() //TODO:有问题，后续再解决，出错在v.erase语句中的men_fun中
    {   
        using RCSPW = std::shared_ptr<Widget>; // RCSP to Widget；RCSP是引用计数的意思
        vector<RCSPW> v;
        v.push_back(make_shared<Widget>(1));
        v.push_back(make_shared<Widget>(-1));

        // ... 
        // 上面再插入相关的push_back操作
        //mem_function 是  <functional>头文件中的。
        v.erase(remove_if(v.begin(),v.end(), not1(mem_fun(&Widget::isCertified))) ,v.end());
        // for (auto p:v)
        // {
        //     cout<<"data_ = "<<p->get_data()<<endl;
        // }

    }*/
};

/****************************************************************
 * ? 条款34：注意哪个算法需要有序区间 
 * ? 1. 有以下函数需要用到：用到了二分查找的函数， binary_search 、lower_bound、 upper_bound、 equal_range
 * ? 2. merge,inplace_merge 执行了有效的单遍历合并排序算法。
 * ? 3. unique: 一般用于从区间去除所有重复值。
 * ? 4. 用于排序的函数，查找的算法要一致，不能一个升序，一个降序。
 * **************************************************************/

namespace item34
{
    vector<int> v{1,3,5,4};
    void test1()
    {
        sort(v.begin(), v.end(),greater<int>());
        bool a5Exists;
        a5Exists = binary_search(v.begin(),v.end(),5,less<int>()); // FIXME:如果用less<int>() 会进入false
        if (a5Exists)
        {
            cout<<"find 5"<<endl;
        }
        else 
        {
            cout<<"no 5"<<endl;
        }
    }

} // namespace item34


int main(int argc ,char** argv)
{
    item34::test1();
    return 0;
}