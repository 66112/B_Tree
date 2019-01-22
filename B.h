#pragma once 
#include <iostream>
using namespace std;
template<class K, class V, size_t M>  //M��Ԫ�أ���һ���ڵ��ܷŶ��ٸ�Ԫ��
struct BTreeNode
{
	pair<K, V> _kvs[M];      //��M��Ҫ���Ԫ��
	BTreeNode<K, V, M>* _subs[M + 1];     //��M+1�����ӣ��������
	size_t _size;      //��¼_kvs���м���Ԫ��
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
template<class K,class V,size_t M>  //M���ӵ�����
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
				if (kv.first < cur->_kvs[i].first){   //������,��һ�����ӵ������ǵڶ������ӵ�����
					break;
				}
				else if (cur->_kvs[i].first == kv.first){
					return pair<Node*, int>(cur, i);
				}
			}
			parent = cur;
			cur = cur->_subs[i];  //��������ڵ�����,�����һ���ڵ������
		}
		return pair<Node*, int>(parent, -1);   //û�ҵ�
	}
	bool Insert(const pair<K, V>& kv)
	{
		if (_root == nullptr){
			_root = new Node;
			_root->_kvs[0] = kv;
			_root->_size = 1;
			return true;
		}
		//ÿ����Ҷ���ϲ���,��������,��Ľڵ㶼�Ƿ��ѳ�����
		pair<Node*, int> ret = Find(kv);
		if (ret.second != -1){   //�ҵ���
			return false;
		}
		Node* cur = ret.first;
		pair<K, V> newkv = kv;
		Node* sub = nullptr;
		while (1){

			InsertKV(cur, newkv, sub);
			if (cur->_size < M){   //û��
				return true;
			}
			else{
				//���˷��ѣ���һ��ֵ���½ڵ�
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
				//�������һ���Һ���
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
					//ת���������ڵ��ͬһ������
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
			_InOrder(root->_subs[i]);      //��������
			cout << root->_kvs[i].first << " ";
		}
		_InOrder(root->_subs[i]);    //���һ��Ϊ���һ��Ԫ�ص��Һ���
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