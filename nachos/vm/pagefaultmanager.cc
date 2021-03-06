/*! \file pagefaultmanager.cc
Routines for the page fault managerPage Fault Manager
*/
//
//  Copyright (c) 1999-2000 INSA de Rennes.
//  All rights reserved.
//  See copyright_insa.h for copyright notice and limitation
//  of liability and disclaimer of warranty provisions.
//

#include "kernel/thread.h"
#include "vm/swapManager.h"
#include "vm/physMem.h"
#include "vm/pagefaultmanager.h"

PageFaultManager::PageFaultManager() {
}

// PageFaultManager::~PageFaultManager()
/*! Nothing for now
*/
PageFaultManager::~PageFaultManager() {
}

// ExceptionType PageFault(uint32_t virtualPage)
/*!
//	This method is called by the Memory Management Unit when there is a
//      page fault. This method loads the page from :
//      - read-only sections (text,rodata) $\Rightarrow$ executive
//        file
//      - read/write sections (data,...) $\Rightarrow$ executive
//        file (1st time only), or swap file
//      - anonymous mappings (stack/bss) $\Rightarrow$ new
//        page from the MemoryManager (1st time only), or swap file
//
//	\param virtualPage the virtual page subject to the page fault
//	  (supposed to be between 0 and the
//        size of the address space, and supposed to correspond to a
//        page mapped to something [code/data/bss/...])
//	\return the exception (generally the NO_EXCEPTION constant)
*/

#ifndef ETUDIANTS_TP
ExceptionType PageFaultManager::PageFault(uint32_t virtualPage)
{
    printf("**** Warning: page fault manager is not implemented yet\n");
    exit(-1);
    return ((ExceptionType)0);
}
#endif

#ifdef ETUDIANTS_TP
ExceptionType PageFaultManager::PageFault(uint32_t virtualPage){

/*
table des pages réelles tpr_c
nb pages réelles et table de pages g_cfg
g_current_thread->TranslationTable //tp processus courant
g_swap_manager
g_machine->mainMemory memoire machine
*/

    //TODODO faire marcher le truc où le bitswap est à 0 et le setadressdisk different de -1
    // tout ça est dans la mmu global avec translationTable

    /* Apparemment il faut tout virer
    */
    //on recalcul l'offset pgdisk
    pgdisk = g_current_thread->TranslationTable->getAddrDisk;
    //on renomme notre virtualPage
    virt_page = virtualPage;

    int pp  = AddPhysicalToVirtualMapping(g_current_thread->GetProcessOwner()->addrspace, virtualPage)
    if (pp == -1) {
        printf("Not enough free space to load physical page from virtual page %X\n", virtualPage);
        //g_machine->interrupt->Halt(-1);
        pp = EvictPage(); //voir 1.8.2
    }
    //on met ça dans une page physique/réelle
    //donc on doit update la tpr
    g_physical_mem_manager->tpr[pp].virtualPage = virt_page;
    g_physical_mem_manager->tpr[pp].owner = this;
    g_physical_mem_manager->tpr[pp].locked= true;
    translationTable->setPhysicalPage(virt_page, pp);

    if (/*section_table[i].sh_type != SHT_NOBITS*/)
        exec_file->ReadAt((char*)&(g_machine->mainMemory[translationTable->getPhysicalPage(virt_page)*g_cfg->PageSize]), g_cfg->PageSize, section_table[i].sh_offset + pgdisk*g_cfg->PageSize);
    else
        memset(&(g_machine->mainMemory[translationTable->getPhysicalPage(virt_page)*g_cfg->PageSize]), 0, g_cfg->PageSize);

    // Fill it with -1
    translationTable->setAddrDisk(virt_page,-1);

    // The entry is valid
    translationTable->setBitValid(virt_page);
    /**/

    return NO_EXCEPTION;
}
#endif
