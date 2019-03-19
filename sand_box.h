
 const byte animation[][2]={
{7,0},{8,0},{6,0},{9,0},{7,1},{8,1},{10,0},{5,0},{4,0},{11,0},{6,1},{9,1},{7,2},{8,2},{3,0},{12,0},{10,1},{5,1},{6,2},{9,2},{7,3},{8,3},{2,0},{13,0},{4,1},
{11,1},{1,1},{14,1},{12,1},{3,1},{5,2},{10,2},{4,2},{11,2},{2,1},{13,1},{12,2},{3,2},{7,4},{8,4},{6,3},{9,3},{5,3},{10,3},{1,2},{14,2},{2,3},{13,3},{1,2},{14,2},{4,4},{3,3},{12,3},
{11,4},{9,5},{6,5},{7,6},{8,6},{3,4},{12,4},{1,3},{14,3},{10,5},{5,5},{6,6},{9,6},{7,7},{8,7},{2,4},{13,4},{14,4},{1,4},{4,5},{11,5},{10,6},{5,6},{3,5},{12,5},{4,4},{11,4},{5,4},{10,4},
{11,6},{4,6},{6,7},{9,7},{7,8},{8,8},{2,5},{13,5},{5,7},{10,7},{3,6},{12,6},{6,8},{9,8},{4,7},{11,7},{12,7},{3,7},{7,9},{8,9},{5,8},{10,8},{6,9},{9,9},{7,10},
{8,10},{4,8},{11,8},{5,9},{10,9},{6,10},{9,10},{4,9},{11,9},{7,11},{8,11},{5,10},{10,10},{6,11},{9,11},{6,12},{9,12}
};
static byte k=0;
const bool RG[512] ={
////    0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31
/*0*/   1,1,1,1,1,0,0,0,0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1,
/*1*/   1,0,0,0,0,1,0,0,0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1,
/*2*/   1,0,0,0,0,0,1,1,0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1,
/*3*/   1,0,0,0,0,0,0,0,1,1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
/*4*/   1,0,0,0,0,0,0,0,0,0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
/*5*/   1,0,0,0,0,0,0,0,0,0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
/*6*/   1,0,0,0,0,0,0,0,0,0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
/*7*/   1,1,1,1,1,1,1,1,1,1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
/*8*/   1,1,1,1,1,1,1,1,1,1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
/*9*/   1,0,0,0,0,0,0,0,0,0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
/*10*/  1,0,0,0,0,0,0,0,0,0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
/*11*/  1,0,0,0,0,0,0,0,0,0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
/*12*/  1,0,0,0,0,0,0,0,1,1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
/*13*/  1,0,0,0,0,0,1,1,0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1,
/*14*/  1,0,0,0,0,1,0,0,0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1,
/*15*/  1,1,1,1,1,0,0,0,0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1
};
const bool test[512] ={
////    0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31
/*0*/   1,1,1,1,1,0,0,0,0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1,
/*1*/   1,1,1,0,0,1,0,0,0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1,
/*2*/   1,1,1,1,0,0,1,1,0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1,
/*3*/   1,1,1,1,1,0,0,0,1,1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1,
/*4*/   1,1,1,1,1,0,0,0,0,0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
/*5*/   1,1,1,1,1,1,0,0,0,0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
/*6*/   1,1,1,1,1,1,1,0,0,0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
/*7*/   1,1,1,1,1,1,1,1,1,1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
/*8*/   1,1,1,1,1,1,1,1,1,1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
/*9*/   1,1,1,1,1,1,1,0,0,0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
/*10*/  1,1,1,1,1,1,0,0,0,0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
/*11*/  1,1,1,1,1,0,0,0,0,0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
/*12*/  1,1,1,1,0,0,0,0,1,1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1,
/*13*/  1,1,1,0,0,0,1,1,0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1,
/*14*/  1,1,1,0,0,1,0,0,0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1,
/*15*/  1,1,1,1,1,0,0,0,0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1
};

 
