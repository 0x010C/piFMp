#include <stdlib.h>
#include <stdio.h>

#include "main.h"

void T()
{
	int i=0;
	FILE *file = NULL;
	file = fopen("trace.log","a+");

	fprintf(file,"=============================================\n\n");

	fprintf(file,"filelist->nbFile : %d#\n",filelist->nbFile);
	fprintf(file,"filelist->currentPath : %s#\n",filelist->currentPath);
	for(i=0;i<filelist->nbFile;i++)
		fprintf(file,"filelist->list[%d] : %s#\n",i,filelist->list[i]);

	fprintf(file,"\nparam->frequence : %d#\n", param->frequence);
	fprintf(file,"param->playedIndex : %d#\n", param->playedIndex);
	fprintf(file,"param->mode : %d#\n\n", param->mode);

	fprintf(file,"=============================================\n\n");

	fclose(file);
}

void test()
{
	fl_init("/home/kiwi/");
	printf("%d\n",isMusic("DSC05558.mp3"));
	fl_end();
}

int main(int argc, char **argv)
{
#ifndef __TEST__
	/* Initialisation des variables globales */
	playlist = NULL;
	filelist = NULL;
	param = NULL;
	
	/* Parsage des arguments */
	pa_parse(argc, argv);
	
	/* Début de la boucle de gestion des évènements */
	ev_loop();
#else
	test();
#endif
	return 0;
}