#include "./nthu_bike.h"
#include <cmath>
#include <array>
#include <fstream>
#include <algorithm>

float loss;
int maxride;
int map[1001][1001];
float bikeinfo[10001];
Bike *stop[1001];
User *people;
station_status *out1;
user_result *out2;
transfer_log *out3;

/*main*/ /*main*/ /*main*/ /*main*/ /*main*/ /*main*/ /*main*/ /*main*/ /*main*/ /*main*/ /*main*/ /*main*/ /*main*/ /*main*/ /*main*/ /*main*/ /*main*/ /*main*/

void basic(string selectedCase)
{
    // insert your code here
    cout << "start your basic version of data structure final from here!" << endl;

    readmap(selectedCase);

    // cout << "cute" << endl
    //<< endl;

    readbike(selectedCase);

    // cout << "cute2" << endl
    //<< endl;

    readbikeinfo(selectedCase);

    // cout << "cute3" << endl
    //<< endl;

    readuser(selectedCase);

    // cout << "cute4" << endl
    //<< endl;

    User *nowpeople = people;

    while (nowpeople)
    {
        int pass = 0;

        if (nowpeople->starttime + map[nowpeople->startstop][nowpeople->endstop] < nowpeople->endtime)
        {
            Bike *nowbike = stop[nowpeople->startstop];

            while (nowbike)
            {
                // cout << "bike" << nowbike->id << endl;

                if (nowpeople->biketype[nowbike->type] == 1)
                {
                    // cout << "in1 " << nowpeople->id << endl;
                    if (nowbike->oktime < nowpeople->starttime || (nowbike->oktime == nowpeople->starttime /*&& nowpeople->id >nowbike->befuserid*/))
                    {
                        // cout << "in2 " << nowpeople->id << endl;
                        if (nowbike->rentalcount < maxride)
                        {
                            // cout << "in3 " << nowpeople->id << endl;

                            // make output
                            insertuser_result(nowpeople->id, 1, nowbike->id, nowpeople->starttime, nowpeople->starttime + map[nowpeople->startstop][nowpeople->endstop], nowbike->rentalprice * (float)map[nowpeople->startstop][nowpeople->endstop]);
                            inserttransfer_log(nowbike->id, nowpeople->startstop, nowpeople->endstop, nowpeople->starttime, nowpeople->starttime + map[nowpeople->startstop][nowpeople->endstop], nowpeople->id);

                            // cout << "userid " << nowpeople->id << " bikeid " << nowbike->id << " starttime " << nowpeople->starttime << " good " << nowbike->oktime << " bef " << nowbike->befuserid << endl;

                            // next state
                            nowbike->oktime = nowpeople->starttime + map[nowpeople->startstop][nowpeople->endstop];
                            nowbike->befuserid = nowpeople->id;
                            nowbike->rentalprice -= loss;
                            nowbike->rentalcount++;
                            removebike(nowbike, nowpeople->startstop);
                            insertbike(nowbike, nowpeople->endstop);

                            pass = 1;
                            break;
                        }
                    }
                }

                nowbike = nowbike->next;
            }
        }

        if (pass == 0)
        {
            insertuser_result(nowpeople->id, 0, 0, 0, 0, 0);
        }

        nowpeople = nowpeople->next;
    }

    outputstation_status_basic(selectedCase);
    outputuser_result_basic(selectedCase);
    outputtransfer_log_basic(selectedCase);
}

void readmap(string selectedCase)
{
    std::ifstream in;

    // char *word = new char[30000];
    char word[100000];
    string good = "testcases/" + selectedCase + "/map.txt";

    // input map

    // word.fill('\0');

    in.open(good);

    if (!in.is_open())
    {
        cout << "not find map" << endl;
    }

    in.read(word, sizeof(word));

    // cout << word;

    // storage map

    int k = 0;
    int isfrom = 1;
    int countword = 0;

    int from, to;

    for (int i = 0; i < 1001; i++)
    {
        for (int j = 0; j < 1001; j++)
        {
            map[i][j] = 0;
        }
    }

    while (word[k] != '\0')
    {
        if (word[k] == 'S')
        {
        }
        else if (word[k] == ' ' || word[k] == '\n')
        {
            int number = 0;

            number = readint(word, k, countword);

            if (isfrom == 1)
            {
                from = number;
                isfrom++;
            }
            else if (isfrom == 2)
            {
                to = number;
                isfrom++;
            }
            else
            {
                map[from][to] = number;
                map[to][from] = number;
                number = 0;
                isfrom = 1;
            }

            countword = 0;
        }
        else
        {
            countword++;
        }

        k++;
    }

    /*for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)

        {
            cout << map[i][j] << " ";
        }
        cout << endl;
    }
    cout << "bef" << endl;*/

    // sort map

    for (int p = 0; p < 1001; p++)
    {
        for (int q = 0; q < 1001; q++)
        {
            for (int r = 0; r < 1001; r++)
            {
                if (map[q][p] + map[p][r] < map[q][r] && map[q][p] > 0 && map[p][r] > 0 && map[q][r] > 0)
                {
                    map[q][r] = map[q][p] + map[p][r];
                }
            }
        }
    }

    /*for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)

        {
            cout << map[i][j] << " ";
        }
        cout << endl;
    }*/

    // delete word;
}

