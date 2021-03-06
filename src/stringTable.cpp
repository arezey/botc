/*
	Copyright 2012-2014 Santeri Piippo
	All rights reserved.

	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions
	are met:

	1. Redistributions of source code must retain the above copyright
	   notice, this list of conditions and the following disclaimer.
	2. Redistributions in binary form must reproduce the above copyright
	   notice, this list of conditions and the following disclaimer in the
	   documentation and/or other materials provided with the distribution.
	3. The name of the author may not be used to endorse or promote products
	   derived from this software without specific prior written permission.

	THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
	IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
	OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
	IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
	INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
	NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
	DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
	THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
	THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

// TODO: Another freeloader...

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stringTable.h"

static StringList g_StringTable;

// ============================================================================
//
const StringList& getStringTable()
{
	return g_StringTable;
}

// ============================================================================
//
// Potentially adds a string to the table and returns the index of it.
//
int getStringTableIndex (const String& a)
{
	// Find a free slot in the table.
	int idx;

	for (idx = 0; idx < g_StringTable.size(); idx++)
	{
		// String is already in the table, thus return it.
		if (g_StringTable[idx] == a)
			return idx;
	}

	// Must not be too long.
	if (a.length() >= gMaxStringLength)
		error ("string `%1` too long (%2 characters, max is %3)\n",
			   a, a.length(), gMaxStringLength);

	// Check if the table is already full
	if (g_StringTable.size() == gMaxStringlistSize - 1)
		error ("too many strings!\n");

	// Now, dump the string into the slot
	g_StringTable.append (a);
	return (g_StringTable.size() - 1);
}

// ============================================================================
//
// Counts the amount of strings in the table.
//
int countStringsInTable()
{
	return g_StringTable.size();
}
