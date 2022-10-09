#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_LINE_SIZE 300

struct Dir;
struct File;

typedef struct Dir{
	char *name;
	struct Dir* parent;
	struct File* head_children_files;
	struct Dir* head_children_dirs;
	struct Dir* next;
} Dir;

typedef struct File {
	char *name;
	struct Dir* parent;
	struct File* next;
} File;
void DezalocRecursivFile(File** elementCaraVaFiSters){//Dezalocare lista de File
	if(*elementCaraVaFiSters == NULL)
		return;

	(*elementCaraVaFiSters)->parent = NULL;
	free((*elementCaraVaFiSters)->name);

	DezalocRecursivFile(&(*elementCaraVaFiSters)->next);//dezaloc urmatorul element din lista
	(*elementCaraVaFiSters)->next = NULL;

	free(*elementCaraVaFiSters);
	*elementCaraVaFiSters = NULL;
}
void DezalocRecursivDir(Dir** elementCaraVaFiSters){//Dezalocare lista de Dir
	if(*elementCaraVaFiSters == NULL)
		return;

	(*elementCaraVaFiSters)->parent = NULL;
	free((*elementCaraVaFiSters)->name);
	DezalocRecursivDir(&(*elementCaraVaFiSters)->head_children_dirs);
	DezalocRecursivFile(&(*elementCaraVaFiSters)->head_children_files);

	DezalocRecursivDir(&(*elementCaraVaFiSters)->next);//dezaloc urmatorul element din lista
	(*elementCaraVaFiSters)->next = NULL;

	free(*elementCaraVaFiSters);
	*elementCaraVaFiSters = NULL;
}

void MakeHome(Dir** home){
	(*home) = (Dir*)calloc(1, sizeof(Dir));
	if((*home) == NULL){
		printf("Eroare la alocare");
		return;
	}

	(*home)->name = (char*)calloc(5, sizeof(char));
	strcpy((*home)->name, "home");

	
	(*home)->parent = NULL;
	(*home)->next = NULL;
	(*home)->head_children_dirs = NULL;
	(*home)->head_children_files = NULL;
}

File* WordExistInFile(File* curent, char* name){
	/* Va returna pointer catre elementul din lista cu numele dat
	   Sau NULL daca nu exista un element in lista cu numele dat*/
	
	while(curent != NULL){
		if(strcmp(name, curent->name) == 0)	
			return curent;
		curent = curent->next;
	}
	return NULL;
}
Dir* WordExistInDir(Dir* curent, char* name){
	while(curent != NULL){
		if(strcmp(name, curent->name) == 0)	
			return curent;
		curent = curent->next;
	}
	return NULL;
}

void touch (Dir* parent, char* name) {
	if(WordExistInDir(parent->head_children_dirs, name) || WordExistInFile(parent->head_children_files, name)){
		printf("File already exists\n");
		return;
	}

	File* aux = (File*)calloc(1, sizeof(File));
	if(aux == NULL){
		printf("Eroare la alocara\n");
		return;
	}

	aux->name = (char*)calloc(strlen(name) + 1, sizeof(char));
	strcpy(aux->name, name);
	aux->parent = parent;
	aux->next = NULL;
	
	if(parent->head_children_files == NULL)
		parent->head_children_files = aux; //inca nu am niciun element in lista
	else
	{
		File* filesFromParent = parent->head_children_files;
		while(filesFromParent->next != NULL){
			filesFromParent = filesFromParent->next;
		}
		filesFromParent->next = aux;
	}
}

void mkdir (Dir* parent, char* name) {
	if(WordExistInDir(parent->head_children_dirs, name) || WordExistInFile(parent->head_children_files, name)){
		printf("Directory already exists\n");
		return;
	}

	Dir* aux = (Dir*)calloc(1, sizeof(Dir));
	if(aux == NULL){
		printf("Eroare la alocara\n");
		return;
	}
	
	aux->name = (char*)calloc(strlen(name) + 1, sizeof(char));
	strcpy(aux->name, name);
	
	aux->parent = parent;
	aux->next = NULL;
	aux->head_children_dirs = NULL;
	aux->head_children_files = NULL;

	if(parent->head_children_dirs == NULL)//inserare la inceputul listei
		parent->head_children_dirs = aux;
	else//inserare la sfarsitul listei listei
	{
		Dir* dirsFromParent = parent->head_children_dirs;
		while(dirsFromParent->next != NULL){
			dirsFromParent = dirsFromParent->next;
		}
		dirsFromParent->next = aux;
	}
}

