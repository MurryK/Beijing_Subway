#include <iostream>
#include <fstream>
#include <string>
#include <assert.h>
#define numOfHead 19
#define MAX 10000
using namespace std;
string From;
string To;
string Order;
class station {
private:
	string name;
	string lineName;
	station * last;
	station * next;
	int crossNum = 0;
	station * crossStation[2];
	int distanceNum = MAX;
	int transferNum = MAX;
	string record;
public:
	station::station(string str1, string str2) {
		lineName = str1;
		name = str2;
		crossStation[0] = NULL;
		crossStation[1] = NULL;
	}
	void station::setLast(station * sta) {
		last = sta;
	}
	void station::setNext(station * sta) {
		next = sta;
	}
	station * station::getLast() {
		return last;
	}
	station * station::getNext() {
		return next;
	}
	string station::getName() {
		return name;
	}
	string station::getLineName() {
		return lineName;
	}
	void station::addCross(station * tmp) {
		crossStation[crossNum++] = tmp;
	}
	int station::getNumOfCross() {
		return crossNum;
	}
	station * station::getCross(int i) {
		return crossStation[i];
	}
	void station::setDistance(int num) {
		distanceNum = num;
	}
	void station::setTransfer(int num) {
		transferNum = num;
	}
	int station::getDistance() {
		return distanceNum;
	}
	int station::getTransfer() {
		return transferNum;
	}

}*Heads[numOfHead];

station * findStation(string name) {
	for (int i = 0; i < numOfHead; i++) {
		station * head = Heads[i];
		int mark = 0;
		while (head != NULL && ((head != Heads[i]) || mark == 0) && ((head->getName() != "三元桥" || head->getLineName() != "机场线") || mark == 1)) {
			mark++;
			if (head->getName() == name)
				return head;
			head = head->getNext();
		}
	}
	return NULL;
}

void markStationDis(station * Goal) {
	station * Last = Goal->getLast();
	station * Next = Goal->getNext();

	if (Last != NULL && Last->getDistance()>Goal->getDistance() + 1) {
		Last->setDistance(Goal->getDistance() + 1);
		Last->setTransfer(Goal->getTransfer());
		markStationDis(Last);
	}
	if (Next != NULL && Next->getDistance()>Goal->getDistance() + 1) {
		Next->setDistance(Goal->getDistance() + 1);
		Next->setTransfer(Goal->getTransfer());

		markStationDis(Next);
	}
	if (Goal->getNumOfCross() > 0) {
		station * Cross1 = Goal->getCross(0);
		station * Cross2 = Goal->getCross(1);
		if (Cross1->getDistance() > Goal->getDistance() || (Cross1->getDistance() == Goal->getDistance() && Cross1->getTransfer() > Goal->getTransfer() + 1)) {
			Cross1->setDistance(Goal->getDistance());
			Cross1->setTransfer(Goal->getTransfer() + 1);


			markStationDis(Cross1);
		}
		if (Goal->getNumOfCross() > 1 && (Cross2->getDistance() > Goal->getDistance() || (Cross2->getDistance() == Goal->getDistance() && Cross2->getTransfer() > Goal->getTransfer() + 1))) {
			Cross2->setDistance(Goal->getDistance());
			Cross2->setTransfer(Goal->getTransfer() + 1);

			markStationDis(Cross2);
		}
	}
}

