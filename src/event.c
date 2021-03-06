#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>
#include <string.h>

#include "main.h"

void ev_break()
{
	getchar();
}

void ev_init()
{
	/* Initialisation des différents modules */
	di_init();
	fl_init("/");
	pl_init();
	co_init();

	/* Génération de l'affichage de départ */
	di_updateBoxing("/","Playlist");
	di_updatePlaylist(0, 0, -1);
	di_updateFilelist(0,0);
	di_refresh();
}

int ev_loop()
{
	int touche = ' ';
	int fl_index = 0, fl_firstIndex = 0;
	int pl_index = 0, pl_firstIndex = 0;

	ev_init();
	while((touche=getch()) != 'q')
	{
		switch(param->mode)
		{
			case mo_file:
				switch(touche)
				{
					case KEY_DOWN: /* Flèche bas */
						fl_index += (fl_index < filelist->nbFile-1);
						fl_firstIndex += (fl_index > fl_firstIndex+HEIGHT-2-1);
						break;

					case KEY_UP: /* Flèche haut */
						fl_index -= (fl_index > 0);
						fl_firstIndex -= (fl_index < fl_firstIndex);
						break;

					case '\n': /* Entrer */
						if(fl_changePath(fl_index))
						{
							fl_index = 0;
							fl_firstIndex = 0;
						}
						break;

					case KEY_BACKSPACE: /* Retour arrière */
						if(fl_changePath(1))
						{
							fl_index = 0;
							fl_firstIndex = 0;
						}
						break;

					case '\t': /* Tab */
						param->mode = mo_play;
						break;

					case 'a': /* a */
						if(isMusic(filelist->list[fl_index]))
							pl_add(filelist->currentPath, filelist->list[fl_index]);
						break;

					case 'A': /* A */
						pl_addAll(fl_index);
						break;
				}
				break;
			
			case mo_play:
				switch(touche)
				{
					case KEY_DOWN: /* Flèche bas */
						pl_index += (pl_index < playlist->nbFile-1);
						pl_firstIndex += (pl_index > pl_firstIndex+HEIGHT-2-1);
						break;

					case KEY_UP: /* Flèche haut */
						pl_index -= (pl_index > 0);
						pl_firstIndex -= (pl_index < pl_firstIndex);
						break;

					case '\n': /* Entrer */
						lp_stop();
						param->playedIndex = pl_index;
						lp_start();
						break;
					
					case ' ': /* Espace */
						if(lpThread == 0)
							lp_continue();
						else
							lp_pause();
						break;
					
					case 's': /* s */
						lp_stop();
						break;

					case '\t': /* Tab */
						param->mode = mo_file;
						break;

					case 'c': /* c */
						pl_remove(pl_index);
						if(pl_index == param->playedIndex)
							lp_stop();
						break;

					case 'C': /* C */
						pl_removeAll();
						pl_index = 0;
						pl_firstIndex = 0;
						
						lp_stop();
						break;
				}
				break;
		}

		di_updateBoxing(filelist->currentPath, "Playlist");
		di_updateFilelist(fl_firstIndex, fl_index);
		di_updatePlaylist(pl_firstIndex, pl_index, param->playedIndex);
		di_refresh();
	}
	ev_end();
	
	return 0;
}

void ev_end()
{
	param->sigEnd = True;
	pthread_join(coThread, NULL);
	if(lpThread != 0)
		lp_stop();
	
	di_end();
	fl_end();
	pl_end();
	pa_end();
}
