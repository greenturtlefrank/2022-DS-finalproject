#include <iostream>
using namespace std;

void basic(string selectedCase);
void advanced(string selectedCase);

/*basic*/

typedef struct Node1
{
    int type;
    int id;
    int oktime;
    int befuserid;

    float rentalprice;
    int rentalcount;

    Node1 *next;
} Bike;

typedef struct Node2
{
    int id;
    int starttime;
    int endtime;
    int startstop;
    int endstop;
    int biketype[10001] = {0};

    Node2 *next;
} User;

typedef struct Node3
{
    int stationid;
    int bikeid;
    int type;
    int rentalprice;
    int rentalcount;

    Node3 *next;
} station_status;

typedef struct Node4
{
    int userid;
    int isrent;
    int bikeid;
    int starttime;
    int endtime;
    int revenue;

    Node4 *next;
} user_result;

typedef struct Node5
{
    int bikeid;
    int startstop;
    int endstop;
    int starttime;
    int endtime;
    int userid;

    Node5 *next;
} transfer_log;

// char word[30000000];

void readmap(string);
void readbike(string);
void readbikeinfo(string);
void readuser(string);
int readint(char *, int, int);
float readfloat(char *, int, int, int);
void insertbike(Bike *, int);
void removebike(Bike *, int);
void insertuser(User *);
// void insertstation_status(int stationid, int bikeid, int type, int rentalprice, int rentalcount);
void insertuser_result(int userid, int isrent, int bikeid, int starttime, int endtime, int revenue);
void inserttransfer_log(int bikeid, int startstop, int endstop, int starttime, int endtime, int userid);
void outputstation_status_basic(string selectedCase);
void outputuser_result_basic(string selectedCase);
void outputtransfer_log_basic(string selectedCase);

/*advance*/

void readmap_advance(string);
void readbike_advance(string);
void readbikeinfo_advance(string);
void readuser_advance(string);
int readint_advance(char *, int, int);
float readfloat_advance(char *, int, int, int);
void insertbike_advance(Bike *, int);
void removebike_advance(Bike *, int);
void insertuser_advance(User *);
// void insertstation_status_advance(int stationid, int bikeid, int type, int rentalprice, int rentalcount);
void insertuser_result_advance(int userid, int isrent, int bikeid, int starttime, int endtime, int revenue);
void inserttransfer_log_advance(int bikeid, int startstop, int endstop, int starttime, int endtime, int userid);
void outputstation_status_advance(string selectedCase);
int outputuser_result_advance(string selectedCase);
void outputtransfer_log_advance(string selectedCase);