#!/bin/bash
g++ main.cpp -o main_PreAndRec -std=c++11;
wait

./main_PreAndRec $1 0 &
./main_PreAndRec $1 1 &
./main_PreAndRec $1 2 &
./main_PreAndRec $1 3 &
./main_PreAndRec $1 4 &
./main_PreAndRec $1 5 &
./main_PreAndRec $1 6 &
./main_PreAndRec $1 7 &
./main_PreAndRec $1 8 &
./main_PreAndRec $1 9 &
./main_PreAndRec $1 10 &
./main_PreAndRec $1 11 &