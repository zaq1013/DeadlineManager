#include "MyBot.h"
#include <dpp/dpp.h>
#include <string>
#include <iostream>
#include <stdio.h>
#include <ctime>
#include <time.h>
using namespace std;


const std::string    BOT_TOKEN = "MTA1MDk0NTYwODAxMTIyNzI1Nw.GDwxHR.P_T3YoV9uBaoSutcfv3LsyQqzG8wrtu_RugImc";
const dpp::snowflake MY_GUILD_ID = 1050945608011227257;

const std::vector<std::string> split(const std::string& str, const std::string& pattern) {
    //�Ψ�
    std::vector<std::string> result;
    std::string::size_type begin, end;

    end = str.find(pattern);
    begin = 0;

    while (end != std::string::npos) {
        if (end - begin != 0) {
            result.push_back(str.substr(begin, end - begin));
        }
        begin = end + pattern.size();
        end = str.find(pattern, begin);
    }

    if (begin != str.length()) {
        result.push_back(str.substr(begin));
    }

    return result;
}

string DatePlus(string nowDate, int addHowManyDay) { //nowDate: yyyy/mm/dd
    string whatAmIGonnaReturn;
    int nowY = stoi(nowDate.substr(0, 4));
    int nowM = stoi(nowDate.substr(5, 2));
    int nowD = stoi(nowDate.substr(8, 2));
    nowD += addHowManyDay;
    while (nowD > 31) {
        switch (nowM) {
        case 1:case 3:case 5:case 7:case 8:case 10:case 12:
            nowD -= 31;
            nowM++;
            break;

        case 4:case 6:case 9:case 11:
            nowD -= 30;
            nowM++;
            break;

        case 2:
            if (nowY % 400 == 0 || (nowY % 4 == 0 && nowY % 100 != 0)) {
                nowD -= 29;
            }
            else {
                nowD -= 28;
            }
            nowM++;
            break;
        }
        if (nowM > 12) {
            nowM = 1;
            nowY++;
        }
    }
    whatAmIGonnaReturn = std::to_string(nowY) + "/";

    if (nowM < 10) {
        whatAmIGonnaReturn += "0" + std::to_string(nowM);
    }
    else {
        whatAmIGonnaReturn += std::to_string(nowM);
    }
    if (nowD < 10) {
        whatAmIGonnaReturn += "/0" + std::to_string(nowD);
    }
    else {
        whatAmIGonnaReturn += "/" + std::to_string(nowD);
    }
    whatAmIGonnaReturn += "/" + std::to_string(nowD);
    return whatAmIGonnaReturn;
};

class Data
{
private:
    int membersNum;
    std::string name;
    std::string manager;
    std::string* members; //�ϥΪ̲M��
    bool* status; //�C��ϥΪ̪����A�C
    std::string date;
    std::string time;
    bool notify = false;
    // something else...
public:
    Data() { name = ""; manager = ""; date = ""; time = ""; };
    Data(std::string date, std::string time, std::string name, std::string members) {
        this->name = name; 
        this->date = date; 
        this->time = time; 
        //����@�U���X�즨���n�i�洣��
        membersNum = 0;

        std::string temp = members; //�ΨӼȦs�n���檺�r�y
        std::vector<std::string> ret0 = split(temp, ",");
        for (auto& s : ret0) {
            membersNum++;
        }

        //�}�l��F��s�i�h
        this->members = new string[membersNum]; //new�X�Ŷ�
        this->status = new bool[membersNum];

        std::vector<std::string> ret = split(members, ",");
        int a = 0;
        for (auto& s : ret) {
            this->members[a] = s;
            this->status[a] = false;
            a++;
            if (a > membersNum)
                break;
        }
    };

    const std::string getName() { return name; };
    const std::string getDate() { return date; };
    const std::string getTime() { return time; };
    const std::string getManager() { return manager; };
    const bool getNotify() { return notify; };
    const std::time_t convertEpochTime();
    void setNotify(bool state) { this->notify = state; };
    //get��
    const int getMembersNum() { return membersNum; };
    const std::string getMembers(int i) { return members[i]; };
    const bool getStatus(int i) { return status[i]; };
    //set��
    void setStatus(int i, bool status) { this->status[i] = status; };

    std::string getExactDate() {
        //���o����deadline���T���ɶ�

        std::string whatAmIGonnaReturn;

        //�]�w�n��e�ɶ�
        time_t now = std::time(NULL);
        char c[26];
        ctime_s(c, sizeof c, &now);
        std::string nowStr = c;
        std::string nowlist[5] = { nowStr.substr(0, 3), nowStr.substr(4, 3), nowStr.substr(8, 2), nowStr.substr(11, 8), nowStr.substr(20, 4) };
        // [0]: www (Mon,Tue...)  [1]: mmm (Jan,Feb...)  [2]: dd (1,10...)  [3]: hh:mm:ss  [4]: yyyy
        std::string weekList[7] = { "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun" };
        std::string monthList[12] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };


        //�H�U�����������榡��: YYYY/MM/DD�A�ñN���������s�JwhatAmIGonnaReturn
        whatAmIGonnaReturn = nowlist[4] + "/"; //�~��
        for (int i = 0; i < 12; i++) { //���
            if (nowlist[1] == monthList[i]) {
                if (i - 9 >= 0) {
                    whatAmIGonnaReturn += std::to_string(i + 1);
                }
                else {
                    whatAmIGonnaReturn += "0" + std::to_string(i + 1);
                }
                break;
            }
        }
        whatAmIGonnaReturn += "/";
        if (stoi(nowlist[2]) < 10) { //���
            whatAmIGonnaReturn += "0" + nowlist[2].substr(1, 1);
        }
        else {
            whatAmIGonnaReturn += nowlist[2];
        }
        //�H�W�����������榡��: YYYY/MM/DD


