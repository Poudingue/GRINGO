/* receive.c
 *	Simple send program
 *
//  Copyright (c) 2018 Myself.
//  All rights reserved.
//  no claim for any copyright notice or limitation involved
//  liability and disclaimer of warranty provisions:
//  USE AT YOUR OWN RISK.
 */

#include "userlib/syscall.h"
#include "userlib/libnachos.h"

#define BUFFER_SIZE 256

int
main()
{
    char buff[BUFFER_SIZE];
    n_printf("On va essayer de lire une chaine\n");

    int count = 0;
    TtyReceive(buff, BUFFER_SIZE);
    n_printf("\n%s",buff);
    n_printf("fait avec %d caractere reçu: la chaîne %s\n", count, buff);

    return 0;
}
