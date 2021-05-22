#include <iostream>
#include <cmath>		
#include <dirent.h>	   
#include <stdio.h>
#include <stdlib.h>
#include <cstdio>
#include <fstream>
#include <map>
#include <omp.h>
#include <sstream>
#include <utility>

namespace TrafficHook{
    class infrastructure;
    class vehicle;
};

// PPPLib (опен суурс библиотека для работы с координатами и геопозицией)
// Реализует связь с хардвейрным интерфейсом


const TYPE = 1;
using namespace std;  
#include "v2i/i.cpp"
#include "v2v/v.cpp"

int main(int argc, char** argv) {
    // Инициализируем приложение
    // Если оно запушено на транспорте 
    if(TYPE == 1){
        TrafficHook::vehicle main_object;
        // Создаем демона в фоне для получения информации
        while(1){

        }
    // Если оно запушено на инфраструктуре
    }else if(TYPE == 2){
        TrafficHook::infrastructure main_object;
        // Создаем демона в фоне для получения информации
        while(1){

        }
    }
    return 0;
}