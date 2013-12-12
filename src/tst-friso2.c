/*
 * friso test program.
 *
 * @author	chenxin
 * @email	chenxin619315@gmail.com
 */
#include "friso_API.h"
#include "friso.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/**
 * File Explain.
 * 
 * @author	chenxin<chenxin619315@gmail.com>
 */

#define __LENGTH__ 15
#define __INPUT_LENGTH__ 20480
#define ___EXIT_INFO___					\
    println("Thanks for trying friso.");		\
break;

#define ___ABOUT___					\
    println("+-----------------------------------------------------------+");	\
    println("| friso - a chinese word segmentation writen by c.          |");	\
    println("| bug report email - chenxin619315@gmail.com.               |");	\
    println("| or: visit http://code.google.com/p/friso.                 |");	\
    println("|     java edition for http://code.google.com/p/jcseg       |");	\
    println("| type 'quit' to exit the program.                          |");	\
    println("+-----------------------------------------------------------+");

//read a line from a command line.
static fstring getLine( FILE *fp, fstring __dst ) {
    register int c;
    register fstring cs;

    cs = __dst;
    while ( ( c = getc( fp ) ) != EOF ) {
	if ( c == '\n' ) break;
	*cs++ = c; 
    }
    *cs = '\0';

    return ( c == EOF && cs == __dst ) ? NULL : __dst;
}

/*static void printcode( fstring str ) {
  int i,length;
  length = strlen( str );
  printf("str:length=%d\n", length );
  for ( i = 0; i < length; i++ ) {
  printf("%d ", str[i] );
  }
  putchar('\n');
  }*/

int main(int argc, char **argv) {

    // clock_t s_time, e_time;
    char line[__INPUT_LENGTH__] = {0};
    int i;
    fstring __path__ = NULL;

    friso_t friso;
    friso_config_t config;
    friso_task_t task;

    //get the lexicon directory
    for ( i = 0; i < argc; i++ ) {
	if ( strcasecmp( "-init", argv[i] ) == 0 ) {
	    __path__ = argv[i+1];
	}
    }
    if ( __path__ == NULL ) {
	// println("Usage: friso -init lexicon path");
	exit(0);
    }

    // s_time = clock();

    //initialize
    friso = friso_new();
    config = friso_new_config();
	/*friso_dic_t dic = friso_dic_new();
	friso_dic_load_from_ifile( dic, __path__, __LENGTH__ );
	friso_set_dic( friso, dic );
	friso_set_mode( friso, __FRISO_COMPLEX_MODE__ );*/
    if ( friso_init_from_ifile(friso, config, __path__) != 1 ) {
	// printf("fail to initialize friso and config.");
	goto err;
    }
    //friso->mode = __FRISO_SIMPLE_MODE__;
    //printf("clr_stw=%d\n", friso->clr_stw);
    //printf("match c++?%d\n", friso_dic_match( friso->dic, __LEX_ENPUN_WORDS__, "c++" ));
    //printf("match(研究)?%d\n", friso_dic_match( friso->dic, __LEX_CJK_WORDS__, "研究"));

    // e_time = clock();

    // printf("friso initialized in %fsec\n", (double) ( e_time - s_time ) / CLOCKS_PER_SEC );
    // ___ABOUT___;

    //set the task.
    task = friso_new_task();

    while ( 1 ) {
	//print("friso>> ");
	//getLine( stdin, line );
    fstring ret = getLine(stdin, line);
	//exit the programe
	//if ( strcasecmp( line, "" ) == 0 ) {
    if ( ret == NULL ){
	    //___EXIT_INFO___
        break;
	} else if ( strcasecmp(ret, "") == 0){
        continue;
    }

	//for ( i = 0; i < 1000000; i++ ) {
	//set the task text.
	friso_set_text( task, line );
	//println("分词结果:");

	//s_time = clock();
	while ( ( friso_next( friso, config, task ) ) != NULL ) {
	    //printf("%s[%d]/ ", task->hits->word, task->hits->offset );
	    printf("%s ", task->hits->word );
	}
	//}
	//e_time = clock();
	//printf("\nDone, cost < %fsec\n", ( (double)(e_time - s_time) ) / CLOCKS_PER_SEC );

    }

    friso_free_task( task );

    //error block.
err:
    friso_free_config(config);
    friso_free(friso);
    

    return 0;
}