    //�}�l�p��
        //�C�Ѵ��������p
        if (this->date == "allDay") {
            int nowhour = stoi(nowlist[3].substr(0, 2));
            int nowmin = stoi(nowlist[3].substr(3, 2));
            int desthour = stoi(this->time.substr(0, 2));
            int destmin = stoi(this->time.substr(3, 2));
        }
        //�C�P���������p
        else if (this->date == "everyWeek") {
            int destDate = 0; //�ؼФ鬰§���X
            int nowDate = 0; //���鬰§���X

            //�榡��§���X�A�d��: 1~7
            for (int i = 0; i < 7; i++) {
                if (weekList[i] == this->date) {
                    nowDate = i + 1;
                }
                if (weekList[i] == nowlist[0]) {
                    destDate = i + 1;
                }
                if (nowDate != 0 && destDate != 0) {
                    break;
                }
            }

            //ú��������U�@�P
            if (destDate < nowDate) {
                int temp = 0;
                temp = 7 - (nowDate - destDate);
                return DatePlus(whatAmIGonnaReturn, temp);
            }

            //ú����������P
            else if (destDate > nowDate) {
                int temp = 0; // �ΨӰO���ۮt�X��
                temp = destDate - nowDate;
                return DatePlus(whatAmIGonnaReturn, temp);
            }

            //ú�������n������
            else {
                int nowhour = stoi(nowlist[3].substr(0, 2));
                int nowmin = stoi(nowlist[3].substr(3, 2));
                int desthour = stoi(this->time.substr(0, 2));
                int destmin = stoi(this->time.substr(3, 2));
                if (nowhour < desthour || (nowhour == desthour && nowmin < destmin)) { //���ѩ|���L�I�����
                    return whatAmIGonnaReturn;
                }
                else { //���Ѥw�L�I������A��O�^�ǤU�@�ӺI�����
                    return DatePlus(whatAmIGonnaReturn, 7);
                }

            }
        }

