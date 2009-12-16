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

static GetIdbDirectory() {
	auto path, ls;
	path = GetIdbPath();
	ls = LastIndexOf(path, "\\");
	return substr(path, 0, ls);
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

static IsPublicLabel(labelname) {
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
		return 1;
	}
	return 0;
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
					fprintf(f, "    extrn %s:proc\n", PortFuncName(labelname));
				} else {
					// only a fraction of the locs need be extrnalized, so use a hardcoded list of allowed labels
					// allowing all labels gives out of memory error
					if (IsPublicLabel(labelname)) {
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

	auto funcea, flags, labelname;

	for (funcea = segstart; funcea != BADADDR; funcea = NextNotTail2(funcea, segend)) {
	
		// print extrns for all data and procs in this segment
		flags = GetFlags(funcea);
		
		if (funcea < segstart || funcea > segend) continue;
		
		if (isAnyName(flags)) {
			labelname = NameEx(BADADDR, funcea);
			if (isCode(flags)) {
			
				if (isFunction(flags) || IsPublicLabel(labelname)) {
					fprintf(f, "    public %s\n", PortFuncName(labelname));
				} else {
					//fprintf(f, "LABEL: %s\n", NameEx(BADADDR, funcea));
				}
			} else 
			if (isData(flags) || isUnknown(flags)) {
				fprintf(f, "    public %s\n", labelname);
			}
		}
	
	}
}

static PrintAsmHeader(f, codestart, codeend) {
	auto segea, nextseg, segtype;
	fprintf(f, ".model large\n");
	fprintf(f, "nosmart\n");
	
	segtype = GetSegmentAttr(codestart, SEGATTR_TYPE);
	if (segtype == SEG_BSS)
		fprintf(f, ".stack %i\n", SegEnd(codestart) - SegStart(codestart));
	
	fprintf(f, "    include structs.inc\n");
	fprintf(f, "    include custom.inc\n");
	
	for (segea = FirstSeg(); segea != BADADDR; segea = nextseg) {
		nextseg = NextSeg(segea);
		
		if (codestart < segea || codestart >= nextseg) {
			fprintf(f, "    include %s.inc\n", SegName(segea));
		}
	}

	PrintSegDecl(f, codestart);
		
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
		funcspec = " near";
	
	fprintf(f, "%s proc%s\n", PortFuncName(funcname), funcspec);
	PrintFrame(f, funcstart);
	fprintf(f, "\n");
	PrintBody(f, funcstart, funcend, 1);
	fprintf(f, "%s endp\n", PortFuncName(funcname));
}

static IsPushCs(ea) {
	auto mnem, op;
	mnem = GetMnem(ea);
	
	if (mnem != "push") return 0;
	
	op = GetOpnd(ea, 0);
	
	return op == "cs";
}

static PrintBody(f, funcstart, funcend, skipfirstlabel) {
	auto funcbody, bodyflags, locname;
	auto i;
	auto fchunkstart, fchunklast;
	auto lastpushcs;
	
	fchunklast = -1;
	lastpushcs = 0;
	for (funcbody = funcstart; funcbody != BADADDR; funcbody = NextNotTail2(funcbody, funcend)) {
		bodyflags = GetFlags(funcbody);
		
		fchunkstart = GetFchunkAttr(funcbody, FUNCATTR_START);
		if (fchunkstart != -1 && fchunkstart != funcstart && fchunkstart != fchunklast) {
			// its a function chunk, write out the frame for the original function
			PrintFrame(f, fchunkstart);
			fchunklast = fchunkstart;
		}
		
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
				fprintf(f, "    db %i\n", Byte(funcbody + i));
			}
		} else 
		if (isData(bodyflags) || (!isCode(bodyflags) && isUnknown(bodyflags))) {
			// jump tables arent written out correctly - what to do??
			//fprintf(f, "    ; JUMP TABLE!");
			//GenerateFile(OFILE_ASM, f, funcbody, funcbody + ItemSize(funcbody), 0);

			if (isOff0(bodyflags)) {
				fprintf(f, "    %s\n", GetDisasm(funcbody));
			} else
			if (isSeg0(bodyflags)) {
				fprintf(f, "    %s\n", GetDisasm(funcbody));
			} else
			if (isDwrd(bodyflags)) {
				for (i = 0; i < ItemSize(funcbody) / 4; i++) {
					if (hasValue(bodyflags))
						fprintf(f, "    dd %i\n", Dword(funcbody + i * 4)); else
					if (SegName(funcbody) == "dseg")
						fprintf(f, "    dd 0\n"); else
						fprintf(f, "    dd ?\n");
				}
			} else
			if (isWord(bodyflags)) {
				for (i = 0; i < ItemSize(funcbody) / 2; i++) {
					if (hasValue(bodyflags))
						fprintf(f, "    dw %i\n", Word(funcbody + i * 2)); else
					if (SegName(funcbody) == "dseg")
						fprintf(f, "    dw 0\n"); else
						fprintf(f, "    dw ?\n");
				}
			} else
			if (isByte(bodyflags) || isUnknown(bodyflags) || isASCII(bodyflags)) {
				for (i = 0; i < ItemSize(funcbody); i++) {
					if (hasValue(bodyflags))
						fprintf(f, "    db %i\n", Byte(funcbody + i)); else
					if (SegName(funcbody) == "dseg")
						fprintf(f, "    db 0\n"); else
						fprintf(f, "    db ?\n");
				}
			} else {
				Message("TODO: unhandled data size\n");
			}
		} else {
			 PrintFixedAsm(f, funcbody, lastpushcs);
			//fprintf(f, "    %s\n", GetDisasm(funcbody));
			
			lastpushcs = IsPushCs(funcbody);
			
			//if (lastpushcs)
			//Message("%s\n", GetDisasm(funcbody));
		}
	}
}