void ls (Dir* parent) {
	Dir* dirsFromParent = parent->head_children_dirs;
	while(dirsFromParent != NULL){
		printf("%s\n", dirsFromParent->name);
		dirsFromParent = dirsFromParent->next;
	}

	File* filesFromParent = parent->head_children_files;
	
	while(filesFromParent != NULL){
		printf("%s\n", filesFromParent->name);
		filesFromParent = filesFromParent->next;
	}
}

void rm (Dir* parent, char* name) {
	File* elementCaraVaFiSters;

	if(elementCaraVaFiSters = WordExistInFile(parent->head_children_files, name)){
		File* curent = parent->head_children_files;
		if(curent == elementCaraVaFiSters){ //trebuie sa sterg primul element
			parent->head_children_files = curent->next;
		
		}
		else{
			while(curent->next != elementCaraVaFiSters)
				curent = curent->next;

			curent->next = elementCaraVaFiSters->next;
			
		}
		elementCaraVaFiSters->next = NULL;//rup legatura de lista
		DezalocRecursivFile(&elementCaraVaFiSters);
	}
	else{
		printf("Could not find the file\n");
	}
	
}

void rmdir (Dir* parent, char* name) {
	Dir* elementCaraVaFiSters;

	if(elementCaraVaFiSters = WordExistInDir(parent->head_children_dirs, name)){
		Dir* curent = parent->head_children_dirs;
		if(curent == elementCaraVaFiSters){ //trebuie sa sterg primul element
			parent->head_children_dirs = curent->next;
		
		}
		else{
			while(curent->next != elementCaraVaFiSters)
				curent = curent->next;

			curent->next = elementCaraVaFiSters->next;
			
		}
		elementCaraVaFiSters->next = NULL;
		DezalocRecursivDir(&elementCaraVaFiSters);
	}
	else{
		printf("Could not find the dir\n");
	}
}

void cd(Dir** target, char *name) {
	Dir* Destinatie;
	if(strcmp("..", name) == 0)
	{
		Destinatie = (*target)->parent;
		if(Destinatie != NULL)
			*target = Destinatie;
		
		return;
	}
	if( Destinatie = WordExistInDir((*target)->head_children_dirs, name) ){
		*target = Destinatie;
		
	}
	else{
		printf("No directories found!\n");
	}
}

char *pwd (Dir* target) {
	//Se insereaza primul nume de director in stringul rezultat
	int len = strlen(target->name) + 1;
	char* pwdResult = (char*)calloc(len, sizeof(char));

	strcpy(pwdResult, target->name);
	char* p = pwdResult + strlen(pwdResult);
	*p = '\0';

	//se insereaza restul de nume de directoare
	target = target->parent;
	while(target != NULL){
		len += strlen(target->name) + 1;
		char* auxString = (char*)calloc(len, sizeof(char));

		strcpy(auxString, target->name);
		char* p = auxString + strlen(auxString);
		*p = '/';
		strcpy(p + 1, pwdResult);
		free(pwdResult);

		pwdResult = auxString;

		target = target->parent;
	}

	return pwdResult;
	
}

void stop (Dir* target) {}

void tree (Dir* target, int level) {
	Dir* dirsAux = target->head_children_dirs;
	File* filesAux = target->head_children_files;

	while(dirsAux != NULL){
		for(int i = 1; i <= level; i++)
			printf("    ");

		printf("%s\n", dirsAux->name);
		tree(dirsAux, level + 1);

		dirsAux = dirsAux->next;
	}

	while(filesAux != NULL){
		for(int i = 1; i <= level; i++)
			printf("    ");

		printf("%s\n", filesAux->name);

		filesAux = filesAux->next;
	}
}

