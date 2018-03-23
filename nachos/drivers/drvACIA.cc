/* \file drvACIA.cc
   \brief Routines of the ACIA device driver
//
//      The ACIA is an asynchronous device (requests return
//      immediately, and an interrupt happens later on).
//      This is a layer on top of the ACIA.
//      Two working modes are to be implemented in assignment 2:
//      a Busy Waiting mode and an Interrupt mode. The Busy Waiting
//      mode implements a synchronous IO whereas IOs are asynchronous
//      IOs are implemented in the Interrupt mode (see the Nachos
//      roadmap for further details).
//
//  Copyright (c) 1999-2000 INSA de Rennes.
//  All rights reserved.
//  See copyright_insa.h for copyright notice and limitation
//  of liability and disclaimer of warranty provisions.
//
*/

/* Includes */

#include "kernel/system.h"         // for the ACIA object
#include "kernel/synch.h"
#include "machine/ACIA.h"
#include "drivers/drvACIA.h"

//-------------------------------------------------------------------------
// DriverACIA::DriverACIA()
/*! Constructor.
  Initialize the ACIA driver. In the ACIA Interrupt mode,
  initialize the reception index and semaphores and allow
  reception and emission interrupts.
  In the ACIA Busy Waiting mode, simply inittialize the ACIA
  working mode and create the semaphore.
  */
//-------------------------------------------------------------------------
#ifndef ETUDIANTS_TP
DriverACIA::DriverACIA()
{
    printf("**** Warning: contructor of the ACIA driver not implemented yet\n");
    exit(-1);
}
#endif
#ifdef ETUDIANTS_TP
DriverACIA::DriverACIA()
{
    printf("**** Warning: ACIA driver will only work in busy waiting mode for now\n");
    //on initialise les index des tampons à 0.
    ind_send = 0;
    ind_rec  = 0;

    //On crée une sémaphore pour la synchronisation des différentes instances
    //d'après drvACIA.h
    send_sema    = new Semaphore("envoi",     1);
    receive_sema = new Semaphore("reception", 1);

    //on demande à l'ACIA de se mettre en mode "attente active"
    //d'après machine/ACIA.cc
    g_machine->acia->SetWorkingMode(BUSY_WAITING);
}
#endif

//-------------------------------------------------------------------------
// DriverACIA::TtySend(char* buff)
/*! Routine to send a message through the ACIA (Busy Waiting or Interrupt mode)
  */
//-------------------------------------------------------------------------
#ifndef ETUDIANTS_TP
int DriverACIA::TtySend(char* buff)
{
    printf("**** Warning: method Tty_Send of the ACIA driver not implemented yet\n");
    exit(-1);
    return 0;
}
#endif
#ifdef ETUDIANTS_TP
int DriverACIA::TtySend(char* buff)
{
    if (g_machine->acia->GetWorkingMode() == BUSY_WAITING){
        //on empêche que plusieurs instances puissent envoyer en même temps!
        send_sema->P();

        //sécurité pour ne pas dépasser la taille du buffer
        buff[BUFFER_SIZE-1] = '\0';

        do{
            //si on a pas fini de tout envoyer, on patiente
            while(g_machine->acia->GetOutputStateReg() == FULL){ //FULL défini à 1 dans machine/ACIA.h
                printf("\rOn patiente activement le temps de l'envoi"); //réecrit sur la même ligne
            }

            puts(""); // '\n' formatage du texte de debug en console
            g_machine->acia->PutChar(buff[ind_send]);

        } while (buff[ind_send++] != '\0');
        //on arrête l'envoi après avoir envoyé NULL (c.a.d '\0').

        //on relache notre envoi
        send_sema->V();

        //tout s'est bien passé ! On remet ind_send à la bonne valeur (il a été incrémenté une fois de trop)
        return --ind_send;
    }else{
        printf("mode interruption non implémenté");
        exit(-1);
    }
}
#endif

//-------------------------------------------------------------------------
// DriverACIA::TtyReceive(char* buff,int length)
/*! Routine to reveive a message through the ACIA
//  (Busy Waiting and Interrupt mode).
  */
//-------------------------------------------------------------------------
#ifndef ETUDIANTS_TP
int DriverACIA::TtyReceive(char* buff,int lg)
{
    printf("**** Warning: method Tty_Receive of the ACIA driver not implemented yet\n");
    exit(-1);
    return 0;
}
#endif
#ifdef ETUDIANTS_TP
int DriverACIA::TtyReceive(char* buff,int lg)
{
    //protection de lg
    if (lg > BUFFER_SIZE){
        printf("**** Warning: trying to receive buffer size of size %d > %d (BUFFER_SIZE)", lg, BUFFER_SIZE);
        lg = BUFFER_SIZE;
    }else if (lg < 0){
        printf("**** Warning: trying to receive buffer size of size %d < 0", lg);
        lg = 0;
    }

    if (g_machine->acia->GetWorkingMode() == BUSY_WAITING) {
        //on empêche que plusieurs instances ne puissent recevoir en même temps!
        receive_sema->P();

        while(ind_rec < BUFFER_SIZE){
            //si on a pas finit de tout recevoir, on patiente
            while(g_machine->acia->GetInputStateReg() == EMPTY){ //EMPTY définit à 0 dans machine/ACIA.h
                // printf("\ron patiente activement le temps de la reception"); //réecrit sur la même ligne
            }
            puts(""); // '\n' formatage du texte de debug en console
            buff[ind_rec] = g_machine->acia->GetChar();

            if(buff[ind_rec] == '\0'){ //si on a reçu NULL, alors on s'arrête
                break;
            }

            //on incrémente
            ind_rec++;

        }

        if (buff[BUFFER_SIZE -1] != '\0'){
            printf("L'envoyeur n'a pas fini par \'0\'. DEFCON -1\n");
            buff[BUFFER_SIZE - 1] = '\0'; //sécurité
            //Dans le cas où l'on n'est pas sorti via le break, on a incrémenté ind_rec une fois de trop, donc on le décrémente
            ind_rec--;
        }

        //on relache notre reception
        receive_sema->V();

        //tout s'est bien passé!
        return ind_rec;
    }else{
        printf("on ne supporte pas les interruptions pour le moment");
        exit(-1);
    }
}
#endif

//-------------------------------------------------------------------------
// DriverACIA::InterruptSend()
/*! Emission interrupt handler.
  Used in the ACIA Interrupt mode only.
  Detects when it's the end of the message (if so, releases the send_sema semaphore), else sends the next character according to index ind_send.
  */
//-------------------------------------------------------------------------
#ifndef ETUDIANTS_TP
void DriverACIA::InterruptSend()
{
    printf("**** Warning: send interrupt handler not implemented yet\n");
    exit(-1);
}
#endif
#ifdef ETUDIANTS_TP
void DriverACIA::InterruptSend()
{

}
#endif

//-------------------------------------------------------------------------
// DriverACIA::Interrupt_receive()
/*! Reception interrupt handler.
  Used in the ACIA Interrupt mode only. Reveices a character through the ACIA.
  Releases the receive_sema semaphore and disables reception
  interrupts when the last character of the message is received
  (character '\0').
  */
//-------------------------------------------------------------------------
#ifndef ETUDIANTS_TP
void DriverACIA::InterruptReceive()
{
    printf("**** Warning: receive interrupt handler not implemented yet\n");
    exit(-1);
}
#endif
#ifdef ETUDIANTS_TP
void DriverACIA::InterruptReceive()
{

}
#endif
