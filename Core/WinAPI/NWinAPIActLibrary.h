/* ***********************************************************
@Copyright Alexander V. Bakhshiev, 2009.
E-mail:            alexab@ailab.ru
url:            http://ailab.ru
Version:        1.0.0

This file - part of the project: NMSDK

File License:        BSD License
Project License:    GNU GPLv3 License
See file license.txt for more information
*********************************************************** */

#ifndef NWinAPIActLibraryH
#define NWinAPIActLibraryH

#include "NWPhysicalManipulator.h"

namespace NMSDK {

class RDK_LIB_TYPE NWinAPIActLibrary: public ULibrary
{
public: // ������
// --------------------------
// ������������ � �����������
// --------------------------
NWinAPIActLibrary(void);
// --------------------------

// --------------------------
// ������ ����������� �����������
// --------------------------
// ��������� ������ ClassSamples �������� ������������ �������� � �� �������.
// �� ��������� ��������������� ������� ������� � ������ ������.
virtual void CreateClassSamples(UStorage *storage);
// --------------------------
};     

extern NWinAPIActLibrary WinAPIActLibrary;

}


#endif