void markStationTrans(station * Goal) {
	station * Last = Goal->getLast();
	station * Next = Goal->getNext();
	if (Last != NULL && (Last->getTransfer()>Goal->getTransfer() || (Last->getTransfer() == Goal->getTransfer() && Last->getDistance()>Goal->getDistance()))) {
		Last->setDistance(Goal->getDistance() + 1);
		Last->setTransfer(Goal->getTransfer());

		markStationTrans(Last);
	}
	if (Next != NULL && (Next->getTransfer()>Goal->getTransfer() || (Next->getTransfer() == Goal->getTransfer() && Next->getDistance()>Goal->getDistance()))) {
		Next->setDistance(Goal->getDistance() + 1);
		Next->setTransfer(Goal->getTransfer());

		markStationTrans(Next);
	}
	if (Goal->getNumOfCross() > 0) {
		station * Cross1 = Goal->getCross(0);
		station * Cross2 = Goal->getCross(1);
		if ((Cross1->getTransfer()  > Goal->getTransfer() + 1) || ((Cross1->getTransfer() == Goal->getTransfer() + 1) && (Cross1->getDistance()>Goal->getDistance()))) {
			Cross1->setDistance(Goal->getDistance());
			Cross1->setTransfer(Goal->getTransfer() + 1);

			markStationTrans(Cross1);
		}
		if (Goal->getNumOfCross() > 1 && (((Cross2->getTransfer()  > Goal->getTransfer() + 1) || ((Cross2->getTransfer() == Goal->getTransfer() + 1) && (Goal->getCross(1)->getDistance()>Goal->getDistance()))))) {
			Cross2->setDistance(Goal->getDistance());
			Cross2->setTransfer(Goal->getTransfer() + 1);

			markStationTrans(Cross2);
		}
	}
}

void printPath(station * S) {
	string path = "";
	station * Start = S;
	while (int curDis = Start->getDistance()) {
		if (Start->getLast() != NULL && Start->getLast()->getDistance() == curDis -1) {
			path = "\n"+Start->getName() + path;
			Start = Start->getLast();
		}
		else if (Start->getNext() != NULL && Start->getNext()->getDistance() == curDis -1) {
			path = "\n" + Start->getName() + path;
			Start = Start->getNext();
		}
		else if (Start->getCross(0) != NULL &&Start->getCross(0)->getDistance() == curDis && Start->getCross(0)->getTransfer() == Start->getTransfer() -1) {
			if(Start != S)
				path = "换乘" + Start->getLineName() + path;
			Start = Start->getCross(0);
		}
		else if (Start->getCross(1) != NULL &&Start->getCross(1)->getDistance() == curDis && Start->getCross(1)->getTransfer() == Start->getTransfer() -1) {
			if (Start != S)
				path = "换乘" + Start->getLineName() + path;
			Start = Start->getCross(1);
		}
		else if (Start->getName() == "2号航站楼") {
			path = "\n2号航站楼" + path;
			Start = findStation("3号航站楼");
		}
		else if (Start->getName() == "三元桥" && Start->getNext()->getNext()->getDistance() +1 == Start->getDistance()) {
			path = "\n三元桥" + path;
			Start = Start->getNext()->getNext();
		}
	}
	if (path == "") {
		cout << "0\n";
	}
	else
		cout << path << endl;;
}

