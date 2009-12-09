/*

version 1:
	- for each segment, a new segN.inc file, containing extrns to all symbols in that segment (procs, locs and data)
	- for each function, a new .asm file with:
		- includes seg0..segN.inc, EXCEPT for the segment where the function belongs
		- extrns for all symbols in the same segment EXCEPT the current function
		- the .asm implementation of the function and related data
		- public definitions of data and procs
	- the datasegment must be split into at least two files, since the source code is too big for tasm

(didnt work too well on the linking bit, too many symbols declared over and over again)

version 2:

	- for each segment:
		- an .inc file to be included from all the other segments
		- an .asm file w/all code and data on that segment



TODO:
	- structs dont work: (workedaround manually for now)

	- must declare arg_N (var_?) in function chunks from the original function! 
	  gives one compile error only but many could be hidden

	- doesnt create an entry point, manually change "end" to "end start" at the end of seg010.asm
	
	- datasegment is too big, must be split up in three parts manually

*/

#include "idc.idc"

static isAnyName(flags) {
	return (flags & FF_ANYNAME) != 0;
}

static isFunction(flags) {
	return isAnyName(flags) && (flags & FF_FUNC) != 0;
}

static GetMemberTypeString(funcframe, memberofs) {
	auto flags;
	flags = GetMemberFlag(funcframe, memberofs);
	if (isByte(flags)) return "byte";
	if (isUnknown(flags)) return "byte";
	if (isWord(flags)) return "word";
	if (isDwrd(flags)) return "dword";
	if (isStruct(flags)) return GetStrucName(GetMemberStrId(funcframe, memberofs));
}

static GetTypeString(flags, ea) {
	if (isByte(flags)) return "byte";
	if (isUnknown(flags)) return "byte";
	if (isASCII(flags)) return "byte";
	if (isWord(flags)) return "word";
	if (isDwrd(flags)) return "dword";
	if (isStruct(flags)) return "struc";
}

static NextNotTail2(ea, maxea) {
	auto b;
	b = NextNotTail(ea);
	if (b >= maxea) return BADADDR;
	return b;
}

static PrintFrame(f, funcstart) {

	auto funcframe, framesize, framelsize, framersize, frameasize;
	auto memberofs, membername, memberflag, memberloc;

	funcframe = GetFrame(funcstart);
	if (funcframe != 0) {
		framesize = GetStrucSize(funcframe);
		framelsize = GetFrameLvarSize(funcstart);
		framersize = GetFrameRegsSize(funcstart);
		frameasize = GetFrameArgsSize(funcstart);
		
		for (memberofs = 0; memberofs < framesize; memberofs = GetStrucNextOff(funcframe, memberofs)) {
			membername = GetMemberName(funcframe, memberofs);
			memberflag = GetMemberFlag(funcframe, memberofs);
			
			memberloc = -framelsize + memberofs;
		
			if (memberflag != -1) {
				fprintf(f, "    %s = %s ptr %i\n", membername, GetMemberTypeString(funcframe, memberofs), memberloc);
			}
		}
	}
}

