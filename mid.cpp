#include <iostream>
using namespace std;

test
test22222

//////////////////////////////////////////////////////////////////////�w�qArray���j�p
const int NKLEN = 31, DAY = 31, TIME = 24, PEOPLE = 100;
//�]��NK�̦h��30�ӡ]���٦��𰲯Z��A�ҥH��31�^�A�Ӥ@�Ӥ�̦h�N�O31�ѡA�@�Ѧ�24�Ӯɬq�C���u�ƶq�̦h��100�H�C


////////////////////////////////////////////////////////////////////�򥻸���x�s���c�}�l
struct NKList{ //��i���Z�����������
    int DayShiftOrNot; //��Z�G1�F�]�Z�G-1
    int WorkingScedule[TIME]; //�ӯZ�����Ǯɶ��b�u�@�C�u�@�G1�A�S�u�@�G0
    int workingHour; //�ӯZ���`�@�u�@�ɶ�
};

struct StaffShift{ //��i����u���������
    int Day[DAY]; //�C��Z��
    int SatisfiedHowManyDemand[DAY]; //�O���C�Ѻ����F�h�ֳҰʻݨD�]�ΨӦw�ƨC�Ӥ�𮧢ڤѮɪ��禡�^
    int TotalRestDay; //�`�@�𮧤Ѽ�
    int ContinueWorking; // > 6 �Y�n��
    int lastTimeNightWork; // > 6 �Y�i�H�w�Ʃ]�Z
};

struct DJI{
    int Time[TIME];
    int LargestTimeInDay;
    int LargestDemandInDay;
    int LargestTimeInNight;
    int LargestDemandInNight;
    int HowManyPeopleCanRestOnToday;
};
////////////////////////////////////////////////////////////////////�򥻸���x�s���c����




////////////////////////////////////////////////////////////////////���²�檺�禡�}�l

//�̧Ǭ��G�O�_�C�ӭ��u�����F����L�Ѫ�����B��J�𰲡B�����𰲡B�ثei���u�ĴX�Ѩ�ĴX�ѳs��W�Z�W�L7�ѡB�N�@�Ӱ}�C�q�p�ƨ�j�B��Ѧ��h�֬����u�i�H�i��]�Z�B����٦��h�֭��u�i�H�W�Z�B��Ω]�Z�O�_�����ҰʻݨD�B�ثe�ݨD�H�Ƴ̤֪��ɬq�B�ֻ̤ݨD���ɬq�����ǡB���ӯZ���i�H�����o�ǳֻ̤ݨD�ɬq�B�p�G��o�ӤH��j�Ѫ��𰲵����������ܡA�o�ӤH���U�����٦��h�֤�

int HowManyPeopleCanWorkAtNight (int nI, int j, StaffShift staffShift[]){
    int peopleWorkAtNight = 0;
    for (int i = 0; i < nI; i++){
        if (staffShift[i].lastTimeNightWork > 6 && staffShift[i].Day[j] == -1){
            peopleWorkAtNight++;
        }
    }
    return peopleWorkAtNight;
}

void RestAssign (int i, int j, StaffShift staffShift[PEOPLE], DJI dJI[DAY]){
    //���禡�M���Ψө�m��
    //�]��staffShift[i][0]�M���Ψө�m�ثe���𰲦��ơA�ҥH�г��Υ��禡�Ӧw�ƥ�
    staffShift[i].Day[j] = 0;
    staffShift[i].TotalRestDay++;
    dJI[j].HowManyPeopleCanRestOnToday--;
}

void RestCancel (int i, int j, StaffShift staffShift[PEOPLE], DJI dJI[DAY]){
    //���禡�M���ΨӨ�����
    //�]��staffShift[i][0]�M���Ψө�m�ثe���𰲦��ơA�ҥH�г��Υ��禡�Ө�����
    staffShift[i].Day[j] = -1;
    staffShift[i].TotalRestDay--;
    dJI[j].HowManyPeopleCanRestOnToday++;
}

