#ifndef TreeNode_H
#define TreeNode_H

#include "DataTypes.h"

#include "Block.h"

class TreeNode {
public:
	TreeNode();
	~TreeNode();
private:
	TreeNode *m_left;
	TreeNode *m_right;
	Block *m_data;
	uint32_t m_headID;
	/* value is used in compressed-path trees. */
	/*uint64_t m_value;*/
public:
	TreeNode *left() const { return m_left; }
	void setLeft(TreeNode *left) { m_left = left; }
	TreeNode *right() const { return m_right; }
	void setRight(TreeNode *right) { m_right = right; }
	Block *data() const { return m_data; }
	void setData(Block *data) { m_data = data; }
	/*uint64_t value() const { return m_value; }
	void setValue(uint64_t value) { m_value = value; }*/
	
	uint32_t headID() const { return m_headID; }
	void setHeadID(uint32_t headID) { m_headID = headID; }
};

#endif