static PrintExterns(f, segstart, segend, exceptstart, exceptend) {

	auto funcea, flags, labelname;

	for (funcea = segstart; funcea != BADADDR; funcea = NextNotTail2(funcea, segend)) {
	
		// print extrns for all data and procs in this segment
		flags = GetFlags(funcea);
		
		if (exceptstart != BADADDR && funcea >= exceptstart && funcea < exceptend) continue;
		
		if (isAnyName(flags)) {
			if (isCode(flags)) {
			
				labelname = NameEx(BADADDR, funcea);
				if (isFunction(flags)) {
					fprintf(f, "    extrn %s:proc\n", labelname);
				} else {
					// only a fraction of the locs need be extrnalized, so use a hardcoded list of allowed labels
					// allowing all labels gives out of memory error
					if (
						labelname == "loc_2CE06" || 
						labelname == "loc_2EB62" || 
						labelname == "loc_30011" || 
						labelname == "loc_3002A" || 
						labelname == "loc_301FD" || 
						labelname == "loc_30329" || 
						labelname == "loc_308C6" || 
						labelname == "loc_309A5" || 
						labelname == "loc_30AFB" || 
						labelname == "loc_30DA1" || 
						labelname == "loc_30E14" || 
						labelname == "loc_30FB2" || 
						labelname == "loc_310CD" || 
						labelname == "loc_31498" || 
						labelname == "loc_317CE" || 
						labelname == "loc_317FB" || 
						labelname == "loc_3180A" || 
						labelname == "locret_31AA3" || 
						labelname == "loc_32334" || 
						labelname == "loc_32519" || 
						labelname == "loc_326E4" || 
						labelname == "loc_3284A" || 
						labelname == "loc_32882" || 
						labelname == "loc_32B78" || 
						labelname == "loc_32B93" || 
						labelname == "loc_32BDE" || 
						labelname == "loc_3301F" || 
						labelname == "loc_335CF" || 
						labelname == "loc_335D7" || 
						labelname == "loc_33622" || 
						labelname == "loc_33697" || 
						labelname == "loc_338C9" || 
						labelname == "loc_33A57" || 
						labelname == "loc_33B1D" || 
						labelname == "loc_33BF5" || 
						labelname == "loc_33D69" || 
						labelname == "loc_33E1B" || 
						labelname == "loc_33E27" || 
						labelname == "loc_33EED" || 
						labelname == "loc_340BD" || 
						labelname == "loc_3424B" || 
						labelname == "loc_34311" || 
						labelname == "loc_343E9" || 
						labelname == "loc_34541" || 
						labelname == "loc_345E5" || 
						labelname == "loc_346A8" || 
						labelname == "loc_34799" || 
						labelname == "loc_35ED9" || 
						labelname == "loc_390C8" || 
						labelname == "loc_3ACD8" || 
						labelname == "loc_2E0BE" || 
						labelname == "loc_2E18F" || 
						labelname == "loc_2E61E" || 
						labelname == "loc_2E626" || 
						labelname == "loc_2E635" || 
						labelname == "__dosretax" ||
						labelname == "__cxtoa" ||
						labelname == "__cltoasub" ||
						labelname == "__amsg_exit"
					) {
						fprintf(f, "    extrn %s\n", labelname);
					}
				}
			} else 
			if (isData(flags) || isUnknown(flags)) {
				fprintf(f, "    extrn %s:%s\n", NameEx(BADADDR, funcea), GetTypeString(flags, funcea));
			}
		}
	
	}
}


static PrintPublics(f, segstart, segend) {

	auto funcea, flags;

	for (funcea = segstart; funcea != BADADDR; funcea = NextNotTail2(funcea, segend)) {
	
		// print extrns for all data and procs in this segment
		flags = GetFlags(funcea);
		
		if (funcea < segstart || funcea > segend) continue;
		
		if (isAnyName(flags)) {
			if (isCode(flags)) {
			
				if (isFunction(flags)) {
					fprintf(f, "    public %s\n", NameEx(BADADDR, funcea));
				} else {
					//fprintf(f, "LABEL: %s\n", NameEx(BADADDR, funcea));
				}
			} else 
			if (isData(flags) || isUnknown(flags)) {
				fprintf(f, "    public %s\n", NameEx(BADADDR, funcea));
			}
		}
	
	}
}

static PrintAsmHeader(f, codestart, codeend) {
	auto segea, nextseg;
	fprintf(f, ".model large\n");
	
	for (segea = FirstSeg(); segea != BADADDR; segea = nextseg) {
		nextseg = NextSeg(segea);
		
		if (codestart < segea || codestart >= nextseg) {
			fprintf(f, "    include %s.inc\n", SegName(segea));
		}
	}

	fprintf(f, "%s segment byte public 'CODE' use16\n", SegName(codestart));
	fprintf(f, "    assume cs:%s\n", SegName(codestart));
	fprintf(f, "    assume es:nothing, ss:nothing, ds:dseg\n");
	
	// place all segment includes here pls
	PrintExterns(f, SegStart(codestart), SegEnd(codestart), codestart, codeend);

	PrintPublics(f, codestart, codeend);

}

static PrintFunction(f, funcstart, funcend) {
//	GenerateFile(OFILE_ASM, f, funcstart, funcend, 0);
//	GenerateFile(OFILE_ASM, f, funcstart, funcend, GENFLG_ASMINC);

	auto segea, nextseg;
	auto funcname, funcflags, funcspec, funcbody, bodyflags;
	auto locname;
	auto maxfuncs, funccount, filename, i;

	funcname = GetFunctionName(funcstart);


	funcflags = GetFunctionFlags(funcstart);

	if (funcflags & FUNC_FAR)
		funcspec = " far"; else
		funcspec = "";
	
	fprintf(f, "%s proc%s\n", funcname, funcspec);

	PrintFrame(f, funcstart);

	fprintf(f, "\n");

	Message("WEHAT SIAIAUS");
	PrintBody(f, funcstart, funcend, 1);

	fprintf(f, "%s endp\n", funcname);

	//fclose(f);

}