bool WhichDayToWhichDayContinueWorkOver7Days (int i, int nJ, int L, StaffShift staffShift[PEOPLE]){
    //���禡�M���ΨӻPDesignedRest�t�X
    //���禡�ΨӬd�ݭ��u�����X�Ѥu�@�W�L7�ѡA�åB�|���K�B���𰲤��@�O�_�X�z�]�O�_�ѤU���𰲤Ѽƥi�H����C�Ӥ�n��L�ѨåB�C7�ѥ�@�����W�w�^�C
    //���禡�^�ǭȬ����L�ȡA�Y�ӦW���u�ثe���𰲬O�X�z���A�h�N�^��0�C�|�����N���׸˦bocntinueWorkOver7Days��
    //�`�N�G�ݭn�b�I�s���a�I�ŧi�uint continueWorkOver7Days[11][3] = {{0}}; // �Ӱ}�C�|�O���s��W�Z�W�L6�Ѫ���l�C[i][0]���ӰϬq�_�l��A[i][1]���ӰϬq������A[i][2]���`�@�s��u�@�h�֤Ѽơv
    /*�����޿�G
     (1) �d�ݨ�ӥ𰲤������s��u�@��O�_�W�L����
     (2) �W�L���ѫK�N�ӰϬq���_�l��bcontinueWorkOver7Days[i][0]�A������bcontinueWorkOver7Days[i][1]�A�s��u�@�ѼƩ�b[i][2]
     (3) �Ytimer == 0�A�N���ۥ𰲥��`�A�S�����D�C�Ytimer != 0�A�h�N���ۻݭn�i���ˬd�O�_�i�H�F������
     (4) �Y�i�H�F������A�h�^��0�C�Y�o�{�L�k�F������A�h�^��1
    */
    int workingDays = 0, timer = 0 , LEdit = L - staffShift[i].TotalRestDay; // workingDays�������O�s��W�Z��ơAtimer�ΨӰO���n�N��Ʀs����ĴX��ALEdit�ΨӬݳѾl���𰲼ƶq
    int continueWorkOver7Days[11][3] = {{0}};
    for (int j = 0; j < nJ; j++){
        if (staffShift[i].Day[j] != 0){
            workingDays++;
        }else{
            if (workingDays > 6){
                continueWorkOver7Days[timer][1] = j - 1;
                continueWorkOver7Days[timer][0] = j - workingDays;
                continueWorkOver7Days[timer][2] = workingDays;
                LEdit -= (workingDays / 7);
                workingDays = 0;
                timer++;
            }else{
                workingDays = 0;
            }
        }
    }
    
    if (LEdit >= 0){ //�Y�n�F��7�ѥ�@�Ѫ��ܡA�i�H��n�N���𧹡A�S�Ϊ̬O�Ʀ��٦����i�H��
        return 0;
    }else{
        return 1; //�ⰲ�𧹫��٬O�S��k�F������@�A�Ϊ̬O�Q�𪺰��W�X�W�w��L��
    }
    
}

void Sorting(int lenOfArray, int array[], int array2[]){
    int breakPoint = 0, temp = 0, temp2 = 0;
    for (int i = 1; i < lenOfArray; i++){
        breakPoint = i;
        temp = array[i];
        temp2 = array2[i];
        for (int j = i - 1; j > -1; j--){
            if (array[j] <= array[i]){
                breakPoint = j + 1;
                break;
            }
            if (j == 0){
                breakPoint = 0;
            }
        }
        for (int j = i; j > breakPoint; j--){
            if (j == 0){
                break;
            }
            array[j] = array[j - 1];
            array2[j] = array2[j - 1];
        }
        array[breakPoint] = temp;
        array2[breakPoint] = temp2;
    }
}



int HowManyPeopleStillCanWork (int nI, int j, StaffShift staffShift[]){
    int peopleStillCanWork = 0;
    for (int i = 0; i < nI; i++){
        if (staffShift[i].Day[j] == -1){
            peopleStillCanWork++;
        }
    }
    return peopleStillCanWork;
}