void readbike(string selectedCase)
{
    std::ifstream in;

    // char *word = new char[300000];
    char word[500000];
    string good = "testcases/" + selectedCase + "/bike.txt";

    // input bike

    in.open(good);

    if (!in.is_open())
    {
        cout << "not find bike" << endl;
    }

    in.read(word, sizeof(word));

    // cout << word;

    // storage bike

    int k = 0;
    int white = 0;
    int countword = 0;
    int startfloat = -1;

    int type, id, stationid, rentalcount;
    float rentalprice;

    while (word[k] != '\0')
    {
        if (word[k] == 'B' || word[k] == 'S')
        {
        }
        else if (word[k] == ' ' || word[k] == '\n')
        {
            int number = 0;
            float numberfloat = 0.0;

            // read

            if (white != 3)
            {
                number = readint(word, k, countword);
            }
            else if (startfloat != -1)
            {
                numberfloat = readfloat(word, k, countword, startfloat);

                startfloat = -1;
            }
            else
            {
                numberfloat = readint(word, k, countword);
            }

            // storage

            if (white == 0)
            {
                type = number;
            }
            else if (white == 1)
            {
                id = number;
            }
            else if (white == 2)
            {
                stationid = number;
            }
            else if (white == 3)
            {
                rentalprice = numberfloat;
            }
            else if (white == 4)
            {
                rentalcount = number;

                Bike *temp = new Bike;
                temp->type = type;
                temp->id = id;
                temp->rentalprice = rentalprice;
                temp->rentalcount = rentalcount;
                temp->oktime = -1;
                temp->befuserid = -1;
                temp->next = NULL;

                // cout << temp->type << " " << temp->id << " " << stationid << " " << temp->rentalprice << " " << temp->rentalcount << endl;

                insertbike(temp, stationid);

                white = -1;
            }

            countword = 0;
            white++;
        }
        else if (word[k] == '.')
        {
            countword++;
            startfloat = 0;
        }
        else
        {
            countword++;

            if (startfloat >= 0)
            {
                startfloat++;
            }
        }

        k++;
    }

    // delete word;
}

void readbikeinfo(string selectedCase)
{
    std::ifstream in;

    // char *word = new char[30000];
    char word[100000];
    string good = "testcases/" + selectedCase + "/bike_info.txt";

    // input bike_info

    in.open(good);

    if (!in.is_open())
    {
        cout << "not find bike_info" << endl;
    }

    in.read(word, sizeof(word));

    // cout << word;

    // storage bike

    int k = 0;
    int white = 0;
    int countword = 0;
    int countnextline = 0;
    int startfloat = -1;

    // float loss;
    // int maxridetime;
    int stationstop;
    float price;

    while (word[k] != '\0')
    {
        if (word[k] == 'B')
        {
        }
        else if (word[k] == ' ')
        {
            stationstop = readint(word, k, countword);

            countword = 0;
            startfloat = -1;
        }
        else if (word[k] == '\n' && countnextline == 0)
        {
            if (startfloat >= 0)
            {
                loss = readfloat(word, k, countword, startfloat);
            }
            else
            {
                loss = (float)readint(word, k, countword);
            }

            countword = 0;
            startfloat = -1;
            countnextline++;
        }
        else if (word[k] == '\n' && countnextline == 1)
        {
            maxride = readint(word, k, countword);

            countword = 0;
            startfloat = -1;
            countnextline++;
        }
        else if (word[k] == '\n')
        {
            bikeinfo[stationstop] = (float)readint(word, k, countword);
            // cout << "in" << stationstop << " " << bikeinfo[stationstop] << endl;

            countword = 0;
            startfloat = -1;
        }
        else if (word[k] == '.')
        {
            countword++;
            startfloat = 0;
        }
        else
        {
            countword++;

            if (startfloat >= 0)
            {
                startfloat++;
            }
        }

        k++;
    }

    /*cout << "loss " << loss << " maxride " << maxride << endl;
    for (int i = 0; i < 3; i++)
    {
        cout << i << " " << bikeinfo[i] << endl;
    }*/

    // delete word;
}

