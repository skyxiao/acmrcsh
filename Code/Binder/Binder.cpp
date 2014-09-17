#include <node.h>
#include <v8.h>
#include <windows.h>
#include <string>
#include <sstream>
#include <iostream>

typedef void (*f1)();
typedef void (*f2)();
typedef int (*f3)(int, int, char*, unsigned);
typedef int (*f4)(const int*, unsigned, char*, unsigned);
typedef int (*f5)(const char*, char*, unsigned);
typedef void (*f6)(int, const char*);
typedef int (*f7)(int, int, bool, char*, unsigned);
typedef int (*f8)(int*, unsigned, bool, char*, unsigned);
typedef int (*f9)(const char*, bool, char*, unsigned);
typedef void (*f10)(int, const char*);
typedef int (*f11)(char*, unsigned);
typedef void (*f12)(int, unsigned, unsigned, unsigned);	
typedef void (*f13)(const char*);	
typedef void (*f14)();	
typedef void (*f15)();
typedef void (*f16)(unsigned mode);

HINSTANCE control_core_handle = NULL;
const unsigned buffer_size = 512*1024;
const unsigned max_id_num = 5*1024;
char buffer[buffer_size];
int id_arr[max_id_num];

struct
{
	f1 		initialize;
	f2 		terminate;
	f3 		fetch_parameters_by_id_range;
	f4 		fetch_parameters_by_ids;
	f5 		fetch_parameters_by_path;
	f6 		modify_parameter;
	f7 		fetch_system_data_by_id_range;
	f8 		fetch_system_data_by_ids;
	f9 		fetch_system_data_by_path;
	f10 	modify_system_data;
	f11 	fetch_recent_event_log;
	f12 	invoke;	
	f13		load_recipe;
	f14		unload_recipe;
	f15		clear_alarm;
	f16		change_mode;
} control_module;

using namespace v8;

Handle<Value> Initialize(const Arguments& args) 
{
	HandleScope scope;

	std::cout<<"initialize control module."<<std::endl;

	if (args.Length() < 1) 
	{
		ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
		return scope.Close(Undefined());
	}

	if (!args[0]->IsString()) 
	{
		ThrowException(Exception::TypeError(String::New("Wrong arguments")));
		return scope.Close(Undefined());
	}

 	//load control core module
	if(control_core_handle != NULL)
		return scope.Close(Undefined());

	v8::String::AsciiValue str(args[0]);
	std::string lib_name(*str);
	lib_name += ".dll";

	control_core_handle = LoadLibrary(lib_name.c_str());
	if(control_core_handle == NULL)
	{
		std::stringstream ss;
		ss<<"Failed to load control module: "<<*str<<".";
		ThrowException(Exception::TypeError(String::New(ss.str().c_str())));
		return scope.Close(Undefined());
	}

	control_module.initialize = (f1)GetProcAddress(control_core_handle, "Initialize");
	if (!control_module.initialize)
	{
		ThrowException(Exception::TypeError(String::New("Wrong control module.")));
		return scope.Close(Undefined());
	}

	control_module.terminate = (f2)GetProcAddress(control_core_handle, "Terminate");
	control_module.fetch_parameters_by_id_range = (f3)GetProcAddress(control_core_handle, "FetchParametersByIDRange");
	control_module.fetch_parameters_by_ids = (f4)GetProcAddress(control_core_handle, "FetchParametersByIDs");
	control_module.fetch_parameters_by_path = (f5)GetProcAddress(control_core_handle, "FetchParametersByPath");
	control_module.modify_parameter = (f6)GetProcAddress(control_core_handle, "ModifyParameter");
	control_module.fetch_system_data_by_id_range = (f7)GetProcAddress(control_core_handle, "FetchSystemDataByIDRange");
	control_module.fetch_system_data_by_ids = (f8)GetProcAddress(control_core_handle, "FetchSystemDataByIDs");
	control_module.fetch_system_data_by_path = (f9)GetProcAddress(control_core_handle, "FetchSystemDataByPath");
	control_module.modify_system_data = (f10)GetProcAddress(control_core_handle, "ModifySystemData");
	control_module.fetch_recent_event_log = (f11)GetProcAddress(control_core_handle, "FetchRecentEventLog");
	control_module.invoke = (f12)GetProcAddress(control_core_handle, "Invoke");
	control_module.load_recipe = (f13)GetProcAddress(control_core_handle, "LoadRecipe");
	control_module.unload_recipe = (f14)GetProcAddress(control_core_handle, "UnloadRecipe");
	control_module.clear_alarm = (f15)GetProcAddress(control_core_handle, "ClearAlarm");
	control_module.change_mode = (f16)GetProcAddress(control_core_handle, "ChangeMode");

	try
	{
		control_module.initialize();
	}
	catch(...)
	{
		ThrowException(Exception::TypeError(String::New("Failed to initialize control module.")));
		return scope.Close(Undefined());
	}

	return scope.Close(Undefined());
}