static PrintFixedAsm(f, ea, lastpushcs) {

	auto tempsmart, nooutput;

	// issues:
	// or si, <byte>, tasm compiles to 3 bytes, must be 4 bytes
	// call <far method in same segment>, tasm compiles to "push cs, call near <method>"
	// lea bx, unk_999, tasm compiles to mov bx, unk_999

	// with "nosmart", these are fixed, but other problems appear

	auto mnem, op1, op2, optype1, optype2;
	auto funcflags, funcofs, funclocal, funcstart;
	
	tempsmart = 0;
	nooutput = 0;

	mnem = GetMnem(ea);
	op1 = GetOpnd(ea, 0);
	optype1 = GetOpType(ea, 0);
	
	if (mnem == "and" /*&& optype1 == o_reg*/) {
		//if (op1 == "si" || op1 == "di" || op1 == "bx" || op1 == "cx" || op1 == "dx" || op1 == "sp") {
			op2 = GetOpType(ea, 1);
			if (op2 == o_imm) {
				//op2 = GetOperandValue(ea, 1);
				//if (op2 >= 0 && op2 < 256) {
					fprintf(f, "smart\n");
					tempsmart = 1;
				//}
			}
		//}
	}
	
	if (mnem == "or" && optype1 == o_reg && op1 == "si") {
		op2 = GetOpType(ea, 1);
		if (op2 == o_imm) {
			op2 = GetOperandValue(ea, 1);
			if (op2 >= 0 && op2 < 256) {
				fprintf(f, "smart\n");
				fprintf(f, "    nop\n");
				tempsmart = 1;
			}
		}
	}
	
	if (mnem == "or" && optype1 == o_reg && op1 == "di") {
		op2 = GetOpType(ea, 1);
		if (op2 == o_imm) {
			op2 = GetOperandValue(ea, 1);
			if (op2 >= 0 && op2 < 256) {
				fprintf(f, "smart\n");
				tempsmart = 1;
			}
		}
	}

	if ((mnem == "or" || mnem == "and") && optype1 == o_displ) {
		//Message("OR PHRASE: %s\n", GetDisasm(ea));
		op2 = GetOpType(ea, 1);
		if (op2 == o_imm) {
			op2 = GetOperandValue(ea, 1);
			if (op2 >= 0 && op2 < 256) {
				fprintf(f, "smart\n");
				tempsmart = 1;
			}
		}
	}
	
	if (mnem == "jmp" && optype1 == o_far) {
		funcofs = Rfirst0(ea);
		funcstart =  GetFchunkAttr(funcofs, FUNCATTR_START);
		if (funcstart != funcofs) {
			Message("Translated %s into jmp far ptr %s\n", GetDisasm(ea), ExtractCallTarget(ea));
			fprintf(f, "    jmp far ptr %s\n", ExtractCallTarget(ea));
			nooutput = 1;
		}
	}
	
	if (nooutput == 0)
		fprintf(f, "    %s\n", GetDisasm(ea));

	if (tempsmart) {
		fprintf(f, "nosmart\n");
	}

/*

abandoned attempts not using nosmart/smart:

	if (mnem == "or") {
		fprintf(f, "    %s\n", GetDisasm(ea));
		op1 = GetOpnd(ea, 0);
		if (op1 == "si") {
			op2 = GetOpType(ea, 1);
			if (op2 == o_imm) {
				op2 = GetOperandValue(ea, 1);
				if (op2 >= 0 && op2 < 256) {
					Message("Adding nop for: %s\n", GetDisasm(ea));
					fprintf(f, "    %s\n", "nop");
				}
			}
		}
	} else 
	if (mnem == "call") {
		fprintf(f, "    %s\n", GetDisasm(ea));
		op1 = GetOpnd(ea, 0);
		op2 = GetOpType(ea, 0);
		if (op2 == o_near || op2 == o_far) {
			funcofs = Rfirst0(ea);
			
			//if (funcofs == GetFchunkAttr(funcofs, FUNCATTR_START)) {
			funclocal = (SegStart(ea) == SegStart(funcofs)) ? 1 : 0;
			
			if (funclocal == 1 && IsFixFunc(ExtractCallTarget(ea))) {
				Message("call to '%s'\n", ExtractCallTarget(ea));
				fprintf(f, "    %s\n", "nop");
			} else {
				if (SegName(ea) == "seg010") Message("Call to: %s\n", ExtractCallTarget(ea));
				if (SegName(ea) == "seg012") {
					Message("Call to: %s\n", ExtractCallTarget(ea));
					
				}
			}
		}
	} else 
	if (mnem == "lea") {
		fprintf(f, "    %s\n", GetDisasm(ea));
		op1 = GetOpnd(ea, 0);
		if (op1 == "bx") {
			op2 = GetOpType(ea, 1);
			if (op2 == o_mem) {
				Message("lea bx = %s %i\n", GetDisasm(ea), op2);
				fprintf(f, "    %s\n", "nop");
			}
		}
	} else 
	if (mnem == "jmp") {
		fprintf(f, "    %s\n", GetDisasm(ea));
		op1 = GetOpnd(ea, 0);
		if (op1 == "__aFldiv") {
			fprintf(f, "    %s\n", "nop");
			fprintf(f, "    %s\n", "nop");
			fprintf(f, "    %s\n", "nop");
		} else
		if (op1 == "unknown_libname_3" ||
		    op1 == "__aFlmul" ||
		    op1 == "__aFFblmul" ||
		    op1 == "unknown_libname_4" ||
		    op1 == "__aFlshr" ||
		    op1 == "__aFuldiv" ||
		    op1 == "unknown_libname_5") 
		{
			fprintf(f, "    %s\n", "nop");
			fprintf(f, "    %s\n", "nop");
		}
	} else {
		fprintf(f, "    %s\n", GetDisasm(ea));
	}
	
*/
}