void readuser(string selectedCase)
{
    std::ifstream in;

    // char *word = new char[300000];
    char word[2000000];
    string good = "testcases/" + selectedCase + "/user.txt";

    // input bike_info

    in.open(good);

    if (!in.is_open())
    {
        cout << "not find user" << endl;
    }

    in.read(word, sizeof(word));

    // cout << word;

    // storage bike

    int k = 0;
    int white = 0;
    int countword = 0;

    int id, starttime, endtime, startstop, endstop, stationid, rentalcount;
    int biketype[10001] = {0};

    while (word[k] != '\0')
    {
        if (word[k] == 'B' || word[k] == 'U' || word[k] == 'S')
        {
        }
        else if (word[k] == ' ' || word[k] == ',' || word[k] == '\n')
        {
            int number = 0;

            // read

            number = readint(word, k, countword);

            // storage

            if (white == 0)
            {
                id = number;
            }
            else if (white == 1)
            {
                biketype[number] = 1;
            }
            else if (white == 2)
            {
                starttime = number;
            }
            else if (white == 3)
            {
                endtime = number;
            }
            else if (white == 4)
            {
                startstop = number;
            }
            else if (white == 5)
            {
                endstop = number;

                User *temp = new User;
                temp->id = id;
                temp->starttime = starttime;
                temp->endtime = endtime;
                temp->startstop = startstop;
                temp->endstop = endstop;
                for (int i = 0; i < 10001; i++)
                {
                    temp->biketype[i] = biketype[i];
                }
                temp->next = NULL;

                // cout << temp->id << " " << temp->biketype[0] << " " << temp->biketype[1] << " " << temp->biketype[2] << " " << temp->starttime << " " << temp->endtime << " " << temp->startstop << " " << temp->endstop << endl;

                insertuser(temp);

                for (int i = 0; i < 10001; i++)
                {
                    biketype[i] = 0;
                }

                white = 0;
            }

            countword = 0;

            if (word[k] == ' ')
            {
                white++;
            }
        }
        else
        {
            countword++;
        }

        k++;
    }
}

int readint(char *word, int k, int countword)
{
    int number = 0;

    for (int j = 1; j <= countword; j++)
    {
        number += (((int)word[k - j] - '0') * pow(10, j - 1));
    }

    return number;
}

float readfloat(char *word, int k, int countword, int startfloat)
{
    float number = 0.0;

    for (int j = 1; j <= startfloat; j++)
    {
        number += ((float)((int)word[k - j] - '0') / (float)pow(10, (startfloat + 1) - j));
    }

    for (int j = startfloat + 2; j <= countword; j++)
    {
        number += (float)(((int)word[k - j] - '0') * pow(10, j - (startfloat + 2)));
    }

    // numberfloat = numberfloat + (float)number;

    return number;
}

void insertbike(Bike *temp, int stationid)
{
    Bike *search = stop[stationid];

    if (search)
    {
        if (temp->rentalprice > search->rentalprice || ((temp->rentalprice == search->rentalprice) && (temp->id < search->id)))
        {
            Bike *before = search;
            stop[stationid] = temp;
            temp->next = before;

            return;
        }

        while (search->next && (temp->rentalprice < search->next->rentalprice || ((temp->rentalprice == search->next->rentalprice) && (temp->id > search->next->id))))
        {
            search = search->next;
        }

        Bike *before = search->next;
        search->next = temp;
        temp->next = before;

        // search->next = temp;
    }
    else
    {
        stop[stationid] = temp;
        temp->next = NULL;
    }
}

void removebike(Bike *temp, int stationid)
{
    Bike *search = stop[stationid];

    if (search && search == temp)
    {
        stop[stationid] = temp->next;
        temp->next = NULL;

        return;
    }
    /*else
    {
        cout << "not found bike remove" << endl;
        return;
    }*/

    while (search->next != temp)
    {
        search = search->next;
    }

    search->next = temp->next;
    temp->next = NULL;
}

void insertuser(User *temp)
{
    User *search = people;

    if (search)
    {
        if (temp->starttime < search->starttime || ((temp->starttime == search->starttime) && (temp->id < search->id)))
        {
            User *before = search;
            people = temp;
            temp->next = before;

            return;
        }

        while (search->next && (temp->starttime > search->next->starttime || ((temp->starttime == search->next->starttime) && (temp->id > search->next->id))))
        {
            search = search->next;
        }

        User *before = search->next;
        search->next = temp;
        temp->next = before;
    }
    else
    {
        people = temp;
    }
}

/*void insertstation_status(int stationid, int bikeid, int type, int rentalprice, int rentalcount)
{
    station_status *search = out1;
    station_status *temp = new station_status;

    temp->stationid = stationid;
    temp->bikeid = bikeid;
    temp->type = type;
    temp->rentalprice = rentalprice;
    temp->rentalcount = rentalcount;
    temp->next = NULL;

    if (search)
    {
        if (bikeid < search->bikeid)
        {
            station_status *before = search;
            out1 = temp;
            temp->next = before;

            return;
        }

        while (search->next && (bikeid > search->next->bikeid))
        {
            search = search->next;
        }

        station_status *before = search->next;
        search->next = temp;
        temp->next = before;
    }
    else
    {
        out1 = temp;
    }
}*/

