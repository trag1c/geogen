/*

    This file is part of GeoGen.

    GeoGen is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Foobar is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with GeoGen.  If not, see <http://www.gnu.org/licenses/>.

*/

// system and SDL headers
#include <stdio.h>   
#include <stdlib.h> 
#include <string>
//#include <SDL/SDL.h> 
#include <math.h>
#include <time.h> 
#include <iostream>
#include <fstream>
#include <sstream>
#include <assert.h>


#include "ggen.h"
#include "ggen_support.h"
#include "ggen_data_1d.h"
#include "ggen_data_2d.h"

#include "sqplus.h"
#include "sqstdmath.h"

#include "ggen_squirrel.h"

//int
//
//constructNewTestObjFixedArgs(const SQChar *s, int val, bool b, HSQUIRRELVM v)
//{
//    using namespace SqPlus;
//    StackHandler sa(v);
//    //int paramCount = sa.GetParamCount();
//    return PostConstruct<NewTestObj>(v,
//                                     new NewTestObj(s,val,b),
//                                     NULL);
//}

DECLARE_ENUM_TYPE(GGen_Normalization_Mode);
DECLARE_ENUM_TYPE(GGen_Overflow_Mode);
DECLARE_ENUM_TYPE(GGen_Direction);
DECLARE_ENUM_TYPE(GGen_Angle);
DECLARE_ENUM_TYPE(GGen_Message_Level);

static void printFunc(HSQUIRRELVM v,const SQChar * s,...) {
  //static SQChar temp[2048];
  //va_list vl;
  //va_start(vl,s);
  //scvsprintf( temp,s,vl);
  //SCPUTS(temp);
  //va_end(vl);
}

GGen_Squirrel* ggen_current_object;

void GGen_ErrorHandler(HSQUIRRELVM,const SQChar * desc,const SQChar * source,SQInteger line,SQInteger column){
	ggen_current_object->ThrowMessage(desc, GGEN_ERROR);
}