Handle<Value> Terminate(const Arguments& args) 
{
	HandleScope scope;

 	//load control core module
	if(control_core_handle == NULL)
		return scope.Close(Undefined());

	control_module.fetch_parameters_by_id_range = NULL;
	control_module.fetch_parameters_by_ids = NULL;
	control_module.fetch_parameters_by_path = NULL;
	control_module.modify_parameter = NULL;
	control_module.fetch_system_data_by_id_range = NULL;
	control_module.fetch_system_data_by_ids = NULL;
	control_module.fetch_system_data_by_path = NULL;
	control_module.modify_system_data = NULL;
	control_module.fetch_recent_event_log = NULL;
	control_module.invoke = NULL;
	control_module.load_recipe = NULL;
	control_module.unload_recipe = NULL;
	control_module.clear_alarm = NULL;
	control_module.change_mode = NULL;

	if (control_module.terminate)
	{
		try
		{
			control_module.terminate();
		}
		catch(...)
		{
			ThrowException(Exception::TypeError(String::New("Failed to initialize control module.")));
			return scope.Close(Undefined());
		}
	}
	control_module.terminate = NULL;

	FreeLibrary(control_core_handle);
	control_core_handle = NULL;

	return scope.Close(Undefined());
}

Handle<Value> FetchParameters(const Arguments& args) 
{
	HandleScope scope;

	if(control_core_handle == NULL)
	{
		ThrowException(Exception::TypeError(String::New("Control module hasn't been initialized.")));
		return scope.Close(Undefined());
	}

	//get by id array
	int size = 0;
	Local<String> except_info = String::Empty();

	if(args.Length() == 1 && args[0]->IsArray() && control_module.fetch_parameters_by_ids)
	{
		Local<Array> arr = args[0]->ToObject().As<Array>();
		unsigned num = arr->Length();

		for(unsigned i=0; i<num; i++)
		{
			Local<Value> id = arr->Get(i);
			if(!id->IsInt32())
			{
				ThrowException(Exception::TypeError(String::New("Wrong data type in array.")));
				return scope.Close(Undefined());
			}
			id_arr[i] = id->Int32Value();
		}

		try
		{
			size = control_module.fetch_parameters_by_ids(id_arr, num, buffer, buffer_size);
		}
		catch(std::exception& e)
		{
			std::cout<<e.what()<<std::endl;
			except_info = String::New(e.what());
		}
	}

	if(args.Length() == 1 && args[0]->IsString() && control_module.fetch_parameters_by_path)
	{
		try
		{
			String::Utf8Value path(args[0]->ToString());
			size = control_module.fetch_parameters_by_path(*path, buffer, buffer_size);
		}
		catch(std::exception& e)
		{
			std::cout<<e.what()<<std::endl;
			except_info = String::New(e.what());
		}
	}

	if(args.Length() == 2 && args[0]->IsInt32() && args[1]->IsInt32() && control_module.fetch_parameters_by_id_range)
	{
		try
		{
			size = control_module.fetch_parameters_by_id_range(args[0]->Int32Value(), args[1]->Int32Value(), buffer, buffer_size);
		}
		catch(std::exception& e)
		{
			except_info = String::New(e.what());
		}
	}

	if(size > 0)
	{
		*(buffer+size) = 0;
		Local<String> json_string = String::New(buffer, size);
		return scope.Close(json_string);
	}

	if(except_info->Length() > 0)
	{
		ThrowException(Exception::TypeError(except_info));
	}

	return scope.Close(Undefined());
}

Handle<Value> ModifyParameter(const Arguments& args) 
{
	HandleScope scope;

	if(args.Length() < 2)
	{
		ThrowException(Exception::TypeError(String::New("Wrong parameter number.")));
		return scope.Close(Undefined());
	}

	if(!args[0]->IsInt32())
	{
		ThrowException(Exception::TypeError(String::New("Wrong parameter type.")));
		return scope.Close(Undefined());
	}

	if(control_module.modify_parameter)
	{
		try
		{
			String::Utf8Value data(args[1]->ToString());
			control_module.modify_parameter(args[0]->Int32Value(), *data);
		}
		catch(std::exception& e)
		{
			ThrowException(Exception::TypeError(String::New(e.what())));
			return scope.Close(Undefined());
		}
	}

	return scope.Close(Undefined());
}

