/* ***********************************************************
@Copyright Alexander V. Bakhshiev, 2010.
E-mail:            alexab@ailab.ru
url:            http://ailab.ru
Version:        1.0.0

This file - part of the project: NMSDK

File License:        BSD License
Project License:    GNU GPLv3 License
See file license.txt for more information
*********************************************************** */

#ifndef NMotionControlLibraryH
#define NMotionControlLibraryH

#include "NEngineMotionControl.h"
#include "../../Kernel/NLibrary.h"
#include "NMotionElement.h"

namespace NMSDK {

class NMotionControlLibrary: public NLibrary
{
public: // ������
// --------------------------
// ������������ � �����������
// --------------------------
NMotionControlLibrary(void);
// --------------------------

// --------------------------
// ������ �������� ��������� �������
// --------------------------
// --------------------------


// --------------------------
// ������ ����������� �����������
// --------------------------
// ��������� ������ ClassSamples �������� ������������ �������� � �� �������.
// �� ��������� ��������������� ������� ������� � ������ ������.
virtual void ACreateClassSamples(NStorage *storage);
// --------------------------
};     

extern NMotionControlLibrary MotionControlLibrary;

}


#endif