        //�C�봣�������p
        else {
            int nowhour = stoi(nowlist[3].substr(0, 2));
            int nowmin = stoi(nowlist[3].substr(3, 2));
            int desthour = stoi(this->time.substr(0, 2));
            int destmin = stoi(this->time.substr(3, 2));
            int nowY = stoi(whatAmIGonnaReturn.substr(0, 4));
            int nowM = stoi(whatAmIGonnaReturn.substr(5, 2));

            //��n�N�O��ѡA�B���L����
            if (this->date == nowlist[2] && (nowhour < desthour || (nowhour == desthour && nowmin < destmin))) {
                return whatAmIGonnaReturn;
            }
            //���Ѥw�L�I������Ω|���Ө�I�����A��O�^�ǤU�@�ӺI������C���ˬd�ϥΪ̩ҿ����O�_��29 30 31�C�Y���H�W����A�h�I��������������몺�̫�@��
            else {
                //����P�I�����P�@�Ӥ��
                if (stoi(this->date) > stoi(nowlist[2])) {
                    switch (stoi(this->date)) {
                    case 29:
                        if (nowlist[1] == "Feb") {
                            if (nowY % 400 == 0 || (nowY % 4 == 0 && nowY % 100 != 0)) {
                                return whatAmIGonnaReturn.substr(0, 8) + "29";
                            }
                            else {
                                whatAmIGonnaReturn = std::to_string(nowY) + "/" + std::to_string(nowM) + "/" + "28";
                                return whatAmIGonnaReturn;
                            }
                        }
                        else {
                            return whatAmIGonnaReturn.substr(0, 8) + this->date[1];
                        }
                        break;
                    case 30:
                        if (nowlist[1] == "Feb") {
                            if (nowY % 400 == 0 || (nowY % 4 == 0 && nowY % 100 != 0)) {
                                return whatAmIGonnaReturn.substr(0, 8) + "29";
                            }
                            else {
                                whatAmIGonnaReturn = std::to_string(nowY) + "/" + std::to_string(nowM) + "/" + "28";
                                return whatAmIGonnaReturn;
                            }
                        }
                        else {
                            return whatAmIGonnaReturn.substr(0, 8) + this->date[1];
                        }
                        break;
                    case 31:
                        if (nowlist[1] == "Feb") {
                            if (nowY % 400 == 0 || (nowY % 4 == 0 && nowY % 100 != 0)) {
                                return whatAmIGonnaReturn.substr(0, 8) + "29";
                            }
                            else {
                                whatAmIGonnaReturn = std::to_string(nowY) + "/" + std::to_string(nowM) + "/" + "28";
                                return whatAmIGonnaReturn;
                            }
                        }
                        else if (nowlist[1] == "Apr" || nowlist[1] == "Jun" || nowlist[1] == "Sep" || nowlist[1] == "Nov") {
                            return whatAmIGonnaReturn.substr(0, 8) + "30";
                        }
                        else {
                            return whatAmIGonnaReturn.substr(0, 8) + this->date[1];
                        }
                        break;
                    default:
                        return whatAmIGonnaReturn.substr(0, 8) + this->date[1];
                        break;
                    }
                }

                //����P�I�������P���
                else {

                    whatAmIGonnaReturn = std::to_string(nowY) + "/";
                    nowM++;
                    if (nowM > 12) {
                        nowY++;
                        nowM = 1;
                    }
                    if (nowM < 10) {
                        whatAmIGonnaReturn += "0" + std::to_string(nowM);
                    }
                    else {
                        whatAmIGonnaReturn += std::to_string(nowM);
                    }
                    whatAmIGonnaReturn += "/";

                    switch (stoi(this->date)) {
                    case 29:
                        if (nowM == 2) {
                            if (nowY % 400 == 0 || (nowY % 4 == 0 && nowY % 100 != 0)) {
                                return whatAmIGonnaReturn.substr(0, 8) + "29";
                            }
                            else {
                                whatAmIGonnaReturn = std::to_string(nowY) + "/" + std::to_string(nowM) + "/" + "28";
                                return whatAmIGonnaReturn;
                            }
                        }
                        else {
                            return whatAmIGonnaReturn.substr(0, 8) + this->date[1];
                        }
                        break;
                    case 30:
                        if (nowM == 2) {
                            if (nowY % 400 == 0 || (nowY % 4 == 0 && nowY % 100 != 0)) {
                                return whatAmIGonnaReturn.substr(0, 8) + "29";
                            }
                            else {
                                whatAmIGonnaReturn = std::to_string(nowY) + "/" + std::to_string(nowM) + "/" + "28";
                                return whatAmIGonnaReturn;
                            }
                        }
                        else {
                            return whatAmIGonnaReturn.substr(0, 8) + this->date[1];
                        }
                        break;
                    case 31:
                        if (nowM == 2) {
                            if (nowY % 400 == 0 || (nowY % 4 == 0 && nowY % 100 != 0)) {
                                return whatAmIGonnaReturn.substr(0, 8) + "29";
                            }
                            else {
                                whatAmIGonnaReturn = std::to_string(nowY) + "/" + std::to_string(nowM) + "/" + "28";
                                return whatAmIGonnaReturn;
                            }
                        }
                        else if (nowM == 4 || nowM == 6 || nowM == 9 || nowM == 11) {
                            return whatAmIGonnaReturn.substr(0, 8) + "30";
                        }
                        else {
                            return whatAmIGonnaReturn.substr(0, 8) + this->date[1];
                        }
                        break;
                    default:
                        return whatAmIGonnaReturn.substr(0, 8) + this->date[1];
                        break;
                    }
                }
            }
        }
    };

    // �NData�o��class�� == �� != overload ���A�H�K����Data�������
    const bool operator==(const Data& m) {
        if ((membersNum != m.membersNum) || (name != m.name) || (date != m.date) || (time != m.time) || (date[0] != m.date[0]) || (date[1] != m.date[1])) {
            return false;
        }
        return true;
    };
    const bool operator!=(const Data& m) {
        if ((membersNum != m.membersNum) || (name != m.name) || (date != m.date) || (time != m.time) || (date[0] != m.date[0]) || (date[1] != m.date[1])) {
            return true;
        }
        return false;
    };
};



//Linked List
class LinkedList;
class ListNode { // Node���U��Data�s�񪺦�m
private:
    Data data; // ����Data���e
    ListNode* next; //�O���U�@��������ƪ��O�����m
public:
    ListNode() :data(), next() {};
    ListNode(Data a) :data(), next() { data = a; next = NULL; };
    Data getData() { return data; };
    ListNode* getNext() { return next; };
    friend class LinkedList;
};

class LinkedList {
private:
    int size; //�����j�p
    ListNode* first;    // ������Ӱ}�C�̪줸������m
public:
    LinkedList() :first() { first = NULL;size = 0; };

    void Push(Data x);    // �s�WData���
    void Delete(Data x);  // �R�����wData���
    const int getSize() { return size; };
    ListNode* getFirst() { return first; };
};
void LinkedList::Push(Data x) {

    ListNode* newNode = new ListNode(x);

    if (size == 0) {   // �Y�ثelist���S�F��
        first = newNode; // �h�N�������]���}�Y
        size += 1;
        return;
    }

    ListNode* current = first;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = newNode;
    size += 1;
}

void LinkedList::Delete(Data x) {

    ListNode* current = first, * previous = NULL;
    while (current != NULL && current->data != x) { // ��List�v�ӻ��j������۲�Data����
        previous = current;
        current = current->next;
    }

    if (current == NULL) {
        std::cout << "Not in list.";
    }
    else if (current == first) { // �Yfirst�N�O���ܫh�R���ӵ���������next�ܦ����list��first
        first = current->next;
        delete current;
        current = NULL;
    }
    else {
        previous->next = current->next;
        // ���O���ܫh�ݦb�R��������⥦�e�̪�next�]�w����̡A�۷����_�@�`��N�ѤU�����s�s��
        delete current;
        current = NULL;
    }
}