int AnyDemandLeft (int j, int start, int end, DJI dJI[]){
    int demand = 0;
    for (int i = start; i < end; i++){
        demand += dJI[j].Time[i];
    }
    return demand;
}

int TheLeastDemand (int j, int start, int end, DJI dJI[]){
    int demand = PEOPLE;
    for (int i = start; i < end; i++){
        if (dJI[j].Time[i] < demand && dJI[j].Time[i] > 0){
            demand = dJI[j].Time[i];
        }
    }
    return demand;
}

void TheLeastDemandInterval (int j, int start, int end, DJI dJI[], int demand, int intervals[TIME]){
    //�n�O�o�b�I�s�a�ŧiint intervals[TIME] = {-1};
    int timer = 0;
    for (int i = start; i < end; i++){
        if (dJI[j].Time[i] == demand){
            intervals[timer] = i;
            timer++;
        }
    }
}

int WhichKInvolvedAllTheIntervals (int j, int nK, int start, int end, NKList nKList[NKLEN], DJI dJI[DAY], int intervals[TIME]){
    int howManyIntervalSatisfied = 0, nowBest = 0, bestK = 0, timer = 0;
    for (int k = 1; k < nK; k++){
        howManyIntervalSatisfied = 0;
        timer = 0;
        int i = 0;
        while(intervals[i] != -1){
            if (nKList[k].WorkingScedule[intervals[i]] == 1){
                howManyIntervalSatisfied++;
                timer++;
            }
            i++;
            if (i>=24)
            {
            	break;
			}
        }
        if (nowBest < howManyIntervalSatisfied){
            nowBest = howManyIntervalSatisfied;
            bestK = k;
        }else if (nowBest == howManyIntervalSatisfied){
            if (nKList[bestK].workingHour < nKList[k].workingHour){
                nowBest = howManyIntervalSatisfied;
                bestK = k;
            }
        }
    }
    return bestK;
}

int HowLongDoesHeOrSheNeedToWork(int i, int j, int nJ, NKList nKList[], StaffShift staffShift[]){
    //i�����u�s���Aj�����
    int counter = 0;
    for (int k = j-1; k > -1; k--){
        counter++;
        if (staffShift[i].Day[k] == 0){
            counter--;
            break;
        }
    }

    for (int k = j; k < nJ; k++){
        counter++;
        if (staffShift[i].Day[k] == 0 && k != j){
            counter--;
            break;
        }
    }
    return counter;
}

////////////////////////////////////////////////////////////////////���²�檺�禡����


////////////////////////////////////////////////////////////////////������n���禡�}�l
///�̧Ǭ��G�p��𰲡B�p���ܷ��Z��

