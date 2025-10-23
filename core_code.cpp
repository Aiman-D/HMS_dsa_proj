#include <iostream>
#include <string>
#include <cctype>
#include <cmath>
using namespace std;

#define MAX_PATIENTS 100
#define MAX_HEAP 100
#define MAX_HASH 10007
#define MAX_NODES 20
#define MAX_EDGES 100
#define MAX_QUEUE 100
#define MAX_STACK 100
#define MAX_EXPRESSION_LENGTH 100

// ----------------------------------- Patient Info -----------------------------------
struct PatientInfo {
    int age;
    string name;
};

// ----------------------------------- AVL Tree -----------------------------------
struct AVLNode {
    int patientID;
    PatientInfo info;
    int height;
    AVLNode* left;
    AVLNode* right;
    AVLNode(int id, PatientInfo pf) : patientID(id), info(pf), height(1), left(NULL), right(NULL) {}
};
int getHeight(AVLNode* node) { return node ? node->height : 0; }
int getBalance(AVLNode* node) { return node ? getHeight(node->left) - getHeight(node->right) : 0; }
AVLNode* rightRotate(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;
    x->right = y; y->left = T2;
    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
    return x;
}
AVLNode* leftRotate(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;
    y->left = x; x->right = T2;
    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
    return y;
}
AVLNode* insertAVL(AVLNode* node, int id, PatientInfo info) {
    if (!node) return new AVLNode(id, info);
    if (id < node->patientID) node->left = insertAVL(node->left, id, info);
    else if (id > node->patientID) node->right = insertAVL(node->right, id, info);
    else return node;
    node->height = max(getHeight(node->left), getHeight(node->right)) + 1;
    int balance = getBalance(node);
    if (balance > 1 && id < node->left->patientID) return rightRotate(node);
    if (balance < -1 && id > node->right->patientID) return leftRotate(node);
    if (balance > 1 && id > node->left->patientID) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }
    if (balance < -1 && id < node->right->patientID) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }
    return node;
}
PatientInfo* searchAVL(AVLNode* node, int id) {
    if (!node) return NULL;
    if (node->patientID == id) return &(node->info);
    else if (id < node->patientID) return searchAVL(node->left, id);
    else return searchAVL(node->right, id);
}

// ----------------------------------- MinHeap -----------------------------------
struct EmergencyPatient {
    int triageScore;
    string name;
};
struct MinHeap {
    EmergencyPatient arr[MAX_HEAP];
    int size;
    MinHeap() : size(0) {}
    void insert(EmergencyPatient ep) {
        int i = size++;
        arr[i] = ep;
        while (i != 0 && arr[(i-1)/2].triageScore > arr[i].triageScore) {
            swap(arr[i], arr[(i-1)/2]);
            i = (i - 1) / 2;
        }
    }
    EmergencyPatient extractMin() {
        if (size == 0) return {999999, ""};
        EmergencyPatient minVal = arr[0];
        arr[0] = arr[--size];
        int i = 0;
        while (true) {
            int left = 2*i + 1;
            int right = 2*i + 2;
            int smallest = i;
            if (left < size && arr[left].triageScore < arr[smallest].triageScore) smallest = left;
            if (right < size && arr[right].triageScore < arr[smallest].triageScore) smallest = right;
            if (smallest != i) {
                swap(arr[i], arr[smallest]);
                i = smallest;
            } else break;
        }
        return minVal;
    }
};

// ----------------------------------- Queue -----------------------------------
struct Queue {
    EmergencyPatient arr[MAX_QUEUE];
    int front, rear;
    Queue() : front(0), rear(0) {}
    bool isEmpty() { return front == rear; }
    bool isFull() { return rear == MAX_QUEUE; }
    void enqueue(EmergencyPatient ep) {
        if (isFull()) { cout << "Queue full\n"; return; }
        arr[rear++] = ep;
    }
    EmergencyPatient dequeue() {
        if (isEmpty()) return {999999, ""};
        return arr[front++];
    }
};