static LastIndexOf(str, c) {
	auto len, testch;
	len = strlen(str);
	while (len > 0) {
		testch = substr(str, len - 1, len);
		if (testch == c) return len - 1;
		len--;
	}
	return -1;
}

static FirstIndexOf(str, c) {
	auto i, len, testch;
	len = strlen(str);
	for (i = 0; i < len; i++) {
		testch = substr(str, i, i + 1);
		if (testch == c) return i;
	}
	return -1;
}

static ExtractCallTarget(ea) {
	auto str, ls;
	str = GetDisasm(ea);
	ls = FirstIndexOf(str, ";");
	if (ls != -1) {
		str = substr(str, 0, ls);
		while (strlen(str) > 0 && LastIndexOf(str, " ") == strlen(str) -1) {
			str = substr(str, 0, strlen(str) - 1);
		}
	}
	ls = LastIndexOf(str, " ");
	return substr(str, ls + 1, -1);
}

// add functions in PortFuncName as they are ported to c
// PortFuncName returns an obscured version of the function name
// to prevent name collisions.
// functions must also be added as externals in custom.inc
static PortFuncName(labelname) {
	return labelname;
/*
	if (
		labelname == "_strcpy" ||
		labelname == "_strcmp"
	)
		return labelname + "2";
	return labelname;*/
}