void mv(Dir* parent, char *oldname, char *newname) {
	File* fileAux;
	Dir* dirAux;

	if(WordExistInDir(parent->head_children_dirs, newname) || WordExistInFile(parent->head_children_files, newname)){
		printf("File/Director already exists\n");
		return;
	}

	if(fileAux = WordExistInFile(parent->head_children_files, oldname)){//s-a gasit file pe care trebuie sa-l modificam
		free(fileAux->name);
		fileAux->name = (char*)calloc(strlen(newname) + 1, sizeof(char));
		strcpy(fileAux->name, newname);


		File* curent = parent->head_children_files;
		if(curent == fileAux){//fileAux este primul in lista de files
			parent->head_children_files = fileAux->next;
			curent = parent->head_children_files;

			if(curent == NULL){//avem doar un element in lista
				parent->head_children_files = fileAux;
				return;
			}
		}
		else{
			while(curent->next != fileAux){
				curent = curent->next;
			}
			curent->next = fileAux->next;
		}

		//Se insereaza structura noua la sfarsitul listei
		fileAux->next = NULL;
		while(curent->next != NULL){
			curent = curent->next;
		}
		curent->next = fileAux;

		return;
	}
	if(dirAux = WordExistInDir(parent->head_children_dirs, oldname)){//s-a gasit dir pe care trebuie sa-l modificam
		free(dirAux->name);
		dirAux->name = (char*)calloc(strlen(newname) + 1, sizeof(char));
		strcpy(dirAux->name, newname);

		Dir* curent = parent->head_children_dirs;
		if(curent == dirAux){//dirAux este primul in lista de dirs
			parent->head_children_dirs = dirAux->next;
			curent = parent->head_children_dirs;

			if(curent == NULL){//avem doar un element in lista
				parent->head_children_dirs = dirAux;
				return;
			}
		}
		else{
			while(curent->next != dirAux){
				curent = curent->next;
			}
			curent->next = dirAux->next;
		}

		//Se insereaza structura noua la sfarsitul listei
		dirAux->next = NULL;
		while(curent->next != NULL){
			curent = curent->next;
		}
		curent->next = dirAux;
		return;
	}

	printf("File/Director not found\n");
}

int main () {

	Dir* home = NULL;
	MakeHome(&home);
	Dir* curentDir = home;

	char *line = (char*)calloc(MAX_INPUT_LINE_SIZE + 1, sizeof(char));
	do
	{

		if(fgets (line, MAX_INPUT_LINE_SIZE, stdin) == NULL)
			continue;
		line[strlen(line) - 1] = '\0';



		char* command = strtok(line, " ");
		if(strcmp(command, "touch") == 0){
			char* name = strtok(NULL, " ");

			touch(curentDir, name);
			continue;
		}

		if(strcmp(command, "ls") == 0){
			
			ls(curentDir);
			continue;
		}

		if(strcmp(command, "mkdir") == 0){
			char* name = strtok(NULL, " ");

			mkdir(curentDir, name);
			continue;
		}
		if(strcmp(command, "rm") == 0){
			char* name = strtok(NULL, " ");

			rm(curentDir, name);
			continue;
		}
		if(strcmp(command, "rmdir") == 0){
			char* name = strtok(NULL, " ");

			rmdir(curentDir, name);
			continue;
		}
		if(strcmp(command, "cd") == 0){
			char* name = strtok(NULL, " ");

			cd(&curentDir, name);
			continue;
		}
		if(strcmp(command, "pwd") == 0){

			char* pwdResult = pwd(curentDir);
			printf("/%s\n", pwdResult);
			free(pwdResult);
			continue;
		}
		if(strcmp(command, "tree") == 0){

			tree(curentDir, 0);
			
			continue;
		}
		if(strcmp(command, "mv") == 0){
			char* oldName = strtok(NULL, " ");
			char* newName = strtok(NULL, " ");

			mv(curentDir, oldName, newName);
			continue;
		}
		//printf("Nu exista comanda%s\n", command);
	} while (strcmp(line, "stop"));

	DezalocRecursivDir(&home);
	free(line);

	return 0;
}