Handle<Value> FetchSystemData(const Arguments& args) 
{
	HandleScope scope;

	if(control_core_handle == NULL)
	{
		ThrowException(Exception::TypeError(String::New("Control module hasn't been initialized.")));
		return scope.Close(Undefined());
	}

	//get by id array
	int size = 0;
	Local<String> except_info = String::Empty();
	bool only_changed = false;

	if(args.Length() == 2 && args[0]->IsArray() && args[1]->IsBoolean() && control_module.fetch_system_data_by_ids)
	{
		Local<Array> arr = args[0]->ToObject().As<Array>();
		unsigned num = arr->Length();
		only_changed = args[1]->BooleanValue();

		for(unsigned i=0; i<num; i++)
		{
			Local<Value> id = arr->Get(i);
			if(!id->IsInt32())
			{
				ThrowException(Exception::TypeError(String::New("Wrong data type in array.")));
				return scope.Close(Undefined());
			}
			id_arr[i] = id->Int32Value();
		}

		try
		{
			size = control_module.fetch_system_data_by_ids(id_arr, num, only_changed, buffer, buffer_size);
		}
		catch(std::exception& e)
		{
			except_info = String::New(e.what());
		}
	}

	if(args.Length() == 2 && args[0]->IsString() && args[1]->IsBoolean() && control_module.fetch_system_data_by_path)
	{
		try
		{
			only_changed = args[1]->BooleanValue();
			String::Utf8Value path(args[0]->ToString());
			size = control_module.fetch_system_data_by_path(*path, only_changed, buffer, buffer_size);
		}
		catch(std::exception& e)
		{
			std::cout<<e.what()<<std::endl;
			except_info = String::New(e.what());
		}
	}

	if(args.Length() == 3 && args[0]->IsInt32() && args[1]->IsInt32() && args[2]->IsBoolean() && control_module.fetch_system_data_by_id_range)
	{
		try
		{
			only_changed = args[2]->BooleanValue();
			size = control_module.fetch_system_data_by_id_range(args[0]->Int32Value(), args[1]->Int32Value(), only_changed, buffer, buffer_size);
		}
		catch(std::exception& e)
		{
			except_info = String::New(e.what());
		}
	}

	if(size > 0)
	{
		*(buffer+size) = 0;
		Local<String> json_string = String::New(buffer, size);
		return scope.Close(json_string);
	}

	if(except_info->Length() > 0)
	{
		ThrowException(Exception::TypeError(except_info));
	}

	return scope.Close(Undefined());
}

Handle<Value> ModifySystemData(const Arguments& args) 
{
	HandleScope scope;

	if(args.Length() < 2)
	{
		ThrowException(Exception::TypeError(String::New("Wrong parameter number.")));
		return scope.Close(Undefined());
	}

	if(!args[0]->IsInt32())
	{
		ThrowException(Exception::TypeError(String::New("Wrong parameter type.")));
		return scope.Close(Undefined());
	}

	if(control_module.modify_system_data)
	{
		try
		{
			String::Utf8Value data(args[1]->ToString());
			control_module.modify_system_data(args[0]->Int32Value(), *data);
		}
		catch(std::exception& e)
		{
			ThrowException(Exception::TypeError(String::New(e.what())));
			return scope.Close(Undefined());
		}
	}

	return scope.Close(Undefined());
}

Handle<Value> FetchRecentEventLog(const Arguments& args) 
{
	HandleScope scope;

	if(control_core_handle == NULL)
	{
		ThrowException(Exception::TypeError(String::New("Control module hasn't been initialized.")));
		return scope.Close(Undefined());
	}

	int size = 0;
	Local<String> except_info = String::Empty();
	
	if(control_module.fetch_recent_event_log)
	{
		try
		{
			size = control_module.fetch_recent_event_log(buffer, buffer_size);
		}
		catch(std::exception& e)
		{
			except_info = String::New(e.what());
		}
	}

	if(size > 0)
	{
		*(buffer+size) = 0;
		Local<String> json_string = String::New(buffer, size);
		return scope.Close(json_string);
	}

	if(except_info->Length() > 0)
	{
		ThrowException(Exception::TypeError(except_info));
	}

	return scope.Close(Undefined());
}

