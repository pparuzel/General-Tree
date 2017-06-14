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
	int getNumberOfChildren(Iterator );
	void erase(Iterator );
	void clear(Iterator );
	bool empty();
	std::size_t size();
	void print(Iterator );
};
 */

/*! \mainpage Drzewo Ogólne
 * \section intro Wstęp
 * Pojemnik o strukturze drzewiastej. Użyta struktura jest\n
 * drzewem ogólnym z jednym możliwym korzeniem.\n
 * Przechowywany jest wskaźnik na korzeń, a więc\n
 * jest możliwość braku korzenia.\n
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

	/**
	 * Klasa Iterator, pełniąca funkcję wskaźnika na elementy drzewa.\n
	 * Iterator jest jednokierunkowy.\n
	 * W drzewie iterator na początkowy węzeł jest wywoływany metodą\n
	 * begin(), a iterator wskazujący za ostatni element metodą end().\n
	 * Przejście po drzewie jest Post-Order.\n
	 */
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

		Iterator operator++(int)
		{
			Iterator temp = *this;
			this->operator++();
			return temp;
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

	/**
	 * Konstruktor domyślny tworzący drzewo\n
	 * bez korzenia o rozmiarze 0.\n
	 */
	Drzewo() : _root(nullptr), _size(0)
	{}

	/**
	 * Konstruktor z wartością typu T określonego szablonem.\n
	 * Tworzy drzewo o rozmiarze 1 i korzeniu o wartości\n
	 * podanej jako parametr.
	 */
	Drzewo(const T& value) : _root(new Node(value)), _size(1)
	{}

	/**
	 * Konstruktor kopiujący drzewo.
	 */
	Drzewo(const Drzewo& tree) : _root(nullptr), _size(tree._size)
	{
		if(tree._root == nullptr) return;
		this->_root = new Node(tree._root->value, nullptr);
		for(Node* n : tree._root->children)
		{
			copy_node(n->value, n, this->_root);
		}
	}

	/**
	 * Destruktor drzewa.\n
	 * Najpierw czyszczone jest drzewo.\n
	 * W przypadku wycieku wyświetlany jest błąd.\n
	 * "POSSIBLE MEMORY LEAK"
	 */
	~Drzewo()
	{
		this->clear();
	}


	/**
	 * Operator przypisania.\n
	 * W przypadku wycieku wyświetlany jest błąd.\n
	 * "POSSIBLE MEMORY LEAK"
	 */
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
	 * Operacja wstawiajaca element do drzewa. Jesli podany\n
	 * wskaznik na rodzica jest pusty, a indeks jest rowny 0,\n
	 * element zostanie utworzony w korzeniu (jeśli to mozliwe).\n
	 * Ponowna próba wstawienia na miejsce korzenia daje zachowanie niezdefiniowane.
	 *
	 * @param  value  	element dodawany do drzewa
	 * @param  parent 	iterator wskazujacy na rodzica
	 * @param  index  	indeks w tablicy dzieci gdzie ma byc dodany element
	 * @return			iterator wskazujacy na nowo utworzony element
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
	 * Iterator na korzeń drzewa.
	 * @return Iterator na korzeń drzewa
	 */
	Iterator root() const
	{
		return Iterator(_root);
	}

	/**
	 * Iterator na początek drzewa.
	 * @return Iterator na "left-most child"
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
	 * Iterator na koniec drzewa.\n
	 * Wskazuje za ostatnim elementem.
	 * @return Iterator za "root"
	 */
	Iterator end() const
	{
		return Iterator(nullptr);
	}

	/**
	 * Metoda zwracająca iterator na dziecko\n
	 * na podanej pozycji pod podanym rodzicem.
	 * @param  parent 	rodzic wskazywanego node'a
	 * @param  index  	pozycja danego dziecka (numerowane od 0)
	 * @return			iterator na dziecko
	 */
	Iterator getChild(Iterator parent, std::size_t index) const
	{
		return Iterator(parent.ptr->children[index]);
	}

	/**
	 * Metoda zwracająca liczbę dzieci danego wezła
	 * @param  parent 	iterator na rodzica
	 * @return			liczba całkowita ilości dzieci
	 */
	int getNumberOfChildren(Iterator parent) const
	{
		return (int) parent.ptr->children.size();
	}


	/**
	 * Operacja usuwająca węzeł wskazany przez iterator.\n
	 * Usuwane są również wszystkie dzieci i poddzieci.
	 * @param  iter iterator na usuwany węzeł
	 */
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

	/**
	 * Metoda rekursywnie czyszcząca drzewo.\n
	 * W przypadku wycieku wyświetlany jest błąd.\n
	 * "POSSIBLE MEMORY LEAK"
	 */
	void clear()
	{
		if (_root != nullptr)
			delete_node(_root);
		_root = nullptr;
		if(_size != 0) std::cerr << "POSSIBLE MEMORY LEAK";
	}

	/**
	 * Metoda zwracająca czy drzewo jest puste.
	 * @return boolean
	 */
	bool empty() const
	{
		return _root == nullptr;
	}

	/**
	 * Metoda zwracająca rozmiar drzewa.
	 * @return std::size_t
	 */
	std::size_t size() const
	{
		return _size;
	}
};