void insertuser_result(int userid, int isrent, int bikeid, int starttime, int endtime, int revenue)
{
    user_result *search = out2;
    user_result *temp = new user_result;

    temp->userid = userid;
    temp->isrent = isrent;
    temp->bikeid = bikeid;
    temp->starttime = starttime;
    temp->endtime = endtime;
    temp->revenue = revenue;
    temp->next = NULL;

    if (search)
    {
        if (userid < search->userid)
        {
            user_result *before = search;
            out2 = temp;
            temp->next = before;

            return;
        }

        while (search->next && (userid > search->next->userid))
        {
            search = search->next;
        }

        user_result *before = search->next;
        search->next = temp;
        temp->next = before;
    }
    else
    {
        out2 = temp;
    }
}

void inserttransfer_log(int bikeid, int startstop, int endstop, int starttime, int endtime, int userid)
{
    transfer_log *search = out3;
    transfer_log *temp = new transfer_log;

    temp->bikeid = bikeid;
    temp->startstop = startstop;
    temp->endstop = endstop;
    temp->starttime = starttime;
    temp->endtime = endtime;
    temp->userid = userid;
    temp->next = NULL;

    if (search)
    {
        if (userid < search->userid)
        {
            transfer_log *before = search;
            out3 = temp;
            temp->next = before;

            return;
        }

        while (search->next && (userid > search->next->userid))
        {
            search = search->next;
        }

        transfer_log *before = search->next;
        search->next = temp;
        temp->next = before;
    }
    else
    {
        out3 = temp;
    }
}

void outputstation_status_basic(string selectedCase)
{
    std::ofstream out;

    out.open("result_basic/" + selectedCase + "/my_station_status.txt");

    if (!out.is_open())
    {
        cout << "Failed to open file.\n";
        return;
    }

    for (int i = 0; i < 1001; i++)
    {
        Bike *temp = stop[i];

        Bike *sort = NULL;

        while (temp)
        {
            Bike *newtemp = new Bike;

            newtemp->type = temp->type;
            newtemp->id = temp->id;
            newtemp->oktime = temp->oktime;
            newtemp->befuserid = temp->befuserid;
            newtemp->rentalprice = temp->rentalprice;
            newtemp->rentalcount = temp->rentalcount;
            newtemp->next = NULL;

            // cout << "idid" << temp->id << endl;

            if (sort)
            {
                // cout << "pass1 " << temp->id << endl;

                if (newtemp->id < sort->id)
                {
                    Bike *before = sort;
                    sort = newtemp;
                    newtemp->next = before;
                }

                Bike *sorttemp = sort;

                while (sorttemp->next && (newtemp->id > sorttemp->next->id))
                {
                    sorttemp = sorttemp->next;
                }

                Bike *before = sorttemp->next;
                sorttemp->next = newtemp;
                newtemp->next = before;
            }
            else
            {
                // cout << "pass2 " << temp->id << endl;
                sort = newtemp;
            }

            // cout << "pass" << temp->id << endl;

            temp = temp->next;
        }

        while (sort)
        {
            out << "S" << i << " " << sort->id << " B" << sort->type << " " << sort->rentalprice << " " << sort->rentalcount << endl;

            sort = sort->next;
        }

        /*while (temp)
        {
            out << "S" << i << " " << temp->id << " " << temp->type << " " << temp->rentalprice << " " << temp->rentalcount << endl;

            temp = temp->next;
        }*/
    }

    out.close();
}

void outputuser_result_basic(string selectedCase)
{
    std::ofstream out;

    out.open("result_basic/" + selectedCase + "/my_user_result.txt");

    if (!out.is_open())
    {
        cout << "Failed to open file.\n";
        return;
    }

    user_result *temp = out2;

    while (temp)
    {
        out << "U" << temp->userid << " " << temp->isrent << " " << temp->bikeid << " " << temp->starttime << " " << temp->endtime << " " << temp->revenue << endl;

        temp = temp->next;
    }

    out.close();
}

void outputtransfer_log_basic(string selectedCase)
{
    std::ofstream out;

    out.open("result_basic/" + selectedCase + "/my_transfer_log.txt");

    if (!out.is_open())
    {
        cout << "Failed to open file.\n";
        return;
    }

    transfer_log *temp = out3;

    while (temp)
    {
        out << "" << temp->bikeid << " S" << temp->startstop << " S" << temp->endstop << " " << temp->starttime << " " << temp->endtime << " U" << temp->userid << endl;

        temp = temp->next;
    }

    out.close();
}