// ----------------------------------- Stack -----------------------------------
template<typename T>
struct Stack {
    T arr[MAX_STACK];
    int top;
    Stack() : top(-1) {}
    bool isEmpty() { return top == -1; }
    bool isFull() { return top == MAX_STACK - 1; }
    void push(T x) {
        if (isFull()) { cout << "Stack overflow\n"; return; }
        arr[++top] = x;
    }
    T pop() {
        if (isEmpty()) return T();  // Return default-constructed T if empty
        return arr[top--];
    }
    T peek() {
        if (isEmpty()) return T();
        return arr[top];
    }
};

// ----------------------------------- Linked Lists -----------------------------------
// Singly Linked List for Patient Visit History
struct Visit {
    string visitDate;
    string notes;
    Visit* next;
    Visit(string date, string note) : visitDate(date), notes(note), next(NULL) {}
};
struct SinglyLinkedList {
    Visit* head;
    SinglyLinkedList() : head(NULL) {}
    void addVisit(string date, string note) {
        Visit* v = new Visit(date, note);
        if (!head) head = v;
        else {
            Visit* cur = head;
            while (cur->next) cur = cur->next;
            cur->next = v;
        }
    }
    void printVisits() {
        Visit* cur = head;
        while (cur) {
            cout << cur->visitDate << ": " << cur->notes << endl;
            cur = cur->next;
        }
    }
};

// Doubly Linked List for Navigation History
struct NavNode {
    string page;
    NavNode* prev;
    NavNode* next;
    NavNode(string p) : page(p), prev(NULL), next(NULL) {}
};
struct DoublyLinkedList {
    NavNode* head;
    NavNode* tail;
    DoublyLinkedList() : head(NULL), tail(NULL) {}
    void visitPage(string page) {
        NavNode* node = new NavNode(page);
        if (!head) head = tail = node;
        else {
            tail->next = node;
            node->prev = tail;
            tail = node;
        }
    }
    void printHistory() {
        NavNode* cur = head;
        while (cur) {
            cout << cur->page << " ";
            cur = cur->next;
        }
        cout << endl;
    }
};

// Circular Linked List for Duty Roster
struct StaffNode {
    string name;
    StaffNode* next;
    StaffNode(string n) : name(n), next(NULL) {}
};
struct CircularLinkedList {
    StaffNode* tail;
    CircularLinkedList() : tail(NULL) {}
    void addStaff(string name) {
        StaffNode* node = new StaffNode(name);
        if (!tail) {
            tail = node;
            tail->next = tail;
        } else {
            node->next = tail->next;
            tail->next = node;
            tail = node;
        }
    }
    void printRoster(int num) {
        if (!tail) return;
        StaffNode* cur = tail->next;
        for (int i = 0; i < num; i++) {
            cout << cur->name << " ";
            cur = cur->next;
        }
        cout << endl;
    }
};

// ----------------------------------- Hash Table for UHID -----------------------------------
struct HashEntry {
    int UHID;
    PatientInfo info;
    HashEntry* next;
    HashEntry(int u, PatientInfo pf) : UHID(u), info(pf), next(NULL) {}
};
struct HashTable {
    HashEntry* buckets[MAX_HASH];
    HashTable() { for (int i = 0; i < MAX_HASH; i++) buckets[i] = NULL; }
    int hashFunction(int uhid) { return uhid % MAX_HASH; }
    void insert(int uhid, PatientInfo info) {
        int idx = hashFunction(uhid);
        HashEntry* entry = new HashEntry(uhid, info);
        entry->next = buckets[idx];
        buckets[idx] = entry;
    }
    PatientInfo* find(int uhid) {
        int idx = hashFunction(uhid);
        HashEntry* curr = buckets[idx];
        while (curr) {
            if (curr->UHID == uhid) return &(curr->info);
            curr = curr->next;
        }
        return NULL;
    }
};

