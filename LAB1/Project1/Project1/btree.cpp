#include <iostream>

class binary {
protected:
	class node {
	protected:
		int data;
		node* left;
		node* right;
	private:
	public:
		node() : data(0), left(nullptr), right(nullptr) {};
	};
	node* root;
public:
	binary() : root(new node()) {};

	void insert() {
		insert1(root);
	};
	void insert1(node* ptr);
};
void binary::insert1(node* ptr) {
	int j;
}

int main(void) {
	std::cout << "Hello World";
	return -1;
}