static IsFixFunc(labelname) {
	if (
		labelname == "__FF_MSGBANNER" ||
		labelname == "__NMSG_WRITE" ||
		labelname == "loc_2CD27+1" ||
		labelname == "__setenvp" ||
		labelname == "__setargv" ||
		labelname == "sub_2CDEC" ||
		labelname == "__nullcheck" ||
		labelname == "__chkstk" ||
		labelname == "__NMSG_TEXT" ||
		labelname == "__maperror" ||
		labelname == "_fflush" ||
		labelname == "__stbuf" ||
		labelname == "__output" ||
		labelname == "__ftbuf" ||
		labelname == "__flsbuf" ||
		labelname == "_isatty" ||
		labelname == "_write" ||
		labelname == "_lseek" ||
		labelname == "unknown_libname_2" ||
		labelname == "_ultoa" ||
		labelname == "_strlen" ||
		labelname == "_stackavail" ||
		labelname == "_brkctl" ||
		labelname == "_raise" ||
		labelname == "sub_2CE03" ||
		labelname == "__sigentry" ||
		labelname == "__aFlmul" ||
		labelname == "__sigentry" ||
		labelname == "__aFldiv" ||
		labelname == "__aFlshr" ||
		labelname == "__aFuldiv"/* ||
		
		// seg012:
		labelname == "sub_35B14" ||
		labelname == "sub_30A5D" || 
		labelname == "sub_2FE59" ||
		labelname == "sub_2EAD4" ||
		labelname == "sub_2FE1C" ||
		labelname == "sub_2EB56" || 
		labelname == "loc_3336C" || 
		labelname == "sub_2EA2A"*/
		
		
	) 
		return 1;
	return 0;
}

static PrintSegDecl(f, ea) {
	auto segtype;
	segtype = GetSegmentAttr(ea, SEGATTR_TYPE);
	if (segtype == SEG_CODE)
		fprintf(f, "%s segment byte public 'STUNTSC' use16\n", SegName(ea)); 
	else if (segtype == SEG_BSS)
		fprintf(f, "%s segment byte public 'STACK' use16\n", SegName(ea)); 
	else 
		fprintf(f, "%s segment byte public 'STUNTSD' use16\n", SegName(ea)); 

}

static PrintSegInc(segstart, segend) {
	auto funcea, flags, f, filename;
	auto segname;

	segname = SegName(segstart);
	filename = form("%s\\src\\%s.inc", GetIdbDirectory(), segname);
	f = fopen(filename, "w");

	PrintSegDecl(f, segstart);
	PrintExterns(f, segstart, segend, BADADDR, BADADDR);
	fprintf(f, "%s ends\n", segname);

	fclose(f);
}

