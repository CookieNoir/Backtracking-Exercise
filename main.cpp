#include <iostream>
#include <fstream>
#include <string>
#include <list>

using namespace std;

struct item {
	string name;
	int weight;
	int value;
	float utility;
	item() {}
	item(string n, int w, int v) {
		name = n;
		weight = w;
		value = v;
		utility = value * 1.0 / weight;
	}
	bool operator<(const item second) {
		return utility < second.utility;
	}
	bool operator!=(const item second) {
		return !(name == second.name && weight == second.weight && value == second.value);
	}
	void show() {
		cout << name << "   weight: " << weight << "   value: " << value << "   utility: " << utility << endl;
	}
};

class ItemList {
public:
	struct node {
		item data;
		node* next;
		node(item d) {
			data = d;
			next = nullptr;
		}
	};
	node* begin;
	node* end;
private:
	int totalWeight;
	int totalValue;

public:
	ItemList() {
		begin = nullptr;
		end = nullptr;
		totalWeight = 0;
		totalValue = 0;
	}

	void add(item newItem) {
		node* p = new node(newItem);
		totalWeight += p->data.weight;
		totalValue += p->data.value;
		if (!begin) begin = p;
		else end->next = p;
		end = p;
	}

	void changeWV(int W, int V) {
		totalWeight += W;
		totalValue += V;
	}

	void deleteLast() {
		if (!begin) return;
		else {
			if (begin == end) {
				totalWeight = 0;
				totalValue = 0;
				delete begin;
				begin = nullptr;
				end = nullptr;
			}
			else {
				node* p = begin;
				while (p->next->next) p = p->next;
				totalWeight -= end->data.weight;
				totalValue -= end->data.value;
				delete end;
				p->next = nullptr;
				end = p;
			}
		}
	}

	void deleteAllAfterNodeWithData(item target) {
		node* p = begin;
		while (p&&p->data != target) p = p->next;
		if (!p) return;
		node* q = p;
		while (q->next != end) {
			p = q->next;
			q->next = p->next;
			totalWeight -= p->data.weight;
			totalValue -= p->data.value;
			delete p;
			p = nullptr;
		}
		p = q->next;
		end = q;
		totalWeight -= p->data.weight;
		totalValue -= p->data.value;
		delete p;
		p = nullptr;
	}

	void clear() {
		while (end) {
			node* p = begin;
			if (begin == end) end = nullptr;
				else begin = p->next;
			totalWeight -= p->data.weight;
			totalValue -= p->data.value;
			delete p;
			p = nullptr;
		}
		begin = nullptr;
	}

	int getWeight() {
		return totalWeight;
	}
	int getValue() {
		return totalValue;
	}

	void operator=(const ItemList &from) {
		clear();
		node* p = from.begin;
		while (p) {
			add(p->data);
			p = p->next;
		}
	}

	void show() {
		cout << "	Item List\n";
		node* p = begin;
		while (p) {
			p->data.show();
			p = p->next;
		}
		cout << "Total Weight: " << totalWeight << "   Total Value: " << totalValue << endl;
	}

	ItemList(const ItemList& item) {}

	~ItemList() {
		clear();
		begin = nullptr;
		end = nullptr;
	}
};

void showList(list<item> shownList) {
	cout << "	List\n";
	list<item>::iterator it;
	for (it = shownList.begin(); it != shownList.end(); ++it)
		(*it).show();
}

void collectList(list<item> listh, ItemList &itemList) {
	list<item>::iterator it;
	for (it = listh.begin(); it != listh.end(); ++it)
	{
		itemList.add(*it);
	}
	cout << "Fine!\n";
}
//what?
void solve(ItemList::node* start, ItemList& curList, ItemList& bestList, int maxWeight) {
	ItemList::node* p = start;
	while (p) {
		if (p->data.weight + curList.getWeight() <= maxWeight) {
			curList.add(p->data);
			solve(p->next, curList, bestList, maxWeight);
			curList.deleteLast();
		}
		p = p->next;
	}
	if (!p) {
		if (curList.getValue() > bestList.getValue()) bestList = curList;
		return;
	}
}

int main() {
	list<item> itemCollection;
	setlocale(LC_ALL, "Russian");
	cout << "¬ведите им€ файла:\n";
	string name;
	cin >> name;
	ifstream file(name);
	while (!file) {
		cout << "‘айл не найден. ¬ведите им€ файла:\n";
		cin >> name;
		file.open(name);
	}
	int w, v;
	while (file >> name >> w >> v) {
		itemCollection.push_back(item(name, w, v));
		if (file.eof()) break;
	}
	file.close();
	itemCollection.sort();
	itemCollection.reverse();
	showList(itemCollection);

	cout << "--------------\n";

	cout << "¬ведите вместимость (максимальный вес):\n";
	cin >> w;
	ItemList itemList; //Laziness won, I've created another class for working with items after sorting
	collectList(itemCollection, itemList);
	ItemList curList;
	ItemList bestList;
	solve(itemList.begin,curList,bestList,w);
	bestList.show();
	system("pause");
	return 0;
}