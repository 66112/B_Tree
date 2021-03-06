/*
1.一棵M（M > 2）阶的B树，是一棵平衡的M路平衡搜索树,可以是空树或满足以下性质
  (1)根节点至少有两个孩子,[2,M]个孩子
  (2)每个非根节点有[M/2,M]个孩子
  (3)每个非根节点有[M/2-1,M-1]个关键字,并且以升序排列
  (4)每个节点孩子的数量比关键字的数量多一个
  (5)key[i]和key[i+1]之间的孩子节点的值介于key[i]、key[i+1]之间
  (6)所有的叶子节点都在同一层
缺点：1.B树的空间浪费巨大
      2.每分裂一次,会有巨大的空间浪费
	  3.插入数据每次要挪数据
	  4.虽然数的高度不高,但每次二分查找也会有消耗
B树的删除，要始终保证每个节点中至少有 M/2 个元素,如果删的不够了,那就合并
*/
/*B+树
	1.基本与B-树相同
	2.非叶子节点的孩子与关键字个数相同
	3.非叶子节点的子树指针p[i],指向关键字值属于[K[i],K[i+1])的子树; 左闭右开 B-树是开区间
	4.为所有叶子节点增加一个链指针
	5.所有关键字都在叶子节点，再将叶子节点链接起来
	6.方便遍历数
*/
/*
B+的特性
	1.所有关键字都在叶子节点(稠密索引)，再将叶子节点链接起来,及其方便遍历
	2.不可能在非叶节点命中
	3.非叶节点相当于叶子节点的索引(稀疏索引),叶子节点相当于是存储数据的数据层
	4.更适合文件索引系统
	B*树主要提高了内存利用率,当元素个数大于 2/3M 时,再分裂
*/
#include "B.h"
int main()
{
	TestBTree();
	return 0;
}