const std::time_t Data::convertEpochTime() {
    std::string copyDate = this->date;
    std::string copyTime = this->time;

    std::string year = copyDate.substr(0, copyDate.find("/"));
    copyDate.erase(0, copyDate.find("/") + 1);
    std::string month = copyDate.substr(0, copyDate.find("/"));
    copyDate.erase(0, copyDate.find("/") + 1);
    std::string whichDate = copyDate;

    std::string hour = copyTime.substr(0, copyTime.find(":"));
    copyTime.erase(0, copyTime.find(":") + 1);
    std::string min = copyTime;

    int send_year = stoi(year) - 1900;
    struct tm t = { 0 };  // Initalize to all 0's
    t.tm_year = send_year;  // This is year-1900, so 112 = 2012
    t.tm_mon = stoi(month) - 1;
    t.tm_mday = stoi(whichDate);
    t.tm_hour = stoi(hour);
    t.tm_min = stoi(min);
    t.tm_sec = 00;
    time_t timeSinceEpoch = mktime(&t);

    return timeSinceEpoch;
}

class PeriodData {
private:
    int membersNum; //�d�ݦ��h�֦W�ϥΪ̡A�Ω�ʺA�}�C
    std::string* members; //�ϥΪ̲M��
    bool* status; //�C��ϥΪ̪����A�C
    std::string missionName; //deadline�W�r
    string date[2]; //�b�C��DATE���n�i�洣���Adate[0] = allDay or everyWeek or everyDate�Cdate[1]��ܥL��ܤF���� 
    std::string time; //�b�ƻ�ɶ��i�洣��
    PeriodData* next = nullptr;
    //����MISSIONCOMPLETE����A��ʺA�}�C�R��
public:
    PeriodData() :membersNum(1), members(nullptr), status(nullptr), missionName(""), date(), time("") {};
    PeriodData(std::string date, std::string time, std::string missionName, std::string members) {

        //���N���ݾ�z���s�@�s
        this->time = time;
        this->missionName = missionName;
        if (date.find("allDay") != std::string::npos) { //�B�z�g��������
            this->date[0] = "allDay";
            date = date.substr(6);
            this->date[1] = date;
        }
        else if (date.find("everyWeek") != std::string::npos) {
            this->date[0] = "everyWeek";
            date = date.substr(9);
            switch (stoi(date)) {
            case 1:
                this->date[1] = "Mon";
                break;
            case 2:
                this->date[1] = "Tue";
                break;
            case 3:
                this->date[1] = "Wed";
                break;
            case 4:
                this->date[1] = "Thu";
                break;
            case 5:
                this->date[1] = "Fri";
                break;
            case 6:
                this->date[1] = "Sat";
                break;
            case 7:
                this->date[1] = "Sun";
                break;
            }
        }
        else {
            this->date[0] = "everyDate";
            date = date.substr(9);
            if (std::stoi(date) < 10) {
                this->date[1] = "0" + date;
            }
            else {
                this->date[1] = date;
            }
        }



        //����@�U���X�즨���n�i�洣��
        this->membersNum = 0;

        std::string temp = members; //�ΨӼȦs�n���檺�r�y
        std::vector<std::string> ret0 = split(temp, ",");
        for (auto& s : ret0) {
            this->membersNum++;
        }



        //�}�l��F��s�i�h
        this->members = new string[membersNum]; //new�X�Ŷ�
        this->status = new bool[membersNum];

        std::vector<std::string> ret = split(members, ",");
        int a = 0;
        for (auto& s : ret) {
            this->members[a] = s;
            this->status[a] = false;
            a++;
            if (a > membersNum)
                break;
        }

    };
    ~PeriodData() {
        delete[] members;
        delete[] status;
    };

    //get��
    const int getMembersNum() { return membersNum; };
    const std::string getMembers(int i) { return members[i]; };
    const bool getStatus(int i) { return status[i]; };
    const std::string getMissionName() { return missionName; };
    const std::string getTime() { return time; };
    PeriodData* getNext() { return next; };

    //set��
    void setStatus(int i, bool status) { this->status[i] = status; };

    //�d�ݩ|���I��e��deadline
    std::string getExactDate() {
        //���o����deadline���T���ɶ�

        std::string whatAmIGonnaReturn;

        //�]�w�n��e�ɶ�
        time_t now = std::time(NULL);
        char c[26];
        ctime_s(c, sizeof c, &now);
        std::string nowStr = c;
        std::string nowlist[5] = { nowStr.substr(0, 3), nowStr.substr(4, 3), nowStr.substr(8, 2), nowStr.substr(11, 8), nowStr.substr(20, 4) };
        // [0]: www (Mon,Tue...)  [1]: mmm (Jan,Feb...)  [2]: dd (1,10...)  [3]: hh:mm:ss  [4]: yyyy
        std::string weekList[7] = { "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun" };
        std::string monthList[12] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };


        //�H�U�����������榡��: YYYY/MM/DD�A�ñN���������s�JwhatAmIGonnaReturn
        whatAmIGonnaReturn = nowlist[4] + "/"; //�~��
        for (int i = 0; i < 12; i++) { //���
            if (nowlist[1] == monthList[i]) {
                if (i - 9 >= 0) {
                    whatAmIGonnaReturn += std::to_string(i + 1);
                }
                else {
                    whatAmIGonnaReturn += "0" + std::to_string(i + 1);
                }
                break;
            }
        }
        whatAmIGonnaReturn += "/";
        if (stoi(nowlist[2]) < 10) { //���
            whatAmIGonnaReturn += "0" + nowlist[2].substr(1, 1);
        }
        else {
            whatAmIGonnaReturn += nowlist[2];
        }
        //�H�W�����������榡��: YYYY/MM/DD


