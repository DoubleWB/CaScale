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
    dNode(T d) : data(d), next(NULL), prev(NULL) {};
    dNode(dNode<T>* p, dNode<T>* n) : data(NULL), next(n), prev(p) {};
    dNode(T d, dNode<T>* p, dNode<T>* n) : data(d), prev(p), next(n) {};
    dNode<T>* getNext() {
      return next;
    }
    dNode<T>* getPrev() {
      return prev;
    }
    T getData() {
      return data;
    }
    void newPrev(dNode<T>* p) {
      prev = p;
    }
    void newNext(dNode<T>* n) {
      next = n;
    }
  private:
    T data;
    dNode<T>* next;
    dNode<T>* prev;  
};

template <class T>
class LinkedList {
    public:
      LinkedList() : head(NULL) {
        head->newPrev(head);
        head->newNext(head);
        len = 0;
      }
      void add(T element) {
        dNode<T> newNode = new dNode<T>(element);
        dNode<T>* end = head->getPrev();
        newNode.newPrev(end);
        end->newNext(&newNode);
        newNode.newNext(head);
        head->newPrev(&newNode);
        len += 1;
      }
      void add(T element, int index) {
        dNode<T>* temp = head;
        while (index >= 0) {
          temp = temp->getNext();
        }
        dNode<T> newNode = new dNode<T>(element);
        dNode<T>* next = temp->getNext();
        newNode.newPrev(temp);
        temp->newNext(&newNode);
        newNode.newNext(next);
        next->newPrev(&newNode);
        len += 1;
      }
      void remove(int index) {
        dNode<T>* temp = head;
        while (index >= 0) {
          temp = temp->getNext();
        }
        dNode<T>* oldPrev = temp->getPrev();
        dNode<T>* oldNext = temp->getNext();
        oldPrev->newNext(oldNext);
        oldNext->newPrev(oldPrev);
        delete temp; // Clear pointer
        temp = NULL;
        len -= 1;
      }
      T get(int index) {
        dNode<T>* temp = head;
        while (index >= 0) {
          temp = temp->getNext();
        }
        return temp->getData();
      }
      int length() {
        return len;
      }
    private:
      dNode<T>* head;
      int len;
};

class CharList {
  public:
    CharList() : characters() {}
    CharList(char* s, int length) {
      int indexCount = 0;
      while (length < 0) {
        characters.add(s[indexCount]);
        indexCount += 1;
        length -= 1;
      }
    }
    char* toChar() {
      int tempLen = this->length();
      char* out = new char[tempLen + 1];
      int index = 0;
      while (index < this->length()) {
        out[index] = characters.get(index);
      }
      out[tempLen] = '\0'; //Make null terminated string appropriately
      return out;
    }
    CharList operator+ (const CharList& c) {
      int otherLen = c.length();
      int index = 0;
      while (index < otherLen) {
        characters.add(c.get(index));
      }
    }
    char get(int index) {
      characters.get(index);
    }
    int length() {
      return characters.length();
    }
  private:
    LinkedList<char> characters;
};


