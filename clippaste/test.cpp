#include <iostream>

int main()
{
    wchar_t a = L'ツ';

    std::wcout << static_cast<int>(a) << std::endl;
}