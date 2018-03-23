# NOTE: this is a GNU Makefile.  You must use "gmake" rather than "make"
#
# The compilation rules are defined in $(TOPDIR)/Makefile.user
# Please don't update them
#
# To add generate a new program, just update the PROGRAMS target below
PROGRAMS = halt hello shell matmult sort send receive

#permet d'ajouter directement tout les .c de test/, de retirer le ".c" final et de les mettre en ligne
#PROGRAMS = `ls -l | grep -E ".*\.c$" | rev | cut -d " " -f 1 | rev | cut -d "." -f 1| tr '\n' ' '`

all: $(PROGRAMS)

TOPDIR = ../
include $(TOPDIR)/Makefile.user

# Dependances
$(PROGRAMS): % : $(USERLIB)/sys.o $(USERLIB)/libnachos.o %.o
