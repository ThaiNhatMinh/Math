#include "iostream.h"
#include "Mesh\SMDModel.h"

void main_SMD()
{
	SMDFile file;
	file.Init("Game\\Model\\SG552\\v_rif_sg552_reference.smd");

	/*FILE* pF = fopen("Text.txt", "rt");
	fpos_t fpos;
	fgetpos(pF, &fpos);
	printf("POS %d\n", fpos);
	char buffer[56];
	fscanf(pF, "%s\n", buffer);
	fgetpos(pF, &fpos);
	printf("POS %d\n", fpos);
	fpos -= (strlen(buffer) + 2);
	fsetpos(pF, &fpos);
	fgetpos(pF, &fpos);
	printf("POS %d\n", fpos);*/
	system("pause");
}