///�p���
void RestDesigned(int nI, int nJ, int nK, int L, DJI dJI[], int RList[][TIME+1], StaffShift staffShift[]){
    //���禡�Ψӥ����t�𰲡A�H�ɶq�����Q�n�𮧪����u���@�欰�D�n�ؼ�
    /*�����޿�G
     (1) �Q�n��5�ѥH�U���H�i�H�H�N���𰲡A���Q�n��6�ѥH�W���H�ݭn�ݥL�̪��𰲤��@�O�_�X�z�C
         �����A���u�@�Ӥ��5�ѴN�i�H�����C7�ѥ�1�����W�h�F�A�Ӥ@�Ӥ�i��|��8-10�����A�åB�@�Ӥ�̦h�N�O31��
         ex. ���X�z�����p�G�𰲪��ɶ����������b�@�_�Q�n���ۤv�C�Ӥ볣��A��K�����H��
     (2) ���Q�n��6�ѥH�W���H�A�����L�̥��ѻݭn�H�Ƴ̤֪����X�ѡA�p�G��ѻݭn�H�Ƥw�g����i�u�@���H�ơA�h���U�@�ӻݭn�H�Ƴ̤֪����ѥh����ܡC
         ��p���G��nI = 40�A�p�G�j�a���Q�n��5���A��5�����Ѫ��̰����ݨD�H�Ƭ�20�H�A�����ڭ̤w�g��F20����u��5���ɡA�N����A����L���u��5���F�C
    */
 
    for (int i = 0; i < nI; i++){
        if (RList[i][0] == 0){ // �S�����@��
            continue;;
        }else{ //�����@�檺�H��
            for (int j = 1; j < nJ+1; j++){
                if (RList[i][j] == 1)
                    RestAssign(i, j-1, staffShift, dJI);
                if (staffShift[i].TotalRestDay == L)
                    break;
            }
            
            if (WhichDayToWhichDayContinueWorkOver7Days(i, nJ, L, staffShift) == 1){ //�p�G�L�����@���X�z����
                for (int j = 0; j < nJ; j++){ //�������@
                    if (staffShift[i].Day[j] == 0)
                        RestCancel(i, j, staffShift, dJI);
                }
                int howManyRestCanYouWish = L - ((nJ - (L - nJ / 7))/7);
                int wishList[DAY] = {0}, wishListTimer = 0; //
                for (int j = 0; j < nJ; j ++){
                    if (RList[i][j] == 1){
                        wishList[wishListTimer] = j;
                        wishListTimer++;
                    }
                }
                wishList[wishListTimer] = -1;
                int wishListHowManyPeopleCanRest[DAY] = {0}; //�x�s�PwishList�ѼƬ�������ѳҰʤO�̤j��
                for (int j = 0; wishList[j] != -1; j++){
                    wishListHowManyPeopleCanRest[j] = dJI[wishList[j]].HowManyPeopleCanRestOnToday;
                    if(j>=nJ)
                    {
                    	break;
					}
                }
                Sorting(wishListTimer, wishListHowManyPeopleCanRest, wishList);
                for (int j = wishListTimer-1; j < -1; j++){
                    if (dJI[wishList[wishListTimer]].HowManyPeopleCanRestOnToday > 0){
                        RestAssign(i, wishList[wishListTimer], staffShift, dJI);
                        howManyRestCanYouWish--;
                    }
                    if (howManyRestCanYouWish == 0)
                        break;
                }
            }
        }
    }
}