// ----------------------------------- Array-Based Graph & Dijkstra -----------------------------------
struct Edge { int to, weight; };
struct Graph {
    Edge adj[MAX_NODES][MAX_EDGES];
    int edgeCount[MAX_NODES];
    int n;
    Graph(int nodes) : n(nodes) { for (int i = 0; i < n; i++) edgeCount[i] = 0; }
    void addEdge(int u, int v, int w) {
        adj[u][edgeCount[u]++] = {v, w};
    }
};
void dijkstra(Graph& g, int src, int dist[]) {
    bool visited[MAX_NODES] = {false};
    for (int i = 0; i < g.n; i++) dist[i] = 999999;
    dist[src] = 0;
    for (int i = 0; i < g.n; i++) {
        int u = -1, minDist = 999999;
        for (int j = 0; j < g.n; j++) {
            if (!visited[j] && dist[j] < minDist) {
                u = j;
                minDist = dist[j];
            }
        }
        if (u == -1) break;
        visited[u] = true;
        for (int k = 0; k < g.edgeCount[u]; k++) {
            int v = g.adj[u][k].to;
            int w = g.adj[u][k].weight;
            if (!visited[v] && dist[u] + w < dist[v]) dist[v] = dist[u] + w;
        }
    }
}

// ----------------------------------- Expression Tree for Billing -----------------------------------
struct ExpNode {
    char op;           // operator + - * / or '\0' for numbers
    double value;      // valid if op == '\0'
    ExpNode* left;
    ExpNode* right;
    ExpNode(char oper) : op(oper), value(0), left(NULL), right(NULL) {}
    ExpNode(double val) : op('\0'), value(val), left(NULL), right(NULL) {}
};
// Evaluate expression tree recursively
double evaluate(ExpNode* root) {
    if (root == NULL) return 0;
    if (root->op == '\0') return root->value;
    double leftVal = evaluate(root->left);
    double rightVal = evaluate(root->right);
    switch (root->op) {
        case '+': return leftVal + rightVal;
        case '-': return leftVal - rightVal;
        case '*': return leftVal * rightVal;
        case '/': return rightVal != 0 ? leftVal / rightVal : 0;
    }
    return 0;
}
// Build sample expression tree: (3 + 5) * 2
ExpNode* buildSampleExpression() {
    ExpNode* root = new ExpNode('*');
    root->left = new ExpNode('+');
    root->left->left = new ExpNode(3.0);
    root->left->right = new ExpNode(5.0);
    root->right = new ExpNode(2.0);
    return root;
}

