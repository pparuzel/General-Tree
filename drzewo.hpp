#include <iostream>
#include <vector>

/*

template <typename T>
class Drzewo
{
	struct Node;
	class Iterator;
public:
	typedef T value_type;
	typedef Iterator iterator;
	typedef std::size_t size_type;

	Node* _root;
	int _size;

	Drzewo();
	Drzewo(const T& );
	Drzewo(const Drzewo& );
	~Drzewo();
	Drzewo& operator=(const Drzewo& );
	Iterator insert(const T& , Iterator , std::size_t );
	Iterator root();
	Iterator begin();
	Iterator end();
	Iterator getChild(Iterator , std::size_t );
	friend int getNumberOfChildren(Iterator );
	void erase(Iterator );
	void clear(Iterator );
	bool empty();
	std::size_t size();
	void print(Iterator );
};

 */
/**
 *
 */
template<typename T>
class Drzewo
{
	struct Node
	{
		T value;
		Node* parent;
		std::vector<Node*> children;

		Node(T val = T(), Node* par = nullptr) : value(val), parent(par)
		{}
	};

	class Iterator
	{
		friend class Drzewo;

		Node* ptr;
	public:
		Iterator(Node* n = nullptr) : ptr(n)
		{}

		T& operator*()
		{
			return ptr->value;
		}

		Iterator& operator++()
		{
			std::vector<Node*>& vec = ptr->parent->children;
			unsigned long vecsize = ptr->parent != nullptr ? vec.size() : 0;
			unsigned long index;
			for(index = 0; index < vecsize; index++)
			{
				if(vec[index] == ptr)
				{
					index++;
					break;
				}
			}
			if(index < vecsize)
			{
				ptr = ptr->parent->children[index];
				while(!ptr->children.empty())
				{
					ptr = ptr->children[0];
				}
			}
			else
			{
				ptr = ptr->parent;
			}
			return *this;
		}

		T* operator->()
		{
			return &ptr->value;
		}

		Iterator& operator++(int)
		{
			return this->operator++();
		}

		bool operator==(Iterator it) const
		{
			return this->ptr == it.ptr;
		}

		bool operator!=(Iterator it) const
		{
			return this->ptr != it.ptr;
		}
	};
	void delete_node(Node* current)
	{
		for (Node* n : current->children)
		{
			delete_node(n);
		}
		current->children.clear();
		delete current;
		--_size;
	}
	void copy_node(T val, Node* other, Node* parent)
	{
		Node* temp = new Node(val, parent);
		parent->children.push_back(temp);
		for (Node* n : other->children)
		{
			copy_node(n->value, n, temp);
		}
	}

public:
	typedef T value_type;
	typedef Iterator iterator;
	typedef std::size_t size_type;
	Node* _root;
	std::size_t _size;

	Drzewo() : _root(nullptr), _size(0)
	{}

	Drzewo(const T& value) : _root(new Node(value)), _size(1)
	{}

	Drzewo(const Drzewo& tree) : _root(nullptr), _size(tree._size)
	{
		if(tree._root == nullptr) return;
		this->_root = new Node(tree._root->value, nullptr);
		for(Node* n : tree._root->children)
		{
			copy_node(n->value, n, this->_root);
		}
	}

	~Drzewo()
	{
		clear();
		if(_size != 0) std::cerr << "TREE ALLOCATION ERROR";
	}

	Drzewo& operator=(const Drzewo& tree)
	{
		this->clear();
		if(tree._root == nullptr) return *this;
		this->_size = tree._size;
		this->_root = new Node(tree._root->value, nullptr);
		for(Node* n : tree._root->children)
		{
			copy_node(n->value, n, this->_root);
		}
	}

	/**
	 * Operacja wstawiajaca element do drzewa. Jesli podany
	 * wskaznik na rodzica jest nullptr, a indeks jest rowny 0,
	 * element zostanie utworzony w korzeniu (jesli to mozliwe)
	 *
	 * @param  value  element dodawany do drzewa
	 * @param  parent iterator wskazujacy na rodzica
	 * @param  index  indeks w tablicy dzieci gdzie ma byc dodany element
	 * @return		iterator wskazujacy na nowo utworzony element
	 */
	Iterator insert(const T& value, Iterator it_parent, std::size_t index)
	{
		_size++;
		if (it_parent.ptr == nullptr && index == 0 && _root == nullptr)
		{
			_root = new Node(value);
			return Iterator(_root);
		}
		Node* node = new Node(value, it_parent.ptr);
		if (index >= it_parent.ptr->children.size())
		{
			it_parent.ptr->children.push_back(node);
			return Iterator(node);
		}
		std::vector<Node*>* children = &(it_parent.ptr->children);
		children->insert(children->begin() + index, node);
		return Iterator(node);
	}

	/**
	 * [root description]
	 * @param  _root [description]
	 * @return	   [description]
	 */
	Iterator root()
	{
		return Iterator(_root);
	}

	/**
	 * [begin description]
	 * @return [description]
	 */
	Iterator begin() const
	{
		Node* temp = _root;
		if(_size <= 1)
		{
			return Iterator(_root);
		}
		while(!temp->children.empty())
		{
			temp = temp->children[0];
		}
		return Iterator(temp);
	}

	/**
	 * [end description]
	 * @return [description]
	 */
	Iterator end() const
	{
		return Iterator(nullptr);
	}

	/**
	 * [getChild description]
	 * @param  parent [description]
	 * @param  index  [description]
	 * @return		[description]
	 */
	Iterator getChild(Iterator parent, std::size_t index)
	{
		return Iterator(parent.ptr->children[index]);
	}

	/**
	 * [getNumberOfChildren description]
	 * @param  parent [description]
	 * @return		[description]
	 */
	int getNumberOfChildren(Iterator parent)
	{
		return (int) parent.ptr->children.size();
	}

	void erase(Iterator iter)
	{
		if (iter.ptr == _root && _root != nullptr)
		{
			delete_node(iter.ptr);
			_root = nullptr;
			return;
		}
		std::vector<Node*>& parent_children = iter.ptr->parent->children;
		int pos = 0;
		for(Node* p : parent_children)
		{
			if(p == iter.ptr) break;
			pos++;
		}
		parent_children.erase(parent_children.begin()+pos);
		delete_node(iter.ptr);
	}

	void clear()
	{
		if (_root != nullptr)
			delete_node(_root);
		_root = nullptr;
	}

	bool empty()
	{
		return _root == nullptr;
	}

	std::size_t size()
	{
		return _size;
	}
};
