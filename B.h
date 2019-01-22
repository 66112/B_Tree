#pragma once 
#include <iostream>
using namespace std;
template<class K, class V, size_t M>  //M个元素，即一个节点能放多少个元素
struct BTreeNode
{
	pair<K, V> _kvs[M];      //有M个要存的元素
	BTreeNode<K, V, M>* _subs[M + 1];     //有M+1个孩子，方便分裂
	size_t _size;      //记录_kvs中有几个元素
	BTreeNode<K, V, M>* _parent;
	BTreeNode()
	{
		memset(_subs, 0, sizeof(BTreeNode<K, V, M>*)*(M + 1));
		_size = 0;
		_parent = nullptr;
		for (size_t i = 0; i <= M; i++){
			_subs[i] = nullptr;
		}
	}
};
template<class K,class V,size_t M>  //M孩子的数量
class BTree
{
	typedef BTreeNode<K, V, M> Node;
public:
	BTree() :_root(nullptr)
	{}
	~BTree()
	{}
	pair<Node*, int> Find(const pair<K, V>& kv)
	{
		Node* parent = nullptr;
		Node* cur = _root;
		size_t i = 0;
		while (cur){
			for (i = 0; i < cur->_size; i++){
				if (kv.first < cur->_kvs[i].first){   //走左树,第一个孩子的右树是第二个孩子的左树
					break;
				}
				else if (cur->_kvs[i].first == kv.first){
					return pair<Node*, int>(cur, i);
				}
			}
			parent = cur;
			cur = cur->_subs[i];  //当把这个节点走完,走最后一个节点的右树
		}
		return pair<Node*, int>(parent, -1);   //没找到
	}
	bool Insert(const pair<K, V>& kv)
	{
		if (_root == nullptr){
			_root = new Node;
			_root->_kvs[0] = kv;
			_root->_size = 1;
			return true;
		}
		//每次往叶子上插入,倒着生长,别的节点都是分裂出来的
		pair<Node*, int> ret = Find(kv);
		if (ret.second != -1){   //找到了
			return false;
		}
		Node* cur = ret.first;
		pair<K, V> newkv = kv;
		Node* sub = nullptr;
		while (1){

			InsertKV(cur, newkv, sub);
			if (cur->_size < M){   //没满
				return true;
			}
			else{
				//满了分裂，拷一半值给新节点
				Node* newnode = new Node;
				size_t mid = M / 2;
				size_t index = 0;
				size_t i = 0;
				for (i = mid + 1; i < M; i++){
					newnode->_kvs[index] = cur->_kvs[i];
					newnode->_subs[index] = cur->_subs[i];
					cur->_subs[i] = nullptr;
					if (cur->_subs[i])
						cur->_subs[i]->_parent = newnode;
					index++;
					newnode->_size++;
				}
				//拷贝最后一个右孩子
				newnode->_subs[index] = cur->_subs[i];
				cur->_subs[i] = nullptr;
				if (cur->_subs[i])
					cur->_subs[i]->_parent = newnode;
				cur->_size -= (newnode->_size + 1);
				if (cur == _root){
					_root = new Node;
					_root->_kvs[0] = cur->_kvs[mid];
					_root->_subs[0] = cur;
					_root->_subs[1] = newnode;
					_root->_size = 1;
					cur->_parent = _root;
					newnode->_parent = _root;
					return true;
				}
				else{
					//转换成往父节点的同一级分裂
					newkv = cur->_kvs[mid];
					sub = newnode;
					cur = cur->_parent;
				}
			}
		}
	}
	void InOrder()
	{
		_InOrder(_root);
	}
private:
	void _InOrder(Node* root)
	{
		if (root == nullptr){
			return;
		}
		size_t i = 0;
		for (i = 0; i < root->_size; i++){
			_InOrder(root->_subs[i]);      //访问左孩子
			cout << root->_kvs[i].first << " ";
		}
		_InOrder(root->_subs[i]);    //最后一个为最后一个元素的右孩子
	}
	void InsertKV(Node* node, const pair<K, V>& kv, Node* sub)
	{
		int end = node->_size - 1;
		while (end >= 0){
			if (node->_kvs[end].first > kv.first){
				node->_kvs[end + 1] = node->_kvs[end];
				node->_subs[end + 2] = node->_subs[end + 1];
				end--;
			}
			else{
				break;
			}
		}
		node->_kvs[end + 1] = kv;
		node->_subs[end + 2] = sub;
		if (sub)
			sub->_parent = node;
		node->_size++;
	}
private:
	Node* _root;
};

void TestBTree()
{
	int arr[] = { 53, 75, 139, 49, 145, 36, 101 };
	BTree<int,int, 3> tree;
	for (size_t i = 0; i < sizeof(arr)/sizeof(arr[0]); i++){
		tree.Insert(make_pair(arr[i], i));
	}
	tree.InOrder();
}