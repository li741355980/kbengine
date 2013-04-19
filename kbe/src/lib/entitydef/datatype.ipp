/*
This source file is part of KBEngine
For the latest info, see http://www.kbengine.org/

Copyright (c) 2008-2012 KBEngine.

KBEngine is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

KBEngine is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.
 
You should have received a copy of the GNU Lesser General Public License
along with KBEngine.  If not, see <http://www.gnu.org/licenses/>.
*/


namespace KBEngine { 

INLINE DATATYPE_UID DataType::id()const 
{ 
	return id_; 
}

INLINE void DataType::aliasName(std::string aliasName)
{ 
	aliasName_ = aliasName; 
}

INLINE const char* DataType::aliasName(void)const
{ 
	return aliasName_.c_str(); 
}
	
//-------------------------------------------------------------------------------------
template <typename SPECIFY_TYPE>
IntType<SPECIFY_TYPE>::IntType(DATATYPE_UID did):
DataType(did)
{
}

//-------------------------------------------------------------------------------------
template <typename SPECIFY_TYPE>
IntType<SPECIFY_TYPE>::~IntType()
{
}

//-------------------------------------------------------------------------------------
template <typename SPECIFY_TYPE>
bool IntType<SPECIFY_TYPE>::isSameType(PyObject* pyValue)
{
	int ival = 0;
	if(PyLong_Check(pyValue))
	{
		ival = (int)PyLong_AsLong(pyValue);
		if(PyErr_Occurred())
		{
			PyErr_Clear();
			ival = (int)PyLong_AsUnsignedLong(pyValue);
			if (PyErr_Occurred())
			{
				OUT_TYPE_ERROR("INT");
				return false;
			}
		}
	}
	else
	{
		OUT_TYPE_ERROR("INT");
		return false;
	}

	SPECIFY_TYPE val = (SPECIFY_TYPE)ival;
	if(ival != int(val))
	{
		ERROR_MSG(boost::format("IntType::isSameType:%1% is out of range (currVal = %2%).\n") %
			ival % int(val));
		
		return false;
	}
	return true;
}


//-------------------------------------------------------------------------------------
template <typename SPECIFY_TYPE>
PyObject* IntType<SPECIFY_TYPE>::parseDefaultStr(std::string defaultVal)
{
	SPECIFY_TYPE i = 0;
	if(!defaultVal.empty())
	{
		std::stringstream stream;
		stream << defaultVal;
		stream >> i;
	}

	PyObject* pyval = PyLong_FromLong(i);

	if (PyErr_Occurred()) 
	{
		PyErr_Clear();
		PyErr_Format(PyExc_TypeError, "IntType::parseDefaultStr: defaultVal(%s) is error! val=[%s]", 
			pyval != NULL ? pyval->ob_type->tp_name : "NULL", defaultVal.c_str());

		PyErr_PrintEx(0);

		S_RELEASE(pyval);
		return PyLong_FromLong(0);
	}

	return pyval;
}

//-------------------------------------------------------------------------------------
template <typename SPECIFY_TYPE>
void IntType<SPECIFY_TYPE>::addToStream(MemoryStream* mstream, 
	PyObject* pyValue)
{
	SPECIFY_TYPE v = (SPECIFY_TYPE)PyLong_AsLong(pyValue);
	
	if(PyErr_Occurred())
	{
		PyErr_Clear();
		
		v = (SPECIFY_TYPE)PyLong_AsUnsignedLong(pyValue);
		
		if(PyErr_Occurred())
		{
			PyErr_Clear();
			PyErr_Format(PyExc_TypeError, "IntType::addToStream: pyValue(%s) is error!", 
				pyValue->ob_type->tp_name);

			PyErr_PrintEx(0);

			v = 0;
		}
	}
			
	(*mstream) << v;
}

//-------------------------------------------------------------------------------------
template <typename SPECIFY_TYPE>
PyObject* IntType<SPECIFY_TYPE>::createFromStream(MemoryStream* mstream)
{
	SPECIFY_TYPE val = 0;
	if(mstream)
		(*mstream) >> val;

	PyObject* pyval = PyLong_FromLong(val);

	if (PyErr_Occurred()) 
	{
		PyErr_Format(PyExc_TypeError, "IntType::createFromStream: errval=%d, default return is 0", val);
		PyErr_PrintEx(0);
		S_RELEASE(pyval);
		return PyLong_FromLong(0);
	}

	return pyval;
}

}