GGen_Squirrel::GGen_Squirrel(){
	ggen_current_object = this;

	SquirrelVM::Init();

	sqstd_register_mathlib(SquirrelVM::GetVMPtr());

	sq_setprintfunc(SquirrelVM::GetVMPtr(), printFunc);
	sq_setcompilererrorhandler(SquirrelVM::GetVMPtr(), GGen_ErrorHandler);
	sq_enabledebuginfo(SquirrelVM::GetVMPtr(), false);

	/* Enum: GGen_Normalization_Mode */
	BindConstant(GGEN_ADDITIVE, _SC("GGEN_ADDITIVE"));
	BindConstant(GGEN_SUBSTRACTIVE, _SC("GGEN_SUBSTRACTIVE"));

	/* Enum: GGen_Overflow_Mode */
	BindConstant(GGEN_CYCLE, _SC("GGEN_CYCLE"));
	BindConstant(GGEN_DISCARD, _SC("GGEN_DISCARD"));
	BindConstant(GGEN_DISCARD_AND_FILL, _SC("GGEN_DISCARD_AND_FILL"));

	/* Enum: GGen_Direction */
	BindConstant(GGEN_HORIZONTAL, _SC("GGEN_HORIZONTAL"));
	BindConstant(GGEN_VERTICAL, _SC("GGEN_VERTICAL"));

	/* Enum: GGen_Angle */
	BindConstant(GGEN_0, _SC("GGEN_0"));
	BindConstant(GGEN_90, _SC("GGEN_90"));
	BindConstant(GGEN_180, _SC("GGEN_180"));
	BindConstant(GGEN_270, _SC("GGEN_270"));

	/* Enum: GGen_Message_Mode */
	BindConstant(GGEN_MESSAGE, _SC("GGEN_MESSAGE"));
	BindConstant(GGEN_NOTICE, _SC("GGEN_NOTICE"));
	BindConstant(GGEN_WARNING, _SC("GGEN_WARNING"));
	BindConstant(GGEN_ERROR, _SC("GGEN_ERROR"));

	/* Class: GGen_Data_1D */
	SQClassDefNoConstructor<GGen_Data_1D>(_SC("GGen_Data_1D")).
		overloadConstructor<GGen_Data_1D(*)(uint16)>().
		overloadConstructor<GGen_Data_1D(*)(uint16, int16)>().
		overloadConstructor<GGen_Data_1D(*)(GGen_Data_1D&)>().

		func(&GGen_Data_1D::SetValue, _T("SetValue")).
		func(&GGen_Data_1D::SetValueInRange, _T("SetValueInRange")).
		overloadFunc<int16(GGen_Data_1D::*)(uint16)>(&GGen_Data_1D::GetValue, _SC("GetValue")).
		overloadFunc<int16(GGen_Data_1D::*)(uint16, uint16)>(&GGen_Data_1D::GetValue,_T("GetValue")).
		
		overloadFunc<void(GGen_Data_1D::*)(int16)>(&GGen_Data_1D::Add, _SC("Add")).
		overloadFunc<void(GGen_Data_1D::*)(GGen_Data_1D*)>(&GGen_Data_1D::Add, _SC("Add")).
		func(&GGen_Data_1D::AddTo, _T("AddTo")).
		overloadFunc<void(GGen_Data_1D::*)(double)>(&GGen_Data_1D::Multiply, _SC("Multiply")).
		overloadFunc<void(GGen_Data_1D::*)(GGen_Data_1D*)>(&GGen_Data_1D::Multiply, _SC("Multiply")).
		func(&GGen_Data_1D::Invert,_T("Invert")).
		func(&GGen_Data_1D::Scale,_T("Scale")).
		func(&GGen_Data_1D::ScaleTo,_T("ScaleTo")).
		func(&GGen_Data_1D::ScaleValuesTo,_T("ScaleValuesTo")).
		func(&GGen_Data_1D::Fill,_T("Fill")).
		func(&GGen_Data_1D::ResizeCanvas,_T("ResizeCanvas")).
		func(&GGen_Data_1D::Clamp,_T("Clamp")).
		func(&GGen_Data_1D::Flip,_T("Flip")).
		func(&GGen_Data_1D::Min,_T("Min")).
		func(&GGen_Data_1D::Max,_T("Max")).
		func(&GGen_Data_1D::Shift,_T("Shift")).
		func(&GGen_Data_1D::Union,_T("Union")).
		func(&GGen_Data_1D::Intersection,_T("Intersection")).
		
		func(&GGen_Data_1D::Monochrome,_T("Monochrome")).
		func(&GGen_Data_1D::Normalize,_T("Normalize")).
		func(&GGen_Data_1D::Gradient,_T("Gradient")).
		overloadFunc<void(GGen_Data_1D::*)(uint16, uint16, GGen_Amplitudes*)>(&GGen_Data_1D::Noise,_T("Noise")).
		overloadFunc<void(GGen_Data_1D::*)(uint16, uint16)>(&GGen_Data_1D::Noise,_T("Noise")).
		func(&GGen_Data_1D::Smooth,_T("Smooth")).
		func(&GGen_Data_1D::Flood,_T("Flood")).
		
		func(&GGen_Data_1D::Print,_T("Print"));
	

	/* Class: GGen_Data_2D */
	SQClassDefNoConstructor<GGen_Data_2D>(_SC("GGen_Data_2D")).
		overloadConstructor<GGen_Data_2D(*)(uint16, uint16)>().
		overloadConstructor<GGen_Data_2D(*)(uint16, uint16, int16)>().
		overloadConstructor<GGen_Data_2D(*)(GGen_Data_2D&)>().

		func(&GGen_Data_2D::SetValue, _T("SetValue")).
		func(&GGen_Data_2D::SetValueInRect, _T("SetValueInRect")).
		overloadFunc<int16(GGen_Data_2D::*)(uint16, uint16)>(&GGen_Data_2D::GetValue, _SC("GetValue")).
		overloadFunc<int16(GGen_Data_2D::*)(uint16, uint16, uint16, uint16)>(&GGen_Data_2D::GetValue,_T("GetValue")).
		
		overloadFunc<void(GGen_Data_2D::*)(int16)>(&GGen_Data_2D::Add, _SC("Add")).
		overloadFunc<void(GGen_Data_2D::*)(GGen_Data_2D*)>(&GGen_Data_2D::Add, _SC("Add")).
		func(&GGen_Data_2D::AddTo, _T("AddTo")).
		overloadFunc<void(GGen_Data_2D::*)(int16, GGen_Data_2D*, bool)>(&GGen_Data_2D::AddMasked, _SC("AddMasked")).		
		overloadFunc<void(GGen_Data_2D::*)(GGen_Data_2D*, GGen_Data_2D*, bool)>(&GGen_Data_2D::AddMasked, _SC("AddMasked")).		
		overloadFunc<void(GGen_Data_2D::*)(double)>(&GGen_Data_2D::Multiply, _SC("Multiply")).
		overloadFunc<void(GGen_Data_2D::*)(GGen_Data_2D*)>(&GGen_Data_2D::Multiply, _SC("Multiply")).
		
		func(&GGen_Data_2D::Invert,_T("Invert")).
		func(&GGen_Data_2D::Scale,_T("Scale")).
		func(&GGen_Data_2D::ScaleTo,_T("ScaleTo")).
		func(&GGen_Data_2D::ScaleValuesTo,_T("ScaleValuesTo")).
		func(&GGen_Data_2D::Fill,_T("Fill")).
		func(&GGen_Data_2D::ResizeCanvas,_T("ResizeCanvas")).
		func(&GGen_Data_2D::Clamp,_T("Clamp")).
		func(&GGen_Data_2D::Min,_T("Min")).
		func(&GGen_Data_2D::Max,_T("Max")).
		func(&GGen_Data_2D::Rotate,_T("Rotate")).
		func(&GGen_Data_2D::Flip,_T("Flip")).
		func(&GGen_Data_2D::Union,_T("Union")).
		func(&GGen_Data_2D::Intersection,_T("Intersection")).
		
		func(&GGen_Data_2D::Shift,_T("Shift")).
		func(&GGen_Data_2D::Project,_T("Project")).

		overloadFunc<void(GGen_Data_2D::*)(uint16, uint16, uint16, uint16, int16, int16, bool)>(&GGen_Data_2D::Gradient, _SC("Gradient")).
		overloadFunc<void(GGen_Data_2D::*)(uint16, uint16, uint16, uint16, GGen_Data_1D*, bool)>(&GGen_Data_2D::Gradient, _SC("Gradient")).
		overloadFunc<void(GGen_Data_2D::*)(uint16, uint16, uint16, int16, int16, bool)>(&GGen_Data_2D::RadialGradient, _SC("RadialGradient")).
		overloadFunc<void(GGen_Data_2D::*)(uint16, uint16, uint16, GGen_Data_1D*, bool)>(&GGen_Data_2D::RadialGradient, _SC("RadialGradient")).
		overloadFunc<void(GGen_Data_2D::*)(uint16, uint16, GGen_Amplitudes*)>(&GGen_Data_2D::Noise,_T("Noise")).
		overloadFunc<void(GGen_Data_2D::*)(uint16, uint16)>(&GGen_Data_2D::Noise,_T("Noise")).
		overloadFunc<void(GGen_Data_2D::*)(uint8, GGen_Direction)>(&GGen_Data_2D::Smooth,_T("Smooth")).
		overloadFunc<void(GGen_Data_2D::*)(uint8)>(&GGen_Data_2D::Smooth,_T("Smooth")).
		func(&GGen_Data_2D::Flood,_T("Flood")).
		func(&GGen_Data_2D::Pattern,_T("Pattern"));

	/* Class: GGen_Amplitudes */
	SQClassDefNoConstructor<GGen_Amplitudes>(_SC("GGen_Amplitudes")).
		overloadConstructor<GGen_Amplitudes(*)(uint8)>().
		func(&GGen_Amplitudes::AddAmplitude,_T("AddAmplitude"));

	/* Constants: GGEN_MIN/MAX/INVALID_HEIGHT */
	BindConstant(GGEN_MIN_HEIGHT, _SC("GGEN_MIN_HEIGHT"));
	BindConstant(GGEN_MAX_HEIGHT, _SC("GGEN_MAX_HEIGHT"));
	BindConstant(GGEN_INVALID_HEIGHT, _SC("GGEN_INVALID_HEIGHT"));
}

