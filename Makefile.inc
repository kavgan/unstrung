# -*- makefile -*-

-include ${UNSTRUNG_SRCDIR}/Makefile.local

default:: programs

# stuff to set... use -m32 by default because of UML testing, should remove this to elsewhere.
INC=${UNSTRUNG_SRCDIR}/include
#-I${UNSTRUNG_SRCDIR}/../tcpdump-4.6.0
CFLAGS+=-I${INC} -fno-show-column
CXXFLAGS+=-I${INC}  -fno-show-column
HOSTFLAGS=-m32

USE_OBJDIR?=false
XMLTO=xmlto
MANTREE=/usr/share/man
INSTALL=install
INSTMANFLAGS=-D
INSTBINFLAGS=-D

ifneq ($(EMBEDDED),)
CFLAGS+=-DEMBEDDED
endif

ARCH?=$(shell uname -m)
ifeq ($(USE_OBJDIR),true)
OBJDIR?=OBJ.${ARCH}
else
OBJDIR=.
endif
OBJDIRTOP?=$(shell cd ${UNSTRUNG_SRCDIR} && mkdir -p ${OBJDIR}; cd ${OBJDIR} && pwd)
UNSTRUNG_OBJDIR?=${OBJDIRTOP}
export OBJDIRTOP
OSDEP=$(shell uname -s).${ARCH}

ifeq ($(ARCH),i386)
CROSSFLAGS?=-m32
endif

LIBNDMGMT=${UNSTRUNG_OBJDIR}/lib/libndmgmt/libndmgmt.a
LIBOSW=${UNSTRUNG_OBJDIR}/lib/libosw/libosw.a
LIBNETLINK=${UNSTRUNG_OBJDIR}/lib/libnetlink/libnetlinkstrung.a
LIBFAKEIFACE=${UNSTRUNG_OBJDIR}/lib/libfakeiface/libfakeiface.a
LIBPCAP?=-lpcap $(pcap-config --libs --static)
LIBPCAP_HOST?=-lpcap
TCPDUMP?=tcpdump

ifeq ($(EMBEDDED),)
# needed by fake interface library
EXTRALIBS:=${LIBPCAP} ${EXTRALIBPATHS} -lusb-1.0 -lpthread
endif

NETDISSECTLIB?=-L${UNSTRUNG_SRCDIR}/../amd64/tcpdump -lnetdissect
NETDISSECTH?=-DNETDISSECT -I${UNSTRUNG_SRCDIR}/../tcpdump

NEEDCHECK?=check

.PHONY: TAGS clean ${NEEDCHECK} programs checkprograms install

ifeq (${MAKEFILETOP},)
TAGS clean ${NEEDCHECK} programs checkprograms install::
	@err=0; for d in $(SUBDIRS) ; \
	do \
		if $(MAKE) -C $$d UNSTRUNG_SRCDIR=${UNSTRUNG_SRCDIR} $@; then true; else err=$$(($$err + 1)); fi; \
	done; exit $$err
endif

env:
	@env | sed -e "s/'/'\\\\''/g" -e "s/\([^=]*\)=\(.*\)/\1='\2'/"

TAGS::
	@if [ -n "${CXXSRCS}${SRCS}" ]; then etags --append -o ${UNSTRUNG_SRCDIR}/TAGS ${CXXSRCS} ${SRCS}; fi

