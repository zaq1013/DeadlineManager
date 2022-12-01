#include <iostream>
using namespace std;

test
test22222

//////////////////////////////////////////////////////////////////////定義Array的大小
const int NKLEN = 31, DAY = 31, TIME = 24, PEOPLE = 100;
//因為NK最多為30個（但還有休假班表，所以取31），而一個月最多就是31天，一天有24個時段。員工數量最多為100人。


////////////////////////////////////////////////////////////////////基本資料儲存結構開始
struct NKList{ //第i份班型的相關資料
    int DayShiftOrNot; //日班：1；夜班：-1
    int WorkingScedule[TIME]; //該班表有哪些時間在工作。工作：1，沒工作：0
    int workingHour; //該班表的總共工作時間
};

struct StaffShift{ //第i位員工的相關資料
    int Day[DAY]; //每日班型
    int SatisfiedHowManyDemand[DAY]; //記錄每天滿足了多少勞動需求（用來安排每個月休息Ｌ天時的函式）
    int TotalRestDay; //總共休息天數
    int ContinueWorking; // > 6 即要休息
    int lastTimeNightWork; // > 6 即可以安排夜班
};

struct DJI{
    int Time[TIME];
    int LargestTimeInDay;
    int LargestDemandInDay;
    int LargestTimeInNight;
    int LargestDemandInNight;
    int HowManyPeopleCanRestOnToday;
};
////////////////////////////////////////////////////////////////////基本資料儲存結構結束




////////////////////////////////////////////////////////////////////比較簡單的函式開始

//依序為：是否每個員工都有達成休息L天的條件、放入休假、取消休假、目前i員工第幾天到第幾天連續上班超過7天、將一個陣列從小排到大、當天有多少為員工可以進行夜班、當天還有多少員工可以上班、日或夜班是否仍有勞動需求、目前需求人數最少的時段、最少需求的時段有哪些、哪個班型可以滿足這些最少需求時段、如果把這個人第j天的休假給取消掉的話，這個人離下次休假還有多少天

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
    //本函式專門用來放置休假
    //因為staffShift[i][0]專門用來放置目前的休假次數，所以請都用本函式來安排休假
    staffShift[i].Day[j] = 0;
    staffShift[i].TotalRestDay++;
    dJI[j].HowManyPeopleCanRestOnToday--;
}

void RestCancel (int i, int j, StaffShift staffShift[PEOPLE], DJI dJI[DAY]){
    //本函式專門用來取消休假
    //因為staffShift[i][0]專門用來放置目前的休假次數，所以請都用本函式來取消休假
    staffShift[i].Day[j] = -1;
    staffShift[i].TotalRestDay--;
    dJI[j].HowManyPeopleCanRestOnToday++;
}