    //�}�l�p��
        //�C�Ѵ��������p
        if (this->date[0] == "allDay") {
            int nowhour = stoi(nowlist[3].substr(0, 2));
            int nowmin = stoi(nowlist[3].substr(3, 2));
            int desthour = stoi(this->time.substr(0, 2));
            int destmin = stoi(this->time.substr(3, 2));
        }
        //�C�P���������p
        else if (this->date[0] == "everyWeek") {
            int destDate = 0; //�ؼФ鬰§���X
            int nowDate = 0; //���鬰§���X

            //�榡��§���X�A�d��: 1~7
            for (int i = 0; i < 7; i++) {
                if (weekList[i] == this->date[1]) {
                    nowDate = i + 1;
                }
                if (weekList[i] == nowlist[0]) {
                    destDate = i + 1;
                }
                if (nowDate != 0 && destDate != 0) {
                    break;
                }
            }

            //ú��������U�@�P
            if (destDate < nowDate) {
                int temp = 0;
                temp = 7 - (nowDate - destDate);
                return DatePlus(whatAmIGonnaReturn, temp);
            }

            //ú����������P
            else if (destDate > nowDate) {
                int temp = 0; // �ΨӰO���ۮt�X��
                temp = destDate - nowDate;
                return DatePlus(whatAmIGonnaReturn, temp);
            }

            //ú�������n������
            else {
                int nowhour = stoi(nowlist[3].substr(0, 2));
                int nowmin = stoi(nowlist[3].substr(3, 2));
                int desthour = stoi(this->time.substr(0, 2));
                int destmin = stoi(this->time.substr(3, 2));
                if (nowhour < desthour || (nowhour == desthour && nowmin < destmin)) { //���ѩ|���L�I�����
                    return whatAmIGonnaReturn;
                }
                else { //���Ѥw�L�I������A��O�^�ǤU�@�ӺI�����
                    return DatePlus(whatAmIGonnaReturn, 7);
                }

            }
        }

        //�C�봣�������p
        else {
            int nowhour = stoi(nowlist[3].substr(0, 2));
            int nowmin = stoi(nowlist[3].substr(3, 2));
            int desthour = stoi(this->time.substr(0, 2));
            int destmin = stoi(this->time.substr(3, 2));
            int nowY = stoi(whatAmIGonnaReturn.substr(0, 4));
            int nowM = stoi(whatAmIGonnaReturn.substr(5, 2));

            //��n�N�O��ѡA�B���L����
            if (this->date[1] == nowlist[2] && (nowhour < desthour || (nowhour == desthour && nowmin < destmin))) {
                return whatAmIGonnaReturn;
            }
            //���Ѥw�L�I������Ω|���Ө�I�����A��O�^�ǤU�@�ӺI������C���ˬd�ϥΪ̩ҿ����O�_��29 30 31�C�Y���H�W����A�h�I��������������몺�̫�@��
            else {
                //����P�I�����P�@�Ӥ��
                if (stoi(this->date[1]) > stoi(nowlist[2])) {
                    switch (stoi(this->date[1])) {
                    case 29:
                        if (nowlist[1] == "Feb") {
                            if (nowY % 400 == 0 || (nowY % 4 == 0 && nowY % 100 != 0)) {
                                return whatAmIGonnaReturn.substr(0, 8) + "29";
                            }
                            else {
                                whatAmIGonnaReturn = std::to_string(nowY) + "/" + std::to_string(nowM) + "/" + "28";
                                return whatAmIGonnaReturn;
                            }
                        }
                        else {
                            return whatAmIGonnaReturn.substr(0, 8) + this->date[1];
                        }
                        break;
                    case 30:
                        if (nowlist[1] == "Feb") {
                            if (nowY % 400 == 0 || (nowY % 4 == 0 && nowY % 100 != 0)) {
                                return whatAmIGonnaReturn.substr(0, 8) + "29";
                            }
                            else {
                                whatAmIGonnaReturn = std::to_string(nowY) + "/" + std::to_string(nowM) + "/" + "28";
                                return whatAmIGonnaReturn;
                            }
                        }
                        else {
                            return whatAmIGonnaReturn.substr(0, 8) + this->date[1];
                        }
                        break;
                    case 31:
                        if (nowlist[1] == "Feb") {
                            if (nowY % 400 == 0 || (nowY % 4 == 0 && nowY % 100 != 0)) {
                                return whatAmIGonnaReturn.substr(0, 8) + "29";
                            }
                            else {
                                whatAmIGonnaReturn = std::to_string(nowY) + "/" + std::to_string(nowM) + "/" + "28";
                                return whatAmIGonnaReturn;
                            }
                        }
                        else if (nowlist[1] == "Apr" || nowlist[1] == "Jun" || nowlist[1] == "Sep" || nowlist[1] == "Nov") {
                            return whatAmIGonnaReturn.substr(0, 8) + "30";
                        }
                        else {
                            return whatAmIGonnaReturn.substr(0, 8) + this->date[1];
                        }
                        break;
                    default:
                        return whatAmIGonnaReturn.substr(0, 8) + this->date[1];
                        break;
                    }
                }

                //����P�I�������P���
                else {

                    whatAmIGonnaReturn = std::to_string(nowY) + "/";
                    nowM++;
                    if (nowM > 12) {
                        nowY++;
                        nowM = 1;
                    }
                    if (nowM < 10) {
                        whatAmIGonnaReturn += "0" + std::to_string(nowM);
                    }
                    else {
                        whatAmIGonnaReturn += std::to_string(nowM);
                    }
                    whatAmIGonnaReturn += "/";

                    switch (stoi(this->date[1])) {
                    case 29:
                        if (nowM == 2) {
                            if (nowY % 400 == 0 || (nowY % 4 == 0 && nowY % 100 != 0)) {
                                return whatAmIGonnaReturn.substr(0, 8) + "29";
                            }
                            else {
                                whatAmIGonnaReturn = std::to_string(nowY) + "/" + std::to_string(nowM) + "/" + "28";
                                return whatAmIGonnaReturn;
                            }
                        }
                        else {
                            return whatAmIGonnaReturn.substr(0, 8) + this->date[1];
                        }
                        break;
                    case 30:
                        if (nowM == 2) {
                            if (nowY % 400 == 0 || (nowY % 4 == 0 && nowY % 100 != 0)) {
                                return whatAmIGonnaReturn.substr(0, 8) + "29";
                            }
                            else {
                                whatAmIGonnaReturn = std::to_string(nowY) + "/" + std::to_string(nowM) + "/" + "28";
                                return whatAmIGonnaReturn;
                            }
                        }
                        else {
                            return whatAmIGonnaReturn.substr(0, 8) + this->date[1];
                        }
                        break;
                    case 31:
                        if (nowM == 2) {
                            if (nowY % 400 == 0 || (nowY % 4 == 0 && nowY % 100 != 0)) {
                                return whatAmIGonnaReturn.substr(0, 8) + "29";
                            }
                            else {
                                whatAmIGonnaReturn = std::to_string(nowY) + "/" + std::to_string(nowM) + "/" + "28";
                                return whatAmIGonnaReturn;
                            }
                        }
                        else if (nowM == 4 || nowM == 6 || nowM == 9 || nowM == 11) {
                            return whatAmIGonnaReturn.substr(0, 8) + "30";
                        }
                        else {
                            return whatAmIGonnaReturn.substr(0, 8) + this->date[1];
                        }
                        break;
                    default:
                        return whatAmIGonnaReturn.substr(0, 8) + this->date[1];
                        break;
                    }
                }
            }
        }
    };
    const bool operator==(const PeriodData& m) {
        if ((membersNum != m.membersNum) || (missionName != m.missionName) || (date != m.date) || (time != m.time) || (date[0] != m.date[0]) || (date[1] != m.date[1])) {
            return false;
        }
        return true;
    };
    const bool operator!=(const PeriodData& m) {
        if ((membersNum != m.membersNum) || (missionName != m.missionName) || (date != m.date) || (time != m.time) || (date[0] != m.date[0]) || (date[1] != m.date[1])) {
            return true;
        }
        return false;
    };

    //friend class
    friend class PeriodList;
};

