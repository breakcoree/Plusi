#include <iostream> #include <string> #include <cstdlib> #include <ctime> using namespace std;

// Заявка (покупатель) struct Request { int id; int arrivalTime; string type; // normal, elderly, disabled int items; int priority; int serviceTime; };

// Узел однонаправленного списка (по шаблону) struct ListNode { Request data; ListNode* next;

ListNode(Request value) {
    this->data = value;
    this->next = nullptr;
}

};

class PriorityQueue { private: // указатель на начало списка ListNode* head;

public:

// конструктор
PriorityQueue() {
    this->head = nullptr;
}

// деструктор
~PriorityQueue() {
    ListNode* current = head;
    while (current != nullptr) {
        ListNode* toDelete = current;
        current = current->next;
        delete toDelete;
    }
    cout << "Очередь очищена" << endl;
}

bool empty() {
    return head == nullptr;
}

// вставка с приоритетом (enqueue)
void enqueue(Request value) {

    ListNode* newNode = new ListNode(value);

    if (head == nullptr || value.priority > head->data.priority) {
        newNode->next = head;
        head = newNode;
        return;
    }

    ListNode* temp = head;

    while (temp->next != nullptr && temp->next->data.priority >= value.priority) {
        temp = temp->next;
    }

    newNode->next = temp->next;
    temp->next = newNode;
}

// извлечение из очереди (dequeue)
Request dequeue() {

    if (head == nullptr) {
        throw runtime_error("Очередь пуста");
    }

    ListNode* temp = head;
    Request value = head->data;

    head = head->next;
    delete temp;

    return value;
}

// вывод очереди
void print() {

    if (head == nullptr) {
        cout << "Очередь пуста" << endl;
        return;
    }

    ListNode* temp = head;

    while (temp != nullptr) {
        cout << "ID:" << temp->data.id << "(" << temp->data.type << ")";

        if (temp->next)
            cout << " -> ";

        temp = temp->next;
    }

    cout << endl;
}

int length() {
    int count = 0;
    ListNode* temp = head;

    while (temp != nullptr) {
        count++;
        temp = temp->next;
    }

    return count;
}

};

// определение приоритета int getPriority(string type) { if (type == "disabled") return 3; if (type == "elderly") return 2; return 1; }

// случайный тип покупателя string randomType() {

int r = rand() % 3;

if (r == 0) return "normal";
if (r == 1) return "elderly";

return "disabled";

}

int main() {

srand(time(0));

PriorityQueue queue;

const int SIM_TIME = 100;

int nextId = 1;
int cashierFreeTime = 0;

double totalWait = 0;
double totalService = 0;

int served = 0;
int busyTime = 0;

for (int time = 0; time < SIM_TIME; time++) {

    // появление нового покупателя
    if (rand() % 3 == 0) {

        Request r;

        r.id = nextId++;
        r.arrivalTime = time;
        r.type = randomType();
        r.items = rand() % 20 + 1;

        r.priority = getPriority(r.type);
        r.serviceTime = r.items / 2 + 1;

        queue.enqueue(r);

        cout << "Новый покупатель: ID=" << r.id << " тип=" << r.type << " товары=" << r.items << endl;
    }

    // обслуживание
    if (time >= cashierFreeTime && !queue.empty()) {

        Request r = queue.dequeue();

        int wait = time - r.arrivalTime;

        totalWait += wait;
        totalService += r.serviceTime;

        served++;

        cashierFreeTime = time + r.serviceTime;

        busyTime += r.serviceTime;

        cout << "Обслуживается покупатель ID=" << r.id << " ожидал=" << wait << endl;
    }

    cout << "Очередь: ";
    queue.print();
}

cout << endl;

cout << "Обслужено покупателей: " << served << endl;

if (served > 0) {

    cout << "Среднее время ожидания: " << totalWait / served << endl;

    cout << "Среднее время обслуживания: " << totalService / served << endl;
}

cout << "Загрузка кассы (%): " << busyTime * 100.0 / SIM_TIME << endl;

return 0;

}
