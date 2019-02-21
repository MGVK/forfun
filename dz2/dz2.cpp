




/*  
 * create by Mike Bayandin
 * 
 * used compiler: g++
 * 
 * neccesary compile options: -masm=intel
 * 
 * */


#include <stdio.h>
#include <iostream>

using namespace std;
void processCPP(unsigned char* str)
{
       unsigned char buf;
    for (unsigned char i = 1; i < 255; i++) 
    {
        buf = str[i-1];
        str[i-1] = str[i];
        str[i] = buf;
    }
}


void processASM(unsigned char* str)
{
    
__asm__(
".intel_syntax noprefix\t\n" // Вот эта строчка убирает необходимость писать % перед именем регистра. Вообще оказывается по-умолчанию синтаксис ассемблера здесь не intel, а AT&T
"   pushf\t\n" // после каждой команды нужно писать \t\n, чтобы оно скушалось нормально. Никакие точки с запятой - ТОЛЬКО \t\n ТОЛЬКО ХАРДКОР
"	push rsi\t\n"
"	push rbx\t\n"
"	push rcx\t\n"

"	mov sil, al\t\n"
"	dec rsi\t\n"  //НЕ ЗНАЮ ПОЧЕМУ, НО БЕЗ ДЕКРЕМЕНТА ЗДЕСЬ НЕ РАБОТАЕТ!!! хотя по идее, должен быть -1 индекс, но оказывается, это 1 индекс. Магия размерностей в памяти.
"   mov rcx, 254\t\n"
"cycle:\t\n"
"	mov dl, WORD[rsi-1]\t\n" // неверным решением сначала было использование 64-битных регистров. 
								//А 8-битные можно, но осторожно и только с указанием размера (WORD), иначе все крашится к ... , ну вы поняли.
"	mov bl, WORD[rsi]\t\n"
"	mov WORD[rsi],dl\t\n"
"	mov WORD[rsi-1],bl\t\n"

"	inc rsi\t\n"
"	dec rcx\t\n"
"	jnz cycle\t\n"

"	pop rcx\t\n"
"	pop rbx\t\n"
"	pop rsi\t\n"
"	popf\t\n"
	:"=a"(str)
	:"a"(str)   //вот тут вообще магия. можно указать, какой регистр будет принимать и возвращать значения. В моем случае - регистр A. Там, где =a - это возрврат, а второе - это прием.
	//Можно сделать 	:"=r"(str):"a"(str), тогда возвращать будет через регистр R.
);

}

// Функция инициализации содержания массива для теста 1
void init_mas_test1(int nelems, unsigned char* mas)
{
    float val;
    val = 0;
    for (int i = 0; i < nelems; i++)
    {
        mas[i] = val;
        val += 1;
    }
}


// Функция сравнения содержания двух массивов
bool compare_masses(int nelems, unsigned char* mas1, unsigned char* mas2)
{
    bool res = true;

    for (int i = 0; i < nelems; i++)
    {
        if (mas1[i] != mas2[i])
        {
            res = false;
        }
    }
    return res;
}


int main()
{
    unsigned char masCPP[256];
    unsigned char masASM[256];
    //unsigned char resultCPP, resultASM;
    int ntest;
    bool massesMatched = false;

    for (ntest = 1; ntest <= 3; ++ntest)
    {
        printf("--------------- Test no. %d ---------------\n", ntest);

        // Заполнение массива исходными данными
        // TODO: Разработать и инициализировать тестовые наборы исходных 
        // данных для проверки корректности работы программы 
        // в трех различных случаях
        switch (ntest)
        {
        case 1:
            // Иницализация исходных данных для теста №1
            init_mas_test1(256, masCPP);
            init_mas_test1(256, masASM);
            break;
        case 2:
            // Иницализация исходных данных для теста №2
            init_mas_test1(256, masCPP);
            init_mas_test1(256, masASM);
            break;
        case 3:
        default:
            // Инициализация исходных данных для теста №3
            init_mas_test1(256, masCPP);
            init_mas_test1(256, masASM);
            break;
        }

        try 
        {
            // Вызов исходной функции на Си++
            processCPP(masCPP);

            // Вызов тестируемой функции на ассемблере
            processASM(masASM);

            // Сравнение результатов 
            massesMatched = compare_masses(256, masCPP, masASM);

			printf("compare: %u\n",massesMatched);

            // Вывод итогов
            
            printf("results:\n mas1:\n");
            for(int i=0;i<256;i++){
				
				printf("%u ", masCPP[i]);
							
            }
            
            printf("\n mas2:\n");
           
            
            for(int i=0;i<256;i++){
				
				printf("%u ", masASM[i]);					
				
            }
            
            printf("\n");
        }
        catch (...)
        {
            // Обработка исключения
            printf("A problem occurred!\n");
        }
    }
    
    fflush(stdin);
    return 0;
}
