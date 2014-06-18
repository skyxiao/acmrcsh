/*
 * Control.h
 *
 *  Created on: 2014Äê3ÔÂ13ÈÕ
 *      Author: acm
 */

#ifndef CONTROL_H_
#define CONTROL_H_

extern "C"
{

void Initialize();
void Terminate();

int FetchParametersByIDRange(int from, int to, char* json, unsigned buffer_size);
int FetchParametersByIDs(const int* id_array, unsigned num, char* json, unsigned buffer_size);
int FetchParametersByPath(const char* path, char* json, unsigned buffer_size);
void ModifyParameter(int id, const char* value);

int FetchSystemDataByIDRange(int from, int to, bool only_changed, char* json, unsigned buffer_size);
int FetchSystemDataByIDs(int* id_array, unsigned num, bool only_changed, char* json, unsigned buffer_size);
int FetchSystemDataByPath(const char* path, bool only_changed, char* json, unsigned buffer_size);
void ModifySystemData(int id, const char* value);

int FetchRecentEventLog(char* json, unsigned buffer_size);

void Invoke(int unit, unsigned cmd, unsigned param1, unsigned param2);

void LoadRecipe(const char* name);
void UnloadRecipe();

void ResetSignalTower();
void ChangeMode(unsigned mode);

}

// __declspec(dllexport)
#endif /* CONTROL_H_ */
