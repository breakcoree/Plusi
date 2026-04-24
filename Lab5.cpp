#include <iostream>
#include <string>
#include <queue>
using namespace std;

// ============================================================
// Предметная область: Маршрутизация беспилотника (Вариант 1)
// Данные о пункте маршрута: код точки (ключ), координаты,
// высота, допустимая скорость, уровень риска.
// ============================================================

struct RoutePoint {
    int    code;       // код точки (ключ)
    double x, y;      // координаты
    double altitude;  // высота (м)
    double maxSpeed;  // допустимая скорость (км/ч)
    int    riskLevel; // уровень риска (1-5)
};

// ============================================================
// Узел бинарного дерева поиска
// ============================================================
struct BinaryTreeNode {
    RoutePoint data;
    BinaryTreeNode* left;
    BinaryTreeNode* right;
    BinaryTreeNode(RoutePoint data) {
        this->data  = data;
        this->left  = nullptr;
        this->right = nullptr;
    }
};

// ============================================================
// Класс бинарного дерева поиска
// ============================================================
class BinaryTree {
private:
    BinaryTreeNode* root = nullptr;

    // Вспомогательная: вывод одного пункта маршрута
    void printPoint(RoutePoint p) {
        cout << "Код=" << p.code
             << " Координаты=(" << p.x << "; " << p.y << ")"
             << " Высота=" << p.altitude << "м"
             << " МаксСкорость=" << p.maxSpeed << "км/ч"
             << " Риск=" << p.riskLevel << "\n";
    }

    // Вспомогательная: вставка узла
    static BinaryTreeNode* insert(BinaryTreeNode* node, RoutePoint data) {
        if (!node)
            return new BinaryTreeNode(data);
        if (data.code < node->data.code)
            node->left  = insert(node->left,  data);
        else if (data.code > node->data.code)
            node->right = insert(node->right, data);
        // если data.code == node->data.code, ничего не делаем (без дублей)
        return node;
    }

    // Вспомогательная: поиск минимального узла
    BinaryTreeNode* findMin(BinaryTreeNode* node) {
        while (node && node->left)
            node = node->left;
        return node;
    }

    // Вспомогательная: поиск максимального узла
    BinaryTreeNode* findMax(BinaryTreeNode* node) {
        while (node && node->right)
            node = node->right;
        return node;
    }

    // Вспомогательная: удаление узла
    BinaryTreeNode* remove(BinaryTreeNode* node, int code) {
        if (!node)
            return nullptr;
        if (code < node->data.code) {
            node->left  = remove(node->left,  code);
        } else if (code > node->data.code) {
            node->right = remove(node->right, code);
        } else {
            // Найден узел к удалению
            if (!node->left && !node->right) {
                // Лист
                delete node;
                return nullptr;
            } else if (!node->left) {
                // Один правый ребёнок
                BinaryTreeNode* r = node->right;
                delete node;
                return r;
            } else if (!node->right) {
                // Один левый ребёнок
                BinaryTreeNode* l = node->left;
                delete node;
                return l;
            } else {
                // Два ребёнка: берём inorder-следующего (минимум в правом поддереве)
                BinaryTreeNode* succ = findMin(node->right);
                node->data  = succ->data;                            // копируем значение
                node->right = remove(node->right, succ->data.code); // удаляем дубль в правом поддереве
            }
        }
        return node;
    }

    // Вспомогательная: поиск узла по коду
    BinaryTreeNode* search(BinaryTreeNode* node, int code) {
        if (!node || node->data.code == code)
            return node;
        if (code < node->data.code)
            return search(node->left,  code);
        return search(node->right, code);
    }

    // Вспомогательная: обход прямой порядок
    void preorder(BinaryTreeNode* BinaryTreeNode) {
        if (!BinaryTreeNode)
            return;
        printPoint(BinaryTreeNode->data);
        preorder(BinaryTreeNode->left);
        preorder(BinaryTreeNode->right);
    }

    // Вспомогательная: обход обратный порядок
    void postorder(BinaryTreeNode* node) {
        if (!node) return;
        postorder(node->left);
        postorder(node->right);
        printPoint(node->data);
    }

    // Вспомогательная: обход симметричный порядок
    void inorder(BinaryTreeNode* BinaryTreeNode) {
        if (!BinaryTreeNode) return;
        inorder(BinaryTreeNode->left);
        printPoint(BinaryTreeNode->data);
        inorder(BinaryTreeNode->right);
    }

    // Вспомогательная: обход в ширину
    void levelOrder(BinaryTreeNode* node) {
        if (!node) return;
        queue<BinaryTreeNode*> q;
        q.push(node);
        while (!q.empty()) {
            BinaryTreeNode* cur = q.front();
            q.pop();
            printPoint(cur->data);
            if (cur->left)  q.push(cur->left);
            if (cur->right) q.push(cur->right);
        }
    }

    // Вспомогательная: проверка равенства деревьев
    bool equals(BinaryTreeNode* a, BinaryTreeNode* b) {
        if (!a && !b) return true;
        if (!a || !b) return false;
        return (a->data.code == b->data.code) &&
               equals(a->left,  b->left)      &&
               equals(a->right, b->right);
    }

    // Вспомогательная: подсчёт вершин на уровне
    int countAtLevel(BinaryTreeNode* node, int level) {
        if (!node)      return 0;
        if (level == 0) return 1;
        return countAtLevel(node->left,  level - 1) +
               countAtLevel(node->right, level - 1);
    }