GGen_Squirrel::~GGen_Squirrel(){
	
}

bool GGen_Squirrel::SetScript(const char* script){
	wchar_t* buf = new wchar_t[strlen(script)];
	
	mbstowcs(buf, script, 9999999);

	try {
		{
		SquirrelObject sqScript2 = SquirrelVM::CompileBuffer(buf);

		SquirrelVM::RunScript(sqScript2);
		}
		

		return true;
    } catch (SquirrelError & e) {
		//SCPUTS(e.desc);
		
		return false;
    }	
}

void GGen_Squirrel::GetProperties(){};
bool GGen_Squirrel::GetNextOption(){return false;};

int16* GGen_Squirrel::Generate(uint16 width, uint16 height){		
	
	
	try {
		int16* return_data;
		GGen_Data_2D* data;

		{
			SquirrelFunction<GGen_Data_2D*> callFunc(_SC("Generate"));
			data =  callFunc(width, height);	
		}

		assert(data != NULL && data->data != NULL);

		return_data = new int16[width*height];

		assert(return_data != NULL);

		memcpy(return_data, data->data, sizeof(int16) * width * height);

		//callFunc.reset();
		//sqScript.Reset();

		//callFunc.reset();

		//sq_collectgarbage( SquirrelVM::GetVMPtr() ); 
		//SquirrelVM::Shutdown();

		//sq_close(SquirrelVM::GetVMPtr());


		SquirrelVM::Shutdown();

		//Squi

		return return_data;
		
    } catch (SquirrelError & e) {
		//this->ThrowMessage((wchar_t) e.desc, GGEN_ERROR);
		//SCPUTS(e.desc);
		
		SquirrelVM::Shutdown();	

		return NULL;
    }	

	

	
}






