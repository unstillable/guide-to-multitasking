#ifndef CONCURRENTPCSTREE_H
#define CONCURRENTPCSTREE_H
#include <atomic>
#include <cassert>
#include <mutex>

//BLOCKING

//three points
struct PCSNode
{
	PCSNode* parent ;
	PCSNode* child  ;
	PCSNode* sibling;
	std::recursive_mutex lock;

	


};


class ConcurrentPCSTree
{
public:
//	std::atomic<PCSNode*>* root;
	PCSNode* root;

	ConcurrentPCSTree();
	~ConcurrentPCSTree();





	//void insert(PCSNode* nodeIn, PCSNode* pParent)
	//{
	//	assert(nodeIn->parent.load() == nullptr);
	//	assert(nodeIn->child.load()  == nullptr);
	//	assert(nodeIn->sibling.load()== nullptr);
	//	
	//	//could lock here, force use of  
	//	
	//	nodeIn->parent.store(pParent);
	//	nodeIn->sibling.store(pParent->child.load());
	//	nodeIn->child.store(pParent->child.load());
	//	//nodeIn->sibling.store(p)
	//
	//}

};

#endif // CONCURRENTPCSTREE_H