static PrintStruct(f, id) {
	auto memberofs, membername, memberflag, strucsize;
	fprintf(f, "%s struc\n", GetStrucName(id));

	strucsize = GetStrucSize(id);

	for (memberofs = 0; memberofs < strucsize; memberofs = GetStrucNextOff(id, memberofs)) {
		membername = GetMemberName(id, memberofs);
		memberflag = GetMemberFlag(id, memberofs);
	
		if (isDwrd(memberflag)) {
			fprintf(f, "%s dd ?\n", membername);
		} else
		if (isWord(memberflag)) {
			fprintf(f, "%s dw ?\n", membername);
		} else
		if (isByte(memberflag) || isUnknown(memberflag) || isASCII(memberflag)) {
			fprintf(f, "%s db ?\n", membername);
		} else {
			Message("TODO: unhandled data size\n");
		}
		//fprintf(f, "%s %s\n", membername, GetMemberTypeString(id, memberofs));
	}

	fprintf(f, "ends\n", id);
}

static main() {
	auto segea, funcea, nextseg, endseg, endfunc, nextfunc, segss;
	auto maxfuncs, funccount, startseg;
	auto i;
	auto f, filename, flags;

	maxfuncs = 5;
	funccount = 0;

	Message("Generating segment includes...\n");
	for (segea = FirstSeg(); segea != BADADDR; segea = nextseg) {
		nextseg = NextSeg(segea);
		PrintSegInc(segea, nextseg);
	}

	Message("Generating structs...\n");
	f = fopen(GetIdbDirectory() + "\\src\\structs.inc", "w");

	for (i = GetFirstStrucIdx(); i != -1; i = GetNextStrucIdx(i)) {
		PrintStruct(f, GetStrucId(i));
	}
	
	fclose(f);

	Message("Generating functions in segment...\n");
	for (segea = FirstSeg(); segea != BADADDR; segea = nextseg) {
		nextseg = NextSeg(segea);
		
		if (nextseg == BADADDR)
			endseg = SegEnd(segea); else
			endseg = nextseg;

		startseg = 0;

		//if (SegName(segea) != "seg012") continue;
		//if (SegName(segea) != "seg037") continue;
		//if (SegName(segea) != "seg002") continue;
		//if (SegName(segea) != "dseg") continue;
		
		// TODO: should use names in alphabetical order to ensure correct linking order
		// or generate a filelist for tlink @-syntax
		filename = form("%s\\src\\%s.asm", GetIdbDirectory(), SegName(segea));
		
		Message("Segment %i, %s: %s\n", segea, SegName(segea), filename);
	
		f = fopen(filename, "w");
		
		PrintAsmHeader(f, segea, endseg);

		//Message("HEISEG: %i\n", GetSegmentAttr(segea, SEGATTR_TYPE));

		for (funcea = segea; funcea != BADADDR; funcea = nextfunc) {
			nextfunc = NextFunction(funcea);
			if (endseg <= nextfunc || nextfunc == BADADDR) {
				endfunc = endseg; 
				nextfunc = BADADDR;
			} else
				endfunc = nextfunc;
			
			flags = GetFlags(funcea);
			if (isFunction(flags) && GetFunctionName(funcea) != "") {
				//Message("function %s at %i-%i\n", GetFunctionName(funcea), funcea, endfunc);
				PrintFunction(f, funcea, endfunc);
				if (GetFunctionName(funcea) == "start") {
					startseg = 1;
				}
			} else 
			if (isCode(flags)) {
				PrintBody(f, funcea, endfunc, 0);
				//Message("unhandlet code at %i-%i\n", funcea, endfunc);
			} else
			if (isData(flags) || hasValue(flags)) {
				//Message("data at %i-%i\n", funcea, endfunc);
				PrintBody(f, funcea, endfunc, 0);
				//Message("unhandled data - should be at the beginning of a segment that doesnt start with a function!\n");
			} else {
				Message("unhandled flags: %i\n", flags);
			}

			//funccount++;
			//if (funccount > maxfuncs) break;
			//continue;
		}
		
		fprintf(f, "%s ends\n", SegName(segea));
		if (startseg)
			fprintf(f, "end start\n"); else
			fprintf(f, "end\n");
		fclose(f);
		
		//break;
	}

	Message("Anders rules");

}