Handle<Value> Invoke(const Arguments& args) 
{
	HandleScope scope;

	if(args.Length() < 2)
	{
		ThrowException(Exception::TypeError(String::New("Wrong parameter number.")));
		return scope.Close(Undefined());
	}

	if(!args[0]->IsInt32() || !args[1]->IsUint32())
	{
		ThrowException(Exception::TypeError(String::New("Wrong parameter type.")));
		return scope.Close(Undefined());
	}

	if(args.Length() > 2 && !args[2]->IsUint32() || args.Length() > 3 && !args[3]->IsUint32())
	{
		ThrowException(Exception::TypeError(String::New("Wrong parameter type.")));
		return scope.Close(Undefined());
	}

	int unit = args[0]->Int32Value();
	unsigned cmd = args[1]->Uint32Value();
	unsigned param1 = 0, param2 = 0;
	if(args.Length() > 2)
		param1 = args[2]->Uint32Value();
	if(args.Length() > 3)
		param2 = args[3]->Uint32Value();

	if(control_module.invoke)
	{
		try
		{
			control_module.invoke(unit, cmd, param1, param2);
		}
		catch(std::exception& e)
		{
			ThrowException(Exception::TypeError(String::New(e.what())));
			return scope.Close(Undefined());
		}
	}

	return scope.Close(Undefined());
}

Handle<Value> LoadRecipe(const Arguments& args) 
{
	HandleScope scope;

	if(args.Length() < 1)
	{
		ThrowException(Exception::TypeError(String::New("Wrong parameter number.")));
		return scope.Close(Undefined());
	}

	if(!args[0]->IsString())
	{
		ThrowException(Exception::TypeError(String::New("Wrong parameter type.")));
		return scope.Close(Undefined());
	}

	if(control_module.load_recipe)
	{
		try
		{
			String::Utf8Value data(args[0]->ToString());
			control_module.load_recipe(*data);
		}
		catch(std::exception& e)
		{
			ThrowException(Exception::TypeError(String::New(e.what())));
			return scope.Close(Undefined());
		}
	}

	return scope.Close(Undefined());
}

Handle<Value> UnloadRecipe(const Arguments& args) 
{
	HandleScope scope;

	if(control_module.unload_recipe)
	{
		try
		{
			control_module.unload_recipe();
		}
		catch(std::exception& e)
		{
			ThrowException(Exception::TypeError(String::New(e.what())));
			return scope.Close(Undefined());
		}
	}

	return scope.Close(Undefined());
}

Handle<Value> ClearAlarm(const Arguments& args) 
{
	HandleScope scope;

	if(control_module.clear_alarm)
	{
		try
		{
			control_module.clear_alarm();
		}
		catch(std::exception& e)
		{
			ThrowException(Exception::TypeError(String::New(e.what())));
			return scope.Close(Undefined());
		}
	}

	return scope.Close(Undefined());
}

Handle<Value> ChangeMode(const Arguments& args) 
{
	HandleScope scope;

	if(args.Length() != 1)
	{
		ThrowException(Exception::TypeError(String::New("Wrong parameter number.")));
		return scope.Close(Undefined());
	}

	if(!args[0]->IsUint32())
	{
		ThrowException(Exception::TypeError(String::New("Wrong parameter type.")));
		return scope.Close(Undefined());
	}

	unsigned mode = args[0]->Uint32Value();
	if(control_module.change_mode)
	{
		try
		{
			control_module.change_mode(mode);
		}
		catch(std::exception& e)
		{
			ThrowException(Exception::TypeError(String::New(e.what())));
			return scope.Close(Undefined());
		}
	}

	return scope.Close(Undefined());
}

void init(Handle<Object> exports)
{
	exports->Set(String::NewSymbol("initialize"), FunctionTemplate::New(Initialize)->GetFunction());
	exports->Set(String::NewSymbol("terminate"), FunctionTemplate::New(Terminate)->GetFunction());
	exports->Set(String::NewSymbol("fetch_parameters"), FunctionTemplate::New(FetchParameters)->GetFunction());
	exports->Set(String::NewSymbol("modify_parameter"), FunctionTemplate::New(ModifyParameter)->GetFunction());
	exports->Set(String::NewSymbol("fetch_system_data"), FunctionTemplate::New(FetchSystemData)->GetFunction());
	exports->Set(String::NewSymbol("modify_system_data"), FunctionTemplate::New(ModifySystemData)->GetFunction());
	exports->Set(String::NewSymbol("fetch_recent_event_log"), FunctionTemplate::New(FetchRecentEventLog)->GetFunction());
	exports->Set(String::NewSymbol("invoke"), FunctionTemplate::New(Invoke)->GetFunction());
	exports->Set(String::NewSymbol("load_recipe"), FunctionTemplate::New(LoadRecipe)->GetFunction());
	exports->Set(String::NewSymbol("unload_recipe"), FunctionTemplate::New(UnloadRecipe)->GetFunction());
	exports->Set(String::NewSymbol("clear_alarm"), FunctionTemplate::New(ClearAlarm)->GetFunction());
	exports->Set(String::NewSymbol("change_mode"), FunctionTemplate::New(ChangeMode)->GetFunction());
}

NODE_MODULE(ControlBinder, init)

