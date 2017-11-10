template <class T>
class Node {
  public:
    Node(T d) : data(d), next(NULL) {};
    Node(T d, Node<T>* n) : data(d), next(n) {};
    void link(Node* n) {
      next = n;
    }
    Node<T>* getNext() {
      return next;
    }
    T getData() {
      return data;
    }
  private:
    T data;
    Node<T>* next;
};

template <class T>
class Stack {
  public:
    Stack() : head(NULL) {};
    void push (Node<T>* n) {
      n->link(head);
      head = n;
    }
    Node<T>* peek() {
      return head;
    }
    Node<T>* pop() {
      Node<T>* temp = head;
      head = head->getNext();
      return temp;
    }
  private:
    Node<T>* head;
};

template <class T>
class dNode {
  public:
    dNode(T d) : data(d), next(NULL) {};
    dNode(T d, dNode<T>* p, dNode<T>* n) : data(d), prev(p), next(n) {};
    dNode<T>* getNext() {
      return next;
    }
    T getData() {
      return data;
    }
  private:
    T data;
    dNode<T>* next;
    dNode<T>* prev;  
};

template <class T>
class LinkedList {
    public:
      LinkedList() : s() {};
    private:
      Stack<T> s;
};