void ChooseShift (int nI, int nJ, int nK, int L, NKList nKList[NKLEN], DJI dJI[DAY], StaffShift staffShift[PEOPLE]){
    //���禡�ΨӦw�Ʒ�Ѫ��Z��
    /*
     �����޿�G
     (1) �@�Ѥ@�Ѱ��w�ơA�w�Ʀn�F����N�|�ߧY�N��骺�Z���o�U�h�C
     (2) �w�ƫe���T�{�O�_�����u�w�g�u�@�F6�ѡA�����ܽХL�𮧡C
     (3) �C�鳣�q�]�Z�H���}�l�i��w�ơC�]�פ���󬰥i����]�Z���H�Ƭ�0�Ϊ̩]�Z���󳣤w�g�Q�����^
        a. �����d�ݥثe�i�H����]�Z���H��
        b. �q�]�Z�ݭn�H�Ƴ̤֪��ɬq�}�l�w�ơA�åB�d�ݩ]�Z�ɬq�O�_����L�H�Ƥ]�O�p���֪��ɬq�A�M��i�H�[�\�o�Ǯɬq���Z���åB�u�@�ɶ��̪����]�Z�Z���C
        c. �N�Z���w�ƤU�h��A�խY���ɤ����H��i�H�w�Ʃ]�Z�A�h�A�d�ݩ]�Z��L�ɬq�H�Ƴ̤֪��ɨ�C�]�Y�^��B�Ja.�^
     (4) �]�Z�Z��������A�}�l�w�Ƥ�Z�C�]�פ���󬰩Ҧ��H��ѳ��Q�w�ƨ�Z���A�]�N�O�Z����W�S��-1�^
        a. �����d�ݤ�Z�ݭn�H�Ƴ̵u���ɶ��q�A�Y���h�Ӯɶ��q���u�ݭn�̤֪��H��A�h�M��O�_���[�\�o�Ǯɶ��q���Z��
        b. �M��n�Z��L��A�@�@�����U�h�A�åB�A�d�ݥثe�H�Ƴ̤֪��ɬq�C�]�^��B�Ja.�^
     */
    
    for (int j = 0; j < nJ; j++){ //�q��j�Ѷ}�l�i��w��
        for (int i = 0; i < nI; i++){
            if (staffShift[i].Day[j] == 0){
                staffShift[i].ContinueWorking = 0;
            }
            if (staffShift[i].ContinueWorking > 5){
                RestAssign(i, j, staffShift, dJI);
                staffShift[i].ContinueWorking = 0;
            }
            staffShift[i].lastTimeNightWork++;
        }
        int HMPCWAN = HowManyPeopleCanWorkAtNight(nI, j, staffShift);
        int ADL = AnyDemandLeft(j, 18, 24, dJI);
        while (HMPCWAN > 0 && ADL > 0){ // �]�Z
            int intervals[TIME] = {-1};
            for (int i = 0; i < TIME; i++){
                intervals[i] = -1;
            }
            TheLeastDemandInterval(j, 18, 24, dJI, TheLeastDemand(j, 18, 24, dJI), intervals);

            int bestK = WhichKInvolvedAllTheIntervals(j, nK, 18, 24, nKList, dJI, intervals);
            for (int i = 0; i < nI; i++){
                if (staffShift[i].lastTimeNightWork > 6 && staffShift[i].Day[j] == -1){
                    HMPCWAN--;
                    staffShift[i].Day[j] = bestK;
                    staffShift[i].lastTimeNightWork = 0;
                    staffShift[i].ContinueWorking++;
                    for (int k = 0; k < TIME; k++){
                        if (nKList[bestK].WorkingScedule[k] == 1 && dJI[j].Time[k] > 0){
                            dJI[j].Time[k]--;
                            staffShift[i].SatisfiedHowManyDemand[j]++;
                            ADL--;
                        }
                    }
                    break;
                }
            }
        }
        int HMPSCW = HowManyPeopleStillCanWork(nI, j, staffShift);
        ADL = AnyDemandLeft(j, 0, 18, dJI);
        while (HMPSCW > 0 && ADL > 0){ // ��Z
            int intervals[TIME] = {-1};
            TheLeastDemandInterval(j, 0, 18, dJI, TheLeastDemand(j, 0, 18, dJI), intervals);
            int bestK = WhichKInvolvedAllTheIntervals(j, nK, 0, 18, nKList, dJI, intervals);
            for (int i = 0; i < nI; i++){
                if (staffShift[i].Day[j] == -1){
                    HMPSCW--;
                    staffShift[i].Day[j] = bestK;
                    staffShift[i].ContinueWorking++;
                    for (int k = 0; k < TIME; k++){
                        if (nKList[bestK].WorkingScedule[k] == 1 && dJI[j].Time[k] > 0){
                            dJI[j].Time[k]--;
                            staffShift[i].SatisfiedHowManyDemand[j]++;
                            ADL--;
                        }
                    }
                    break;
                }
            }
        }
        for (int i = 0; i < nI; i++){
            if (staffShift[i].Day[j] == -1){
                RestAssign(i, j, staffShift, dJI);
                staffShift[i].ContinueWorking = 0;
            }
        }
    }
}

////////////////////////////////////////////////////////////////////������n���禡����


