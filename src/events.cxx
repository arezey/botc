/*
 *	botc source code
 *	Copyright (C) 2012 Santeri `Dusk` Piippo
 *	All rights reserved.
 *	
 *	Redistribution and use in source and binary forms, with or without
 *	modification, are permitted provided that the following conditions are met:
 *	
 *	1. Redistributions of source code must retain the above copyright notice,
 *	   this list of conditions and the following disclaimer.
 *	2. Redistributions in binary form must reproduce the above copyright notice,
 *	   this list of conditions and the following disclaimer in the documentation
 *	   and/or other materials provided with the distribution.
 *	3. Neither the name of the developer nor the names of its contributors may
 *	   be used to endorse or promote products derived from this software without
 *	   specific prior written permission.
 *	4. Redistributions in any form must be accompanied by information on how to
 *	   obtain complete source code for the software and any accompanying
 *	   software that uses the software. The source code must either be included
 *	   in the distribution or be available for no more than the cost of
 *	   distribution plus a nominal fee, and must be freely redistributable
 *	   under reasonable conditions. For an executable file, complete source
 *	   code means the source code for all modules it contains. It does not
 *	   include source code for modules or files that typically accompany the
 *	   major components of the operating system on which the executable file
 *	   runs.
 *	
 *	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *	AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *	IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *	ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 *	LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *	CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *	SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *	INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *	CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *	ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *	POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdlib.h>
#include <stdio.h>
#include "main.h"
#include "scriptreader.h"
#include "str.h"
#include "events.h"

EventDef* g_EventDef;

// ============================================================================
// Read event definitions from file
void ReadEvents () {
	ScriptReader* r = new ScriptReader ("events.def");
	g_EventDef = null;
	EventDef* curdef = g_EventDef;
	unsigned int numEventDefs = 0;
	while (r->Next()) {
		EventDef* e = new EventDef;
		e->name = r->token;
		e->number = numEventDefs;
		e->next = null;
		
		// g_EventDef becomes the first eventdef
		if (!g_EventDef)
			g_EventDef = e;
		
		if (!curdef) {
			curdef = e;
		} else {
			curdef->next = e;
			curdef = e;
		}
		numEventDefs++;
	}
	
	delete r;
	printf ("%d event definitions read.\n", numEventDefs);
}

// ============================================================================
// Delete event definitions recursively
void UnlinkEvents (EventDef* e) {
	if (e->next)
		UnlinkEvents (e->next);
	delete e;
}

// ============================================================================
// Finds an event definition by index
EventDef* FindEventByIdx (unsigned int idx) {
	EventDef* e = g_EventDef;
	while (idx > 0) {
		if (!e->next)
			return null;
		e = e->next;
		idx--;
	}
	return e;
}

// ============================================================================
// Finds an event definition by name
EventDef* FindEventByName (string a) {
	EventDef* e;
	for (e = g_EventDef; e->next != null; e = e->next) {
		if (a.to_uppercase() == e->name.to_uppercase())
			return e;
	}
	
	return null;
}