int main(int argc, char * argv[])
{

	//Check:
	if (argc <4 || argc>4) {
		cout << "未输入参数或输入参数不合法 请重新尝试" << endl;
		return -1;
	}
	else {
		Order = string(argv[1]);
		From = string(argv[2]);
		To = string(argv[3]);
		if (Order == "-a") {
			cout << "不支持此项功能" << endl;
			return -1;
		}
		else if (Order == "-b" || Order == "-c") {

		}
		else {
			cout << "输入参数不合法 请重新尝试" << endl;
			return -1;
		}
	}

	//create the map ( scan create cross mark
	ifstream ifs("beijing-subway.txt");
	streambuf * buf = cin.rdbuf(ifs.rdbuf());
	string temStr;
	string temStation;
	int count = 0;
	bool headFlag = false;
	station * lastStation = NULL;
	bool onewayFlag = false;
	while (cin >> temStr) {
		if ((temStr == "1号线") || (temStr == "2号线") || (temStr == "4号线")
			|| (temStr == "5号线") || (temStr == "6号线") || (temStr == "7号线")
			|| (temStr == "8号线") || (temStr == "9号线") || (temStr == "10号线")
			|| (temStr == "13号线") || (temStr == "14号线西段") || (temStr == "14号线东段") || (temStr == "15号线")
			|| (temStr == "八通线") || (temStr == "昌平线") || (temStr == "亦庄线")
			|| (temStr == "房山线") || (temStr == "机场线") || (temStr == "大兴线")) {
			temStation = temStr;
			headFlag = true;
			continue;
		}
		//cout << i++<<": "<<temStation <<" : "<< temStr << endl;
		if (headFlag == true) {
			Heads[count] = new station(temStation, temStr);
			lastStation = Heads[count];
			Heads[count]->setLast(NULL);
			Heads[count]->setNext(NULL);
			count++;
			headFlag = false;
			continue;
		}
		if (lastStation == NULL) {
			cout << "文件格式有误" << endl;
			return -1;
		}
		if (temStr == "CIRCLE") {
			lastStation->setNext(Heads[count - 1]);
			Heads[count - 1]->setLast(lastStation);
			continue;
		}

		if (temStr == "ONEWAY") {
			onewayFlag = true;
			continue;
		}
		station * curStation = new station(temStation, temStr);
		if (onewayFlag == false) {
			lastStation->setNext(curStation);
			curStation->setLast(lastStation);
			curStation->setNext(NULL);
		}
		else {
			lastStation->setNext(curStation);
			curStation->setLast(NULL);
			curStation->setNext(lastStation->getLast());
			onewayFlag = false;
		}
		lastStation = curStation;
	}
	if (Heads[numOfHead - 1] == NULL) {
		cout << "文件格式有误" << endl;
		return -1;
	}
	int c = 0;
	cin.rdbuf(buf);
	if (Heads[0] == NULL) {
		cout << "路径文件不存在" << endl;
		return -1;
	}
	//
	for (int i = 0; i < numOfHead - 1; i++) {
		station * curStation = Heads[i];
		int markFirstVisit1 = 0;
		string curStationName = curStation->getName();
		while (curStation != NULL && (curStation != Heads[i] || markFirstVisit1 == 0) && ((curStationName != "三元桥" || curStation->getLineName() != "机场线") || markFirstVisit1 == 1)) {
			//recycle body
			curStationName = curStation->getName();
			markFirstVisit1++;

			//
			int j = i + 1;
			for (; j < numOfHead; j++) {
				station * aimStation = Heads[j];
				int markFirstVisit2 = 0;
				string aimStationName = aimStation->getName();
				while (aimStation != NULL && (aimStation != Heads[j] || markFirstVisit2 == 0) && ((aimStationName != "三元桥" || aimStation->getLineName() != "机场线") || markFirstVisit2 == 1)) {
					markFirstVisit2++;
					aimStationName = aimStation->getName();
					if (curStationName == aimStationName && curStation->getLineName() != aimStation->getLineName()) {
						//cout << curStation->getName() << endl;
						curStation->addCross(aimStation);
						aimStation->addCross(curStation);
					}
					aimStation = aimStation->getNext();
				}
			}

			//
			curStation = curStation->getNext();
		}
	}


	//double check of station:
	station * FromStation = findStation(From);
	station * ToStation = findStation(To);
	if (FromStation == NULL || ToStation == NULL) {
		cout << "输入站名有误" << endl;
		return -1;
	}
	else {
		cout << "指令要求：从" << From + "到" + To << endl;
		cout << "寻路模式：";
		if (Order == "-b")
			cout << "距离最短" << endl;
		else
			cout << "换乘最小" << endl;
	}
	FromStation->setDistance(0);
	FromStation->setTransfer(0);
	if (string(argv[1]) == "-b") {
		markStationDis(FromStation);
	}
	else if (string(argv[1]) == "-c") {
		markStationTrans(FromStation);
	}

	cout << ToStation->getDistance();
	printPath(ToStation);
	return 0;
}