bool WhichDayToWhichDayContinueWorkOver7Days (int i, int nJ, int L, StaffShift staffShift[PEOPLE]){
    //本函式專門用來與DesignedRest配合
    //本函式用來查看員工有哪幾天工作超過7天，並且會順便運算其休假心願是否合理（是否剩下的休假天數可以支持每個月要休假L天並且每7天休一次的規定）。
    //本函式回傳值為布林值，若該名員工目前的休假是合理的，則將回傳0。會直接將答案裝在ocntinueWorkOver7Days中
    //注意：需要在呼叫的地點宣告「int continueWorkOver7Days[11][3] = {{0}}; // 該陣列會記錄連續上班超過6天的日子。[i][0]為該區段起始日，[i][1]為該區段結束日，[i][2]為總共連續工作多少天數」
    /*執行邏輯：
     (1) 查看兩個休假之間的連續工作日是否超過六天
     (2) 超過六天便將該區段的起始放在continueWorkOver7Days[i][0]，結尾放在continueWorkOver7Days[i][1]，連續工作天數放在[i][2]
     (3) 若timer == 0，意味著休假正常，沒有問題。若timer != 0，則意味著需要進行檢查是否可以達成條件
     (4) 若可以達成條件，則回傳0。若發現無法達成條件，則回傳1
    */
    int workingDays = 0, timer = 0 , LEdit = L - staffShift[i].TotalRestDay; // workingDays紀錄的是連續上班日數，timer用來記錄要將資料存取到第幾行，LEdit用來看剩餘的休假數量
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
    
    if (LEdit >= 0){ //若要達成7天休一天的話，可以恰好將假休完，又或者是甚至還有假可以休
        return 0;
    }else{
        return 1; //把假休完後還是沒辦法達成其心願，或者是想休的假超出規定的L天
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
    //要記得在呼叫地宣告int intervals[TIME] = {-1};
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
    //i為員工編號，j為日期
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

////////////////////////////////////////////////////////////////////比較簡單的函式結束


////////////////////////////////////////////////////////////////////比較重要的函式開始
///依序為：如何休假、如何選擇當日班型

///如何休假
void RestDesigned(int nI, int nJ, int nK, int L, DJI dJI[], int RList[][TIME+1], StaffShift staffShift[]){
    //本函式用來先分配休假，以盡量滿足想要休息的員工的願望為主要目標
    /*執行邏輯：
     (1) 想要休5天以下的人可以隨意的休假，但想要修6天以上的人需要看他們的休假心願是否合理。
         畢竟，員工一個月休假5天就可以滿足每7天休1次的規則了，而一個月可能會放8-10的假，並且一個月最多就是31天
         ex. 不合理的狀況：休假的時間全部都擠在一起想要讓自己每個月都放農曆春假的人類
     (2) 對於想要休6天以上的人，先讓他們休當天需要人數最少的那幾天，如果當天需要人數已經等於可工作的人數，則往下一個需要人數最少的那天去做選擇。
         比如說：當nI = 40，如果大家都想要休5號，而5號那天的最高的需求人數為20人，那麼當我們已經放了20位員工休5號時，就不能再讓其他員工休5號了。
    */
 
    for (int i = 0; i < nI; i++){
        if (RList[i][0] == 0){ // 沒有休假願望
            continue;;
        }else{ //有休假願望的人們
            for (int j = 1; j < nJ+1; j++){
                if (RList[i][j] == 1)
                    RestAssign(i, j-1, staffShift, dJI);
                if (staffShift[i].TotalRestDay == L)
                    break;
            }
            
            if (WhichDayToWhichDayContinueWorkOver7Days(i, nJ, L, staffShift) == 1){ //如果他的心願不合理的話
                for (int j = 0; j < nJ; j++){ //取消心願
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
                int wishListHowManyPeopleCanRest[DAY] = {0}; //儲存與wishList天數相應的當天勞動力最大值
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
    //本函式用來安排當天的班表
    /*
     執行邏輯：
     (1) 一天一天做安排，安排好了之後就會立即將當日的班表派發下去。
     (2) 安排前須確認是否有員工已經工作了6天，有的話請他休息。
     (3) 每日都從夜班人員開始進行安排。（終止條件為可執行夜班的人數為0或者夜班條件都已經被滿足）
        a. 首先查看目前可以執行夜班的人數
        b. 從夜班需要人數最少的時段開始安排，並且查看夜班時段是否有其他人數也是如此少的時段，尋找可以涵蓋這些時段的班型並且工作時間最長的夜班班型。
        c. 將班型安排下去後，倘若此時仍有人手可以安排夜班，則再查看夜班其他時段人數最少的時刻。（即回到步驟a.）
     (4) 夜班班型完成後，開始安排日班。（終止條件為所有人當天都被安排到班型，也就是班型表上沒有-1）
        a. 首先查看日班需要人數最短的時間段，若有多個時間段都只需要最少的人手，則尋找是否有涵蓋這些時間段的班型
        b. 尋找好班表過後，一一分派下去，並且再查看目前人數最少的時段。（回到步驟a.）
     */
    
    for (int j = 0; j < nJ; j++){ //從第j天開始進行安排
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
        while (HMPCWAN > 0 && ADL > 0){ // 夜班
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
        while (HMPSCW > 0 && ADL > 0){ // 日班
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

////////////////////////////////////////////////////////////////////比較重要的函式結束


int main(){
    //////////////////////////////////////////////////////////////////////////////////////////////////////基本設定///////////////////////////////////////////////////////////////////////////////////////////////////
    
    int nI = 0, nJ = 0, nK = 0, L = 0, w1 = 0, w2 = 0, R = 0;
        //nI = 員工數量, nJ = 安排天數, nK = 班型數量-1, L = 在nJ天中，要有L天休假。
        //w1 = 未滿足的休假需求的權重, w2 = 超額夜班的權重, R = 休假需求數量。
    cin >> nI >> nJ >> nK >> L >> w1 >> w2 >> R;
    nK++;
        
    
    ///可以使用的班型
    NKList nKList[NKLEN];
    for (int i = 1; i < nK+1; i++){ //從第i份班型資料開始儲存
        
        nKList[i].DayShiftOrNot = 1; //先標上日班，如果發現是夜班的話再更改
        nKList[i].workingHour = 0; //先設定為0，之後再慢慢加上去
        
        for (int j = 0; j < TIME; j++){
            if (i == nK){ //存班型
                cin >> nKList[0].WorkingScedule[j];
            }else{
                cin >> nKList[i].WorkingScedule[j];
            }
            
            if (nKList[i].WorkingScedule[j] == 1){ //確定該班型的工作時間以及是不是夜班
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

    

    ///每日時段人數需求表
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

    
    ///休假需求，original為input的格式，RList為我自己修改過的儲存方式。
    int originalRList[2][3100] = {{0}}, RList[PEOPLE][DAY+1] = {{0}};
        /* RList[i][j]
            i：哪一位員工。j：哪個日子。希望休假日＝1，無需求的日子＝0。
            需注意：[i][0]表示其想要休假的天數總量。所以日子從[i][1]開始 */
    for (int i = 0; i < 2; i++){ //original版本
        for (int j = 0; j < R; j++){
            cin >> originalRList[i][j];
        }
    }
    for(int i = 0; i < R; i++){ //修改過的版本
        RList[originalRList[0][i]-1][originalRList[1][i]] = 1; //員工編號從0開始，但日期從1開始
        RList[originalRList[0][i]-1][0]++; //[i][0]放的是他想要休假的心願有幾個
    }


    ///staffShift為要排的班表，[i][0]用來計算已休假的天數；Goal為目標函數，[0]為重視缺工人數的結果、[1]為重視休假的結果、[2]為重視夜班的結果，最後輸出數字最小的那一個方法。
    StaffShift staffShift[PEOPLE];
    
    for (int i = 0; i < nI; i++){ //先從第i位將staffShift歸零
        staffShift[i].lastTimeNightWork = 7;
        for (int j = 0; j < nJ; j++){
            staffShift[i].Day[j] = -1;
        }
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////基本設定結束///////////////////////////////////////////////////////////////////////////////////////////////////
    
    for (int i = 0; i < nI; i++){
        if (RList[i][0] == 0){ // 沒有休假願望
            continue;
        }else{ //有休假願望的人們
            for (int j = 1; j < nJ+1; j++){
                if (RList[i][j] == 1)
                    RestAssign(i, j-1, staffShift, dJI);
                if (staffShift[i].TotalRestDay == L)
                    break;
            }
            int continueWorkOver7Days[11][3] = {{0}};
            // 該陣列會記錄連續上班超過6天的日子。[i][0]為該區段起始日，[i][1]為該區段結束日
            
            if (WhichDayToWhichDayContinueWorkOver7Days(i, nJ, L, staffShift) == 1){ //如果他的心願不合理的話
                for (int j = 0; j < nJ; j++){ //取消心願
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
                int wishListHowManyPeopleCanRest[DAY] = {0}; //儲存與wishList天數相應的當天勞動力最大值
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
                        howManyRestCanYouWish--; //這邊可以加上讓HowManyPeopleCanRestOnToday-1
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