class PeriodList {
private:
    int size = 0;
    PeriodData* head;
public:
    PeriodList() :head(0) {};

    PeriodData* getHead() { return head; };
    int getSize() { return size; };
    void Push(std::string infoList[]) {
        PeriodData* newPeriodData = new PeriodData(infoList[0], infoList[1], infoList[2], infoList[3]);
        if (head == 0) {
            head = newPeriodData;
            size++;
            return;
        }
        PeriodData* current = head;
        while (current->next != 0) {
            current = current->next;
        }
        current->next = newPeriodData;
        size++;
    };
    void Delete(PeriodData p) {
        PeriodData* current = head, * previous = NULL;
        while (current != NULL && *current != p) {
            previous = current;
            current = current->next;
        }

        if (current == NULL) {
            std::cout << "Not in list.";
            // return;
        }
        else if (current == head) {
            head = current->next;
            delete current;
            current = NULL;
            // return;
        }
        else {
            previous->next = current->next;
            delete current;
            current = NULL;
            // return;
        }

    }
};

PeriodList PList;
LinkedList deadlist;
Data deadlineList[10000]; //�ثe�̦h�N�O�x�s10000��deadline�ШD�Ӥw��
int deadlineCounter = 0; //�ΨӰO���ثe�x�s����@�Ӱ}�C�����A�������ӷ|�令�ʺA�}�C...�a


