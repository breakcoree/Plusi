#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

// структура заявки
struct Request {
    int id;
    int arrivalTime;
    string type;
    int items;
    int priority;
    int serviceTime;
};

// ===== УЗЕЛ СПИСКА (как в шаблоне) =====
struct ListNode {
    Request data;
    ListNode* next;

    ListNode(Request value){
        this->data = value;
        this->next = nullptr;
    }
};

// ===== ОДНОНАПРАВЛЕННЫЙ СПИСОК =====
class LinkedList {
private:
    // указатель на начало списка
    ListNode* head;

public:

    // конструктор
    LinkedList(){
        this->head = nullptr;
    }

    // деструктор
    ~LinkedList(){
        ListNode* current = head;
        while(current != nullptr){
            ListNode* toDelete = current;
            current = current->next;
            delete toDelete;
        }
        cout << "Список очищен" << endl;
    }

    // проверка пустоты
    bool empty(){
        return head == nullptr;
    }

    // ===== ВСТАВКА С ПРИОРИТЕТОМ (enqueue) =====
    void add(Request value){

        ListNode* newNode = new ListNode(value);

        if(head == nullptr || value.priority > head->data.priority){
            newNode->next = head;
            head = newNode;
            return;
        }

        ListNode* temp = head;

        while(temp->next != nullptr && temp->next->data.priority >= value.priority){
            temp = temp->next;
        }

        newNode->next = temp->next;
        temp->next = newNode;
    }

    // ===== ИЗВЛЕЧЕНИЕ ИЗ ОЧЕРЕДИ (dequeue) =====
    Request pop(){

        ListNode* temp = head;
        Request value = temp->data;

        head = head->next;
        delete temp;

        return value;
    }

    // ===== ВЫВОД СПИСКА =====
    void print(){

        if(head == nullptr){
            cout << "Очередь пуста" << endl;
            return;
        }

        ListNode* temp = head;

        while(temp != nullptr){
            cout << temp->data.id;

            if(temp->next)
                cout << " -> ";

            temp = temp->next;
        }

        cout << endl;
    }
};

// ===== ПРИОРИТЕТ =====
int getPriority(string type){

    if(type == "disabled") return 3;
    if(type == "elderly") return 2;

    return 1;
}

// ===== СЛУЧАЙНЫЙ ТИП =====
string randomType(){

    int r = rand() % 3;

    if(r == 0) return "normal";
    if(r == 1) return "elderly";

    return "disabled";
}

int main(){

    srand(time(0));

    LinkedList queue;

    const int SIM_TIME = 100;

    int nextId = 1;
    int cashierFreeTime = 0;

    double totalWait = 0;
    double totalService = 0;

    int served = 0;
    int busyTime = 0;

    for(int time = 0; time < SIM_TIME; time++){

        // появление покупателя
        if(rand() % 3 == 0){

            Request r;

            r.id = nextId++;
            r.arrivalTime = time;
            r.type = randomType();
            r.items = rand() % 20 + 1;

            r.priority = getPriority(r.type);
            r.serviceTime = r.items / 2 + 1;

            queue.add(r);

            cout << "Новый покупатель ID=" << r.id << endl;
        }

        // обслуживание
        if(time >= cashierFreeTime && !queue.empty()){

            Request r = queue.pop();

            int wait = time - r.arrivalTime;

            totalWait += wait;
            totalService += r.serviceTime;

            served++;

            cashierFreeTime = time + r.serviceTime;
            busyTime += r.serviceTime;

            cout << "Обслуживается ID=" << r.id << endl;
        }

        cout << "Очередь: ";
        queue.print();
    }

    cout << endl;

    cout << "Обслужено покупателей: " << served << endl;

    if(served > 0){
        cout << "Среднее ожидание: " << totalWait / served << endl;
        cout << "Среднее обслуживание: " << totalService / served << endl;
    }

    cout << "Загрузка кассы (%): " << busyTime * 100.0 / SIM_TIME << endl;

    return 0;
}