static PrintBody(f, funcstart, funcend, skipfirstlabel) {
	auto funcbody, bodyflags, locname;
	auto i;
	
	for (funcbody = funcstart; funcbody != BADADDR; funcbody = NextNotTail2(funcbody, funcend)) {
		bodyflags = GetFlags(funcbody);
		
		if (((skipfirstlabel && funcbody != funcstart) || !skipfirstlabel) && isAnyName(bodyflags)) {
			locname = GetTrueNameEx(BADADDR, funcbody);
			if (isCode(bodyflags))
				fprintf(f, "%s:\n", locname); else
			if (isData(bodyflags) || isUnknown(bodyflags))
				fprintf(f, "%s ", locname); else
				Message("Unhandled name!");
		}

		if (isAlign(bodyflags)) {
			fprintf(f, "    ; %s\n", GetDisasm(funcbody));
			for (i = 0; i < ItemSize(funcbody); i++) {
				fprintf(f, "    db %i\n", Byte(funcbody));
			}
		} else {
			fprintf(f, "    %s\n", GetDisasm(funcbody));
		}
	}

}

static PrintSegInc(segstart, segend) {

	auto funcea, flags, f, filename;
	auto segname;

	segname = SegName(segstart);
	filename = form("src\\%s.inc", segname);
	f = fopen(filename, "w");

	fprintf(f, "%s segment byte public 'CODE' use16\n", segname);
	//fprintf(f, "    assume cs:%s\n", segname);
	//fprintf(f, "    assume es:nothing, ss:nothing, ds:%s, fs:nothing, gs:nothing\n", segname);

	PrintExterns(f, segstart, segend, BADADDR, BADADDR);

	fprintf(f, "%s ends\n", segname);
	fclose(f);

}

static main() {

	auto segea, funcea, nextseg, endfunc, nextfunc;
	auto maxfuncs, funccount;
	auto f, filename, flags;

	
	maxfuncs = 5;
	funccount = 0;

	Message("Generating segment includes...\n");
	for (segea = FirstSeg(); segea != BADADDR; segea = nextseg) {
		nextseg = NextSeg(segea);
		PrintSegInc(segea, nextseg);
	}

// TODO: SISTE FUNKSJONEN I SEG37 FUCXER OP!         .. .MEN IKKE NÅ?

	Message("Generating functions in segment...\n");
	for (segea = FirstSeg(); segea != BADADDR; segea = nextseg) {
		nextseg = NextSeg(segea);
		
		//if (SegName(segea) != "seg037") continue;
		//if (SegName(segea) != "seg002") continue;
		if (SegName(segea) != "dseg") continue;
		
		Message("Segment %i, %s\n", segea, SegName(segea));

		filename = form("src\\%s.asm", SegName(segea));
		
		Message("%s\n", filename);
	
		f = fopen(filename, "w");
		
		PrintAsmHeader(f, segea, nextseg);

		for (funcea = segea; funcea != BADADDR; funcea = nextfunc) {
			nextfunc = NextFunction(funcea);
			if (nextseg <= nextfunc || nextfunc == BADADDR) {
				endfunc = nextseg; 
				nextfunc = BADADDR;
			} else
				endfunc = nextfunc;
			
			flags = GetFlags(funcea);
			if (isFunction(flags)) {
				Message("function at %i-%i\n", funcea, endfunc);
				PrintFunction(f, funcea, endfunc);
			} else 
			if (isCode(flags)) {
				Message("unhandlet code at %i-%i\n", funcea, endfunc);
			} else
			if (isData(flags)) {
				Message("data at %i-%i\n", funcea, endfunc);
				PrintBody(f, funcea, endfunc, 0);
				//Message("unhandled data - should be at the beginning of a segment that doesnt start with a function!\n");
			}

			//funccount++;
			//if (funccount > maxfuncs) break;
			//continue;
		}
		
		fprintf(f, "%s ends\n", SegName(segea));
		fprintf(f, "end\n");
		fclose(f);
		
		//break;
	}

	Message("Anders rules");

}