int main()
{

    /* Create bot cluster */
    uint64_t intents = dpp::i_default_intents | dpp::i_message_content;
    dpp::cluster bot(BOT_TOKEN, intents);
    
    /* Output simple log messages to stdout */
    bot.on_log(dpp::utility::cout_logger());

    /* Register slash command here in on_ready */
    bot.on_ready([&bot](const dpp::ready_t& event) {

        //set up deadline
        if (dpp::run_once<struct register_bot_commands>()) {

            /* Create a new global command on ready event */
            dpp::slashcommand newcommand("setdeadline", "Set up deadline", bot.me.id);
            newcommand.add_option(
                dpp::command_option(dpp::co_string, "input", "date/time/name/mission like this (note spaces!):2022/12/31 23:59 Amy hw1", true)
            );

            /* Register the command */
            bot.global_command_create(newcommand);
        }

    //period option
    if (dpp::run_once<struct register_bot_commands>()) {

        /* Create a new global command on ready event */
        dpp::slashcommand newcommand("how_to_set_period_deadline", "Period option", bot.me.id);

        /* Register the command */
        bot.global_command_create(newcommand);
    }

    //set up period deadline
    if (dpp::run_once<struct register_bot_commands>()) {

        /* Create a new global command on ready event */
        dpp::slashcommand newcommand("set_period_deadline", "Set up period deadline", bot.me.id);
        newcommand.add_option(
            dpp::command_option(dpp::co_string, "input", "(allDay or everyWeek? or everyDate?)/time/mission/name like this (note spaces!):every 23:59 hw1 Amy", true)
        );

        /* Register the command */
        bot.global_command_create(newcommand);
    }

    //check user's deadline
    if (dpp::run_once<struct register_bot_commands>()) {

        /* Create a new global command on ready event */
        dpp::slashcommand newcommand("mydeadline", "To check your unfinished deadline", bot.me.id);
        newcommand.add_option(
            dpp::command_option(dpp::co_string, "input", "your name", true)
        );

        /* Register the command */
        bot.global_command_create(newcommand);
    }

    //mission complete
    if (dpp::run_once<struct register_bot_commands>()) {

        /* Create a new global command on ready event */
        dpp::slashcommand newcommand("mission_complete", "Congratulation!!", bot.me.id);
        newcommand.add_option(
            dpp::command_option(dpp::co_string, "input", "input deadline of the mission which you complete like this (note spaces!):2022/12/31 23:59 Amy hw1", true)
        );

        /* Register the command */
        bot.global_command_create(newcommand);
    }

    //check all mission's status
    if (dpp::run_once<struct register_bot_commands>()) {
        /* Create a new global command on ready event */
        dpp::slashcommand newcommand("check", "find out who did not complete on time", bot.me.id);

        /* Register the command */
        bot.global_command_create(newcommand);
    }
        });


    // BOT�p��i��^��
    bot.on_slashcommand([&bot](const dpp::slashcommand_t& event) {

        //set up deadline
        if (event.command.get_command_name() == "setdeadline") {
            /* Fetch a parameter value from the command parameters */
            std::string input = std::get<std::string>(event.get_parameter("input"));

            std::vector<std::string> ret = split(input, " ");
            std::string list[4];
            int i = 0;
            for (auto& s : ret) {
                list[i] = s;
                i++;
                if (i > 3)
                    break;
            }

            Data deadline(list[0], list[1], list[2], list[3]);
            deadlist.Push(deadline);

            /* Reply to the command. There is an overloaded version of this
            * call that accepts a dpp::message so you can send embeds.
            */
            ListNode *current = deadlist.getFirst();

            for (int i = 0; i < deadlist.getSize() - 1; i++) {
                current = current->getNext();
            }
            string N = current->getData().getName();
            string D = current->getData().getDate();
            string T = current->getData().getTime();
            event.reply(std::string("Mission: ") + N + std::string("\nDeadline: ") + D + std::string(" ") + T + std::string("\nMember's name:") + list[3]);
        }

    //period option
    if (event.command.get_command_name() == "how_to_set_period_deadline") {
        event.reply("You can enter those periods: \n\n--\nAllDay :we will remind you every day on which time you set\n-- \neveryWeek1 ~ everyWeek7: \nyou can choose one day to remind\nFor example 'everyWeek1' , then we will remind you on every Monday\nnote. you can only enter 1-7\nnote.if you need more days to remind, just set up another period deadline\n--\neveryDate1 ~ everyDate31: \nif you need more days to remind, just set up another period deadline. \nFor example 'everyDate31' , then we will remind you on every month's 31th.\nnote. you can only enter 1-31\nnote. if you need more days to remind, just set up another period deadline\nnote. if there is no such day in that month(for example, you choose 31, But it's April), then we will remind you on the last day of that month.\n--\n\nSo, a correct input might be like this: \neveryDate1 23:59 homework1 Amy,Becky,Carly");
    }

    //set up period deadline
    if (event.command.get_command_name() == "set_period_deadline") {
        /* Fetch a parameter value from the command parameters */
        std::string input = std::get<std::string>(event.get_parameter("input"));

        std::vector<std::string> ret = split(input, " ");
        std::string list[4];
        int i = 0;
        for (auto& s : ret) {
            list[i] = s;
            i++;
            if (i > 3)
                break;
        }

        PList.Push(list);
        PeriodData* current = PList.getHead();

        for (int i = 0; i < PList.getSize() - 1; i++) {
            current = current->getNext();
        }

        std::string output = std::string("Mission: ") + current->getMissionName() + std::string("\nDeadline: ") + current->getExactDate() + std::string(" ") + current->getTime() + std::string("\nmember's name: ");
        for (int i = 0; i < current->getMembersNum(); i++) {
            output += current->getMembers(i) + " ";
        }
        event.reply(output);
    }

    //check user's unfinished deadline
    if (event.command.get_command_name() == "mydeadline") {

        std::string input = std::get<std::string>(event.get_parameter("input"));

        std::string whatAmIGoingToOutput1 = "For your normal deadline:\n";

        
        ListNode* current = deadlist.getFirst();
        for (int ii = 0; ii < deadlist.getSize(); ii++) { // ��List�v�ӻ��j������۲�Data����
            for (int i = 0; i < current->getData().getMembersNum(); i++) {
                if (current->getData().getMembers(i) == input && current->getData().getStatus(i) == 0) {
                    whatAmIGoingToOutput1 += current->getData().getDate() + " " + current->getData().getTime() + " " + current->getData().getName() + "\n";
                }
            }
            current = current->getNext();
        }
        if (whatAmIGoingToOutput1 == "For your normal deadline:\n") {
            whatAmIGoingToOutput1 += "You have nothing to do!\n\n";
        }

        std::string whatAmIGoingToOutput2 = "For your routine deadline:\n";
        PeriodData* temp = PList.getHead();

        for (int ii = 0; ii < PList.getSize(); ii++) {
            for (int i = 0; i < temp->getMembersNum(); i++) {
                if (temp->getMembers(i) == input && temp->getStatus(i) == false) {
                    whatAmIGoingToOutput2 += temp->getExactDate() + " " + temp->getTime() + " " + temp->getMissionName() + "\n";
                }
            }
            temp = temp->getNext();
        }

        if (whatAmIGoingToOutput2 == "For your routine deadline:\n") {
            whatAmIGoingToOutput2 += "You have nothing to do!\n\n";
        }

        event.reply(whatAmIGoingToOutput1 + whatAmIGoingToOutput2);
    }

    //complete the mission
    if (event.command.get_command_name() == "mission_complete") {
        /* Fetch a parameter value from the command parameters */
        std::string input = std::get<std::string>(event.get_parameter("input"));

        std::vector<std::string> ret = split(input, " ");
        std::string list[4];
        int i = 0;
        for (auto& s : ret) {
            list[i] = s;
            i++;
            if (i > 3)
                break;
        }

        std::string date = input.substr(0, 10);
        /* Reply to the command. There is an overloaded version of this
        * call that accepts a dpp::message so you can send embeds.
        */

        ListNode* current = deadlist.getFirst();
        for (int ii = 0; ii < deadlist.getSize(); ii++) {
            for (int i = 0; i < current->getData().getMembersNum(); i++) {
                if (list[0] == current->getData().getDate() && list[1] == current->getData().getTime() && list[2] == current->getData().getMembers(i) && list[3] == current->getData().getName()) {
                    current->getData().setStatus(i,1);
                    event.reply(current->getData().getMembers(i) + std::string(" just finished his / her mission --") + current->getData().getName() + std::string("!"));
                }
            }
            current = current->getNext();
        }

        PeriodData* temp = PList.getHead();

        for (int ii = 0; ii < PList.getSize(); ii++) {
            for (int i = 0; i < temp->getMembersNum(); i++) {
                if (list[0] == temp->getExactDate() && list[1] == temp->getTime() && list[2] == temp->getMembers(i) && list[3] == temp->getMissionName()) {
                    temp->setStatus(i, true);
                    event.reply(temp->getMembers(i) + std::string(" just finished his / her mission --") + temp->getMissionName() + std::string("!"));
                }
            }
            temp = temp->getNext();
        }
        event.reply("You have enter the wrong information.\nnote. for period deadline, you can only mark the complete_mission before the deadline hasn't expired.");
    }

    //check all deadline
    if (event.command.get_command_name() == "check") {

        std::string output = "For normal deadline:\n";

        ListNode* current = deadlist.getFirst();
        for (int ii = 0; ii < deadlist.getSize(); ii++) { // ��List�v�ӻ��j����L�X�Ҧ�Data����
            std::string output2 = current->getData().getName() + ":\t (due" + current->getData().getExactDate() + " " + current->getData().getTime() + ")\n\tunfinish: ";
            for (int i = 0; i < current->getData().getMembersNum(); i++) {
                if (current->getData().getStatus(i) == false) {
                    if (output2 == current->getData().getName() + ":\t (due" + current->getData().getExactDate() + " " + current->getData().getTime() + ")\n\tunfinish: ")
                        output2 += current->getData().getMembers(i);
                    else
                        output2 += ", " + current->getData().getMembers(i);
                }
            }
            std::string output3 = "\n\t\tfinished: ";
            for (int i = 0; i < current->getData().getMembersNum(); i++) { //�A�C�X�������H
                if (current->getData().getStatus(i) == true) {
                    if (output3 == "\n\t\tfinished: ")
                        output3 += current->getData().getMembers(i);
                    else
                        output3 += ", " + current->getData().getMembers(i);
                }
            }
            output += output2 + output3 + "\n\n";
            current = current->getNext();
        }
        
        if (output == "For normal deadline:\n")
            output += "there isn't any deadline existed.\n";

        output += "\nFor period deadline:\n";

        PeriodData* temp = PList.getHead();

        for (int ii = 0; ii < PList.getSize(); ii++) {
            std::string output2 = temp->getMissionName() + ":\t (due" + temp->getExactDate() + " " + temp->getTime() + ")\n\tunfinish: ";
            for (int i = 0; i < temp->getMembersNum(); i++) {
                if (temp->getStatus(i) == false) {
                    if (output2 == temp->getMissionName() + ":\t (due" + temp->getExactDate() + " " + temp->getTime() + ")\n\tunfinish: ")
                        output2 += temp->getMembers(i);
                    else
                        output2 += ", " + temp->getMembers(i);
                }
            }
            std::string output3 = "\n\t\tfinished: ";
            for (int i = 0; i < temp->getMembersNum(); i++) { //�A�C�X�������H
                if (temp->getStatus(i) == true) {
                    if (output3 == "\n\t\tfinished: ")
                        output3 += temp->getMembers(i);
                    else
                        output3 += ", " + temp->getMembers(i);
                }
            }
            output += output2 + output3 + "\n\n";
            temp = temp->getNext();
        }
        event.reply(output);
    }

        });

    /* Start the bot */
    bot.start(dpp::st_wait);

    return 0;
}