    // Вспомогательная: копирование дерева
    BinaryTreeNode* copy(BinaryTreeNode* node) {
        if (!node) return nullptr;
        BinaryTreeNode* newNode = new BinaryTreeNode(node->data);
        newNode->left  = copy(node->left);
        newNode->right = copy(node->right);
        return newNode;
    }

    // Вспомогательная: удаление всех узлов
    void clear(BinaryTreeNode* node) {
        if (!node) return;
        clear(node->left);
        clear(node->right);
        delete node;
    }

public:
    // Конструктор, инициализация пустого двоичного дерева
    BinaryTree() {
        this->root = nullptr;
    }

    // Деструктор — освобождение всех узлов
    ~BinaryTree() {
        clear(root);
    }

    // Добавление узла в дерево
    void insert(RoutePoint data) {
        root = insert(root, data);
    }

    // Удаляем узел по коду точки
    void remove(int code) {
        root = remove(root, code);
    }

    // Поиск узла по коду точки
    bool search(int code) {
        return search(root, code) != nullptr;
    }

    // Обход дерева в прямом порядке (в глубину)
    void preorder() {
        preorder(root);
    }

    // Обход дерева в обратном порядке
    void postorder() {
        postorder(root);
    }

    // Обход дерева в симметричном порядке
    void inorder() {
        inorder(root);
    }

    // Обход дерева в ширину
    void levelOrder() {
        levelOrder(root);
    }

    // Нахождение наименьшего элемента
    void printMin() {
        BinaryTreeNode* node = findMin(root);
        if (node) printPoint(node->data);
        else cout << "Дерево пусто\n";
    }

    // Нахождение наибольшего элемента
    void printMax() {
        BinaryTreeNode* node = findMax(root);
        if (node) printPoint(node->data);
        else cout << "Дерево пусто\n";
    }

    // Проверка двух деревьев на равенство
    bool equals(BinaryTree& other) {
        return equals(this->root, other.root);
    }

    // Подсчёт числа вершин на заданном уровне (корень = уровень 0)
    int countAtLevel(int level) {
        return countAtLevel(root, level);
    }

    // Копирование дерева
    BinaryTree* copy() {
        BinaryTree* newTree = new BinaryTree();
        newTree->root = copy(root);
        return newTree;
    }
};

// ============================================================
// Пример использования
// ============================================================
int main() {
    BinaryTree tree;

    // Вариант 1: Маршрутизация беспилотника
    // Вершины: пункты маршрута. Ключ: код точки.
    // Данные: координаты, высота, допустимая скорость, уровень риска.

    // Добавление пунктов маршрута
    tree.insert({50,  55.7,  37.6,  120.0, 60.0, 2});
    tree.insert({30,  55.5,  37.4,  100.0, 50.0, 1});
    tree.insert({70,  55.9,  37.8,  150.0, 80.0, 3});
    tree.insert({20,  55.3,  37.2,   80.0, 40.0, 1});
    tree.insert({40,  55.6,  37.5,  110.0, 55.0, 2});
    tree.insert({60,  55.8,  37.7,  130.0, 70.0, 4});
    tree.insert({80,  56.0,  38.0,  200.0, 90.0, 5});
    tree.insert({10,  55.1,  37.1,   60.0, 30.0, 1});
    tree.insert({35,  55.55, 37.45, 105.0, 52.0, 2});

    // Обход в прямом порядке (в глубину)
    cout << "Прямой порядок (preorder):\n";
    tree.preorder();

    // Обход в обратном порядке
    cout << "\nОбратный порядок (postorder):\n";
    tree.postorder();

    // Обход в симметричном порядке
    cout << "\nСимметричный порядок (inorder):\n";
    tree.inorder();

    // Обход в горизонтальном порядке (в ширину)
    cout << "\nГоризонтальный порядок (levelOrder):\n";
    tree.levelOrder();

    // Поиск элемента
    cout << "\nПоиск кода 40: "
         << (tree.search(40) ? "найден" : "не найден") << "\n";
    cout << "Поиск кода 99: "
         << (tree.search(99) ? "найден" : "не найден") << "\n";

    // Нахождение наибольшего и наименьшего элемента
    cout << "\nНаименьший элемент:\n";
    tree.printMin();
    cout << "Наибольший элемент:\n";
    tree.printMax();

    // Удаление элемента из дерева
    cout << "\nУдаление узла с кодом 30...\n";
    tree.remove(30);
    cout << "Симметричный порядок после удаления:\n";
    tree.inorder();

    // Проверка двух деревьев на равенство + копирование дерева
    BinaryTree* treeCopy = tree.copy();
    cout << "\nДеревья равны после копирования: "
         << (tree.equals(*treeCopy) ? "да" : "нет") << "\n";
    treeCopy->remove(10);
    cout << "Деревья равны после изменения копии: "
         << (tree.equals(*treeCopy) ? "да" : "нет") << "\n";

    // Подсчёт числа вершин на заданном уровне
    cout << "\nЧисло вершин на уровне 0: " << tree.countAtLevel(0) << "\n";
    cout << "Число вершин на уровне 1: " << tree.countAtLevel(1) << "\n";
    cout << "Число вершин на уровне 2: " << tree.countAtLevel(2) << "\n";
    cout << "Число вершин на уровне 3: " << tree.countAtLevel(3) << "\n";

    delete treeCopy;
    return 0;
}
