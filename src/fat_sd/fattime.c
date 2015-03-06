/* Martin Thomas 4/2009 */

#include "integer.h"
#include "fattime.h"


DWORD get_fattime (void)
{

// ���������� ����������� �����
    return ((DWORD)(2015 - 1980) << 25)   // ���
            | ((DWORD)9 << 21)            // �����
            | ((DWORD)4 << 16)            // ����
            | ((DWORD)12 << 11)           // ���
            | ((DWORD)0 << 5)             // ������
            | ((DWORD)0 >> 1);            // �������

}

