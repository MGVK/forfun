
// Заготовка программы для домашнего задания №2 по АИС
// Компилировать с опцией (для обработки исключений): /EHa 
// Свойства проекта >> Свойства конфигурации >> C/C++ >> Командная строка

#include <stdio.h>


// Функция для обработки массива на Си++
//
// TODO: Отредактировать функцию на Си++ в соответствии с вариантом задания
//
unsigned char processCPP(unsigned char* data)
{
    unsigned char result = 5;

    for (int i = 0; i < 256; i++)
        result -= data[i];

    return result;
}


// Функция для обработки массива на ассемблере
//
// TODO: Отредактировать функцию на ассемблере в соответствии с вариантом 
// задания. 
//
// Внимание!!! Для работы с массивом используйте регистры esi/edi вместо
// регистров si/di. Регистр bp/ebp использовать не рекомендуется. 
//
unsigned char processASM(unsigned char* data)
{
    asm("\
        pushf\
        push esi\
        push ebx\
        push ecx\
        mov esi, data\
        mov bl, 5\
        mov cx, 256\
cycle:\
        mov al, ds:[esi]    ; эти две команды можно заменить\
        inc esi             ; одной командой lodsb\
        sub bl, al\
        dec cx\
        jnz cycle\
        ; Результат возвращается через регистр AL\
        mov al, bl\
        pop ecx\
        pop ebx\
        pop esi\
        popf\
            ");
    
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
    float delta;
    float base;

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
    unsigned char resultCPP, resultASM;
    char str[80];
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
            resultCPP = processCPP(masCPP);

            // Вызов тестируемой функции на ассемблере
            resultASM = processASM(masASM);

            // Сравнение результатов 
            massesMatched = compare_masses(256, masCPP, masASM);

            // Вывод итогов
            printf("ResultCPP = %u\n", (unsigned int) resultCPP);
            printf("ResultASM = %u\n", (unsigned int) resultASM);
            if (resultCPP == resultASM)
                printf("Result is correct!\n");
            else
                printf("Result is not correct!\n");
            if (massesMatched)
                printf("Mass content is correct!\n");
            else
                printf("Mass content is not correct!\n");
        }
        catch (...)
        {
            // Обработка исключения
            printf("A problem occurred!\n");
        }
    }
    
    fflush(stdin);
    gets(str);
    return 0;
}
