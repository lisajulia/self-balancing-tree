#include <iostream>

using namespace std;

template<class T>
struct Node {
	Node(T value) {
		this->value = value;
	}
	void print() {
		if (child[0])
			child[0]->print();
		cout << value << " ";
		if (child[1])
			child[1]->print();
	}

	void print(int depth) {
		if (child[0])
			child[0]->print(depth+1);
		cout << "D:" << depth << "->(L:" << level << ", V:" << value << ")   ";
		if (child[1])
			child[1]->print(depth+1);
	}

	Node<T>* insert(Node<T>* otherNode) {
		int index = (this->value < otherNode->value) ? 1 : 0;
		if (this->child[index]) {
			this->child[index] = this->child[index]->insert(otherNode);
		} else {
			this->child[index] = otherNode;
		}
		return rightRotate(leftRotate(this));
	}

	Node<T>* leftRotate(Node<T>* node) {
		// The left child of the input node may not have the same level as the input node
		if (!node or !node->child[0] or (node->child[0]->level != node->level)) { 
			return node;
		} else {
			auto leftChild = node->child[0];
			node->child[0] = leftChild->child[1];
			leftChild->child[1] = node;
			return leftChild;
		}
	}

	Node<T>* rightRotate(Node<T>* node) {
		// The second right child of the input node may not have the same level as the input node
		if (!node or !node->child[1] or !node->child[1]->child[1] or (node->child[1]->child[1]->level != node->level)) {
			return node;
		} else {
			auto rightChild = node->child[1];
			node->child[1] = rightChild->child[0];
			rightChild->child[0] = node;
			rightChild->level = rightChild->level + 1;
			return rightChild;
		}
	}

	bool find(T value) {
		if (this->value == value)
			return true;
		int index = (this->value < value) ? 1 : 0;
		if (child[index])
			return child[index]->find(value);
		cout << value << " is not in the tree." << endl;
		return false;
	}
	int depth() {
		if (!child[0] and !child[1])
			return 1;
		if (!child[0] and child[1])
			return child[1]->depth() + 1;
		if (child[0] and !child[1])
			return child[0]->depth() + 1;
		int leftDepth = child[0]->depth();
		int rightDepth = child[1]->depth();
		if (abs(leftDepth - rightDepth) > 2) { // We allow an imbalance of 2
			cerr << "The node with " << value << " has unbalanced subtrees, the left one is " << leftDepth << " deep and the right one is " << rightDepth << " deep." << endl;
		}
		return max(leftDepth, rightDepth) + 1;
	}

	~Node() {
		if (child[0])
			delete (child[0]);
		if (child[1])
			delete (child[1]);
	}

	int level = 1;
	Node<T>* child[2] = {nullptr,nullptr};
	T value;
};

int main(int argc, char** argv) {
	cout << "Received " << (argc - 1) << " arguments..." << endl;
	if (argc > 1) {
		auto rootNode = new Node<double>(stod(argv[1]));
		for (int i = 2; i < argc; i++) {
			auto newNode = new Node<double>(stod(argv[i]));
			rootNode = rootNode->insert(newNode);
			rootNode->print(0);
			cout << endl;
		}
		cout << "Overall depth " << rootNode->depth() << endl;
		rootNode->find(4);
		rootNode->find(770);
		rootNode->find(0);
		delete rootNode;
	} else {
		cout << "Call this function with at least one arguments!" << endl;
	}
}