// ----------------------------------- Sorting Algorithms -----------------------------------
// Insertion Sort
void insertionSort(int arr[], int n) {
    for (int i = 1; i < n; i++) {
        int key = arr[i], j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j+1] = arr[j];
            j--;
        }
        arr[j+1] = key;
    }
}
// Merge Sort helpers
void merge(int arr[], int l, int m, int r) {
    int n1 = m - l + 1, n2 = r - m;
    int L[n1], R[n2];
    for (int i = 0; i < n1; i++) L[i] = arr[l+i];
    for (int i = 0; i < n2; i++) R[i] = arr[m+1+i];
    int i=0, j=0, k=l;
    while(i < n1 && j < n2) {
        if (L[i] <= R[j]) arr[k++] = L[i++];
        else arr[k++] = R[j++];
    }
    while(i < n1) arr[k++] = L[i++];
    while(j < n2) arr[k++] = R[j++];
}
void mergeSort(int arr[], int l, int r) {
    if(l < r) {
        int m = l + (r-l)/2;
        mergeSort(arr, l, m);
        mergeSort(arr, m+1, r);
        merge(arr,l,m,r);
    }
}
// Quick Sort helpers
int partition(int arr[], int low, int high) {
    int pivot = arr[high], i = low - 1;
    for(int j = low; j < high; j++) {
        if(arr[j] <= pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i+1], arr[high]);
    return i+1;
}
void quickSort(int arr[], int low, int high) {
    if(low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi-1);
        quickSort(arr, pi+1, high);
    }
}
// Heap Sort helpers
void heapify(int arr[], int n, int i) {
    int largest = i, left = 2*i + 1, right = 2*i + 2;
    if(left < n && arr[left] > arr[largest]) largest = left;
    if(right < n && arr[right] > arr[largest]) largest = right;
    if(largest != i) {
        swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}
void heapSort(int arr[], int n) {
    for(int i = n/2 - 1; i >= 0; i--)
        heapify(arr, n, i);
    for(int i = n-1; i >= 0; i--) {
        swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
}

// ----------------------------------- Main Demonstration -----------------------------------
int main() {
    AVLNode* patientTree = NULL;
    MinHeap heap;
    Queue q;
    CircularLinkedList dutyRoster;
    HashTable ht;
    Graph g(3);
    int dist[3];
    ExpNode* billingFormula = buildSampleExpression();

    // Preload some sample data
    dutyRoster.addStaff("Dr. Smith");
    dutyRoster.addStaff("Nurse Lee");
    dutyRoster.addStaff("Dr. Johnson");
    g.addEdge(0, 1, 10); g.addEdge(1, 2, 5); g.addEdge(0, 2, 20);

    int choice;
    while (true) {
        cout << "\n============================\n";
        cout << " HOSPITAL DSA PROJECT MENU\n";
        cout << "============================\n";
        cout << "1. Add Patient (AVL Tree)\n";
        cout << "2. Search Patient (AVL Tree)\n";
        cout << "3. Add Emergency Case (MinHeap)\n";
        cout << "4. Extract Next Emergency Case\n";
        cout << "5. Enqueue Patient (Queue)\n";
        cout << "6. Dequeue Patient (Queue)\n";
        cout << "7. Show Duty Roster (Circular Linked List)\n";
        cout << "8. Add UHID Patient (Hash Table)\n";
        cout << "9. Search UHID Patient (Hash Table)\n";
        cout << "10. Find Shortest Path (Graph Dijkstra)\n";
        cout << "11. Evaluate Billing Formula (Expression Tree)\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 0) {
            cout << "Exiting program. Goodbye!\n";
            break;
        }

        switch (choice) {
            case 1: {
                int id, age;
                string name;
                cout << "Enter Patient ID, Age, Name: ";
                cin >> id >> age >> name;
                patientTree = insertAVL(patientTree, id, {age, name});
                cout << "Patient inserted successfully.\n";
                break;
            }
            case 2: {
                int id;
                cout << "Enter Patient ID to search: ";
                cin >> id;
                PatientInfo* p = searchAVL(patientTree, id);
                if (p) cout << "Found: " << p->name << " (Age " << p->age << ")\n";
                else cout << "Patient not found.\n";
                break;
            }
            case 3: {
                EmergencyPatient ep;
                cout << "Enter Emergency Case Name and Triage Score: ";
                cin >> ep.name >> ep.triageScore;
                heap.insert(ep);
                cout << "Emergency case added.\n";
                break;
            }
            case 4: {
                EmergencyPatient ep = heap.extractMin();
                if (ep.name != "") cout << "Next emergency case: " << ep.name << "\n";
                else cout << "No emergency cases pending.\n";
                break;
            }
            case 5: {
                EmergencyPatient ep;
                cout << "Enter Patient Name and Triage Score: ";
                cin >> ep.name >> ep.triageScore;
                q.enqueue(ep);
                cout << "Patient added to queue.\n";
                break;
            }
            case 6: {
                EmergencyPatient ep = q.dequeue();
                if (ep.name != "") cout << "Dequeued: " << ep.name << "\n";
                else cout << "Queue is empty.\n";
                break;
            }
            case 7: {
                cout << "Enter number of staff to display in rotation: ";
                int num; cin >> num;
                dutyRoster.printRoster(num);
                break;
            }
            case 8: {
                int uhid, age;
                string name;
                cout << "Enter UHID, Age, Name: ";
                cin >> uhid >> age >> name;
                ht.insert(uhid, {age, name});
                cout << "Patient inserted in Hash Table.\n";
                break;
            }
            case 9: {
                int uhid;
                cout << "Enter UHID to search: ";
                cin >> uhid;
                PatientInfo* p = ht.find(uhid);
                if (p) cout << "Found UHID: " << p->name << " (Age " << p->age << ")\n";
                else cout << "UHID not found.\n";
                break;
            }
            case 10: {
                dijkstra(g, 0, dist);
                cout << "Shortest path from Reception to ICU: " << dist[2] << "\n";
                break;
            }
            case 11: {
                cout << "Billing formula result: " << evaluate(billingFormula) << "\n";
                break;
            }
            default:
                cout << "Invalid choice. Try again.\n";
        }
    }
    return 0;
}