int main(){
    //////////////////////////////////////////////////////////////////////////////////////////////////////�򥻳]�w///////////////////////////////////////////////////////////////////////////////////////////////////
    
    int nI = 0, nJ = 0, nK = 0, L = 0, w1 = 0, w2 = 0, R = 0;
        //nI = ���u�ƶq, nJ = �w�ƤѼ�, nK = �Z���ƶq-1, L = �bnJ�Ѥ��A�n��L�ѥ𰲡C
        //w1 = ���������𰲻ݨD���v��, w2 = �W�B�]�Z���v��, R = �𰲻ݨD�ƶq�C
    cin >> nI >> nJ >> nK >> L >> w1 >> w2 >> R;
    nK++;
        
    
    ///�i�H�ϥΪ��Z��
    NKList nKList[NKLEN];
    for (int i = 1; i < nK+1; i++){ //�q��i���Z����ƶ}�l�x�s
        
        nKList[i].DayShiftOrNot = 1; //���ФW��Z�A�p�G�o�{�O�]�Z���ܦA���
        nKList[i].workingHour = 0; //���]�w��0�A����A�C�C�[�W�h
        
        for (int j = 0; j < TIME; j++){
            if (i == nK){ //�s�Z��
                cin >> nKList[0].WorkingScedule[j];
            }else{
                cin >> nKList[i].WorkingScedule[j];
            }
            
            if (nKList[i].WorkingScedule[j] == 1){ //�T�w�ӯZ�����u�@�ɶ��H�άO���O�]�Z
                nKList[i].workingHour++;
                if (j > 17){
                nKList[i].DayShiftOrNot = -1;
                }
            }
        }
    }
    for (int i = nK; i < NKLEN; i++){
        for (int j = 0; j < TIME; j++){
            nKList[i].WorkingScedule[j] = 0;
        }
    }

    

    ///�C��ɬq�H�ƻݨD��
    DJI dJI[DAY];
    for (int j = 0; j < nJ; j++){
        dJI[j].LargestTimeInDay = 0;
        dJI[j].LargestDemandInDay = 0;
        dJI[j].LargestTimeInNight = 0;
        dJI[j].LargestDemandInNight = 0;
        for (int i = 0; i < TIME; i++){
            cin >> dJI[j].Time[i];
            if (i < 18){
                if (dJI[j].Time[i] > dJI[j].LargestDemandInDay){
                    dJI[j].LargestDemandInDay = dJI[j].Time[i];
                    dJI[j].LargestTimeInDay = i;
                }
            }else if (i > 17){
                if (dJI[j].Time[i] > dJI[j].LargestDemandInNight){
                    dJI[j].LargestDemandInNight = dJI[j].Time[i];
                    dJI[j].LargestTimeInNight = i;
                }
            }
        }
        dJI[j].HowManyPeopleCanRestOnToday = nI - (dJI[j].LargestDemandInNight + dJI[j].LargestDemandInDay);
    }

    
    ///�𰲻ݨD�Aoriginal��input���榡�ARList���ڦۤv�ק�L���x�s�覡�C
    int originalRList[2][3100] = {{0}}, RList[PEOPLE][DAY+1] = {{0}};
        /* RList[i][j]
            i�G���@����u�Cj�G���Ӥ�l�C�Ʊ�𰲤��1�A�L�ݨD����l��0�C
            �ݪ`�N�G[i][0]��ܨ�Q�n�𰲪��Ѽ��`�q�C�ҥH��l�q[i][1]�}�l */
    for (int i = 0; i < 2; i++){ //original����
        for (int j = 0; j < R; j++){
            cin >> originalRList[i][j];
        }
    }
    for(int i = 0; i < R; i++){ //�ק�L������
        RList[originalRList[0][i]-1][originalRList[1][i]] = 1; //���u�s���q0�}�l�A������q1�}�l
        RList[originalRList[0][i]-1][0]++; //[i][0]�񪺬O�L�Q�n�𰲪����@���X��
    }


    ///staffShift���n�ƪ��Z��A[i][0]�Ψӭp��w�𰲪��ѼơFGoal���ؼШ�ơA[0]�������ʤu�H�ƪ����G�B[1]�������𰲪����G�B[2]�������]�Z�����G�A�̫��X�Ʀr�̤p�����@�Ӥ�k�C
    StaffShift staffShift[PEOPLE];
    
    for (int i = 0; i < nI; i++){ //���q��i��NstaffShift�k�s
        staffShift[i].lastTimeNightWork = 7;
        for (int j = 0; j < nJ; j++){
            staffShift[i].Day[j] = -1;
        }
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////�򥻳]�w����///////////////////////////////////////////////////////////////////////////////////////////////////
    
    for (int i = 0; i < nI; i++){
        if (RList[i][0] == 0){ // �S�����@��
            continue;
        }else{ //�����@�檺�H��
            for (int j = 1; j < nJ+1; j++){
                if (RList[i][j] == 1)
                    RestAssign(i, j-1, staffShift, dJI);
                if (staffShift[i].TotalRestDay == L)
                    break;
            }
            int continueWorkOver7Days[11][3] = {{0}};
            // �Ӱ}�C�|�O���s��W�Z�W�L6�Ѫ���l�C[i][0]���ӰϬq�_�l��A[i][1]���ӰϬq������
            
            if (WhichDayToWhichDayContinueWorkOver7Days(i, nJ, L, staffShift) == 1){ //�p�G�L�����@���X�z����
                for (int j = 0; j < nJ; j++){ //�������@
                    if (staffShift[i].Day[j] == 0)
                        RestCancel(i, j, staffShift, dJI);
                }
                int howManyRestCanYouWish = L - ((nJ - (L - nJ / 7))/7);
                int wishList[DAY] = {0}, wishListTimer = 0; //
                for (int j = 0; j < nJ; j ++){
                    if (RList[i][j] == 1){
                        wishList[wishListTimer] = j;
                        wishListTimer++;
                    }
                }
                int wishListHowManyPeopleCanRest[DAY] = {0}; //�x�s�PwishList�ѼƬ�������ѳҰʤO�̤j��
                for (int j = 0; wishList[j] != -1; j++){
                    wishListHowManyPeopleCanRest[j] = dJI[wishList[j]].HowManyPeopleCanRestOnToday;
                    if(j>=nJ)
                    {
                    	break;
					}
                }
                Sorting(wishListTimer, wishListHowManyPeopleCanRest, wishList);
                for (int j = wishListTimer-1; j < -1; j++){
                    if (dJI[wishList[wishListTimer]].HowManyPeopleCanRestOnToday > 0){
                        RestAssign(i, wishList[wishListTimer], staffShift, dJI);
                        howManyRestCanYouWish--; //�o��i�H�[�W��HowManyPeopleCanRestOnToday-1
                    }
                    if (howManyRestCanYouWish == 0)
                        break;
                }
            }
        }
    }

    ChooseShift(nI, nJ, nK, L, nKList, dJI, staffShift);

    
    for (int i = 0; i < nI; i++){
        while (staffShift[i].TotalRestDay < L){
            int changeOrNot = 0;
            for (int ii = nI-1; ii > i; ii--){
                if (staffShift[ii].TotalRestDay > L){
                    for (int j = 0; j < nJ; j++){
                        if (staffShift[i].Day[j] != 0 && staffShift[ii].Day[j] == 0 && RList[ii][j+1] != 1 && HowLongDoesHeOrSheNeedToWork(ii, j, nJ, nKList, staffShift) < 7){
                            RestCancel(ii, j, staffShift, dJI);
                            staffShift[ii].Day[j] = staffShift[i].Day[j];
                            RestAssign(i, j, staffShift, dJI);
                            changeOrNot = 1;
                            break;
                        }
                    }
                    if (changeOrNot == 1){
                        break;
                    }
                }
            }
            if (changeOrNot == 0 && staffShift[i].TotalRestDay < L){
                int smallestSatisFied = TIME, smallestDay = 0;
                for (int j = 0; j < nJ; j++){
                    if (staffShift[i].SatisfiedHowManyDemand[j] < smallestSatisFied && staffShift[i].Day[j] != 0){
                        smallestDay = j;
                        smallestSatisFied = staffShift[i].SatisfiedHowManyDemand[j];
                    }
                }
                RestAssign(i, smallestDay, staffShift, dJI);
            }
        }
    }
            
    for (int i = 0; i < nI; i++){
        for (int j = 0; j < nJ; j++){
            if (j == 0){
                cout << staffShift[i].Day[j];
            }else{
                cout << "," << staffShift[i].Day[j];
            }
        }
        cout << "\n";
    }

}



