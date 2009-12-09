.model large
    include seg000.inc
    include seg001.inc
    include seg002.inc
    include seg003.inc
    include seg004.inc
    include seg005.inc
    include seg006.inc
    include seg007.inc
    include seg008.inc
    include seg009.inc
    include seg010.inc
    include seg011.inc
    include seg012.inc
    include seg013.inc
    include seg014.inc
    include seg015.inc
    include seg016.inc
    include seg017.inc
    include seg018.inc
    include seg019.inc
    include seg020.inc
    include seg021.inc
    include seg022.inc
    include seg023.inc
    include seg024.inc
    include seg025.inc
    include seg026.inc
    include seg027.inc
    include seg028.inc
    include seg029.inc
    include seg030.inc
    include seg031.inc
    include seg032.inc
    include seg033.inc
    include seg034.inc
    include seg035.inc
    include seg036.inc
    include seg037.inc
    include seg038.inc
    include seg039.inc
    include seg041.inc
dseg segment byte public 'DATA' use16
    assume cs:dseg
    assume es:nothing, ss:nothing, ds:dseg
    public byte_3B770
    public word_3B772
    public word_3B774
    public word_3B778
    public word_3B77A
    public byte_3B77C
    public word_3B77E
    public word_3B780
    public word_3B782
    public byte_3B784
    public word_3B79C
    public word_3B808
    public word_3B80A
    public word_3B870
    public word_3B8EA
    public word_3B8EC
    public word_3B8EE
    public word_3B8F0
    public byte_3B8F2
    public byte_3B8F3
    public byte_3B8F4
    public byte_3B8F5
    public byte_3B8F6
    public byte_3B8F7
    public byte_3B8F8
    public byte_3B8F9
    public byte_3B8FA
    public byte_3B8FB
    public byte_3B8FC
    public aKevin
    public byte_3B907
    public byte_3B90D
    public byte_3B90E
    public byte_3B90F
    public byte_3B910
    public aTitl
    public aSkidms
    public aSkidtitl
    public aSdtitl
    public aSdcred
    public aProd
    public aProd_0
    public aTitl_0
    public aCred
    public aArowarrwarw1ar
    public aCre
    public aGds0
    public aGds1
    public aDes
    public aGdon
    public aGkev
    public aGbra
    public aGrob
    public aGsta
    public aMus
    public aGmsy
    public aGkri
    public aGbri
    public aPro
    public aGkev_0
    public aOpr
    public aGbra_0
    public aGric
    public aArt
    public aGmsm
    public aGdav
    public aGnic
    public aGkev_1
    public aSdmsel
    public aScrn
    public a______________
    public a_____________0
    public a______
    public aHs1
    public aHs2
    public aHs3
    public aHs5
    public aHs4
    public a_hig
    public a_res_0
    public aCar
    public aCsel
    public aMisc_0
    public aGrap
    public a150
    public a100
    public a50
    public a0
    public a02040
    public aBdo_0
    public aBnx_0
    public aBla_0
    public aBau
    public aBma
    public aBco
    public aDes_1
    public aStop_1
    public aBau_0
    public aBma_0
    public word_3BB28
    public word_3BB32
    public word_3BB3C
    public word_3BB3E
    public word_3BB40
    public word_3BB42
    public word_3BB44
    public word_3BB4E
    public aLnam
    public word_3BB58
    public word_3BB5C
    public word_3BB60
    public word_3BB62
    public aMisc
    public aSdosel
    public aOpp0opp1opp2op
    public aScrn_0
    public aBla
    public aBnx
    public aBcl
    public aBca
    public aBdo
    public aClip
    public aDes_0
    public aRac
    public word_3BBC8
    public word_3BCEC
    public word_3BCEE
    public word_3BCF0
    public word_3BCF2
    public word_3BD00
    public word_3BD0A
    public byte_3BD17
    public byte_3BD1F
    public aLose
    public byte_3BD29
    public byte_3BD4C
    public byte_3BD50
    public byte_3BE02
    public word_3BE14
    public word_3BE16
    public aSimd
    public aGnam
    public aSimd_0
    public aGsna
    public dword_3BE2C
    public word_3BE30
    public word_3BE32
    public word_3C108
    public word_3C10A
    public word_3C10C
    public word_3C10E
    public word_3C110
    public word_3C112
    public word_3C1B6
    public word_3C1B8
    public word_3C1BA
    public word_3C1BC
    public word_3C1BE
    public aBarn
    public aZbrn
    public aBrid
    public aZbri
    public aBtur
    public aZbtu
    public aChi1
    public aZch1
    public aChi2
    public aZch2
    public aElrd
    public aZelr
    public aFini
    public aZfin
    public aGass
    public aZgas
    public aLban
    public aZlba
    public aLoop
    public aZloo
    public aOffi
    public aZoff
    public aPipe
    public aZpip
    public aRamp
    public aZram
    public aRban
    public aZrba
    public aRdup
    public aZrdu
    public aRoad
    public aZroa
    public aStur
    public aZstu
    public aTenn
    public aZten
    public aTunn
    public aZtun
    public aTurn
    public aZtur
    public aGoui
    public aGouo
    public aGoup
    public aHigh
    public aLakc
    public aLake
    public aCld1
    public aCld2
    public aCld3
    public aSigl
    public aSigr
    public aTree
    public aInte
    public aZint
    public aOffl
    public aZofl
    public aOffr
    public aZofr
    public aPalm
    public aZpal
    public aBank
    public aZban
    public aSofl
    public aZsol
    public aSofr
    public aZsor
    public aSram
    public aZsra
    public aSelr
    public aZser
    public aElsp
    public aZesp
    public aCact
    public aCact_0
    public aSpip
    public aZspi
    public aSest
    public aZses
    public aWroa
    public aZwro
    public aBarr
    public aZbar
    public aLco0
    public aZlco
    public aRco0
    public aZrco
    public aGwro
    public aZgwr
    public aLco1
    public aRco1
    public aLoo1
    public aHig1
    public aHig2
    public aHig3
    public aWind
    public aZwin
    public aBoat
    public aZboa
    public aRest
    public aZres
    public aHpip
    public aZhpi
    public aVcor
    public aZvco
    public aTun2
    public aPip2
    public aFenc
    public aZfen
    public aCfen
    public aZcfe
    public aFlag
    public aTruk
    public aExp0
    public aExp1
    public aExp2
    public aExp3
    public unk_3D19B
    public word_3E590
    public byte_3E70C
    public byte_3E70D
    public byte_3E70E
    public byte_3E70F
    public aEngi
    public aEngi_0
    public aStar
    public aStop
    public aBlow
    public aCras
    public aSkid
    public aSki2
    public aBump
    public aScra
    public aEngi_1
    public aEngi_2
    public aStar_0
    public aStop_0
    public aBlow_0
    public aCras_0
    public aSkid_0
    public aSki2_0
    public aBump_0
    public aScra_0
    public aDefault
    public aRbf
    public aCop
    public byte_3E916
    public byte_3E917
    public byte_3E918
    public byte_3E919
    public byte_3E920
    public byte_3E921
    public byte_3E922
    public byte_3E923
    public byte_3E9DB
    public word_3EA18
    public word_3EA2A
    public word_3EA3A
    public word_3EA3C
    public word_3EA4C
    public word_3EA4E
    public word_3EB02
    public word_3EB2A
    public aStartengineNew
    public aId1
    public aId2
    public aId3
    public aId4
    public aSetup_exe
    public aSdtitl_
    public aTedit_
    public aOpp1_
    public word_3EB90
    public word_3EBB0
    public word_3EBB2
    public word_3EBB4
    public word_3EBB6
    public word_3EBB8
    public word_3EBBA
    public word_3EBBC
    public word_3EBBE
    public word_3EBC0
    public word_3EBC2
    public word_3EBC4
    public a_res
    public a_pre
    public aWai
    public a_
    public byte_3EBD8
    public byte_3ECBB
    public word_3ED74
    public word_3ED76
    public off_3ED78
    public word_3ED7A
    public word_3EDCA
    public aC_file_info
    public dword_3EDD9
    public word_3EDE5
    public word_3EDEB
    public byte_3EDED
    public byte_3EDF0
    public word_3EDF2
    public argc
    public argv
    public word_3EE0C
    public word_3EE0E
    public seg_3EE10
    public word_3EE14
    public byte_3EE16
    public byte_3EE17
    public dword_3EE18
    public unk_3EE1C
    public word_3EE1E
    public dword_3EE20
    public word_3EE24
    public dword_3EE26
    public word_3EE3E
    public word_3EF58
    public aNull
    public aNull_0
    public word_3EF6E
    public word_3EF70
    public word_3EF74
    public word_3EF78
    public word_3EF7C
    public word_3EF7E
    public byte_3EF82
    public off_3EF84
    public off_3EF88
    public off_3EF90
    public off_3EF94
    public word_3EF98
    public word_3EF9A
    public word_3F0A0
    public word_3F0A2
    public word_3F0A4
    public word_3F0A6
    public dword_3F0BC
    public word_3F1C2
    public word_3F1C4
    public word_3F7D8
    public byte_3F85A
    public dword_3F874
    public word_3F878
    public word_3F87A
    public word_3F87C
    public word_3F87E
    public byte_3F880
    public byte_3F881
    public word_3F882
    public word_3F884
    public word_3F886
    public word_3F888
    public word_3F88A
    public byte_3F88C
    public word_3F88E
    public word_3F890
    public word_3F892
    public unk_3F9CC
    public byte_3F9E0
    public word_3FB02
    public word_3FB04
    public byte_3FB06
    public byte_3FB08
    public byte_3FB09
    public byte_3FB0A
    public byte_3FB0B
    public byte_3FB0C
    public byte_3FB0D
    public byte_3FB0E
    public byte_3FB0F
    public byte_3FB10
    public byte_3FB11
    public word_3FB12
    public word_3FB14
    public byte_3FB16
    public byte_3FB17
    public word_3FB18
    public word_3FB1A
    public word_3FB1C
    public word_3FB1E
    public word_3FB20
    public word_3FB22
    public word_3FB24
    public word_3FB26
    public word_3FB28
    public word_3FB2A
    public word_3FB2C
    public word_3FB2E
    public word_3FB30
    public word_3FB32
    public word_3FB34
    public word_3FB36
    public word_3FB48
    public word_3FB4A
    public word_3FB4C
    public word_3FB4E
    public word_3FBD0
    public word_3FBD2
    public word_3FBD4
    public word_3FBD6
    public word_3FBD8
    public byte_3FBF7
    public byte_3FC04
    public byte_3FC10
    public byte_3FC12
    public byte_3FC14
    public word_3FDFC
    public seg_3FDFE
    public byte_3FE00
    public aSFileError
    public aSNotPackedFile
    public aSInvalidPackTy
    public aLocateshape4_4
    public aReservememoryO
    public aMemoryManagerB
    public aResizememoryCa
    public aResizememoryNo
    public word_3FF82
    public word_3FF84
    public word_3FF86
    public word_3FF88
    public word_3FF8A
    public word_40310
    public word_40312
    public word_40314
    public word_40316
    public word_40318
    public word_4031A
    public word_4031C
    public word_4031E
    public word_40320
    public off_40322
    public off_40326
    public byte_4032A
    public byte_4032B
    public byte_4032C
    public word_4032E
    public word_40330
    public word_40332
    public word_40334
    public word_403AE
    public word_403B0
    public word_403B2
    public word_403B4
    public word_403B6
    public word_403B8
    public word_403BA
    public word_403BC
    public word_403BE
    public word_403C0
    public aWindowReleased
    public aSFileError_0
    public byte_403F2
    public byte_403F3
    public word_405F6
    public word_405F8
    public word_405FA
    public word_405FC
    public off_405FE
    public dword_4060A
    public byte_40630
    public byte_40631
    public byte_40632
    public byte_40633
    public byte_40634
    public byte_40635
    public byte_40639
    public word_4063A
    public word_4063C
    public word_407AA
    public word_407CA
    public word_407CC
    public word_407CE
    public word_407D0
    public word_407D2
    public word_407D4
    public word_407D6
    public word_407D8
    public word_407DA
    public word_407DC
    public word_407DE
    public word_407E0
    public word_407E2
    public word_407E4
    public word_407E6
    public word_407E8
    public word_407EA
    public word_407EC
    public word_407EE
    public word_407F0
    public word_407F2
    public word_407F4
    public word_407F6
    public word_407F8
    public word_407FA
    public word_407FC
    public word_407FE
    public word_40800
    public word_40802
    public word_40B0A
    public word_40B0C
    public word_40B0E
    public word_40B10
    public byte_40B12
    public byte_40B13
    public byte_40B86
    public dword_40C1E
    public dword_40C22
    public dword_40C26
    public aNmsg
    public aR6000StackOver
    public word_40D3A
    public word_40D3C
    public word_40D3E
byte_3B770     db 0
    db    0
word_3B772     dw 0
word_3B774     dw 0
    db    0
    db    0
word_3B778     dw 534Dh
word_3B77A     dw 5220h
byte_3B77C     db 75h
    db  6Eh ; n
word_3B77E     dw 542Dh
word_3B780     dw 6D69h
word_3B782     dw 2065h
byte_3B784     db 4Ch
    db  69h ; i
    db  62h ; b
    db  72h ; r
    db  61h ; a
    db  72h ; r
    db  79h ; y
    db  20h
    db  2Dh ; -
    db  20h
    db  43h ; C
    db  6Fh ; o
    db  70h ; p
    db  79h ; y
    db  72h ; r
    db  69h ; i
    db  67h ; g
    db  68h ; h
    db  74h ; t
    db  20h
    db  28h ; (
    db  63h ; c
    db  29h ; )
    db  20h
word_3B79C     dw 3931h
    db  38h ; 8
    db  38h ; 8
    db  2Ch ; ,
    db  20h
    db  4Dh ; M
    db  69h ; i
    db  63h ; c
    db  72h ; r
    db  6Fh ; o
    db  73h ; s
    db  6Fh ; o
    db  66h ; f
    db  74h ; t
    db  20h
    db  43h ; C
    db  6Fh ; o
    db  72h ; r
    db  70h ; p
    db  11h
    db    0
    db  6Dh ; m
    db  61h ; a
    db  69h ; i
    db  6Eh ; n
    db    0
    db  66h ; f
    db  6Fh ; o
    db  6Eh ; n
    db  74h ; t
    db  64h ; d
    db  65h ; e
    db  66h ; f
    db  2Eh ; .
    db  66h ; f
    db  6Eh ; n
    db  74h ; t
    db    0
    db  66h ; f
    db  6Fh ; o
    db  6Eh ; n
    db  74h ; t
    db  6Eh ; n
    db  2Eh ; .
    db  66h ; f
    db  6Eh ; n
    db  74h ; t
    db    0
    db  74h ; t
    db  72h ; r
    db  61h ; a
    db  6Bh ; k
    db  64h ; d
    db  61h ; a
    db  74h ; t
    db  61h ; a
    db    0
    db  44h ; D
    db  45h ; E
    db  46h ; F
    db  41h ; A
    db  55h ; U
    db  4Ch ; L
    db  54h ; T
    db    0
    db  2Eh ; .
    db  74h ; t
    db  72h ; r
    db  6Bh ; k
    db    0
    db  53h ; S
    db  4Ch ; L
    db  43h ; C
    db  54h ; T
    db    0
    db  73h ; s
    db  6Bh ; k
    db  69h ; i
    db  64h ; d
    db  6Dh ; m
    db  73h ; s
    db    0
    db  73h ; s
    db  6Bh ; k
    db  69h ; i
    db  64h ; d
    db  73h ; s
    db  6Ch ; l
    db  63h ; c
    db  74h ; t
    db    0
    db  74h ; t
    db  65h ; e
    db  64h ; d
    db  69h ; i
    db  74h ; t
    db  2Eh ; .
    db  2Ah ; *
    db    0
    db  63h ; c
    db  76h ; v
    db  78h ; x
    db    0
    db  64h ; d
    db  6Fh ; o
    db  73h ; s
    db    0
word_3B808     dw 0
word_3B80A     dw 0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
word_3B870     dw 0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db  44h ; D
    db  45h ; E
    db  46h ; F
    db  41h ; A
    db  55h ; U
    db  4Ch ; L
    db  54h ; T
    db    0
    db    0
    db    0
    db  64h ; d
    db  65h ; e
    db  73h ; s
    db  65h ; e
    db  72h ; r
    db  74h ; t
    db    0
    db    0
    db    0
    db  74h ; t
    db  72h ; r
    db  6Fh ; o
    db  70h ; p
    db  69h ; i
    db  63h ; c
    db  61h ; a
    db  6Ch ; l
    db    0
    db  61h ; a
    db  6Ch ; l
    db  70h ; p
    db  69h ; i
    db  6Eh ; n
    db  65h ; e
    db    0
    db    0
    db    0
    db  63h ; c
    db  69h ; i
    db  74h ; t
    db  79h ; y
    db    0
    db    0
    db    0
    db    0
    db    0
    db  63h ; c
    db  6Fh ; o
    db  75h ; u
    db  6Eh ; n
    db  74h ; t
    db  72h ; r
    db  79h ; y
    db    0
    db    0
    db    0
    db    0
    db    0
word_3B8EA     dw 1C2h
word_3B8EC     dw 0D2h
word_3B8EE     dw 1D0h
word_3B8F0     dw 50h
byte_3B8F2     db 0
byte_3B8F3     db 0
byte_3B8F4     db 0
byte_3B8F5     db 0
byte_3B8F6     db 0
byte_3B8F7     db 0
byte_3B8F8     db 0
byte_3B8F9     db 0
byte_3B8FA     db 0
byte_3B8FB     db 0
byte_3B8FC     db 0
    db    0
aKevin     db 'kevin',0
    db  6Fh ; o
    db  70h ; p
    db  70h ; p
byte_3B907     db 31h
    db    0
    db    0
    db  63h ; c
    db  61h ; a
    db  72h ; r
byte_3B90D     db 63h
byte_3B90E     db 6Fh
byte_3B90F     db 75h
byte_3B910     db 6Eh
    db    0
aTitl     db 'TITL',0
aSkidms     db 'skidms',0
aSkidtitl     db 'skidtitl',0
aSdtitl     db 'sdtitl',0
aSdcred     db 'sdcred',0
aProd     db 'prod',0
aProd_0     db 'prod',0
aTitl_0     db 'titl',0
aCred     db 'cred',0
aArowarrwarw1ar     db 'arowarrwarw1arw2arw3arw4arw5arw6arw7arw8type',0
aCre     db 'cre',0
aGds0     db 'gds0',0
aGds1     db 'gds1',0
aDes     db 'des',0
aGdon     db 'gdon',0
aGkev     db 'gkev',0
aGbra     db 'gbra',0
aGrob     db 'grob',0
aGsta     db 'gsta',0
aMus     db 'mus',0
aGmsy     db 'gmsy',0
aGkri     db 'gkri',0
aGbri     db 'gbri',0
aPro     db 'pro',0
aGkev_0     db 'gkev',0
aOpr     db 'opr',0
aGbra_0     db 'gbra',0
aGric     db 'gric',0
aArt     db 'art',0
aGmsm     db 'gmsm',0
aGdav     db 'gdav',0
aGnic     db 'gnic',0
aGkev_1     db 'gkev',0
aSdmsel     db 'sdmsel',0
aScrn     db 'scrn',0
    db    0
    db    1
    db    2
    db    4
    db    0
    db    3
    db    0
    db    3
    db    0
    db    1
    db    4
    db    2
    db    0
    db  69h ; i
    db    0
    db  42h ; B
    db    0
    db    5
    db    0
    db 0BEh ; æ
    db    0
    db 0FFh
    db    0
    db 0D0h ; –
    db    0
    db  6Bh ; k
    db    0
    db  43h ; C
    db    0
    db 0FDh ; ˝
    db    0
    db  38h ; 8
    db    1
    db  77h ; w
    db    0
    db  4Dh ; M
    db    0
    db  72h ; r
    db    0
    db  4Ch ; L
    db    0
    db  74h ; t
    db    0
    db 0C5h ; ≈
    db    0
    db  78h ; x
    db    0
    db 0AAh ; ™
    db    0
    db  7Ah ; z
    db    0
    db 0A6h ; ¶
    db    0
    db  27h ; '
    db    0
    db  27h ; '
    db    0
    db  68h ; h
    db  73h ; s
    db  30h ; 0
    db    0
    db  74h ; t
    db  65h ; e
    db  64h ; d
    db  69h ; i
    db  74h ; t
    db    0
    db  62h ; b
    db  6Dh ; m
    db  74h ; t
    db    0
    db  62h ; b
    db  65h ; e
    db  74h ; t
    db    0
    db  62h ; b
    db  6Dh ; m
    db  6Dh ; m
    db    0
    db  74h ; t
    db  72h ; r
    db  6Bh ; k
    db    0
    db  2Eh ; .
    db  74h ; t
    db  72h ; r
    db  6Bh ; k
    db    0
    db  2Eh ; .
    db  74h ; t
    db  72h ; r
    db  6Bh ; k
    db    0
    db  10h
    db    0
    db  70h ; p
    db    0
    db 0D0h ; –
    db    0
    db  70h ; p
    db    0
    db 0D0h ; –
    db    0
    db  30h ; 0
    db    1
    db 0ABh ; ´
    db    0
    db 0ABh ; ´
    db    0
    db 0ABh ; ´
    db    0
    db 0C5h ; ≈
    db    0
    db 0C5h ; ≈
    db    0
    db 0C5h ; ≈
    db    0
    db  2Eh ; .
    db  68h ; h
    db  69h ; i
    db  67h ; g
    db    0
a______________     db '....................',0
    db  2Eh ; .
    db  2Eh ; .
    db  2Eh ; .
    db  2Eh ; .
a_____________0     db '...................',0
a______     db '../....',0
aHs1     db 'hs1',0
    db ' ',27h,0
    db 27h,0
aHs2     db 'hs2',0
aHs3     db 'hs3',0
aHs5     db 'hs5',0
aHs4     db 'hs4',0
    db '(',0
    db ')',0
    db ' ',0
a_hig     db '.hig',0
a_res_0     db '.res',0
aCar     db 'car*',0
    db  73h ; s
    db  64h ; d
aCsel     db 'csel',0
aMisc_0     db 'misc',0
aGrap     db 'grap',0
a150     db '150',0
a100     db '100',0
a50     db ' 50',0
a0     db '  0',0
a02040     db '0  20  40',0
aBdo_0     db 'bdo',0
aBnx_0     db 'bnx',0
aBla_0     db 'bla',0
aBau     db 'bau',0
aBma     db 'bma',0
aBco     db 'bco',0
aDes_1     db 'des',0
aStop_1     db 'stop',0
aBau_0     db 'bau',0
aBma_0     db 'bma',0
    db    0
    db    0
    db  40h ; @
    db    1
    db    0
    db    0
    db  5Fh ; _
    db    0
word_3BB28     dw 0E5h
    db 0E5h ; Â
    db    0
    db 0E5h ; Â
    db    0
    db 0E5h ; Â
    db    0
    db 0E5h ; Â
    db    0
word_3BB32     dw 13Ch
    db  3Ch ; <
    db    1
    db  3Ch ; <
    db    1
    db  3Ch ; <
    db    1
    db  3Ch ; <
    db    1
word_3BB3C     dw 6Bh
word_3BB3E     dw 7Dh
word_3BB40     dw 8Fh
word_3BB42     dw 0A1h
word_3BB44     dw 0B3h
    db  7Ch ; |
    db    0
    db  8Eh ; é
    db    0
    db 0A0h ; †
    db    0
    db 0B2h ; ≤
    db    0
word_3BB4E     dw 0C4h
aLnam     db 'lnam',0
    db    0
    db    0
    db    0
word_3BB58     dw 140h
    db    0
    db    0
word_3BB5C     dw 0
    db    0
    db    0
word_3BB60     dw 0FCB8h
word_3BB62     dw 0B40h
aMisc     db 'misc',0
aSdosel     db 'sdosel',0
aOpp0opp1opp2op     db 'opp0opp1opp2opp3opp4opp5opp6',0
aScrn_0     db 'scrn',0
aBla     db 'bla',0
aBnx     db 'bnx',0
aBcl     db 'bcl',0
aBca     db 'bca',0
aBdo     db 'bdo',0
aClip     db 'clip',0
aDes_0     db 'des',0
aRac     db 'rac',0
    db    0
    db  14h
    db    0
    db  4Ch ; L
    db    0
    db  84h ; Ñ
    db    0
    db 0BCh ; º
    db    0
    db 0F4h ; Ù
    db    0
    db  4Ch ; L
    db    0
    db  84h ; Ñ
    db    0
    db 0BCh ; º
    db    0
    db 0F4h ; Ù
    db    0
    db  2Ch ; ,
    db    1
word_3BBC8     dw 0B1h
    db 0B1h ; ±
    db    0
    db 0B1h ; ±
    db    0
    db 0B1h ; ±
    db    0
    db 0B1h ; ±
    db    0
    db 0C5h ; ≈
    db    0
    db 0C5h ; ≈
    db    0
    db 0C5h ; ≈
    db    0
    db 0C5h ; ≈
    db    0
    db 0C5h ; ≈
    db    0
    db  6Dh ; m
    db  69h ; i
    db  73h ; s
    db  63h ; c
    db    0
    db  67h ; g
    db  73h ; s
    db  74h ; t
    db  75h ; u
    db    0
    db  67h ; g
    db  76h ; v
    db  65h ; e
    db  72h ; r
    db    0
    db  6Dh ; m
    db  6Fh ; o
    db  70h ; p
    db    0
    db  6Dh ; m
    db  69h ; i
    db  64h ; d
    db    0
    db  72h ; r
    db  65h ; e
    db  70h ; p
    db    0
    db  2Eh ; .
    db  72h ; r
    db  70h ; p
    db  6Ch ; l
    db    0
    db  6Dh ; m
    db  69h ; i
    db  73h ; s
    db  63h ; c
    db    0
    db  65h ; e
    db  6Ch ; l
    db  74h ; t
    db    0
    db  63h ; c
    db  6Fh ; o
    db  6Eh ; n
    db    0
    db  70h ; p
    db  70h ; p
    db  74h ; t
    db    0
    db  64h ; d
    db  6Eh ; n
    db  66h ; f
    db    0
    db  6Fh ; o
    db  6Ch ; l
    db  74h ; t
    db    0
    db  64h ; d
    db  6Eh ; n
    db  66h ; f
    db    0
    db  6Fh ; o
    db  77h ; w
    db  74h ; t
    db    0
    db  6Fh ; o
    db  6Ch ; l
    db  74h ; t
    db    0
    db  56h ; V
    db  49h ; I
    db  43h ; C
    db  54h ; T
    db    0
    db  73h ; s
    db  6Bh ; k
    db  69h ; i
    db  64h ; d
    db  6Dh ; m
    db  73h ; s
    db    0
    db  73h ; s
    db  6Bh ; k
    db  69h ; i
    db  64h ; d
    db  76h ; v
    db  69h ; i
    db  63h ; c
    db  74h ; t
    db    0
    db  4Fh ; O
    db  56h ; V
    db  45h ; E
    db  52h ; R
    db    0
    db  73h ; s
    db  6Bh ; k
    db  69h ; i
    db  64h ; d
    db  6Dh ; m
    db  73h ; s
    db    0
    db  73h ; s
    db  6Bh ; k
    db  69h ; i
    db  64h ; d
    db  6Fh ; o
    db  76h ; v
    db  65h ; e
    db  72h ; r
    db    0
    db  61h ; a
    db  76h ; v
    db  73h ; s
    db    0
    db  6Dh ; m
    db  70h ; p
    db  68h ; h
    db    0
    db  69h ; i
    db  6Dh ; m
    db  70h ; p
    db    0
    db  6Dh ; m
    db  70h ; p
    db  68h ; h
    db    0
    db  74h ; t
    db  6Fh ; o
    db  70h ; p
    db    0
    db  6Dh ; m
    db  70h ; p
    db  68h ; h
    db    0
    db  6Ah ; j
    db  75h ; u
    db  6Dh ; m
    db    0
    db  77h ; w
    db  69h ; i
    db  6Eh ; n
    db  6Eh ; n
    db    0
    db  6Ch ; l
    db  6Fh ; o
    db  73h ; s
    db  65h ; e
    db    0
    db  2Eh ; .
    db  74h ; t
    db  72h ; r
    db  6Bh ; k
    db    0
    db  69h ; i
    db  68h ; h
    db  64h ; d
    db    0
    db  64h ; d
    db  34h ; 4
    db  61h ; a
    db    0
    db  62h ; b
    db  63h ; c
    db  74h ; t
    db    0
    db  69h ; i
    db  6Eh ; n
    db  68h ; h
    db    0
    db  69h ; i
    db  6Eh ; n
    db  68h ; h
    db    0
    db  68h ; h
    db  6Eh ; n
    db  61h ; a
    db    0
    db  62h ; b
    db  65h ; e
    db  76h ; v
    db    0
    db  62h ; b
    db  68h ; h
    db  69h ; i
    db    0
    db  62h ; b
    db  72h ; r
    db  70h ; p
    db    0
    db  62h ; b
    db  72h ; r
    db  61h ; a
    db    0
    db  62h ; b
    db  64h ; d
    db  72h ; r
    db    0
    db  62h ; b
    db  6Dh ; m
    db  6Dh ; m
    db    0
    db    2
    db    0
    db    1
    db    0
    db    2
    db    0
    db    3
    db    0
    db    4
    db    0
    db    1
    db    0
    db    4
    db    0
    db    0
    db    0
    db    5
    db    0
    db    0
    db    0
    db    0
    db    0
    db    6
    db    0
    db    5
    db    0
    db    6
    db    0
    db    5
    db    0
    db    1
    db    0
    db    1
    db    0
    db    2
    db    0
    db    3
    db    0
    db    5
    db    0
    db    0
    db    0
    db    6
    db    0
    db    2
    db    0
    db    3
    db    0
    db    4
    db    0
    db    4
    db    0
    db    0
    db    0
    db    6
    db    0
    db    2
    db    0
    db    0
    db    0
    db    1
    db    0
    db    1
    db    0
    db    0
    db    0
    db    3
    db    0
    db    2
    db    0
word_3BCEC     dw 4
word_3BCEE     dw 54h
word_3BCF0     dw 0A4h
word_3BCF2     dw 0F4h
    db  80h ; Ä
    db    0
    db  4Bh ; K
    db    0
    db  9Bh ; õ
    db    0
    db 0EBh ; Î
    db    0
    db  3Bh ; ;
    db    1
    db 0C7h ; «
    db    0
word_3BD00     dw 0AEh
    db 0AEh ; Æ
    db    0
    db 0AEh ; Æ
    db    0
    db 0AEh ; Æ
    db    0
    db 0AEh ; Æ
    db    0
word_3BD0A     dw 0C5h
    db 0C5h ; ≈
    db    0
    db 0C5h ; ≈
    db    0
    db 0C5h ; ≈
    db    0
    db 0C5h ; ≈
    db    0
    db  6Fh ; o
    db  70h ; p
    db  70h ; p
byte_3BD17     db 32h
    db  77h ; w
    db  69h ; i
    db  6Eh ; n
    db    0
    db  6Fh ; o
    db  70h ; p
    db  70h ; p
byte_3BD1F     db 32h
aLose     db 'lose',0
    db    0
    db  6Fh ; o
    db  70h ; p
    db  30h ; 0
byte_3BD29     db 31h
    db    0
    db  6Dh ; m
    db  69h ; i
    db  73h ; s
    db  63h ; c
    db    0
    db  63h ; c
    db  6Fh ; o
    db  70h ; p
    db    0
    db  30h ; 0
    db  31h ; 1
    db  32h ; 2
    db  33h ; 3
    db  34h ; 4
    db  35h ; 5
    db  36h ; 6
    db  37h ; 7
    db  38h ; 8
    db  39h ; 9
    db  61h ; a
    db  62h ; b
    db  63h ; c
    db  64h ; d
    db  65h ; e
    db  66h ; f
    db  67h ; g
    db  68h ; h
    db  69h ; i
    db  6Ah ; j
    db    0
    db    0
    db  71h ; q
    db  30h ; 0
byte_3BD4C     db 30h
    db    0
    db  61h ; a
    db  30h ; 0
byte_3BD50     db 30h
    db    0
    db  0Ah
    db    0
    db  32h ; 2
    db    0
    db  0Ah
    db    0
    db  14h
    db    0
    db    5
    db    0
    db  28h ; (
    db    0
    db    5
    db    0
    db  0Ah
    db    0
    db    6
    db    0
    db  79h ; y
    db    0
    db    6
    db    0
    db    9
    db    0
    db    1
    db    0
    db  0Ah
    db    0
    db    1
    db    0
    db  0Ah
    db    0
    db  15h
    db    0
    db  15h
    db    0
    db  0Fh
    db    0
    db  0Fh
    db    0
    db    0
    db    8
    db 0F8h ; ¯
    db    0
    db    0
    db    7
    db 0F9h ; ˘
    db    0
    db    0
    db    6
    db 0FAh ; ˙
    db    0
    db    0
    db    5
    db 0FBh ; ˚
    db    0
    db    0
    db    4
    db 0FCh ; ¸
    db    0
    db    0
    db    4
    db 0FCh ; ¸
    db    0
    db    0
    db    3
    db 0FDh ; ˝
    db    0
    db    0
    db    3
    db 0FDh ; ˝
    db    0
    db    0
    db    2
    db 0FEh ; ˛
    db    0
    db    0
    db    2
    db 0FEh ; ˛
    db    0
    db    0
    db    2
    db 0FEh ; ˛
    db    0
    db    0
    db    1
    db 0FFh
    db    0
    db    0
    db    1
    db 0FFh
    db    0
    db    0
    db    1
    db 0FFh
    db    0
    db    0
    db    1
    db 0FFh
    db    0
    db    0
    db    1
    db 0FFh
    db    0
    db    0
    db  10h
    db 0F0h ; 
    db    0
    db    0
    db  0Eh
    db 0F2h ; Ú
    db    0
    db    0
    db  0Ch
    db 0F4h ; Ù
    db    0
    db    0
    db  0Ah
    db 0F6h ; ˆ
    db    0
    db    0
    db    8
    db 0F8h ; ¯
    db    0
    db    0
    db    8
    db 0F8h ; ¯
    db    0
    db    0
    db    6
    db 0FAh ; ˙
    db    0
    db    0
    db    6
    db 0FAh ; ˙
    db    0
    db    0
    db    4
    db 0FCh ; ¸
    db    0
    db    0
    db    4
    db 0FCh ; ¸
    db    0
    db    0
    db    4
    db 0FCh ; ¸
    db    0
    db    0
    db    2
    db 0FEh ; ˛
    db    0
    db    0
    db    2
    db 0FEh ; ˛
    db    0
    db    0
    db    1
    db 0FFh
    db    0
    db    0
    db    1
    db 0FFh
    db    0
    db    0
    db    1
    db 0FFh
    db    0
    db    0
    db    1
    db 0C0h ; ¿
    db    0
    db  80h ; Ä
    db    0
    db  40h ; @
    db    0
byte_3BE02     db 0FFh
    db    0
    db    1
    db    0
    db    0
    db    0
    db    0
    db    0
    db    1
    db    0
    db    0
    db    0
    db    0
    db    0
    db    1
    db    0
    db    1
    db    0
word_3BE14     dw 270Fh
word_3BE16     dw 270Fh
aSimd     db 'simd',0
aGnam     db 'gnam',0
aSimd_0     db 'simd',0
aGsna     db 'gsna',0
dword_3BE2C     dd 0
word_3BE30     dw 0
word_3BE32     dw 0
    db  1Eh
    db    0
    db 0C8h ; »
    db    0
    db  40h ; @
    db    1
    db  90h ; ê
    db    1
    db  12h
    db    2
    db 0BCh ; º
    db    2
    db  70h ; p
    db    3
    db 0C0h ; ¿
    db    3
    db  6Ch ; l
    db  7Ah ; z
    db  56h ; V
    db  7Ah ; z
    db  40h ; @
    db  7Ah ; z
    db  6Ch ; l
    db  7Ah ; z
    db  56h ; V
    db  7Ah ; z
    db  40h ; @
    db  7Ah ; z
    db  6Ch ; l
    db  7Ah ; z
    db  56h ; V
    db  7Ah ; z
    db 0FEh ; ˛
    db 0FCh ; ¸
    db    2
    db 0FFh
    db 0FCh ; ¸
    db    2
    db    0
    db 0FCh ; ¸
    db    2
    db    1
    db 0FCh ; ¸
    db    2
    db    2
    db 0FCh ; ¸
    db    2
    db 0FEh ; ˛
    db 0FDh ; ˝
    db    1
    db 0FFh
    db 0FDh ; ˝
    db    1
    db    0
    db 0FDh ; ˝
    db    1
    db    1
    db 0FDh ; ˝
    db    1
    db    2
    db 0FDh ; ˝
    db    1
    db 0FEh ; ˛
    db 0FEh ; ˛
    db    1
    db 0FFh
    db 0FEh ; ˛
    db    0
    db    0
    db 0FEh ; ˛
    db    0
    db    1
    db 0FEh ; ˛
    db    0
    db    2
    db 0FEh ; ˛
    db    1
    db 0FEh ; ˛
    db 0FFh
    db    0
    db 0FFh
    db 0FFh
    db    0
    db    0
    db 0FFh
    db    0
    db    1
    db 0FFh
    db    0
    db    2
    db 0FFh
    db    0
    db 0FFh
    db    0
    db    0
    db    1
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    2
    db 0FCh ; ¸
    db    2
    db    1
    db 0FCh ; ¸
    db    2
    db    0
    db 0FCh ; ¸
    db    2
    db 0FFh
    db 0FCh ; ¸
    db    2
    db 0FEh ; ˛
    db 0FCh ; ¸
    db    2
    db    2
    db 0FDh ; ˝
    db    1
    db    1
    db 0FDh ; ˝
    db    1
    db    0
    db 0FDh ; ˝
    db    1
    db 0FFh
    db 0FDh ; ˝
    db    1
    db 0FEh ; ˛
    db 0FDh ; ˝
    db    1
    db    2
    db 0FEh ; ˛
    db    1
    db    1
    db 0FEh ; ˛
    db    0
    db    0
    db 0FEh ; ˛
    db    0
    db 0FFh
    db 0FEh ; ˛
    db    0
    db 0FEh ; ˛
    db 0FEh ; ˛
    db    1
    db    2
    db 0FFh
    db    0
    db    1
    db 0FFh
    db    0
    db    0
    db 0FFh
    db    0
    db 0FFh
    db 0FFh
    db    0
    db 0FEh ; ˛
    db 0FFh
    db    0
    db    1
    db    0
    db    0
    db 0FFh
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    4
    db 0FEh ; ˛
    db    2
    db    4
    db 0FFh
    db    2
    db    4
    db    0
    db    2
    db    4
    db    1
    db    2
    db    4
    db    2
    db    2
    db    3
    db 0FEh ; ˛
    db    1
    db    3
    db 0FFh
    db    1
    db    3
    db    0
    db    1
    db    3
    db    1
    db    1
    db    3
    db    2
    db    1
    db    2
    db 0FEh ; ˛
    db    1
    db    2
    db 0FFh
    db    0
    db    2
    db    0
    db    0
    db    2
    db    1
    db    0
    db    2
    db    2
    db    1
    db    1
    db 0FEh ; ˛
    db    0
    db    1
    db 0FFh
    db    0
    db    1
    db    0
    db    0
    db    1
    db    1
    db    0
    db    2
    db    2
    db    0
    db    0
    db 0FFh
    db    0
    db    0
    db    1
    db    0
    db    0
    db    0
    db    0
    db    0
    db    4
    db    2
    db    2
    db    4
    db    1
    db    2
    db    4
    db    0
    db    2
    db    4
    db 0FFh
    db    2
    db    4
    db 0FEh ; ˛
    db    2
    db    3
    db    2
    db    1
    db    3
    db    1
    db    1
    db    3
    db    0
    db    1
    db    3
    db 0FFh
    db    1
    db    3
    db 0FEh ; ˛
    db    1
    db    2
    db    2
    db    1
    db    2
    db    1
    db    0
    db    2
    db    0
    db    0
    db    2
    db 0FFh
    db    0
    db    2
    db 0FEh ; ˛
    db    1
    db    1
    db    2
    db    0
    db    1
    db    1
    db    0
    db    1
    db    0
    db    0
    db    1
    db 0FFh
    db    0
    db    1
    db 0FEh ; ˛
    db    0
    db    0
    db    1
    db    0
    db    0
    db 0FFh
    db    0
    db    0
    db    0
    db    0
    db    0
    db    2
    db    4
    db    2
    db    1
    db    4
    db    2
    db    0
    db    4
    db    2
    db 0FFh
    db    4
    db    2
    db 0FEh ; ˛
    db    4
    db    2
    db    2
    db    3
    db    1
    db    1
    db    3
    db    1
    db    0
    db    3
    db    1
    db 0FFh
    db    3
    db    1
    db 0FEh ; ˛
    db    3
    db    1
    db    2
    db    2
    db    0
    db    1
    db    2
    db    0
    db    0
    db    2
    db    0
    db 0FFh
    db    2
    db    0
    db 0FEh ; ˛
    db    2
    db    1
    db    2
    db    1
    db    0
    db    1
    db    1
    db    0
    db    0
    db    1
    db    0
    db 0FFh
    db    1
    db    0
    db 0FEh ; ˛
    db    1
    db    0
    db    1
    db    0
    db    0
    db 0FFh
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db 0FEh ; ˛
    db    4
    db    2
    db 0FFh
    db    4
    db    2
    db    0
    db    4
    db    2
    db    1
    db    4
    db    2
    db    2
    db    4
    db    2
    db 0FEh ; ˛
    db    3
    db    1
    db 0FFh
    db    3
    db    1
    db    0
    db    3
    db    1
    db    1
    db    3
    db    1
    db    2
    db    3
    db    1
    db 0FEh ; ˛
    db    2
    db    1
    db 0FFh
    db    2
    db    0
    db    0
    db    2
    db    0
    db    1
    db    2
    db    0
    db    2
    db    2
    db    1
    db 0FEh ; ˛
    db    1
    db    0
    db 0FFh
    db    1
    db    0
    db    0
    db    1
    db    0
    db    1
    db    1
    db    0
    db    2
    db    1
    db    0
    db 0FFh
    db    0
    db    0
    db    1
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db 0FCh ; ¸
    db    2
    db    2
    db 0FCh ; ¸
    db    1
    db    2
    db 0FCh ; ¸
    db    0
    db    2
    db 0FCh ; ¸
    db 0FFh
    db    2
    db 0FCh ; ¸
    db 0FEh ; ˛
    db    2
    db 0FDh ; ˝
    db    2
    db    1
    db 0FDh ; ˝
    db    1
    db    1
    db 0FDh ; ˝
    db    0
    db    1
    db 0FDh ; ˝
    db 0FFh
    db    1
    db 0FDh ; ˝
    db 0FEh ; ˛
    db    1
    db 0FEh ; ˛
    db    2
    db    1
    db 0FEh ; ˛
    db    1
    db    0
    db 0FEh ; ˛
    db    0
    db    0
    db 0FEh ; ˛
    db 0FFh
    db    0
    db 0FEh ; ˛
    db 0FEh ; ˛
    db    1
    db 0FFh
    db    2
    db    0
    db 0FFh
    db    1
    db    0
    db 0FFh
    db    0
    db    0
    db 0FFh
    db 0FFh
    db    0
    db 0FFh
    db 0FEh ; ˛
    db    0
    db    0
    db    1
    db    0
    db    0
    db 0FFh
    db    0
    db    0
    db    0
    db    0
    db    0
    db 0FCh ; ¸
    db 0FEh ; ˛
    db    2
    db 0FCh ; ¸
    db 0FFh
    db    2
    db 0FCh ; ¸
    db    0
    db    2
    db 0FCh ; ¸
    db    1
    db    2
    db 0FCh ; ¸
    db    2
    db    2
    db 0FDh ; ˝
    db 0FEh ; ˛
    db    1
    db 0FDh ; ˝
    db 0FFh
    db    1
    db 0FDh ; ˝
    db    0
    db    1
    db 0FDh ; ˝
    db    1
    db    1
    db 0FDh ; ˝
    db    2
    db    1
    db 0FEh ; ˛
    db 0FEh ; ˛
    db    1
    db 0FEh ; ˛
    db 0FFh
    db    0
    db 0FEh ; ˛
    db    0
    db    0
    db 0FEh ; ˛
    db    1
    db    0
    db 0FEh ; ˛
    db    2
    db    1
    db 0FFh
    db 0FEh ; ˛
    db    0
    db 0FFh
    db 0FFh
    db    0
    db 0FFh
    db    0
    db    0
    db 0FFh
    db    1
    db    0
    db 0FFh
    db    2
    db    0
    db    0
    db 0FFh
    db    0
    db    0
    db    1
    db    0
    db    0
    db    0
    db    0
    db    0
    db  2Ah ; *
    db    7
    db  70h ; p
    db    7
    db 0B6h ; ∂
    db    7
    db 0FCh ; ¸
    db    7
    db  42h ; B
    db    8
    db  88h ; à
    db    8
    db 0CEh ; Œ
    db    8
    db 0E4h ; ‰
    db    6
    db    0
    db    0
    db  40h ; @
    db    1
    db    0
    db    0
    db 0C8h ; »
    db    0
    db    2
    db    2
    db    1
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    2
    db    0
    db    0
    db    0
    db 0FEh ; ˛
    db    0
    db    2
    db    0
    db    0
    db    0
    db 0FEh ; ˛
    db    0
    db    0
    db    0
    db 0FEh ; ˛
    db    0
    db    2
    db    0
    db 0FEh ; ˛
    db    0
    db 0FEh ; ˛
    db    0
    db    2
    db    0
    db    2
    db    0
    db    2
    db    0
    db 0FEh ; ˛
    db    0
    db    0
    db    0
    db    0
    db    1
    db    1
    db    1
    db    1
    db    2
    db    2
    db    2
    db    2
    db    3
    db    3
    db    3
    db    3
    db    0
    db    0
    db    0
    db    0
    db    0
    db    1
    db    0
    db    1
    db    0
    db    2
    db    0
    db    2
    db    0
    db    3
    db    0
    db    3
    db 0D6h ; ÷
    db 0D7h ; ◊
    db 0D6h ; ÷
    db 0D7h ; ◊
    db 0D6h ; ÷
    db 0D7h ; ◊
    db 0D6h ; ÷
    db 0D7h ; ◊
    db    0
    db    0
    db    0
    db    0
    db    0
    db    1
    db    0
    db    0
    db    1
    db    0
    db    0
    db    0
    db    1
    db    0
    db    0
    db    1
    db    1
    db    1
    db  80h ; Ä
    db    0
    db  80h ; Ä
    db    1
    db  80h ; Ä
    db    2
    db  80h ; Ä
    db    3
word_3C108     dw 3C00h
word_3C10A     dw 4EE8h
word_3C10C     dw 0F510h
word_3C10E     dw 3C00h
word_3C110     dw 0AF0h
word_3C112     dw 2AD0h
    db    0
    db    0
    db  8Ch ; å
    db 0D8h ; ÿ
    db  78h ; x
    db  41h ; A
    db    0
    db    0
    db  40h ; @
    db    1
    db    0
    db    0
    db 0C8h ; »
    db    0
    db  64h ; d
    db  6Dh ; m
    db  31h ; 1
    db    0
    db  64h ; d
    db  6Dh ; m
    db  32h ; 2
    db    0
    db  70h ; p
    db  72h ; r
    db  65h ; e
    db    0
    db  73h ; s
    db  65h ; e
    db  31h ; 1
    db    0
    db  73h ; s
    db  65h ; e
    db  32h ; 2
    db    0
    db  77h ; w
    db  77h ; w
    db  77h ; w
    db    0
    db  6Fh ; o
    db  70h ; p
    db  70h ; p
    db    0
    db  6Fh ; o
    db  70h ; p
    db  70h ; p
    db    0
    db  70h ; p
    db  65h ; e
    db  6Eh ; n
    db    0
    db  72h ; r
    db  70h ; p
    db  6Ch ; l
    db    0
    db  94h ; î
    db    0
    db 0ACh ; ¨
    db    0
    db  5Dh ; ]
    db    0
    db  6Ch ; l
    db    0
    db  44h ; D
    db    0
    db  5Ch ; \
    db    0
    db  71h ; q
    db    0
    db  80h ; Ä
    db    0
    db 0E4h ; ‰
    db    0
    db 0FCh ; ¸
    db    0
    db  71h ; q
    db    0
    db  80h ; Ä
    db    0
    db  63h ; c
    db  72h ; r
    db  61h ; a
    db  6Bh ; k
    db    0
    db  63h ; c
    db  69h ; i
    db  6Eh ; n
    db  66h ; f
    db    0
    db  73h ; s
    db  63h ; c
    db  65h ; e
    db  6Eh ; n
    db  73h ; s
    db  63h ; c
    db  65h ; e
    db  32h ; 2
    db  73h ; s
    db  63h ; c
    db  65h ; e
    db  33h ; 3
    db  73h ; s
    db  63h ; c
    db  65h ; e
    db  34h ; 4
    db    0
    db  73h ; s
    db  64h ; d
    db  67h ; g
    db  61h ; a
    db  6Dh ; m
    db  65h ; e
    db  32h ; 2
    db    0
    db  65h ; e
    db  78h ; x
    db  30h ; 0
    db  31h ; 1
    db  65h ; e
    db  78h ; x
    db  30h ; 0
    db  32h ; 2
    db  65h ; e
    db  78h ; x
    db  30h ; 0
    db  33h ; 3
    db  6Ch ; l
    db  65h ; e
    db  66h ; f
    db  74h ; t
    db  72h ; r
    db  69h ; i
    db  67h ; g
    db  68h ; h
    db    0
    db  74h ; t
    db  69h ; i
    db  74h ; t
    db  6Ch ; l
    db  65h ; e
    db    0
    db  6Ch ; l
    db  6Fh ; o
    db  67h ; g
    db  6Fh ; o
    db  6Ch ; l
    db  6Fh ; o
    db  67h ; g
    db  32h ; 2
    db  62h ; b
    db  72h ; r
    db  61h ; a
    db  76h ; v
    db    0
    db    0
    db  63h ; c
    db  61h ; a
    db  72h ; r
    db  63h ; c
    db  6Fh ; o
    db  75h ; u
    db  6Eh ; n
    db    0
word_3C1B6     dw 0
word_3C1B8     dw 140h
word_3C1BA     dw 0
word_3C1BC     dw 0C8h
word_3C1BE     dw 1
aBarn     db 'barn',0
aZbrn     db 'zbrn',0
aBrid     db 'brid',0
aZbri     db 'zbri',0
aBtur     db 'btur',0
aZbtu     db 'zbtu',0
aChi1     db 'chi1',0
aZch1     db 'zch1',0
aChi2     db 'chi2',0
aZch2     db 'zch2',0
aElrd     db 'elrd',0
aZelr     db 'zelr',0
aFini     db 'fini',0
aZfin     db 'zfin',0
aGass     db 'gass',0
aZgas     db 'zgas',0
aLban     db 'lban',0
aZlba     db 'zlba',0
aLoop     db 'loop',0
aZloo     db 'zloo',0
aOffi     db 'offi',0
aZoff     db 'zoff',0
aPipe     db 'pipe',0
aZpip     db 'zpip',0
aRamp     db 'ramp',0
aZram     db 'zram',0
aRban     db 'rban',0
aZrba     db 'zrba',0
aRdup     db 'rdup',0
aZrdu     db 'zrdu',0
aRoad     db 'road',0
aZroa     db 'zroa',0
aStur     db 'stur',0
aZstu     db 'zstu',0
aTenn     db 'tenn',0
aZten     db 'zten',0
aTunn     db 'tunn',0
aZtun     db 'ztun',0
aTurn     db 'turn',0
aZtur     db 'ztur',0
aGoui     db 'goui',0
aGouo     db 'gouo',0
aGoup     db 'goup',0
aHigh     db 'high',0
aLakc     db 'lakc',0
aLake     db 'lake',0
aCld1     db 'cld1',0
aCld2     db 'cld2',0
aCld3     db 'cld3',0
aSigl     db 'sigl',0
aSigr     db 'sigr',0
aTree     db 'tree',0
aInte     db 'inte',0
aZint     db 'zint',0
aOffl     db 'offl',0
aZofl     db 'zofl',0
aOffr     db 'offr',0
aZofr     db 'zofr',0
aPalm     db 'palm',0
aZpal     db 'zpal',0
aBank     db 'bank',0
aZban     db 'zban',0
aSofl     db 'sofl',0
aZsol     db 'zsol',0
aSofr     db 'sofr',0
aZsor     db 'zsor',0
aSram     db 'sram',0
aZsra     db 'zsra',0
aSelr     db 'selr',0
aZser     db 'zser',0
aElsp     db 'elsp',0
aZesp     db 'zesp',0
aCact     db 'cact',0
aCact_0     db 'cact',0
aSpip     db 'spip',0
aZspi     db 'zspi',0
aSest     db 'sest',0
aZses     db 'zses',0
aWroa     db 'wroa',0
aZwro     db 'zwro',0
aBarr     db 'barr',0
aZbar     db 'zbar',0
aLco0     db 'lco0',0
aZlco     db 'zlco',0
aRco0     db 'rco0',0
aZrco     db 'zrco',0
aGwro     db 'gwro',0
aZgwr     db 'zgwr',0
aLco1     db 'lco1',0
aRco1     db 'rco1',0
aLoo1     db 'loo1',0
aHig1     db 'hig1',0
aHig2     db 'hig2',0
aHig3     db 'hig3',0
aWind     db 'wind',0
aZwin     db 'zwin',0
aBoat     db 'boat',0
aZboa     db 'zboa',0
aRest     db 'rest',0
aZres     db 'zres',0
aHpip     db 'hpip',0
aZhpi     db 'zhpi',0
aVcor     db 'vcor',0
aZvco     db 'zvco',0
aTun2     db 'tun2',0
aPip2     db 'pip2',0
aFenc     db 'fenc',0
aZfen     db 'zfen',0
aCfen     db 'cfen',0
aZcfe     db 'zcfe',0
aFlag     db 'flag',0
aTruk     db 'truk',0
aExp0     db 'exp0',0
aExp1     db 'exp1',0
aExp2     db 'exp2',0
aExp3     db 'exp3',0
    db  88h ; à
    db 0FFh
    db 0FFh
    db 0FFh
    db 0B2h ; ≤
    db 0FEh ; ˛
    db  78h ; x
    db    0
    db 0FFh
    db 0FFh
    db 0B2h ; ≤
    db 0FEh ; ˛
    db  88h ; à
    db 0FFh
    db 0FFh
    db 0FFh
    db  4Eh ; N
    db    1
    db  78h ; x
    db    0
    db 0FFh
    db 0FFh
    db  4Eh ; N
    db    1
    db 0C8h ; »
    db    0
    db    0
    db    0
    db    0
    db    0
    db 0C8h ; »
    db    0
    db    0
    db    0
    db 0B2h ; ≤
    db 0FEh ; ˛
    db  38h ; 8
    db 0FFh
    db    0
    db    0
    db  4Eh ; N
    db    1
    db  88h ; à
    db 0FFh
    db 0FFh
    db 0FFh
    db  80h ; Ä
    db    0
    db  78h ; x
    db    0
    db 0FFh
    db 0FFh
    db  80h ; Ä
    db    0
    db  88h ; à
    db 0FFh
    db 0FFh
    db 0FFh
    db  4Eh ; N
    db    1
    db  78h ; x
    db    0
    db 0FFh
    db 0FFh
    db  4Eh ; N
    db    1
    db 0C8h ; »
    db    0
    db    0
    db    0
    db    0
    db    0
    db 0C8h ; »
    db    0
    db    0
    db    0
    db 0B2h ; ≤
    db 0FEh ; ˛
    db  38h ; 8
    db 0FFh
    db    0
    db    0
    db  4Eh ; N
    db    1
    db  88h ; à
    db 0FFh
    db 0FFh
    db 0FFh
    db 0B2h ; ≤
    db 0FEh ; ˛
    db  78h ; x
    db    0
    db 0FFh
    db 0FFh
    db 0B2h ; ≤
    db 0FEh ; ˛
    db  88h ; à
    db 0FFh
    db 0FFh
    db 0FFh
    db  4Eh ; N
    db    1
    db  78h ; x
    db    0
    db 0FFh
    db 0FFh
    db  4Eh ; N
    db    1
    db  78h ; x
    db    0
    db 0A5h ; •
    db    0
    db    0
    db    0
    db 0ACh ; ¨
    db 0FFh
    db 0FFh
    db 0FFh
    db 0B2h ; ≤
    db 0FEh ; ˛
    db  54h ; T
    db    0
    db 0FFh
    db 0FFh
    db 0B2h ; ≤
    db 0FEh ; ˛
    db 0ACh ; ¨
    db 0FFh
    db 0FFh
    db 0FFh
    db  4Eh ; N
    db    1
    db  54h ; T
    db    0
    db 0FFh
    db 0FFh
    db  4Eh ; N
    db    1
    db  73h ; s
    db    0
    db 0B4h ; ¥
    db    0
    db    0
    db    0
    db 0C8h ; »
    db    0
    db    0
    db    0
    db 0B2h ; ≤
    db 0FEh ; ˛
    db  38h ; 8
    db 0FFh
    db    0
    db    0
    db  4Eh ; N
    db    1
    db 0ACh ; ¨
    db 0FFh
    db  23h ; #
    db    0
    db 0B2h ; ≤
    db 0FEh ; ˛
    db  8Dh ; ç
    db 0FFh
    db  58h ; X
    db    0
    db 0B2h ; ≤
    db 0FEh ; ˛
    db 0ACh ; ¨
    db 0FFh
    db  23h ; #
    db    0
    db  4Eh ; N
    db    1
    db  8Dh ; ç
    db 0FFh
    db  58h ; X
    db    0
    db  4Eh ; N
    db    1
    db  73h ; s
    db    0
    db 0B4h ; ¥
    db    0
    db    0
    db    0
    db 0C8h ; »
    db    0
    db    0
    db    0
    db 0B2h ; ≤
    db 0FEh ; ˛
    db  38h ; 8
    db 0FFh
    db    0
    db    0
    db  4Eh ; N
    db    1
    db  88h ; à
    db 0FFh
    db 0FFh
    db 0FFh
    db 0B2h ; ≤
    db 0FEh ; ˛
    db  78h ; x
    db    0
    db 0FFh
    db 0FFh
    db 0B2h ; ≤
    db 0FEh ; ˛
    db  88h ; à
    db 0FFh
    db 0FFh
    db 0FFh
    db  4Eh ; N
    db    1
    db  78h ; x
    db    0
    db 0FFh
    db 0FFh
    db  4Eh ; N
    db    1
    db 0C8h ; »
    db    0
    db    0
    db    0
    db 0C8h ; »
    db    0
    db 0C8h ; »
    db    0
    db    0
    db    0
    db 0B2h ; ≤
    db 0FEh ; ˛
    db  38h ; 8
    db 0FFh
    db    0
    db    0
    db  4Eh ; N
    db    1
    db  88h ; à
    db    1
    db 0FFh
    db 0FFh
    db  64h ; d
    db 0FDh ; ˝
    db  78h ; x
    db    2
    db 0FFh
    db 0FFh
    db  64h ; d
    db 0FDh ; ˝
    db  88h ; à
    db    1
    db 0FFh
    db 0FFh
    db    0
    db    0
    db  78h ; x
    db    2
    db 0FFh
    db 0FFh
    db    0
    db    0
    db  88h ; à
    db    1
    db 0FFh
    db 0FFh
    db  9Ch ; ú
    db    2
    db  78h ; x
    db    2
    db 0FFh
    db 0FFh
    db  9Ch ; ú
    db    2
    db  2Ch ; ,
    db    1
    db    0
    db    0
    db 0C8h ; »
    db    0
    db 0C8h ; »
    db    2
    db    0
    db    0
    db  64h ; d
    db 0FDh ; ˝
    db 0C8h ; »
    db 0FEh ; ˛
    db    0
    db    0
    db  9Ch ; ú
    db    2
    db  88h ; à
    db 0FDh ; ˝
    db 0FFh
    db 0FFh
    db  64h ; d
    db 0FDh ; ˝
    db  78h ; x
    db 0FEh ; ˛
    db 0FFh
    db 0FFh
    db  64h ; d
    db 0FDh ; ˝
    db  88h ; à
    db 0FDh ; ˝
    db 0FFh
    db 0FFh
    db    0
    db    0
    db  78h ; x
    db 0FEh ; ˛
    db 0FFh
    db 0FFh
    db    0
    db    0
    db  88h ; à
    db 0FDh ; ˝
    db 0FFh
    db 0FFh
    db  9Ch ; ú
    db    2
    db  78h ; x
    db 0FEh ; ˛
    db 0FFh
    db 0FFh
    db  9Ch ; ú
    db    2
    db 0D4h ; ‘
    db 0FEh ; ˛
    db    0
    db    0
    db 0C8h ; »
    db    0
    db 0C8h ; »
    db 0FEh ; ˛
    db    0
    db    0
    db  64h ; d
    db 0FDh ; ˝
    db  38h ; 8
    db 0FDh ; ˝
    db    0
    db    0
    db  9Ch ; ú
    db    2
    db  7Bh ; {
    db 0FFh
    db 0FFh
    db 0FFh
    db  64h ; d
    db 0FEh ; ˛
    db  63h ; c
    db    0
    db 0FFh
    db 0FFh
    db 0A1h ; °
    db 0FEh ; ˛
    db  54h ; T
    db 0FFh
    db 0FFh
    db 0FFh
    db 0C3h ; √
    db 0FEh ; ˛
    db  24h ; $
    db    0
    db 0FFh
    db 0FFh
    db  3Bh ; ;
    db 0FFh
    db  15h
    db 0FFh
    db 0FFh
    db 0FFh
    db  15h
    db 0FFh
    db 0BFh ; ø
    db 0FFh
    db 0FFh
    db 0FFh
    db 0BFh ; ø
    db 0FFh
    db 0C5h ; ≈
    db 0FEh ; ˛
    db 0FFh
    db 0FFh
    db  53h ; S
    db 0FFh
    db  3Eh ; >
    db 0FFh
    db 0FFh
    db 0FFh
    db  22h ; "
    db    0
    db  66h ; f
    db 0FEh ; ˛
    db 0FFh
    db 0FFh
    db  7Ah ; z
    db 0FFh
    db 0A5h ; •
    db 0FEh ; ˛
    db 0FFh
    db 0FFh
    db  62h ; b
    db    0
    db  38h ; 8
    db 0FFh
    db    0
    db    0
    db 0C8h ; »
    db    0
    db  9Dh ; ù
    db 0FFh
    db 0FFh
    db 0FFh
    db 0A1h ; °
    db 0FEh ; ˛
    db  85h ; Ö
    db    0
    db 0FFh
    db 0FFh
    db  64h ; d
    db 0FEh ; ˛
    db 0DCh ; ‹
    db 0FFh
    db 0FFh
    db 0FFh
    db  3Bh ; ;
    db 0FFh
    db 0ACh ; ¨
    db    0
    db 0FFh
    db 0FFh
    db 0C3h ; √
    db 0FEh ; ˛
    db  41h ; A
    db    0
    db 0FFh
    db 0FFh
    db 0BFh ; ø
    db 0FFh
    db 0EBh ; Î
    db    0
    db 0FFh
    db 0FFh
    db  15h
    db 0FFh
    db 0C2h ; ¬
    db    0
    db 0FFh
    db 0FFh
    db  22h ; "
    db    0
    db  3Bh ; ;
    db    1
    db 0FFh
    db 0FFh
    db  53h ; S
    db 0FFh
    db  5Bh ; [
    db    1
    db 0FFh
    db 0FFh
    db  62h ; b
    db    0
    db  9Ah ; ö
    db    1
    db 0FFh
    db 0FFh
    db  7Ah ; z
    db 0FFh
    db 0C8h ; »
    db    0
    db    0
    db    0
    db 0C8h ; »
    db    0
    db  83h ; É
    db    1
    db 0FFh
    db 0FFh
    db  7Ah ; z
    db 0FCh ; ¸
    db  72h ; r
    db    2
    db 0FFh
    db 0FFh
    db  8Eh ; é
    db 0FCh ; ¸
    db  73h ; s
    db    1
    db 0FFh
    db 0FFh
    db 0F2h ; Ú
    db 0FCh ; ¸
    db  60h ; `
    db    2
    db 0FFh
    db 0FFh
    db  1Bh
    db 0FDh ; ˝
    db  59h ; Y
    db    1
    db 0FFh
    db 0FFh
    db  69h ; i
    db 0FDh ; ˝
    db  41h ; A
    db    2
    db 0FFh
    db 0FFh
    db 0A6h ; ¶
    db 0FDh ; ˝
    db  35h ; 5
    db    1
    db 0FFh
    db 0FFh
    db 0DDh ; ›
    db 0FDh ; ˝
    db  17h
    db    2
    db 0FFh
    db 0FFh
    db  2Eh ; .
    db 0FEh ; ˛
    db    4
    db    1
    db 0FFh
    db 0FFh
    db  56h ; V
    db 0FEh ; ˛
    db 0DDh ; ›
    db    1
    db 0FFh
    db 0FFh
    db 0BBh ; ª
    db 0FEh ; ˛
    db 0CCh ; Ã
    db    0
    db 0FFh
    db 0FFh
    db 0C1h ; ¡
    db 0FEh ; ˛
    db  9Ch ; ú
    db    1
    db 0FFh
    db 0FFh
    db  39h ; 9
    db 0FFh
    db  8Bh ; ã
    db    0
    db 0FFh
    db 0FFh
    db  28h ; (
    db 0FFh
    db  50h ; P
    db    1
    db 0FFh
    db 0FFh
    db 0B1h ; ±
    db 0FFh
    db  41h ; A
    db    0
    db 0FFh
    db 0FFh
    db  89h ; â
    db 0FFh
    db 0FAh ; ˙
    db    0
    db 0FFh
    db 0FFh
    db  22h ; "
    db    0
    db 0E9h ; È
    db 0FFh
    db 0FFh
    db 0FFh
    db 0E9h ; È
    db 0FFh
    db  93h ; ì
    db    0
    db 0FFh
    db 0FFh
    db  93h ; ì
    db    0
    db  90h ; ê
    db 0FFh
    db 0FFh
    db 0FFh
    db  3Ch ; <
    db    0
    db  2Ah ; *
    db    0
    db 0FFh
    db 0FFh
    db 0F3h ; Û
    db    0
    db  2Fh ; /
    db 0FFh
    db 0FFh
    db 0FFh
    db  86h ; Ü
    db    0
    db 0BAh ; ∫
    db 0FFh
    db 0FFh
    db 0FFh
    db  4Ah ; J
    db    1
    db 0C9h ; …
    db 0FEh ; ˛
    db 0FFh
    db 0FFh
    db 0C7h ; «
    db    0
    db  42h ; B
    db 0FFh
    db 0FFh
    db 0FFh
    db  97h ; ó
    db    1
    db  5Dh ; ]
    db 0FEh ; ˛
    db 0FFh
    db 0FFh
    db    0
    db    1
    db 0C4h ; ƒ
    db 0FEh ; ˛
    db 0FFh
    db 0FFh
    db 0D9h ; Ÿ
    db    1
    db 0E5h ; Â
    db 0FDh ; ˝
    db 0FFh
    db 0FFh
    db  32h ; 2
    db    1
    db  37h ; 7
    db 0FEh ; ˛
    db 0FFh
    db 0FFh
    db  14h
    db    2
    db  71h ; q
    db 0FDh ; ˝
    db 0FFh
    db 0FFh
    db  57h ; W
    db    1
    db 0B0h ; ∞
    db 0FDh ; ˝
    db 0FFh
    db 0FFh
    db  3Fh ; ?
    db    2
    db 0FBh ; ˚
    db 0FCh ; ¸
    db 0FFh
    db 0FFh
    db  72h ; r
    db    1
    db  25h ; %
    db 0FDh ; ˝
    db 0FFh
    db 0FFh
    db  5Eh ; ^
    db    2
    db  82h ; Ç
    db 0FCh ; ¸
    db 0FFh
    db 0FFh
    db  82h ; Ç
    db    1
    db  98h ; ò
    db 0FCh ; ¸
    db 0FFh
    db 0FFh
    db  71h ; q
    db    2
    db  2Ch ; ,
    db    1
    db    0
    db    0
    db 0C8h ; »
    db    0
    db  8Eh ; é
    db 0FDh ; ˝
    db 0FFh
    db 0FFh
    db  8Eh ; é
    db 0FCh ; ¸
    db  7Dh ; }
    db 0FEh ; ˛
    db 0FFh
    db 0FFh
    db  7Ah ; z
    db 0FCh ; ¸
    db 0A0h ; †
    db 0FDh ; ˝
    db 0FFh
    db 0FFh
    db  1Bh
    db 0FDh ; ˝
    db  8Dh ; ç
    db 0FEh ; ˛
    db 0FFh
    db 0FFh
    db 0F2h ; Ú
    db 0FCh ; ¸
    db 0BFh ; ø
    db 0FDh ; ˝
    db 0FFh
    db 0FFh
    db 0A6h ; ¶
    db 0FDh ; ˝
    db 0A7h ; ß
    db 0FEh ; ˛
    db 0FFh
    db 0FFh
    db  69h ; i
    db 0FDh ; ˝
    db 0E9h ; È
    db 0FDh ; ˝
    db 0FFh
    db 0FFh
    db  2Eh ; .
    db 0FEh ; ˛
    db 0CBh ; À
    db 0FEh ; ˛
    db 0FFh
    db 0FFh
    db 0DDh ; ›
    db 0FDh ; ˝
    db  23h ; #
    db 0FEh ; ˛
    db 0FFh
    db 0FFh
    db 0BBh ; ª
    db 0FEh ; ˛
    db 0FCh ; ¸
    db 0FEh ; ˛
    db 0FFh
    db 0FFh
    db  56h ; V
    db 0FEh ; ˛
    db  64h ; d
    db 0FEh ; ˛
    db 0FFh
    db 0FFh
    db  39h ; 9
    db 0FFh
    db  34h ; 4
    db 0FFh
    db 0FFh
    db 0FFh
    db 0C1h ; ¡
    db 0FEh ; ˛
    db 0B0h ; ∞
    db 0FEh ; ˛
    db 0FFh
    db 0FFh
    db 0B1h ; ±
    db 0FFh
    db  75h ; u
    db 0FFh
    db 0FFh
    db 0FFh
    db  28h ; (
    db 0FFh
    db    6
    db 0FFh
    db 0FFh
    db 0FFh
    db  22h ; "
    db    0
    db 0BFh ; ø
    db 0FFh
    db 0FFh
    db 0FFh
    db  89h ; â
    db 0FFh
    db  6Dh ; m
    db 0FFh
    db 0FFh
    db 0FFh
    db  93h ; ì
    db    0
    db  17h
    db    0
    db 0FFh
    db 0FFh
    db 0E9h ; È
    db 0FFh
    db 0D6h ; ÷
    db 0FFh
    db 0FFh
    db 0FFh
    db 0F3h ; Û
    db    0
    db  70h ; p
    db    0
    db 0FFh
    db 0FFh
    db  3Ch ; <
    db    0
    db  46h ; F
    db    0
    db 0FFh
    db 0FFh
    db  4Ah ; J
    db    1
    db 0D1h ; —
    db    0
    db 0FFh
    db 0FFh
    db  86h ; Ü
    db    0
    db 0BEh ; æ
    db    0
    db 0FFh
    db 0FFh
    db  97h ; ó
    db    1
    db  37h ; 7
    db    1
    db 0FFh
    db 0FFh
    db 0C7h ; «
    db    0
    db  3Ch ; <
    db    1
    db 0FFh
    db 0FFh
    db 0D9h ; Ÿ
    db    1
    db 0A3h ; £
    db    1
    db 0FFh
    db 0FFh
    db    0
    db    1
    db 0C9h ; …
    db    1
    db 0FFh
    db 0FFh
    db  14h
    db    2
    db  1Bh
    db    2
    db 0FFh
    db 0FFh
    db  32h ; 2
    db    1
    db  50h ; P
    db    2
    db 0FFh
    db 0FFh
    db  3Fh ; ?
    db    2
    db  8Fh ; è
    db    2
    db 0FFh
    db 0FFh
    db  57h ; W
    db    1
    db 0DBh ; €
    db    2
    db 0FFh
    db 0FFh
    db  5Eh ; ^
    db    2
    db    5
    db    3
    db 0FFh
    db 0FFh
    db  72h ; r
    db    1
    db  68h ; h
    db    3
    db 0FFh
    db 0FFh
    db  71h ; q
    db    2
    db  7Eh ; ~
    db    3
    db 0FFh
    db 0FFh
    db  82h ; Ç
    db    1
    db 0D4h ; ‘
    db 0FEh ; ˛
    db    0
    db    0
    db 0C8h ; »
    db    0
    db  88h ; à
    db 0FFh
    db 0FFh
    db 0FFh
    db 0B2h ; ≤
    db 0FEh ; ˛
    db  78h ; x
    db    0
    db 0FFh
    db 0FFh
    db 0B2h ; ≤
    db 0FEh ; ˛
    db  88h ; à
    db 0FFh
    db 0FFh
    db 0FFh
    db  4Eh ; N
    db    1
    db  78h ; x
    db    0
    db 0FFh
    db 0FFh
    db  4Eh ; N
    db    1
    db  78h ; x
    db    0
    db 0C2h ; ¬
    db    1
    db    0
    db    0
    db  8Ch ; å
    db    0
    db 0C2h ; ¬
    db    1
    db 0B2h ; ≤
    db 0FEh ; ˛
    db  74h ; t
    db 0FFh
    db 0C2h ; ¬
    db    1
    db  4Eh ; N
    db    1
    db  88h ; à
    db 0FFh
    db 0FFh
    db 0FFh
    db 0B2h ; ≤
    db 0FEh ; ˛
    db  78h ; x
    db    0
    db 0FFh
    db 0FFh
    db 0B2h ; ≤
    db 0FEh ; ˛
    db  88h ; à
    db 0FFh
    db 0FFh
    db 0FFh
    db  4Eh ; N
    db    1
    db  78h ; x
    db    0
    db 0FFh
    db 0FFh
    db  4Eh ; N
    db    1
    db  78h ; x
    db    0
    db 0C2h ; ¬
    db    1
    db 0C8h ; »
    db    0
    db  8Ch ; å
    db    0
    db 0C2h ; ¬
    db    1
    db 0B2h ; ≤
    db 0FEh ; ˛
    db  74h ; t
    db 0FFh
    db 0C2h ; ¬
    db    1
    db  4Eh ; N
    db    1
    db  83h ; É
    db    1
    db 0FFh
    db 0FFh
    db  7Ah ; z
    db 0FCh ; ¸
    db  72h ; r
    db    2
    db 0FFh
    db 0FFh
    db  8Eh ; é
    db 0FCh ; ¸
    db  73h ; s
    db    1
    db 0FFh
    db 0FFh
    db 0F2h ; Ú
    db 0FCh ; ¸
    db  60h ; `
    db    2
    db 0FFh
    db 0FFh
    db  1Bh
    db 0FDh ; ˝
    db  59h ; Y
    db    1
    db 0FFh
    db 0FFh
    db  69h ; i
    db 0FDh ; ˝
    db  41h ; A
    db    2
    db 0FFh
    db 0FFh
    db 0A6h ; ¶
    db 0FDh ; ˝
    db  35h ; 5
    db    1
    db 0FFh
    db 0FFh
    db 0DDh ; ›
    db 0FDh ; ˝
    db  17h
    db    2
    db 0FFh
    db 0FFh
    db  2Eh ; .
    db 0FEh ; ˛
    db    4
    db    1
    db 0FFh
    db 0FFh
    db  56h ; V
    db 0FEh ; ˛
    db 0DDh ; ›
    db    1
    db 0FFh
    db 0FFh
    db 0BBh ; ª
    db 0FEh ; ˛
    db 0CCh ; Ã
    db    0
    db 0FFh
    db 0FFh
    db 0C1h ; ¡
    db 0FEh ; ˛
    db  9Ch ; ú
    db    1
    db 0FFh
    db 0FFh
    db  39h ; 9
    db 0FFh
    db  8Bh ; ã
    db    0
    db 0FFh
    db 0FFh
    db  28h ; (
    db 0FFh
    db  50h ; P
    db    1
    db 0FFh
    db 0FFh
    db 0B1h ; ±
    db 0FFh
    db  41h ; A
    db    0
    db 0FFh
    db 0FFh
    db  89h ; â
    db 0FFh
    db 0FAh ; ˙
    db    0
    db 0FFh
    db 0FFh
    db  22h ; "
    db    0
    db 0E9h ; È
    db 0FFh
    db 0FFh
    db 0FFh
    db 0E9h ; È
    db 0FFh
    db  93h ; ì
    db    0
    db 0FFh
    db 0FFh
    db  93h ; ì
    db    0
    db  90h ; ê
    db 0FFh
    db 0FFh
    db 0FFh
    db  3Ch ; <
    db    0
    db  2Ah ; *
    db    0
    db 0FFh
    db 0FFh
    db 0F3h ; Û
    db    0
    db  2Fh ; /
    db 0FFh
    db 0FFh
    db 0FFh
    db  86h ; Ü
    db    0
    db 0BAh ; ∫
    db 0FFh
    db 0FFh
    db 0FFh
    db  4Ah ; J
    db    1
    db 0C9h ; …
    db 0FEh ; ˛
    db 0FFh
    db 0FFh
    db 0C7h ; «
    db    0
    db  42h ; B
    db 0FFh
    db 0FFh
    db 0FFh
    db  97h ; ó
    db    1
    db  5Dh ; ]
    db 0FEh ; ˛
    db 0FFh
    db 0FFh
    db    0
    db    1
    db 0C4h ; ƒ
    db 0FEh ; ˛
    db 0FFh
    db 0FFh
    db 0D9h ; Ÿ
    db    1
    db 0E5h ; Â
    db 0FDh ; ˝
    db 0FFh
    db 0FFh
    db  32h ; 2
    db    1
    db  37h ; 7
    db 0FEh ; ˛
    db 0FFh
    db 0FFh
    db  14h
    db    2
    db  71h ; q
    db 0FDh ; ˝
    db 0FFh
    db 0FFh
    db  57h ; W
    db    1
    db 0B0h ; ∞
    db 0FDh ; ˝
    db 0FFh
    db 0FFh
    db  3Fh ; ?
    db    2
    db 0FBh ; ˚
    db 0FCh ; ¸
    db 0FFh
    db 0FFh
    db  72h ; r
    db    1
    db  25h ; %
    db 0FDh ; ˝
    db 0FFh
    db 0FFh
    db  5Eh ; ^
    db    2
    db  82h ; Ç
    db 0FCh ; ¸
    db 0FFh
    db 0FFh
    db  82h ; Ç
    db    1
    db  98h ; ò
    db 0FCh ; ¸
    db 0FFh
    db 0FFh
    db  71h ; q
    db    2
    db 0A0h ; †
    db    0
    db 0C2h ; ¬
    db    1
    db 0A0h ; †
    db    0
    db  6Fh ; o
    db    1
    db 0FFh
    db 0FFh
    db  6Dh ; m
    db 0FCh ; ¸
    db  58h ; X
    db    2
    db 0FFh
    db 0FFh
    db  7Eh ; ~
    db 0FCh ; ¸
    db  55h ; U
    db    1
    db 0FFh
    db 0FFh
    db 0D8h ; ÿ
    db 0FCh ; ¸
    db  37h ; 7
    db    2
    db 0FFh
    db 0FFh
    db 0FAh ; ˙
    db 0FCh ; ¸
    db  37h ; 7
    db    1
    db 0FFh
    db 0FFh
    db  41h ; A
    db 0FDh ; ˝
    db  12h
    db    2
    db 0FFh
    db 0FFh
    db  74h ; t
    db 0FDh ; ˝
    db  13h
    db    1
    db 0FFh
    db 0FFh
    db 0A6h ; ¶
    db 0FDh ; ˝
    db 0E8h ; Ë
    db    1
    db 0FFh
    db 0FFh
    db 0EAh ; Í
    db 0FDh ; ˝
    db 0E8h ; Ë
    db    0
    db 0FFh
    db 0FFh
    db    6
    db 0FEh ; ˛
    db 0B6h ; ∂
    db    1
    db 0FFh
    db 0FFh
    db  5Bh ; [
    db 0FEh ; ˛
    db 0A5h ; •
    db    0
    db 0FFh
    db 0FFh
    db  78h ; x
    db 0FEh ; ˛
    db  6Ah ; j
    db    1
    db 0FFh
    db 0FFh
    db 0E2h ; ‚
    db 0FEh ; ˛
    db  58h ; X
    db    0
    db 0FFh
    db 0FFh
    db 0E2h ; ‚
    db 0FEh ; ˛
    db  14h
    db    1
    db 0FFh
    db 0FFh
    db  62h ; b
    db 0FFh
    db    3
    db    0
    db 0FFh
    db 0FFh
    db  48h ; H
    db 0FFh
    db 0B6h ; ∂
    db    0
    db 0FFh
    db 0FFh
    db 0DDh ; ›
    db 0FFh
    db 0ABh ; ´
    db 0FFh
    db 0FFh
    db 0FFh
    db 0ABh ; ´
    db 0FFh
    db  55h ; U
    db    0
    db 0FFh
    db 0FFh
    db  55h ; U
    db    0
    db  4Ah ; J
    db 0FFh
    db 0FFh
    db 0FFh
    db  23h ; #
    db    0
    db 0FDh ; ˝
    db 0FFh
    db 0FFh
    db 0FFh
    db 0B8h ; ∏
    db    0
    db 0ECh ; Ï
    db 0FEh ; ˛
    db 0FFh
    db 0FFh
    db  9Eh ; û
    db    0
    db 0A8h ; ®
    db 0FFh
    db 0FFh
    db 0FFh
    db  1Eh
    db    1
    db  96h ; ñ
    db 0FEh ; ˛
    db 0FFh
    db 0FFh
    db  1Eh
    db    1
    db  5Bh ; [
    db 0FFh
    db 0FFh
    db 0FFh
    db  88h ; à
    db    1
    db  4Ah ; J
    db 0FEh ; ˛
    db 0FFh
    db 0FFh
    db 0A5h ; •
    db    1
    db  18h
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FAh ; ˙
    db    1
    db  18h
    db 0FEh ; ˛
    db 0FFh
    db 0FFh
    db  16h
    db    2
    db 0EDh ; Ì
    db 0FEh ; ˛
    db 0FFh
    db 0FFh
    db  5Ah ; Z
    db    2
    db 0EEh ; Ó
    db 0FDh ; ˝
    db 0FFh
    db 0FFh
    db  8Ch ; å
    db    2
    db 0C9h ; …
    db 0FEh ; ˛
    db 0FFh
    db 0FFh
    db 0BFh ; ø
    db    2
    db 0C9h ; …
    db 0FDh ; ˝
    db 0FFh
    db 0FFh
    db    6
    db    3
    db 0ABh ; ´
    db 0FEh ; ˛
    db 0FFh
    db 0FFh
    db  28h ; (
    db    3
    db 0A8h ; ®
    db 0FDh ; ˝
    db 0FFh
    db 0FFh
    db  82h ; Ç
    db    3
    db  91h ; ë
    db 0FEh ; ˛
    db 0FFh
    db 0FFh
    db  93h ; ì
    db    3
    db  7Dh ; }
    db    0
    db    0
    db    0
    db  7Dh ; }
    db    0
    db 0A8h ; ®
    db 0FDh ; ˝
    db 0FFh
    db 0FFh
    db  7Eh ; ~
    db 0FCh ; ¸
    db  91h ; ë
    db 0FEh ; ˛
    db 0FFh
    db 0FFh
    db  6Dh ; m
    db 0FCh ; ¸
    db 0C9h ; …
    db 0FDh ; ˝
    db 0FFh
    db 0FFh
    db 0FAh ; ˙
    db 0FCh ; ¸
    db 0ABh ; ´
    db 0FEh ; ˛
    db 0FFh
    db 0FFh
    db 0D8h ; ÿ
    db 0FCh ; ¸
    db 0EEh ; Ó
    db 0FDh ; ˝
    db 0FFh
    db 0FFh
    db  74h ; t
    db 0FDh ; ˝
    db 0C9h ; …
    db 0FEh ; ˛
    db 0FFh
    db 0FFh
    db  41h ; A
    db 0FDh ; ˝
    db  18h
    db 0FEh ; ˛
    db 0FFh
    db 0FFh
    db 0EAh ; Í
    db 0FDh ; ˝
    db 0EDh ; Ì
    db 0FEh ; ˛
    db 0FFh
    db 0FFh
    db 0A6h ; ¶
    db 0FDh ; ˝
    db  4Ah ; J
    db 0FEh ; ˛
    db 0FFh
    db 0FFh
    db  5Bh ; [
    db 0FEh ; ˛
    db  18h
    db 0FFh
    db 0FFh
    db 0FFh
    db    6
    db 0FEh ; ˛
    db  96h ; ñ
    db 0FEh ; ˛
    db 0FFh
    db 0FFh
    db 0E2h ; ‚
    db 0FEh ; ˛
    db  5Bh ; [
    db 0FFh
    db 0FFh
    db 0FFh
    db  78h ; x
    db 0FEh ; ˛
    db 0ECh ; Ï
    db 0FEh ; ˛
    db 0FFh
    db 0FFh
    db  62h ; b
    db 0FFh
    db 0A8h ; ®
    db 0FFh
    db 0FFh
    db 0FFh
    db 0E2h ; ‚
    db 0FEh ; ˛
    db  4Ah ; J
    db 0FFh
    db 0FFh
    db 0FFh
    db 0DDh ; ›
    db 0FFh
    db 0FDh ; ˝
    db 0FFh
    db 0FFh
    db 0FFh
    db  48h ; H
    db 0FFh
    db 0ABh ; ´
    db 0FFh
    db 0FFh
    db 0FFh
    db  55h ; U
    db    0
    db  55h ; U
    db    0
    db 0FFh
    db 0FFh
    db 0ABh ; ´
    db 0FFh
    db    3
    db    0
    db 0FFh
    db 0FFh
    db 0B8h ; ∏
    db    0
    db 0B6h ; ∂
    db    0
    db 0FFh
    db 0FFh
    db  23h ; #
    db    0
    db  58h ; X
    db    0
    db 0FFh
    db 0FFh
    db  1Eh
    db    1
    db  14h
    db    1
    db 0FFh
    db 0FFh
    db  9Eh ; û
    db    0
    db 0A5h ; •
    db    0
    db 0FFh
    db 0FFh
    db  88h ; à
    db    1
    db  6Ah ; j
    db    1
    db 0FFh
    db 0FFh
    db  1Eh
    db    1
    db 0E8h ; Ë
    db    0
    db 0FFh
    db 0FFh
    db 0FAh ; ˙
    db    1
    db 0B6h ; ∂
    db    1
    db 0FFh
    db 0FFh
    db 0A5h ; •
    db    1
    db  13h
    db    1
    db 0FFh
    db 0FFh
    db  5Ah ; Z
    db    2
    db 0E8h ; Ë
    db    1
    db 0FFh
    db 0FFh
    db  16h
    db    2
    db  37h ; 7
    db    1
    db 0FFh
    db 0FFh
    db 0BFh ; ø
    db    2
    db  12h
    db    2
    db 0FFh
    db 0FFh
    db  8Ch ; å
    db    2
    db  55h ; U
    db    1
    db 0FFh
    db 0FFh
    db  28h ; (
    db    3
    db  37h ; 7
    db    2
    db 0FFh
    db 0FFh
    db    6
    db    3
    db  6Fh ; o
    db    1
    db 0FFh
    db 0FFh
    db  93h ; ì
    db    3
    db  58h ; X
    db    2
    db 0FFh
    db 0FFh
    db  82h ; Ç
    db    3
    db  7Dh ; }
    db    0
    db    0
    db    0
    db  83h ; É
    db 0FFh
    db  88h ; à
    db 0FFh
    db 0FFh
    db 0FFh
    db 0B2h ; ≤
    db 0FEh ; ˛
    db  78h ; x
    db    0
    db 0FFh
    db 0FFh
    db 0B2h ; ≤
    db 0FEh ; ˛
    db  88h ; à
    db 0FFh
    db 0FFh
    db 0FFh
    db  4Eh ; N
    db    1
    db  78h ; x
    db    0
    db 0FFh
    db 0FFh
    db  4Eh ; N
    db    1
    db  78h ; x
    db    0
    db 0C2h ; ¬
    db    1
    db 0F4h ; Ù
    db    1
    db  8Ch ; å
    db    0
    db  51h ; Q
    db    0
    db 0B2h ; ≤
    db 0FEh ; ˛
    db  74h ; t
    db 0FFh
    db  77h ; w
    db    1
    db  4Eh ; N
    db    1
    db  52h ; R
    db 0FFh
    db    0
    db    0
    db  9Ch ; ú
    db 0FCh ; ¸
    db  64h ; d
    db    0
    db    0
    db    0
    db  9Ch ; ú
    db 0FCh ; ¸
    db  1Eh
    db 0FFh
    db    0
    db    0
    db  40h ; @
    db 0FDh ; ˝
    db  50h ; P
    db    0
    db    0
    db    0
    db  40h ; @
    db 0FDh ; ˝
    db 0ECh ; Ï
    db 0FEh ; ˛
    db    0
    db    0
    db 0E8h ; Ë
    db 0FDh ; ˝
    db  3Ch ; <
    db    0
    db    0
    db    0
    db 0E8h ; Ë
    db 0FDh ; ˝
    db 0C0h ; ¿
    db 0FEh ; ˛
    db    0
    db    0
    db  98h ; ò
    db 0FEh ; ˛
    db  28h ; (
    db    0
    db    0
    db    0
    db  98h ; ò
    db 0FEh ; ˛
    db  97h ; ó
    db 0FEh ; ˛
    db    0
    db    0
    db  4Eh ; N
    db 0FFh
    db  14h
    db    0
    db    0
    db    0
    db  4Eh ; N
    db 0FFh
    db  70h ; p
    db 0FEh ; ˛
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db  70h ; p
    db 0FEh ; ˛
    db  87h ; á
    db    0
    db 0E0h ; ‡
    db    0
    db    0
    db    0
    db  87h ; á
    db    0
    db 0E0h ; ‡
    db    0
    db 0A0h ; †
    db 0FEh ; ˛
    db  2Bh ; +
    db    1
    db  85h ; Ö
    db    1
    db  2Eh ; .
    db    0
    db  2Bh ; +
    db    1
    db  85h ; Ö
    db    1
    db 0D0h ; –
    db 0FEh ; ˛
    db  0Ch
    db    2
    db 0C1h ; ¡
    db    1
    db  5Fh ; _
    db    0
    db  0Ch
    db    2
    db 0C1h ; ¡
    db    1
    db 0F2h ; Ú
    db 0FEh ; ˛
    db 0EDh ; Ì
    db    2
    db  85h ; Ö
    db    1
    db  81h ; Å
    db    0
    db 0EDh ; Ì
    db    2
    db  85h ; Ö
    db    1
    db  15h
    db 0FFh
    db  92h ; í
    db    3
    db 0E1h ; ·
    db    0
    db 0A4h ; §
    db    0
    db  92h ; í
    db    3
    db 0E1h ; ·
    db    0
    db  38h ; 8
    db 0FFh
    db 0CFh ; œ
    db    3
    db    0
    db    0
    db 0C8h ; »
    db    0
    db 0CFh ; œ
    db    3
    db    0
    db    0
    db  5Ch ; \
    db 0FFh
    db  92h ; í
    db    3
    db  1Fh
    db 0FFh
    db 0EBh ; Î
    db    0
    db  92h ; í
    db    3
    db  1Fh
    db 0FFh
    db  7Fh ; 
    db 0FFh
    db 0EDh ; Ì
    db    2
    db  7Bh ; {
    db 0FEh ; ˛
    db  0Eh
    db    1
    db 0EDh ; Ì
    db    2
    db  7Bh ; {
    db 0FEh ; ˛
    db 0A1h ; °
    db 0FFh
    db  0Ch
    db    2
    db  3Fh ; ?
    db 0FEh ; ˛
    db  30h ; 0
    db    1
    db  0Ch
    db    2
    db  3Fh ; ?
    db 0FEh ; ˛
    db 0D2h ; “
    db 0FFh
    db  2Bh ; +
    db    1
    db  7Bh ; {
    db 0FEh ; ˛
    db  60h ; `
    db    1
    db  2Bh ; +
    db    1
    db  7Bh ; {
    db 0FEh ; ˛
    db    0
    db    0
    db  87h ; á
    db    0
    db  20h
    db 0FFh
    db  90h ; ê
    db    1
    db  87h ; á
    db    0
    db  20h
    db 0FFh
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db  90h ; ê
    db    1
    db    0
    db    0
    db    0
    db    0
    db 0ECh ; Ï
    db 0FFh
    db    0
    db    0
    db 0B2h ; ≤
    db    0
    db  69h ; i
    db    1
    db    0
    db    0
    db 0B2h ; ≤
    db    0
    db 0D8h ; ÿ
    db 0FFh
    db    0
    db    0
    db  68h ; h
    db    1
    db  40h ; @
    db    1
    db    0
    db    0
    db  68h ; h
    db    1
    db 0C4h ; ƒ
    db 0FFh
    db    0
    db    0
    db  18h
    db    2
    db  14h
    db    1
    db    0
    db    0
    db  18h
    db    2
    db 0B0h ; ∞
    db 0FFh
    db    0
    db    0
    db 0C0h ; ¿
    db    2
    db 0E2h ; ‚
    db    0
    db    0
    db    0
    db 0C0h ; ¿
    db    2
    db  9Ch ; ú
    db 0FFh
    db    0
    db    0
    db  64h ; d
    db    3
    db 0AEh ; Æ
    db    0
    db    0
    db    0
    db  64h ; d
    db    3
    db 0E0h ; ‡
    db    1
    db    0
    db    0
    db    0
    db    0
    db 0C8h ; »
    db    0
    db    0
    db    0
    db  64h ; d
    db 0FDh ; ˝
    db  38h ; 8
    db 0FFh
    db    0
    db    0
    db  9Ch ; ú
    db    2
    db 0E1h ; ·
    db 0FFh
    db    0
    db    0
    db  7Eh ; ~
    db 0FCh ; ¸
    db  1Fh
    db    0
    db    0
    db    0
    db  7Eh ; ~
    db 0FCh ; ¸
    db  1Fh
    db    0
    db    0
    db    0
    db  13h
    db 0FDh ; ˝
    db  54h ; T
    db    0
    db  23h ; #
    db    0
    db  13h
    db 0FDh ; ˝
    db  54h ; T
    db    0
    db  23h ; #
    db    0
    db 0A8h ; ®
    db 0FDh ; ˝
    db  73h ; s
    db    0
    db  58h ; X
    db    0
    db 0A8h ; ®
    db 0FDh ; ˝
    db  73h ; s
    db    0
    db  58h ; X
    db    0
    db  3Dh ; =
    db 0FEh ; ˛
    db  73h ; s
    db    0
    db  97h ; ó
    db    0
    db  3Dh ; =
    db 0FEh ; ˛
    db  73h ; s
    db    0
    db  97h ; ó
    db    0
    db 0D4h ; ‘
    db 0FEh ; ˛
    db  54h ; T
    db    0
    db 0CCh ; Ã
    db    0
    db 0D4h ; ‘
    db 0FEh ; ˛
    db  54h ; T
    db    0
    db 0CCh ; Ã
    db    0
    db  67h ; g
    db 0FFh
    db  1Fh
    db    0
    db 0EBh ; Î
    db    0
    db  67h ; g
    db 0FFh
    db  1Fh
    db    0
    db 0EBh ; Î
    db    0
    db    0
    db    0
    db 0E1h ; ·
    db 0FFh
    db 0EBh ; Î
    db    0
    db    0
    db    0
    db 0E1h ; ·
    db 0FFh
    db 0EBh ; Î
    db    0
    db  99h ; ô
    db    0
    db 0ACh ; ¨
    db 0FFh
    db 0CCh ; Ã
    db    0
    db  99h ; ô
    db    0
    db 0ACh ; ¨
    db 0FFh
    db 0CCh ; Ã
    db    0
    db  2Ch ; ,
    db    1
    db  8Dh ; ç
    db 0FFh
    db  97h ; ó
    db    0
    db  2Ch ; ,
    db    1
    db  8Dh ; ç
    db 0FFh
    db  97h ; ó
    db    0
    db 0C3h ; √
    db    1
    db  8Dh ; ç
    db 0FFh
    db  58h ; X
    db    0
    db 0C3h ; √
    db    1
    db  8Dh ; ç
    db 0FFh
    db  58h ; X
    db    0
    db  58h ; X
    db    2
    db 0ACh ; ¨
    db 0FFh
    db  23h ; #
    db    0
    db  58h ; X
    db    2
    db 0ACh ; ¨
    db 0FFh
    db  23h ; #
    db    0
    db 0EDh ; Ì
    db    2
    db 0E1h ; ·
    db 0FFh
    db    0
    db    0
    db 0EDh ; Ì
    db    2
    db 0E1h ; ·
    db 0FFh
    db    0
    db    0
    db  82h ; Ç
    db    3
    db  1Fh
    db    0
    db    0
    db    0
    db  82h ; Ç
    db    3
    db 0E0h ; ‡
    db    1
    db    0
    db    0
    db    0
    db    0
    db 0C8h ; »
    db    0
    db    0
    db    0
    db  64h ; d
    db 0FDh ; ˝
    db  38h ; 8
    db 0FFh
    db    0
    db    0
    db  9Ch ; ú
    db    2
    db  88h ; à
    db 0FFh
    db 0FFh
    db 0FFh
    db 0B2h ; ≤
    db 0FEh ; ˛
    db  78h ; x
    db    0
    db 0FFh
    db 0FFh
    db 0B2h ; ≤
    db 0FEh ; ˛
    db  88h ; à
    db 0FFh
    db 0FFh
    db 0FFh
    db  4Eh ; N
    db    1
    db  78h ; x
    db    0
    db 0FFh
    db 0FFh
    db  4Eh ; N
    db    1
    db  38h ; 8
    db 0FFh
    db    0
    db    0
    db    0
    db    0
    db  78h ; x
    db    0
    db  82h ; Ç
    db    0
    db 0B2h ; ≤
    db 0FEh ; ˛
    db  74h ; t
    db 0FFh
    db    0
    db    0
    db  4Eh ; N
    db    1
    db  88h ; à
    db 0FFh
    db 0FFh
    db 0FFh
    db 0B2h ; ≤
    db 0FEh ; ˛
    db  78h ; x
    db    0
    db 0FFh
    db 0FFh
    db 0B2h ; ≤
    db 0FEh ; ˛
    db  88h ; à
    db 0FFh
    db 0FFh
    db 0FFh
    db  4Eh ; N
    db    1
    db  78h ; x
    db    0
    db 0FFh
    db 0FFh
    db  4Eh ; N
    db    1
    db  38h ; 8
    db 0FFh
    db    0
    db    0
    db    0
    db    0
    db  78h ; x
    db    0
    db  82h ; Ç
    db    0
    db 0B2h ; ≤
    db 0FEh ; ˛
    db  74h ; t
    db 0FFh
    db    0
    db    0
    db  4Eh ; N
    db    1
    db  88h ; à
    db 0FFh
    db 0FFh
    db 0FFh
    db 0B2h ; ≤
    db 0FEh ; ˛
    db  78h ; x
    db    0
    db 0FFh
    db 0FFh
    db 0B2h ; ≤
    db 0FEh ; ˛
    db  88h ; à
    db 0FFh
    db 0FFh
    db 0FFh
    db  4Eh ; N
    db    1
    db  78h ; x
    db    0
    db 0FFh
    db 0FFh
    db  4Eh ; N
    db    1
    db 0C8h ; »
    db    0
    db    0
    db    0
    db    0
    db    0
    db  8Ch ; å
    db    0
    db    0
    db    0
    db 0B2h ; ≤
    db 0FEh ; ˛
    db  88h ; à
    db 0FFh
    db  7Bh ; {
    db    0
    db  4Eh ; N
    db    1
    db  83h ; É
    db    1
    db 0FFh
    db 0FFh
    db  7Ah ; z
    db 0FCh ; ¸
    db  72h ; r
    db    2
    db 0FFh
    db 0FFh
    db  8Eh ; é
    db 0FCh ; ¸
    db  73h ; s
    db    1
    db 0FFh
    db 0FFh
    db 0F2h ; Ú
    db 0FCh ; ¸
    db  60h ; `
    db    2
    db 0FFh
    db 0FFh
    db  1Bh
    db 0FDh ; ˝
    db  59h ; Y
    db    1
    db 0FFh
    db 0FFh
    db  69h ; i
    db 0FDh ; ˝
    db  41h ; A
    db    2
    db 0FFh
    db 0FFh
    db 0A6h ; ¶
    db 0FDh ; ˝
    db  35h ; 5
    db    1
    db 0FFh
    db 0FFh
    db 0DDh ; ›
    db 0FDh ; ˝
    db  17h
    db    2
    db 0FFh
    db 0FFh
    db  2Eh ; .
    db 0FEh ; ˛
    db    4
    db    1
    db 0FFh
    db 0FFh
    db  56h ; V
    db 0FEh ; ˛
    db 0DDh ; ›
    db    1
    db 0FFh
    db 0FFh
    db 0BBh ; ª
    db 0FEh ; ˛
    db 0CCh ; Ã
    db    0
    db 0FFh
    db 0FFh
    db 0C1h ; ¡
    db 0FEh ; ˛
    db  9Ch ; ú
    db    1
    db 0FFh
    db 0FFh
    db  39h ; 9
    db 0FFh
    db  8Bh ; ã
    db    0
    db 0FFh
    db 0FFh
    db  28h ; (
    db 0FFh
    db  50h ; P
    db    1
    db 0FFh
    db 0FFh
    db 0B1h ; ±
    db 0FFh
    db  41h ; A
    db    0
    db 0FFh
    db 0FFh
    db  89h ; â
    db 0FFh
    db 0FAh ; ˙
    db    0
    db 0FFh
    db 0FFh
    db  22h ; "
    db    0
    db 0E9h ; È
    db 0FFh
    db 0FFh
    db 0FFh
    db 0E9h ; È
    db 0FFh
    db  93h ; ì
    db    0
    db 0FFh
    db 0FFh
    db  93h ; ì
    db    0
    db  90h ; ê
    db 0FFh
    db 0FFh
    db 0FFh
    db  3Ch ; <
    db    0
    db  2Ah ; *
    db    0
    db 0FFh
    db 0FFh
    db 0F3h ; Û
    db    0
    db  2Fh ; /
    db 0FFh
    db 0FFh
    db 0FFh
    db  86h ; Ü
    db    0
    db 0BAh ; ∫
    db 0FFh
    db 0FFh
    db 0FFh
    db  4Ah ; J
    db    1
    db 0C9h ; …
    db 0FEh ; ˛
    db 0FFh
    db 0FFh
    db 0C7h ; «
    db    0
    db  42h ; B
    db 0FFh
    db 0FFh
    db 0FFh
    db  97h ; ó
    db    1
    db  5Dh ; ]
    db 0FEh ; ˛
    db 0FFh
    db 0FFh
    db    0
    db    1
    db 0C4h ; ƒ
    db 0FEh ; ˛
    db 0FFh
    db 0FFh
    db 0D9h ; Ÿ
    db    1
    db 0E5h ; Â
    db 0FDh ; ˝
    db 0FFh
    db 0FFh
    db  32h ; 2
    db    1
    db  37h ; 7
    db 0FEh ; ˛
    db 0FFh
    db 0FFh
    db  14h
    db    2
    db  71h ; q
    db 0FDh ; ˝
    db 0FFh
    db 0FFh
    db  57h ; W
    db    1
    db 0B0h ; ∞
    db 0FDh ; ˝
    db 0FFh
    db 0FFh
    db  3Fh ; ?
    db    2
    db 0FBh ; ˚
    db 0FCh ; ¸
    db 0FFh
    db 0FFh
    db  72h ; r
    db    1
    db  25h ; %
    db 0FDh ; ˝
    db 0FFh
    db 0FFh
    db  5Eh ; ^
    db    2
    db  82h ; Ç
    db 0FCh ; ¸
    db 0FFh
    db 0FFh
    db  82h ; Ç
    db    1
    db  98h ; ò
    db 0FCh ; ¸
    db 0FFh
    db 0FFh
    db  71h ; q
    db    2
    db 0D4h ; ‘
    db 0FEh ; ˛
    db    0
    db    0
    db  38h ; 8
    db 0FFh
    db  88h ; à
    db 0FDh ; ˝
    db    0
    db    0
    db 0B2h ; ≤
    db 0FCh ; ¸
    db  78h ; x
    db 0FEh ; ˛
    db    0
    db    0
    db 0B2h ; ≤
    db 0FCh ; ¸
    db  88h ; à
    db 0FDh ; ˝
    db    0
    db    0
    db 0B2h ; ≤
    db 0FEh ; ˛
    db  78h ; x
    db 0FEh ; ˛
    db    0
    db    0
    db 0B2h ; ≤
    db 0FEh ; ˛
    db  88h ; à
    db 0FDh ; ˝
    db    0
    db    0
    db    0
    db    0
    db  78h ; x
    db 0FEh ; ˛
    db    0
    db    0
    db    0
    db    0
    db  9Dh ; ù
    db 0FDh ; ˝
    db  13h
    db    0
    db 0A1h ; °
    db    0
    db  85h ; Ö
    db 0FEh ; ˛
    db  13h
    db    0
    db  64h ; d
    db    0
    db 0DCh ; ‹
    db 0FDh ; ˝
    db  26h ; &
    db    0
    db  3Bh ; ;
    db    1
    db 0ACh ; ¨
    db 0FEh ; ˛
    db  26h ; &
    db    0
    db 0C3h ; √
    db    0
    db  41h ; A
    db 0FEh ; ˛
    db  39h ; 9
    db    0
    db 0BFh ; ø
    db    1
    db 0EBh ; Î
    db 0FEh ; ˛
    db  39h ; 9
    db    0
    db  15h
    db    1
    db 0C2h ; ¬
    db 0FEh ; ˛
    db  4Ch ; L
    db    0
    db  22h ; "
    db    2
    db  3Bh ; ;
    db 0FFh
    db  4Ch ; L
    db    0
    db  53h ; S
    db    1
    db  5Bh ; [
    db 0FFh
    db  5Fh ; _
    db    0
    db  62h ; b
    db    2
    db  9Ah ; ö
    db 0FFh
    db  5Fh ; _
    db    0
    db  7Ah ; z
    db    1
    db    0
    db    0
    db  72h ; r
    db    0
    db  78h ; x
    db    2
    db    0
    db    0
    db  72h ; r
    db    0
    db  88h ; à
    db    1
    db 0A1h ; °
    db    0
    db  85h ; Ö
    db    0
    db  63h ; c
    db    2
    db  64h ; d
    db    0
    db  85h ; Ö
    db    0
    db  7Bh ; {
    db    1
    db  3Bh ; ;
    db    1
    db  98h ; ò
    db    0
    db  24h ; $
    db    2
    db 0C3h ; √
    db    0
    db  98h ; ò
    db    0
    db  54h ; T
    db    1
    db 0BFh ; ø
    db    1
    db 0ABh ; ´
    db    0
    db 0BFh ; ø
    db    1
    db  15h
    db    1
    db 0ABh ; ´
    db    0
    db  15h
    db    1
    db  22h ; "
    db    2
    db 0BEh ; æ
    db    0
    db  3Eh ; >
    db    1
    db  53h ; S
    db    1
    db 0BEh ; æ
    db    0
    db 0C5h ; ≈
    db    0
    db  62h ; b
    db    2
    db 0D1h ; —
    db    0
    db 0A5h ; •
    db    0
    db  7Ah ; z
    db    1
    db 0D1h ; —
    db    0
    db  66h ; f
    db    0
    db  78h ; x
    db    2
    db 0E4h ; ‰
    db    0
    db    0
    db    0
    db  88h ; à
    db    1
    db 0E4h ; ‰
    db    0
    db    0
    db    0
    db  63h ; c
    db    2
    db 0F7h ; ˜
    db    0
    db  5Fh ; _
    db 0FFh
    db  7Bh ; {
    db    1
    db 0F7h ; ˜
    db    0
    db  9Ch ; ú
    db 0FFh
    db  24h ; $
    db    2
    db  0Ah
    db    1
    db 0C5h ; ≈
    db 0FEh ; ˛
    db  54h ; T
    db    1
    db  0Ah
    db    1
    db  3Dh ; =
    db 0FFh
    db 0BFh ; ø
    db    1
    db  1Dh
    db    1
    db  41h ; A
    db 0FEh ; ˛
    db  15h
    db    1
    db  1Dh
    db    1
    db 0EBh ; Î
    db 0FEh ; ˛
    db  3Eh ; >
    db    1
    db  30h ; 0
    db    1
    db 0DEh ; ﬁ
    db 0FDh ; ˝
    db 0C5h ; ≈
    db    0
    db  30h ; 0
    db    1
    db 0ADh ; ≠
    db 0FEh ; ˛
    db 0A5h ; •
    db    0
    db  43h ; C
    db    1
    db  9Eh ; û
    db 0FDh ; ˝
    db  66h ; f
    db    0
    db  43h ; C
    db    1
    db  86h ; Ü
    db 0FEh ; ˛
    db    0
    db    0
    db  56h ; V
    db    1
    db  88h ; à
    db 0FDh ; ˝
    db    0
    db    0
    db  56h ; V
    db    1
    db  78h ; x
    db 0FEh ; ˛
    db  5Fh ; _
    db 0FFh
    db  69h ; i
    db    1
    db  9Dh ; ù
    db 0FDh ; ˝
    db  9Ch ; ú
    db 0FFh
    db  69h ; i
    db    1
    db  85h ; Ö
    db 0FEh ; ˛
    db 0C5h ; ≈
    db 0FEh ; ˛
    db  7Ch ; |
    db    1
    db 0DCh ; ‹
    db 0FDh ; ˝
    db  3Dh ; =
    db 0FFh
    db  7Ch ; |
    db    1
    db 0ACh ; ¨
    db 0FEh ; ˛
    db  41h ; A
    db 0FEh ; ˛
    db  8Fh ; è
    db    1
    db  41h ; A
    db 0FEh ; ˛
    db 0EBh ; Î
    db 0FEh ; ˛
    db  8Fh ; è
    db    1
    db 0EBh ; Î
    db 0FEh ; ˛
    db 0DEh ; ﬁ
    db 0FDh ; ˝
    db 0A2h ; ¢
    db    1
    db 0C2h ; ¬
    db 0FEh ; ˛
    db 0ADh ; ≠
    db 0FEh ; ˛
    db 0A2h ; ¢
    db    1
    db  3Bh ; ;
    db 0FFh
    db  9Eh ; û
    db 0FDh ; ˝
    db 0B5h ; µ
    db    1
    db  5Bh ; [
    db 0FFh
    db  86h ; Ü
    db 0FEh ; ˛
    db 0B5h ; µ
    db    1
    db  9Ah ; ö
    db 0FFh
    db  88h ; à
    db 0FDh ; ˝
    db 0C2h ; ¬
    db    1
    db    0
    db    0
    db  78h ; x
    db 0FEh ; ˛
    db 0C2h ; ¬
    db    1
    db    0
    db    0
    db  88h ; à
    db 0FDh ; ˝
    db 0C2h ; ¬
    db    1
    db  4Eh ; N
    db    1
    db  78h ; x
    db 0FEh ; ˛
    db 0C2h ; ¬
    db    1
    db  4Eh ; N
    db    1
    db  88h ; à
    db 0FDh ; ˝
    db 0C2h ; ¬
    db    1
    db  4Eh ; N
    db    3
    db  78h ; x
    db 0FEh ; ˛
    db 0C2h ; ¬
    db    1
    db  4Eh ; N
    db    3
    db    0
    db    0
    db 0C2h ; ¬
    db    1
    db    0
    db    0
    db  88h ; à
    db    1
    db    0
    db    0
    db 0B2h ; ≤
    db 0FCh ; ¸
    db  78h ; x
    db    2
    db    0
    db    0
    db 0B2h ; ≤
    db 0FCh ; ¸
    db  88h ; à
    db    1
    db    0
    db    0
    db 0B2h ; ≤
    db 0FEh ; ˛
    db  78h ; x
    db    2
    db    0
    db    0
    db 0B2h ; ≤
    db 0FEh ; ˛
    db  88h ; à
    db    1
    db    0
    db    0
    db    0
    db    0
    db  78h ; x
    db    2
    db    0
    db    0
    db    0
    db    0
    db  7Bh ; {
    db    1
    db  13h
    db    0
    db  64h ; d
    db    0
    db  63h ; c
    db    2
    db  13h
    db    0
    db 0A1h ; °
    db    0
    db  54h ; T
    db    1
    db  26h ; &
    db    0
    db 0C3h ; √
    db    0
    db  24h ; $
    db    2
    db  26h ; &
    db    0
    db  3Bh ; ;
    db    1
    db  15h
    db    1
    db  39h ; 9
    db    0
    db  15h
    db    1
    db 0BFh ; ø
    db    1
    db  39h ; 9
    db    0
    db 0BFh ; ø
    db    1
    db 0C5h ; ≈
    db    0
    db  4Ch ; L
    db    0
    db  53h ; S
    db    1
    db  3Eh ; >
    db    1
    db  4Ch ; L
    db    0
    db  22h ; "
    db    2
    db  66h ; f
    db    0
    db  5Fh ; _
    db    0
    db  7Ah ; z
    db    1
    db 0A5h ; •
    db    0
    db  5Fh ; _
    db    0
    db  62h ; b
    db    2
    db    0
    db    0
    db  72h ; r
    db    0
    db  88h ; à
    db    1
    db    0
    db    0
    db  72h ; r
    db    0
    db  78h ; x
    db    2
    db  9Ch ; ú
    db 0FFh
    db  85h ; Ö
    db    0
    db  7Bh ; {
    db    1
    db  5Fh ; _
    db 0FFh
    db  85h ; Ö
    db    0
    db  63h ; c
    db    2
    db  3Dh ; =
    db 0FFh
    db  98h ; ò
    db    0
    db  54h ; T
    db    1
    db 0C5h ; ≈
    db 0FEh ; ˛
    db  98h ; ò
    db    0
    db  24h ; $
    db    2
    db 0EBh ; Î
    db 0FEh ; ˛
    db 0ABh ; ´
    db    0
    db  15h
    db    1
    db  41h ; A
    db 0FEh ; ˛
    db 0ABh ; ´
    db    0
    db 0BFh ; ø
    db    1
    db 0ADh ; ≠
    db 0FEh ; ˛
    db 0BEh ; æ
    db    0
    db 0C5h ; ≈
    db    0
    db 0DEh ; ﬁ
    db 0FDh ; ˝
    db 0BEh ; æ
    db    0
    db  3Eh ; >
    db    1
    db  86h ; Ü
    db 0FEh ; ˛
    db 0D1h ; —
    db    0
    db  66h ; f
    db    0
    db  9Eh ; û
    db 0FDh ; ˝
    db 0D1h ; —
    db    0
    db 0A5h ; •
    db    0
    db  78h ; x
    db 0FEh ; ˛
    db 0E4h ; ‰
    db    0
    db    0
    db    0
    db  88h ; à
    db 0FDh ; ˝
    db 0E4h ; ‰
    db    0
    db    0
    db    0
    db  85h ; Ö
    db 0FEh ; ˛
    db 0F7h ; ˜
    db    0
    db  9Ch ; ú
    db 0FFh
    db  9Dh ; ù
    db 0FDh ; ˝
    db 0F7h ; ˜
    db    0
    db  5Fh ; _
    db 0FFh
    db 0ACh ; ¨
    db 0FEh ; ˛
    db  0Ah
    db    1
    db  3Dh ; =
    db 0FFh
    db 0DCh ; ‹
    db 0FDh ; ˝
    db  0Ah
    db    1
    db 0C5h ; ≈
    db 0FEh ; ˛
    db 0EBh ; Î
    db 0FEh ; ˛
    db  1Dh
    db    1
    db 0EBh ; Î
    db 0FEh ; ˛
    db  41h ; A
    db 0FEh ; ˛
    db  1Dh
    db    1
    db  41h ; A
    db 0FEh ; ˛
    db  3Bh ; ;
    db 0FFh
    db  30h ; 0
    db    1
    db 0ADh ; ≠
    db 0FEh ; ˛
    db 0C2h ; ¬
    db 0FEh ; ˛
    db  30h ; 0
    db    1
    db 0DEh ; ﬁ
    db 0FDh ; ˝
    db  9Ah ; ö
    db 0FFh
    db  43h ; C
    db    1
    db  86h ; Ü
    db 0FEh ; ˛
    db  5Bh ; [
    db 0FFh
    db  43h ; C
    db    1
    db  9Eh ; û
    db 0FDh ; ˝
    db    0
    db    0
    db  56h ; V
    db    1
    db  78h ; x
    db 0FEh ; ˛
    db    0
    db    0
    db  56h ; V
    db    1
    db  88h ; à
    db 0FDh ; ˝
    db  64h ; d
    db    0
    db  69h ; i
    db    1
    db  85h ; Ö
    db 0FEh ; ˛
    db 0A1h ; °
    db    0
    db  69h ; i
    db    1
    db  9Dh ; ù
    db 0FDh ; ˝
    db 0C3h ; √
    db    0
    db  7Ch ; |
    db    1
    db 0ACh ; ¨
    db 0FEh ; ˛
    db  3Bh ; ;
    db    1
    db  7Ch ; |
    db    1
    db 0DCh ; ‹
    db 0FDh ; ˝
    db  15h
    db    1
    db  8Fh ; è
    db    1
    db 0EBh ; Î
    db 0FEh ; ˛
    db 0BFh ; ø
    db    1
    db  8Fh ; è
    db    1
    db  41h ; A
    db 0FEh ; ˛
    db  53h ; S
    db    1
    db 0A2h ; ¢
    db    1
    db  3Bh ; ;
    db 0FFh
    db  22h ; "
    db    2
    db 0A2h ; ¢
    db    1
    db 0C2h ; ¬
    db 0FEh ; ˛
    db  7Ah ; z
    db    1
    db 0B5h ; µ
    db    1
    db  9Ah ; ö
    db 0FFh
    db  62h ; b
    db    2
    db 0B5h ; µ
    db    1
    db  5Bh ; [
    db 0FFh
    db  88h ; à
    db    1
    db 0C2h ; ¬
    db    1
    db    0
    db    0
    db  78h ; x
    db    2
    db 0C2h ; ¬
    db    1
    db    0
    db    0
    db  88h ; à
    db    1
    db 0C2h ; ¬
    db    1
    db  4Eh ; N
    db    1
    db  78h ; x
    db    2
    db 0C2h ; ¬
    db    1
    db  4Eh ; N
    db    1
    db  88h ; à
    db    1
    db 0C2h ; ¬
    db    1
    db  4Eh ; N
    db    3
    db  78h ; x
    db    2
    db 0C2h ; ¬
    db    1
    db  4Eh ; N
    db    3
    db    0
    db    0
    db 0C2h ; ¬
    db    1
    db    0
    db    0
    db 0B8h ; ∏
    db 0FFh
    db 0FFh
    db 0FFh
    db 0B2h ; ≤
    db 0FEh ; ˛
    db 0A8h ; ®
    db    0
    db 0FFh
    db 0FFh
    db 0B2h ; ≤
    db 0FEh ; ˛
    db    0
    db    0
    db 0FFh
    db 0FFh
    db 0A8h ; ®
    db    0
    db  38h ; 8
    db    1
    db 0FFh
    db 0FFh
    db 0A8h ; ®
    db    0
    db  78h ; x
    db    0
    db 0FFh
    db 0FFh
    db  4Eh ; N
    db    1
    db  68h ; h
    db    1
    db 0FFh
    db 0FFh
    db  4Eh ; N
    db    1
    db    0
    db    0
    db    0
    db    0
    db 0B4h ; ¥
    db    0
    db  48h ; H
    db    0
    db 0FFh
    db 0FFh
    db 0B2h ; ≤
    db 0FEh ; ˛
    db  58h ; X
    db 0FFh
    db 0FFh
    db 0FFh
    db 0B2h ; ≤
    db 0FEh ; ˛
    db    0
    db    0
    db 0FFh
    db 0FFh
    db 0A8h ; ®
    db    0
    db 0C8h ; »
    db 0FEh ; ˛
    db 0FFh
    db 0FFh
    db 0A8h ; ®
    db    0
    db  98h ; ò
    db 0FEh ; ˛
    db 0FFh
    db 0FFh
    db  4Eh ; N
    db    1
    db  88h ; à
    db 0FFh
    db 0FFh
    db 0FFh
    db  4Eh ; N
    db    1
    db    0
    db    0
    db    0
    db    0
    db 0B4h ; ¥
    db    0
    db  78h ; x
    db    0
    db 0FFh
    db 0FFh
    db 0B2h ; ≤
    db 0FEh ; ˛
    db  68h ; h
    db    1
    db 0FFh
    db 0FFh
    db 0B2h ; ≤
    db 0FEh ; ˛
    db  78h ; x
    db    0
    db 0FFh
    db 0FFh
    db  4Eh ; N
    db    1
    db  68h ; h
    db    1
    db 0FFh
    db 0FFh
    db  4Eh ; N
    db    1
    db  90h ; ê
    db    1
    db    0
    db    0
    db    0
    db    0
    db  90h ; ê
    db    1
    db    0
    db    0
    db 0B2h ; ≤
    db 0FEh ; ˛
    db  90h ; ê
    db    1
    db    0
    db    0
    db 0B2h ; ≤
    db 0FEh ; ˛
    db  88h ; à
    db 0FFh
    db 0FFh
    db 0FFh
    db 0B2h ; ≤
    db 0FEh ; ˛
    db  98h ; ò
    db 0FEh ; ˛
    db 0FFh
    db 0FFh
    db 0B2h ; ≤
    db 0FEh ; ˛
    db  88h ; à
    db 0FFh
    db 0FFh
    db 0FFh
    db  4Eh ; N
    db    1
    db  98h ; ò
    db 0FEh ; ˛
    db 0FFh
    db 0FFh
    db  4Eh ; N
    db    1
    db  70h ; p
    db 0FEh ; ˛
    db    0
    db    0
    db    0
    db    0
    db  70h ; p
    db 0FEh ; ˛
    db    0
    db    0
    db 0B2h ; ≤
    db 0FEh ; ˛
    db  70h ; p
    db 0FEh ; ˛
    db    0
    db    0
    db 0B2h ; ≤
    db 0FEh ; ˛
    db  88h ; à
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FBh ; ˚
    db 0FEh ; ˛
    db  17h
    db    0
    db 0FFh
    db 0FFh
    db 0FBh ; ˚
    db 0FEh ; ˛
    db 0C4h ; ƒ
    db 0FFh
    db 0FFh
    db 0FFh
    db    0
    db    0
    db  3Ch ; <
    db    0
    db 0FFh
    db 0FFh
    db    0
    db    0
    db 0E9h ; È
    db 0FFh
    db 0FFh
    db 0FFh
    db    5
    db    1
    db  78h ; x
    db    0
    db 0FFh
    db 0FFh
    db    5
    db    1
    db 0C8h ; »
    db    0
    db    0
    db    0
    db    0
    db    0
    db 0C8h ; »
    db    0
    db    0
    db    0
    db 0B2h ; ≤
    db 0FEh ; ˛
    db  38h ; 8
    db 0FFh
    db    0
    db    0
    db  4Eh ; N
    db    1
    db  8Dh ; ç
    db 0FFh
    db  58h ; X
    db    0
    db 0B2h ; ≤
    db 0FEh ; ˛
    db  8Dh ; ç
    db 0FFh
    db  97h ; ó
    db    0
    db 0B2h ; ≤
    db 0FEh ; ˛
    db  8Dh ; ç
    db 0FFh
    db  58h ; X
    db    0
    db    0
    db    0
    db  8Dh ; ç
    db 0FFh
    db  97h ; ó
    db    0
    db    0
    db    0
    db  8Dh ; ç
    db 0FFh
    db  58h ; X
    db    0
    db  4Eh ; N
    db    1
    db  8Dh ; ç
    db 0FFh
    db  97h ; ó
    db    0
    db  4Eh ; N
    db    1
    db  73h ; s
    db    0
    db 0B4h ; ¥
    db    0
    db    0
    db    0
    db 0C8h ; »
    db    0
    db    0
    db    0
    db 0B2h ; ≤
    db 0FEh ; ˛
    db  38h ; 8
    db 0FFh
    db    0
    db    0
    db  4Eh ; N
    db    1
    db    1
    db  0Bh
    db    1
    db    0
    db    0
    db  0Dh
    db    0
    db    0
    db  32h ; 2
    db  14h
    db    0
    db    0
    db    0
    db  0Fh
    db    1
    db    4
    db    8
    db    0
    db    0
    db  0Dh
    db    0
    db    1
    db  32h ; 2
    db  14h
    db    0
    db    0
    db    0
    db  0Fh
    db    1
    db    2
    db    1
    db    0
    db    0
    db    2
    db    0
unk_3D19B     db    0
    db 0BEh ; æ
    db  0Ch
    db    0
    db    0
    db    0
    db    0
    db    1
    db    4
    db    3
    db    0
    db    0
    db    2
    db    0
    db    1
    db 0BEh ; æ
    db  0Ch
    db    0
    db    0
    db    0
    db    0
    db    1
    db    1
    db    2
    db    0
    db    0
    db    2
    db    0
    db    2
    db 0BEh ; æ
    db  0Ch
    db    0
    db    0
    db    0
    db    0
    db    1
    db    3
    db    4
    db    0
    db    0
    db    2
    db    0
    db    3
    db 0BEh ; æ
    db  0Ch
    db    0
    db    0
    db    0
    db    0
    db    1
    db    2
    db    1
    db    0
    db    0
    db    2
    db    0
    db    0
    db  94h ; î
    db  0Ch
    db    0
    db    0
    db    0
    db    0
    db    1
    db    3
    db    4
    db    0
    db    0
    db    2
    db    0
    db    3
    db  94h ; î
    db  0Ch
    db    0
    db    0
    db    0
    db    0
    db    1
    db    2
    db    1
    db    0
    db    0
    db    2
    db    0
    db    0
    db 0E8h ; Ë
    db  0Ch
    db    0
    db    0
    db 0FFh
    db    0
    db    1
    db    3
    db    4
    db    0
    db    0
    db    2
    db    0
    db    3
    db 0E8h ; Ë
    db  0Ch
    db    0
    db    0
    db 0FFh
    db    0
    db    2
    db    1
    db    2
    db    0
    db    0
    db    2
    db    0
    db    2
    db  5Ah ; Z
    db  0Dh
    db    0
    db    0
    db    0
    db    0
    db    0
    db    3
    db    4
    db    0
    db    0
    db    2
    db    0
    db    3
    db  5Ah ; Z
    db  0Dh
    db    0
    db    0
    db    0
    db    0
    db    1
    db    3
    db    2
    db    0
    db    0
    db    5
    db    0
    db    3
    db 0F0h ; 
    db  0Dh
    db    0
    db    0
    db    1
    db    3
    db    1
    db    2
    db    4
    db    0
    db    0
    db    5
    db    0
    db    0
    db 0F0h ; 
    db  0Dh
    db    0
    db    0
    db    1
    db    3
    db    1
    db    1
    db    3
    db    0
    db    0
    db    5
    db    0
    db    2
    db 0F0h ; 
    db  0Dh
    db    0
    db    0
    db    1
    db    3
    db    1
    db    4
    db    1
    db    0
    db    0
    db    5
    db    0
    db    1
    db 0F0h ; 
    db  0Dh
    db    0
    db    0
    db    1
    db    3
    db    1
    db    8
    db  0Bh
    db    0
    db    0
    db  11h
    db    0
    db    3
    db  74h ; t
    db  0Eh
    db    0
    db    0
    db    1
    db    6
    db    1
    db  0Ch
    db    4
    db    0
    db    0
    db  11h
    db    0
    db    0
    db  74h ; t
    db  0Eh
    db    0
    db    0
    db    1
    db    6
    db    1
    db    1
    db    9
    db    0
    db    0
    db  11h
    db    0
    db    2
    db  74h ; t
    db  0Eh
    db    0
    db    0
    db    1
    db    6
    db    1
    db    6
    db    5
    db    0
    db    0
    db  11h
    db    0
    db    1
    db  74h ; t
    db  0Eh
    db    0
    db    0
    db    1
    db    6
    db    1
    db    1
    db    2
    db    1
    db    1
    db    2
    db    0
    db    2
    db  18h
    db  10h
    db    0
    db    0
    db    0
    db    0
    db    1
    db    3
    db    4
    db    1
    db    1
    db    2
    db    0
    db    3
    db  18h
    db  10h
    db    0
    db    0
    db    0
    db    0
    db    2
    db    1
    db    2
    db    1
    db    1
    db    2
    db    0
    db    2
    db  42h ; B
    db  10h
    db    0
    db    0
    db    0
    db    0
    db    0
    db    3
    db    4
    db    0
    db    0
    db    2
    db    0
    db    3
    db  5Ah ; Z
    db  0Dh
    db    0
    db    0
    db    0
    db    0
    db    2
    db    3
    db    4
    db    1
    db    1
    db    2
    db    0
    db    3
    db  42h ; B
    db  10h
    db    0
    db    0
    db    0
    db    0
    db    0
    db    1
    db    2
    db    0
    db    0
    db    2
    db    0
    db    2
    db  5Ah ; Z
    db  0Dh
    db    0
    db    0
    db    0
    db    0
    db    1
    db    8
    db  0Bh
    db    1
    db    1
    db  11h
    db    0
    db    3
    db  6Ch ; l
    db  10h
    db    0
    db    0
    db 0FFh
    db    6
    db    1
    db  0Ch
    db    4
    db    1
    db    1
    db  11h
    db    0
    db    0
    db  6Ch ; l
    db  10h
    db    0
    db    0
    db 0FFh
    db    6
    db    1
    db    1
    db    9
    db    1
    db    1
    db  11h
    db    0
    db    2
    db  6Ch ; l
    db  10h
    db    0
    db    0
    db 0FFh
    db    6
    db    1
    db    6
    db    5
    db    1
    db    1
    db  11h
    db    0
    db    1
    db  6Ch ; l
    db  10h
    db    0
    db    0
    db 0FFh
    db    6
    db    1
    db    1
    db    2
    db    0
    db    0
    db    2
    db    0
    db    2
    db 0E2h ; ‚
    db  12h
    db    0
    db    0
    db    0
    db    0
    db    1
    db    3
    db    4
    db    0
    db    0
    db    2
    db    0
    db    3
    db 0E2h ; ‚
    db  12h
    db    0
    db    0
    db    0
    db    0
    db    1
    db    2
    db    1
    db    0
    db    0
    db    2
    db    0
    db    0
    db 0E2h ; ‚
    db  12h
    db    0
    db    0
    db    0
    db    0
    db    1
    db    4
    db    3
    db    0
    db    0
    db    2
    db    0
    db    1
    db 0E2h ; ‚
    db  12h
    db    0
    db    0
    db    0
    db    0
    db    1
    db    1
    db    2
    db    0
    db    1
    db    2
    db    0
    db    2
    db 0E2h ; ‚
    db  12h
    db    0
    db    0
    db    0
    db  0Ah
    db    1
    db    2
    db    1
    db    0
    db    1
    db    2
    db    0
    db    0
    db 0E2h ; ‚
    db  12h
    db    0
    db    0
    db    0
    db  0Ah
    db    1
    db    3
    db    4
    db    0
    db    1
    db    2
    db    0
    db    3
    db 0E2h ; ‚
    db  12h
    db    0
    db    0
    db    0
    db  0Ah
    db    1
    db    4
    db    3
    db    0
    db    1
    db    2
    db    0
    db    1
    db 0E2h ; ‚
    db  12h
    db    0
    db    0
    db    0
    db  0Ah
    db    1
    db    1
    db    2
    db    0
    db    1
    db    2
    db    0
    db    2
    db  18h
    db  10h
    db    0
    db    0
    db    0
    db  0Ah
    db    1
    db    2
    db    1
    db    0
    db    1
    db    2
    db    0
    db    0
    db  18h
    db  10h
    db    0
    db    0
    db    0
    db  0Ah
    db    1
    db    3
    db    4
    db    0
    db    1
    db    2
    db    0
    db    3
    db  18h
    db  10h
    db    0
    db    0
    db    0
    db  0Ah
    db    1
    db    4
    db    3
    db    0
    db    1
    db    2
    db    0
    db    1
    db  18h
    db  10h
    db    0
    db    0
    db    0
    db  0Ah
    db    1
    db    1
    db    2
    db    0
    db    2
    db    2
    db    0
    db    2
    db  34h ; 4
    db  15h
    db    0
    db    0
    db    0
    db    0
    db    1
    db    3
    db    4
    db    0
    db    3
    db    2
    db    0
    db    3
    db  34h ; 4
    db  15h
    db    0
    db    0
    db    0
    db    0
    db    1
    db    2
    db    1
    db    0
    db    3
    db    2
    db    0
    db    0
    db  34h ; 4
    db  15h
    db    0
    db    0
    db    0
    db    0
    db    1
    db    4
    db    3
    db    0
    db    2
    db    2
    db    0
    db    1
    db  34h ; 4
    db  15h
    db    0
    db    0
    db    0
    db    0
    db    1
    db    1
    db    2
    db    0
    db    3
    db    2
    db    0
    db    2
    db  0Ah
    db  15h
    db    0
    db    0
    db    0
    db    0
    db    1
    db    3
    db    4
    db    0
    db    2
    db    2
    db    0
    db    3
    db  0Ah
    db  15h
    db    0
    db    0
    db    0
    db    0
    db    1
    db    2
    db    1
    db    0
    db    2
    db    2
    db    0
    db    0
    db  0Ah
    db  15h
    db    0
    db    0
    db    0
    db    0
    db    1
    db    4
    db    3
    db    0
    db    3
    db    2
    db    0
    db    1
    db  0Ah
    db  15h
    db    0
    db    0
    db    0
    db    0
    db    1
    db    2
    db    1
    db    2
    db    2
    db    2
    db    0
    db    0
    db 0E0h ; ‡
    db  14h
    db    0
    db    0
    db    0
    db    0
    db    1
    db    4
    db    3
    db    3
    db    3
    db    2
    db    0
    db    1
    db 0E0h ; ‡
    db  14h
    db    0
    db    0
    db    0
    db    0
    db    1
    db    1
    db    2
    db    3
    db    3
    db    2
    db    0
    db    2
    db 0E0h ; ‡
    db  14h
    db    0
    db    0
    db    0
    db    0
    db    1
    db    3
    db    4
    db    2
    db    2
    db    2
    db    0
    db    3
    db 0E0h ; ‡
    db  14h
    db    0
    db    0
    db    0
    db    0
    db    1
    db    8
    db  0Bh
    db    2
    db    3
    db  11h
    db    0
    db    3
    db  5Eh ; ^
    db  15h
    db    0
    db    0
    db 0FFh
    db    9
    db    1
    db  0Ch
    db    4
    db    2
    db    2
    db  11h
    db    0
    db    0
    db  5Eh ; ^
    db  15h
    db    0
    db    0
    db 0FFh
    db    9
    db    1
    db    1
    db    9
    db    3
    db    3
    db  11h
    db    0
    db    2
    db  5Eh ; ^
    db  15h
    db    0
    db    0
    db 0FFh
    db    9
    db    1
    db    6
    db    5
    db    3
    db    2
    db  11h
    db    0
    db    1
    db  5Eh ; ^
    db  15h
    db    0
    db    0
    db 0FFh
    db    9
    db    1
    db  0Bh
    db    1
    db    0
    db    0
    db  17h
    db    0
    db    0
    db  0Ch
    db  13h
    db    0
    db    0
    db    0
    db  0Eh
    db    1
    db    4
    db    8
    db    0
    db    0
    db  17h
    db    0
    db    1
    db  0Ch
    db  13h
    db    0
    db    0
    db    0
    db  0Eh
    db    1
    db    2
    db    1
    db    4
    db    4
    db    2
    db    0
    db    0
    db  30h ; 0
    db  0Dh
    db    0
    db    0
    db    0
    db  0Fh
    db    1
    db    3
    db    4
    db    4
    db    4
    db    2
    db    0
    db    3
    db  30h ; 0
    db  0Dh
    db    0
    db    0
    db    0
    db  0Fh
    db    1
    db    2
    db    1
    db    4
    db    4
    db    3
    db    0
    db    0
    db 0D2h ; “
    db  19h
    db    0
    db    0
    db    0
    db  0Fh
    db    1
    db    3
    db    4
    db    4
    db    4
    db    3
    db    0
    db    3
    db 0D2h ; “
    db  19h
    db    0
    db    0
    db    0
    db  0Fh
    db    1
    db    1
    db    2
    db    0
    db    4
    db    2
    db    0
    db    2
    db    6
    db  0Dh
    db    0
    db    0
    db    0
    db  0Fh
    db    1
    db    2
    db    1
    db    0
    db    4
    db    2
    db    0
    db    0
    db    6
    db  0Dh
    db    0
    db    0
    db    0
    db  0Fh
    db    1
    db    3
    db    4
    db    0
    db    4
    db    2
    db    0
    db    3
    db    6
    db  0Dh
    db    0
    db    0
    db    0
    db  0Fh
    db    1
    db    4
    db    3
    db    0
    db    4
    db    2
    db    0
    db    1
    db    6
    db  0Dh
    db    0
    db    0
    db    0
    db  0Fh
    db    1
    db  0Bh
    db    5
    db    0
    db    0
    db  11h
    db    0
    db    0
    db  10h
    db  12h
    db    0
    db    0
    db 0FFh
    db  0Dh
    db    1
    db    6
    db    8
    db    0
    db    0
    db  11h
    db    0
    db    1
    db  3Eh ; >
    db  11h
    db    0
    db    0
    db 0FFh
    db  0Dh
    db    1
    db  0Ch
    db    1
    db    0
    db    0
    db  11h
    db    0
    db    0
    db  3Eh ; >
    db  11h
    db    0
    db    0
    db 0FFh
    db  0Dh
    db    1
    db    4
    db    9
    db    0
    db    0
    db  11h
    db    0
    db    1
    db  10h
    db  12h
    db    0
    db    0
    db 0FFh
    db  0Dh
    db    2
    db    2
    db    1
    db    0
    db    0
    db    2
    db    0
    db    0
    db  94h ; î
    db  0Ch
    db    0
    db    0
    db    0
    db    0
    db    0
    db    2
    db    4
    db    0
    db    0
    db    5
    db    0
    db    0
    db 0F0h ; 
    db  0Dh
    db    0
    db    0
    db    1
    db    3
    db    2
    db    3
    db    4
    db    0
    db    0
    db    2
    db    0
    db    3
    db  94h ; î
    db  0Ch
    db    0
    db    0
    db    0
    db    0
    db    0
    db    3
    db    2
    db    0
    db    0
    db    5
    db    0
    db    3
    db 0F0h ; 
    db  0Dh
    db    0
    db    0
    db    1
    db    3
    db    2
    db    1
    db    2
    db    0
    db    0
    db    2
    db    0
    db    2
    db  94h ; î
    db  0Ch
    db    0
    db    0
    db    0
    db    0
    db    0
    db    1
    db    3
    db    0
    db    0
    db    5
    db    0
    db    2
    db 0F0h ; 
    db  0Dh
    db    0
    db    0
    db    1
    db    3
    db    2
    db    4
    db    3
    db    0
    db    0
    db    2
    db    0
    db    1
    db  94h ; î
    db  0Ch
    db    0
    db    0
    db    0
    db    0
    db    0
    db    4
    db    1
    db    0
    db    0
    db    5
    db    0
    db    1
    db 0F0h ; 
    db  0Dh
    db    0
    db    0
    db    1
    db    3
    db    2
    db    2
    db    1
    db    0
    db    0
    db    2
    db    0
    db    0
    db  94h ; î
    db  0Ch
    db    0
    db    0
    db    0
    db    0
    db    0
    db    2
    db    3
    db    0
    db    0
    db    5
    db    0
    db    0
    db  32h ; 2
    db  0Eh
    db    0
    db    0
    db    2
    db    3
    db    2
    db    3
    db    4
    db    0
    db    0
    db    2
    db    0
    db    3
    db  94h ; î
    db  0Ch
    db    0
    db    0
    db    0
    db    0
    db    0
    db    3
    db    1
    db    0
    db    0
    db    5
    db    0
    db    3
    db  32h ; 2
    db  0Eh
    db    0
    db    0
    db    2
    db    3
    db    2
    db    1
    db    2
    db    0
    db    0
    db    2
    db    0
    db    2
    db  94h ; î
    db  0Ch
    db    0
    db    0
    db    0
    db    0
    db    0
    db    1
    db    4
    db    0
    db    0
    db    5
    db    0
    db    2
    db  32h ; 2
    db  0Eh
    db    0
    db    0
    db    2
    db    3
    db    2
    db    4
    db    3
    db    0
    db    0
    db    2
    db    0
    db    1
    db  94h ; î
    db  0Ch
    db    0
    db    0
    db    0
    db    0
    db    0
    db    4
    db    2
    db    0
    db    0
    db    5
    db    0
    db    1
    db  32h ; 2
    db  0Eh
    db    0
    db    0
    db    2
    db    3
    db    2
    db  0Ch
    db    5
    db    0
    db    0
    db    3
    db    0
    db    0
    db  84h ; Ñ
    db  0Dh
    db    0
    db    0
    db    0
    db    0
    db    0
    db  0Ch
    db    4
    db    0
    db    0
    db  11h
    db    0
    db    0
    db  74h ; t
    db  0Eh
    db    0
    db    0
    db    1
    db    6
    db    2
    db    8
    db    4
    db    0
    db    0
    db    3
    db    0
    db    3
    db  84h ; Ñ
    db  0Dh
    db    0
    db    0
    db    0
    db    0
    db    0
    db    8
    db  0Bh
    db    0
    db    0
    db  11h
    db    0
    db    3
    db  74h ; t
    db  0Eh
    db    0
    db    0
    db    1
    db    6
    db    2
    db    1
    db  0Bh
    db    0
    db    0
    db    3
    db    0
    db    2
    db  84h ; Ñ
    db  0Dh
    db    0
    db    0
    db    0
    db    0
    db    0
    db    1
    db    9
    db    0
    db    0
    db  11h
    db    0
    db    2
    db  74h ; t
    db  0Eh
    db    0
    db    0
    db    1
    db    6
    db    2
    db    6
    db    9
    db    0
    db    0
    db    3
    db    0
    db    1
    db  84h ; Ñ
    db  0Dh
    db    0
    db    0
    db    0
    db    0
    db    0
    db    6
    db    5
    db    0
    db    0
    db  11h
    db    0
    db    1
    db  74h ; t
    db  0Eh
    db    0
    db    0
    db    1
    db    6
    db    2
    db  0Bh
    db    1
    db    0
    db    0
    db    3
    db    0
    db    0
    db 0BAh ; ∫
    db  0Dh
    db    0
    db    0
    db    0
    db    0
    db    0
    db  0Bh
    db    8
    db    0
    db    0
    db  11h
    db    0
    db    0
    db  46h ; F
    db  0Fh
    db    0
    db    0
    db    2
    db    6
    db    2
    db    9
    db    6
    db    0
    db    0
    db    3
    db    0
    db    3
    db 0BAh ; ∫
    db  0Dh
    db    0
    db    0
    db    0
    db    0
    db    0
    db    9
    db    1
    db    0
    db    0
    db  11h
    db    0
    db    3
    db  46h ; F
    db  0Fh
    db    0
    db    0
    db    2
    db    6
    db    2
    db    5
    db  0Ch
    db    0
    db    0
    db    3
    db    0
    db    2
    db 0BAh ; ∫
    db  0Dh
    db    0
    db    0
    db    0
    db    0
    db    0
    db    5
    db    6
    db    0
    db    0
    db  11h
    db    0
    db    2
    db  46h ; F
    db  0Fh
    db    0
    db    0
    db    2
    db    6
    db    2
    db    4
    db    8
    db    0
    db    0
    db    3
    db    0
    db    1
    db 0BAh ; ∫
    db  0Dh
    db    0
    db    0
    db    0
    db    0
    db    0
    db    4
    db  0Ch
    db    0
    db    0
    db  11h
    db    0
    db    1
    db  46h ; F
    db  0Fh
    db    0
    db    0
    db    2
    db    6
    db    1
    db    2
    db    1
    db    5
    db    5
    db    2
    db    0
    db    0
    db  48h ; H
    db  19h
    db  72h ; r
    db  19h
    db    0
    db    0
    db    1
    db    3
    db    4
    db    5
    db    5
    db    2
    db    0
    db    3
    db  48h ; H
    db  19h
    db  72h ; r
    db  19h
    db    0
    db    0
    db    1
    db    1
    db    2
    db    0
    db    5
    db    3
    db    0
    db    2
    db 0F4h ; Ù
    db  18h
    db  1Eh
    db  19h
    db    0
    db  0Dh
    db    1
    db    2
    db    1
    db    0
    db    5
    db    3
    db    0
    db    0
    db 0F4h ; Ù
    db  18h
    db  1Eh
    db  19h
    db    0
    db  0Dh
    db    1
    db    3
    db    4
    db    0
    db    5
    db    3
    db    0
    db    3
    db 0F4h ; Ù
    db  18h
    db  1Eh
    db  19h
    db    0
    db  0Dh
    db    1
    db    4
    db    3
    db    0
    db    5
    db    3
    db    0
    db    1
    db 0F4h ; Ù
    db  18h
    db  1Eh
    db  19h
    db    0
    db  0Dh
    db    1
    db    2
    db    1
    db    0
    db    0
    db    3
    db    0
    db    0
    db  9Ch ; ú
    db  19h
    db    0
    db    0
    db    0
    db  0Bh
    db    1
    db    3
    db    4
    db    0
    db    0
    db    3
    db    0
    db    3
    db  9Ch ; ú
    db  19h
    db    0
    db    0
    db    0
    db  0Bh
    db    1
    db  0Bh
    db    1
    db    0
    db    1
    db  1Dh
    db    0
    db    0
    db  30h ; 0
    db  16h
    db    0
    db    0
    db    0
    db  0Ch
    db    1
    db    9
    db    6
    db    0
    db    1
    db  1Dh
    db    0
    db    3
    db  30h ; 0
    db  16h
    db    0
    db    0
    db    0
    db  0Ch
    db    1
    db    5
    db  0Ch
    db    0
    db    1
    db  1Dh
    db    0
    db    2
    db  30h ; 0
    db  16h
    db    0
    db    0
    db    0
    db  0Ch
    db    1
    db    4
    db    8
    db    0
    db    1
    db  1Dh
    db    0
    db    1
    db  30h ; 0
    db  16h
    db    0
    db    0
    db    0
    db  0Ch
    db    1
    db  0Ch
    db    5
    db    0
    db    1
    db  1Dh
    db    0
    db    0
    db  92h ; í
    db  17h
    db    0
    db    0
    db    0
    db  0Ch
    db    1
    db    8
    db    4
    db    0
    db    1
    db  1Dh
    db    0
    db    3
    db  92h ; í
    db  17h
    db    0
    db    0
    db    0
    db  0Ch
    db    1
    db    1
    db  0Bh
    db    0
    db    1
    db  1Dh
    db    0
    db    2
    db  92h ; í
    db  17h
    db    0
    db    0
    db    0
    db  0Ch
    db    1
    db    6
    db    9
    db    0
    db    1
    db  1Dh
    db    0
    db    1
    db  92h ; í
    db  17h
    db    0
    db    0
    db    0
    db  0Ch
    db    0
    db    0
    db    0
    db    0
    db 0E0h ; ‡
    db  78h ; x
    db 0E0h ; ‡
    db  78h ; x
    db    0
    db    0
    db    0
    db    0
    db  40h ; @
    db    0
    db  24h ; $
    db  1Ah
    db    0
    db    0
    db  54h ; T
    db  77h ; w
    db  6Ah ; j
    db  77h ; w
    db    4
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db  20h
    db  81h ; Å
    db  4Ch ; L
    db  81h ; Å
    db    0
    db    0
    db    0
    db    0
    db  40h ; @
    db    0
    db    0
    db    0
    db    0
    db    0
    db  36h ; 6
    db  81h ; Å
    db  62h ; b
    db  81h ; Å
    db    0
    db    0
    db    0
    db    0
    db  40h ; @
    db    0
    db  5Ch ; \
    db  1Ah
    db    0
    db    0
    db 0E0h ; ‡
    db  78h ; x
    db 0F6h ; ˆ
    db  78h ; x
    db    0
    db    0
    db    1
    db    0
    db    1
    db    0
    db  6Ah ; j
    db  1Ah
    db    0
    db    1
    db 0E0h ; ‡
    db  78h ; x
    db 0F6h ; ˆ
    db  78h ; x
    db    0
    db    0
    db    1
    db    0
    db    1
    db    0
    db 0B0h ; ∞
    db  1Ah
    db    0
    db    3
    db  90h ; ê
    db  79h ; y
    db 0A6h ; ¶
    db  79h ; y
    db    0
    db    0
    db    1
    db    0
    db    2
    db    0
    db 0BEh ; æ
    db  1Ah
    db    0
    db    0
    db  90h ; ê
    db  79h ; y
    db 0A6h ; ¶
    db  79h ; y
    db    0
    db    0
    db    1
    db    0
    db    2
    db    0
    db 0CCh ; Ã
    db  1Ah
    db    0
    db    2
    db  90h ; ê
    db  79h ; y
    db 0A6h ; ¶
    db  79h ; y
    db    0
    db    0
    db    1
    db    0
    db    2
    db    0
    db 0DAh ; ⁄
    db  1Ah
    db    0
    db    1
    db  90h ; ê
    db  79h ; y
    db 0A6h ; ¶
    db  79h ; y
    db    0
    db    0
    db    1
    db    0
    db    2
    db    0
    db 0E8h ; Ë
    db  1Ah
    db    0
    db    3
    db  0Ch
    db  79h ; y
    db  22h ; "
    db  79h ; y
    db    0
    db    0
    db    1
    db    3
    db    3
    db    0
    db 0F6h ; ˆ
    db  1Ah
    db    0
    db    0
    db  0Ch
    db  79h ; y
    db  22h ; "
    db  79h ; y
    db    0
    db    0
    db    1
    db    3
    db    3
    db    0
    db    4
    db  1Bh
    db    0
    db    2
    db  0Ch
    db  79h ; y
    db  22h ; "
    db  79h ; y
    db    0
    db    0
    db    1
    db    3
    db    3
    db    0
    db  12h
    db  1Bh
    db    0
    db    1
    db  0Ch
    db  79h ; y
    db  22h ; "
    db  79h ; y
    db    0
    db    0
    db    1
    db    3
    db    3
    db    0
    db  5Ch ; \
    db  1Ah
    db    0
    db    0
    db 0E0h ; ‡
    db  78h ; x
    db 0F6h ; ˆ
    db  78h ; x
    db    0
    db    1
    db    1
    db    0
    db    1
    db    0
    db  6Ah ; j
    db  1Ah
    db    0
    db    1
    db 0E0h ; ‡
    db  78h ; x
    db 0F6h ; ˆ
    db  78h ; x
    db    0
    db    1
    db    1
    db    0
    db    1
    db    0
    db 0B0h ; ∞
    db  1Ah
    db    0
    db    3
    db  90h ; ê
    db  79h ; y
    db 0A6h ; ¶
    db  79h ; y
    db    0
    db    1
    db    1
    db    0
    db    2
    db    0
    db 0BEh ; æ
    db  1Ah
    db    0
    db    0
    db  90h ; ê
    db  79h ; y
    db 0A6h ; ¶
    db  79h ; y
    db    0
    db    1
    db    1
    db    0
    db    2
    db    0
    db 0CCh ; Ã
    db  1Ah
    db    0
    db    2
    db  90h ; ê
    db  79h ; y
    db 0A6h ; ¶
    db  79h ; y
    db    0
    db    1
    db    1
    db    0
    db    2
    db    0
    db 0DAh ; ⁄
    db  1Ah
    db    0
    db    1
    db  90h ; ê
    db  79h ; y
    db 0A6h ; ¶
    db  79h ; y
    db    0
    db    1
    db    1
    db    0
    db    2
    db    0
    db 0E8h ; Ë
    db  1Ah
    db    0
    db    3
    db  0Ch
    db  79h ; y
    db  22h ; "
    db  79h ; y
    db    0
    db    1
    db    1
    db    3
    db    3
    db    0
    db 0F6h ; ˆ
    db  1Ah
    db    0
    db    0
    db  0Ch
    db  79h ; y
    db  22h ; "
    db  79h ; y
    db    0
    db    1
    db    1
    db    3
    db    3
    db    0
    db    4
    db  1Bh
    db    0
    db    2
    db  0Ch
    db  79h ; y
    db  22h ; "
    db  79h ; y
    db    0
    db    1
    db    1
    db    3
    db    3
    db    0
    db  12h
    db  1Bh
    db    0
    db    1
    db  0Ch
    db  79h ; y
    db  22h ; "
    db  79h ; y
    db    0
    db    1
    db    1
    db    3
    db    3
    db    0
    db  5Ch ; \
    db  1Ah
    db    0
    db    0
    db 0E0h ; ‡
    db  78h ; x
    db 0F6h ; ˆ
    db  78h ; x
    db    0
    db    2
    db    1
    db    0
    db    1
    db    0
    db  6Ah ; j
    db  1Ah
    db    0
    db    1
    db 0E0h ; ‡
    db  78h ; x
    db 0F6h ; ˆ
    db  78h ; x
    db    0
    db    2
    db    1
    db    0
    db    1
    db    0
    db 0B0h ; ∞
    db  1Ah
    db    0
    db    3
    db  90h ; ê
    db  79h ; y
    db 0A6h ; ¶
    db  79h ; y
    db    0
    db    2
    db    1
    db    0
    db    2
    db    0
    db 0BEh ; æ
    db  1Ah
    db    0
    db    0
    db  90h ; ê
    db  79h ; y
    db 0A6h ; ¶
    db  79h ; y
    db    0
    db    2
    db    1
    db    0
    db    2
    db    0
    db 0CCh ; Ã
    db  1Ah
    db    0
    db    2
    db  90h ; ê
    db  79h ; y
    db 0A6h ; ¶
    db  79h ; y
    db    0
    db    2
    db    1
    db    0
    db    2
    db    0
    db 0DAh ; ⁄
    db  1Ah
    db    0
    db    1
    db  90h ; ê
    db  79h ; y
    db 0A6h ; ¶
    db  79h ; y
    db    0
    db    2
    db    1
    db    0
    db    2
    db    0
    db 0E8h ; Ë
    db  1Ah
    db    0
    db    3
    db  0Ch
    db  79h ; y
    db  22h ; "
    db  79h ; y
    db    0
    db    2
    db    1
    db    3
    db    3
    db    0
    db 0F6h ; ˆ
    db  1Ah
    db    0
    db    0
    db  0Ch
    db  79h ; y
    db  22h ; "
    db  79h ; y
    db    0
    db    2
    db    1
    db    3
    db    3
    db    0
    db    4
    db  1Bh
    db    0
    db    2
    db  0Ch
    db  79h ; y
    db  22h ; "
    db  79h ; y
    db    0
    db    2
    db    1
    db    3
    db    3
    db    0
    db  12h
    db  1Bh
    db    0
    db    1
    db  0Ch
    db  79h ; y
    db  22h ; "
    db  79h ; y
    db    0
    db    2
    db    1
    db    3
    db    3
    db    0
    db  20h
    db  1Bh
    db    0
    db    0
    db  28h ; (
    db  77h ; w
    db  3Eh ; >
    db  77h ; w
    db    0
    db    0
    db    0
    db    0
    db  12h
    db    0
    db  2Eh ; .
    db  1Bh
    db    0
    db    3
    db  28h ; (
    db  77h ; w
    db  3Eh ; >
    db  77h ; w
    db    0
    db    0
    db    0
    db    0
    db  12h
    db    0
    db  0Eh
    db  1Ch
    db    0
    db    1
    db  5Ch ; \
    db  78h ; x
    db  72h ; r
    db  78h ; x
    db    0
    db    0
    db    0
    db    0
    db  10h
    db    0
    db    0
    db  1Ch
    db    0
    db    3
    db  5Ch ; \
    db  78h ; x
    db  72h ; r
    db  78h ; x
    db    0
    db    0
    db    0
    db    0
    db  10h
    db    0
    db 0F2h ; Ú
    db  1Bh
    db    0
    db    0
    db  5Ch ; \
    db  78h ; x
    db  72h ; r
    db  78h ; x
    db    0
    db    0
    db    0
    db    0
    db  10h
    db    0
    db 0E4h ; ‰
    db  1Bh
    db    0
    db    2
    db  5Ch ; \
    db  78h ; x
    db  72h ; r
    db  78h ; x
    db    0
    db    0
    db    0
    db    0
    db  10h
    db    0
    db  70h ; p
    db  1Ch
    db    0
    db    0
    db  88h ; à
    db  78h ; x
    db  9Eh ; û
    db  78h ; x
    db    0
    db    0
    db    0
    db    0
    db  18h
    db    0
    db  7Eh ; ~
    db  1Ch
    db    0
    db    1
    db  88h ; à
    db  78h ; x
    db  9Eh ; û
    db  78h ; x
    db    0
    db    0
    db    0
    db    0
    db  18h
    db    0
    db  54h ; T
    db  1Ch
    db    0
    db    2
    db  88h ; à
    db  78h ; x
    db  9Eh ; û
    db  78h ; x
    db    0
    db    0
    db    0
    db    0
    db  18h
    db    0
    db  62h ; b
    db  1Ch
    db    0
    db    3
    db  88h ; à
    db  78h ; x
    db  9Eh ; û
    db  78h ; x
    db    0
    db    0
    db    0
    db    0
    db  18h
    db    0
    db 0A8h ; ®
    db  1Ch
    db    0
    db    0
    db 0ACh ; ¨
    db  77h ; w
    db 0C2h ; ¬
    db  77h ; w
    db    0
    db    0
    db    0
    db    0
    db  17h
    db    0
    db 0B6h ; ∂
    db  1Ch
    db    0
    db    1
    db 0ACh ; ¨
    db  77h ; w
    db 0C2h ; ¬
    db  77h ; w
    db    0
    db    0
    db    0
    db    0
    db  17h
    db    0
    db  8Ch ; å
    db  1Ch
    db    0
    db    2
    db 0ACh ; ¨
    db  77h ; w
    db 0C2h ; ¬
    db  77h ; w
    db    0
    db    0
    db    0
    db    0
    db  17h
    db    0
    db  9Ah ; ö
    db  1Ch
    db    0
    db    3
    db 0ACh ; ¨
    db  77h ; w
    db 0C2h ; ¬
    db  77h ; w
    db    0
    db    0
    db    0
    db    0
    db  17h
    db    0
    db 0E0h ; ‡
    db  1Ch
    db    0
    db    2
    db  74h ; t
    db  7Bh ; {
    db  8Ah ; ä
    db  7Bh ; {
    db    0
    db    0
    db    0
    db    0
    db  19h
    db    0
    db 0C4h ; ƒ
    db  1Ch
    db    0
    db    0
    db  74h ; t
    db  7Bh ; {
    db  8Ah ; ä
    db  7Bh ; {
    db    0
    db    0
    db    0
    db    0
    db  19h
    db    0
    db 0EEh ; Ó
    db  1Ch
    db    0
    db    3
    db  74h ; t
    db  7Bh ; {
    db  8Ah ; ä
    db  7Bh ; {
    db    0
    db    0
    db    0
    db    0
    db  19h
    db    0
    db 0D2h ; “
    db  1Ch
    db    0
    db    1
    db  74h ; t
    db  7Bh ; {
    db  8Ah ; ä
    db  7Bh ; {
    db    0
    db    0
    db    0
    db    0
    db  19h
    db    0
    db 0FCh ; ¸
    db  1Ch
    db    0
    db    3
    db 0A4h ; §
    db  76h ; v
    db 0BAh ; ∫
    db  76h ; v
    db    0
    db    0
    db    0
    db    3
    db  1Ah
    db    0
    db  0Ah
    db  1Dh
    db    0
    db    0
    db 0A4h ; §
    db  76h ; v
    db 0BAh ; ∫
    db  76h ; v
    db    0
    db    0
    db    0
    db    3
    db  1Ah
    db    0
    db  18h
    db  1Dh
    db    0
    db    2
    db 0A4h ; §
    db  76h ; v
    db 0BAh ; ∫
    db  76h ; v
    db    0
    db    0
    db    0
    db    3
    db  1Ah
    db    0
    db  26h ; &
    db  1Dh
    db    0
    db    1
    db 0A4h ; §
    db  76h ; v
    db 0BAh ; ∫
    db  76h ; v
    db    0
    db    0
    db    0
    db    3
    db  1Ah
    db    0
    db  0Eh
    db  1Ch
    db    0
    db    1
    db  78h ; x
    db  76h ; v
    db  8Eh ; é
    db  76h ; v
    db    0
    db    0
    db    0
    db    0
    db  10h
    db    0
    db    0
    db  1Ch
    db    0
    db    3
    db  78h ; x
    db  76h ; v
    db  8Eh ; é
    db  76h ; v
    db    0
    db    0
    db    0
    db    0
    db  10h
    db    0
    db 0F2h ; Ú
    db  1Bh
    db    0
    db    0
    db  78h ; x
    db  76h ; v
    db  8Eh ; é
    db  76h ; v
    db    0
    db    0
    db    0
    db    0
    db  10h
    db    0
    db 0E4h ; ‰
    db  1Bh
    db    0
    db    2
    db  78h ; x
    db  76h ; v
    db  8Eh ; é
    db  76h ; v
    db    0
    db    0
    db    0
    db    0
    db  10h
    db    0
    db 0C0h ; ¿
    db  1Dh
    db    0
    db    0
    db 0FCh ; ¸
    db  76h ; v
    db  12h
    db  77h ; w
    db    0
    db    0
    db    1
    db    3
    db    5
    db    0
    db 0CEh ; Œ
    db  1Dh
    db    0
    db    1
    db 0D0h ; –
    db  76h ; v
    db 0E6h ; Ê
    db  76h ; v
    db    0
    db    0
    db    1
    db    3
    db    4
    db    0
    db 0DCh ; ‹
    db  1Dh
    db    0
    db    0
    db 0D0h ; –
    db  76h ; v
    db 0E6h ; Ê
    db  76h ; v
    db    0
    db    0
    db    1
    db    3
    db    4
    db    0
    db 0EAh ; Í
    db  1Dh
    db    0
    db    1
    db 0FCh ; ¸
    db  76h ; v
    db  12h
    db  77h ; w
    db    0
    db    0
    db    1
    db    3
    db    5
    db    0
    db  34h ; 4
    db  1Dh
    db    0
    db    0
    db 0D8h ; ÿ
    db  77h ; w
    db 0EEh ; Ó
    db  77h ; w
    db 0CEh ; Œ
    db    0
    db    0
    db    1
    db  1Bh
    db    0
    db  42h ; B
    db  1Dh
    db    0
    db    1
    db 0D8h ; ÿ
    db  77h ; w
    db 0EEh ; Ó
    db  77h ; w
    db 0CFh ; œ
    db    0
    db    0
    db    2
    db  1Bh
    db    0
    db  78h ; x
    db  1Ah
    db    0
    db    0
    db  64h ; d
    db  79h ; y
    db  7Ah ; z
    db  79h ; y
    db 0D0h ; –
    db    0
    db    0
    db    0
    db  1Ch
    db    0
    db  86h ; Ü
    db  1Ah
    db    0
    db    3
    db  64h ; d
    db  79h ; y
    db  7Ah ; z
    db  79h ; y
    db 0D1h ; —
    db    0
    db    0
    db    0
    db  1Ch
    db    0
    db  50h ; P
    db  1Dh
    db    0
    db    0
    db  30h ; 0
    db  78h ; x
    db  46h ; F
    db  78h ; x
    db 0D2h ; “
    db    0
    db    0
    db    0
    db  1Eh
    db    0
    db  5Eh ; ^
    db  1Dh
    db    0
    db    1
    db  30h ; 0
    db  78h ; x
    db  46h ; F
    db  78h ; x
    db 0D3h ; ”
    db    0
    db    0
    db    0
    db  1Eh
    db    0
    db  96h ; ñ
    db  1Dh
    db    0
    db    0
    db 0A8h ; ®
    db  7Ch ; |
    db 0BEh ; æ
    db  7Ch ; |
    db    0
    db    0
    db    0
    db    0
    db  1Dh
    db    0
    db  88h ; à
    db  1Dh
    db    0
    db    2
    db 0A8h ; ®
    db  7Ch ; |
    db 0BEh ; æ
    db  7Ch ; |
    db    0
    db    0
    db    0
    db    0
    db  1Dh
    db    0
    db 0A4h ; §
    db  1Dh
    db    0
    db    3
    db 0A8h ; ®
    db  7Ch ; |
    db 0BEh ; æ
    db  7Ch ; |
    db    0
    db    0
    db    0
    db    0
    db  1Dh
    db    0
    db 0B2h ; ≤
    db  1Dh
    db    0
    db    1
    db 0A8h ; ®
    db  7Ch ; |
    db 0BEh ; æ
    db  7Ch ; |
    db    0
    db    0
    db    0
    db    0
    db  1Dh
    db    0
    db  94h ; î
    db  1Ah
    db    0
    db    0
    db 0C4h ; ƒ
    db  7Ah ; z
    db 0DAh ; ⁄
    db  7Ah ; z
    db    0
    db    0
    db    0
    db    0
    db  0Ch
    db    0
    db 0F8h ; ¯
    db  1Dh
    db    0
    db    0
    db 0F0h ; 
    db  7Ah ; z
    db    6
    db  7Bh ; {
    db    0
    db    0
    db    0
    db    0
    db    6
    db    0
    db  14h
    db  1Eh
    db    0
    db    3
    db 0F0h ; 
    db  7Ah ; z
    db    6
    db  7Bh ; {
    db    0
    db    0
    db    0
    db    0
    db    6
    db    0
    db  30h ; 0
    db  1Eh
    db    0
    db    2
    db 0F0h ; 
    db  7Ah ; z
    db    6
    db  7Bh ; {
    db    0
    db    0
    db    0
    db    0
    db    6
    db    0
    db  4Ch ; L
    db  1Eh
    db    0
    db    1
    db 0F0h ; 
    db  7Ah ; z
    db    6
    db  7Bh ; {
    db    0
    db    0
    db    0
    db    0
    db    6
    db    0
    db  68h ; h
    db  1Eh
    db    0
    db    0
    db  1Ch
    db  7Bh ; {
    db  32h ; 2
    db  7Bh ; {
    db    0
    db    0
    db    0
    db    0
    db    7
    db    0
    db  84h ; Ñ
    db  1Eh
    db    0
    db    3
    db  1Ch
    db  7Bh ; {
    db  32h ; 2
    db  7Bh ; {
    db    0
    db    0
    db    0
    db    0
    db    7
    db    0
    db 0A0h ; †
    db  1Eh
    db    0
    db    2
    db  1Ch
    db  7Bh ; {
    db  32h ; 2
    db  7Bh ; {
    db    0
    db    0
    db    0
    db    0
    db    7
    db    0
    db 0BCh ; º
    db  1Eh
    db    0
    db    1
    db  1Ch
    db  7Bh ; {
    db  32h ; 2
    db  7Bh ; {
    db    0
    db    0
    db    0
    db    0
    db    7
    db    0
    db  6Ch ; l
    db  1Dh
    db    0
    db    0
    db 0E4h ; ‰
    db  7Eh ; ~
    db 0FAh ; ˙
    db  7Eh ; ~
    db 0D2h ; “
    db    0
    db    0
    db    0
    db  1Fh
    db    0
    db  7Ah ; z
    db  1Dh
    db    0
    db    1
    db 0E4h ; ‰
    db  7Eh ; ~
    db 0FAh ; ˙
    db  7Eh ; ~
    db 0D3h ; ”
    db    0
    db    0
    db    0
    db  1Fh
    db    0
    db    8
    db  1Ah
    db    0
    db    0
    db  10h
    db  7Fh ; 
    db  26h ; &
    db  7Fh ; 
    db    0
    db    0
    db    0
    db    1
    db  23h ; #
    db    0
    db  16h
    db  1Ah
    db    0
    db    1
    db  10h
    db  7Fh ; 
    db  26h ; &
    db  7Fh ; 
    db    0
    db    0
    db    0
    db    2
    db  23h ; #
    db    0
    db 0D8h ; ÿ
    db  1Eh
    db    0
    db    0
    db 0A0h ; †
    db  7Bh ; {
    db 0B6h ; ∂
    db  7Bh ; {
    db    0
    db    0
    db    1
    db    3
    db    8
    db    0
    db 0F4h ; Ù
    db  1Eh
    db    0
    db    3
    db 0A0h ; †
    db  7Bh ; {
    db 0B6h ; ∂
    db  7Bh ; {
    db    0
    db    0
    db    1
    db    3
    db    8
    db    0
    db  10h
    db  1Fh
    db    0
    db    2
    db 0A0h ; †
    db  7Bh ; {
    db 0B6h ; ∂
    db  7Bh ; {
    db    0
    db    0
    db    1
    db    3
    db    8
    db    0
    db  2Ch ; ,
    db  1Fh
    db    0
    db    1
    db 0A0h ; †
    db  7Bh ; {
    db 0B6h ; ∂
    db  7Bh ; {
    db    0
    db    0
    db    1
    db    3
    db    8
    db    0
    db  48h ; H
    db  1Fh
    db    0
    db    0
    db 0CCh ; Ã
    db  7Bh ; {
    db 0E2h ; ‚
    db  7Bh ; {
    db    0
    db    0
    db    1
    db    3
    db    9
    db    0
    db  64h ; d
    db  1Fh
    db    0
    db    3
    db 0CCh ; Ã
    db  7Bh ; {
    db 0E2h ; ‚
    db  7Bh ; {
    db    0
    db    0
    db    1
    db    3
    db    9
    db    0
    db  80h ; Ä
    db  1Fh
    db    0
    db    2
    db 0CCh ; Ã
    db  7Bh ; {
    db 0E2h ; ‚
    db  7Bh ; {
    db    0
    db    0
    db    1
    db    3
    db    9
    db    0
    db  9Ch ; ú
    db  1Fh
    db    0
    db    1
    db 0CCh ; Ã
    db  7Bh ; {
    db 0E2h ; ‚
    db  7Bh ; {
    db    0
    db    0
    db    1
    db    3
    db    9
    db    0
    db  0Eh
    db  1Ch
    db    0
    db    1
    db 0F8h ; ¯
    db  7Bh ; {
    db  0Eh
    db  7Ch ; |
    db    0
    db    0
    db    0
    db    0
    db  11h
    db    0
    db    0
    db  1Ch
    db    0
    db    3
    db 0F8h ; ¯
    db  7Bh ; {
    db  0Eh
    db  7Ch ; |
    db    0
    db    0
    db    0
    db    0
    db  11h
    db    0
    db 0F2h ; Ú
    db  1Bh
    db    0
    db    0
    db 0F8h ; ¯
    db  7Bh ; {
    db  0Eh
    db  7Ch ; |
    db    0
    db    0
    db    0
    db    0
    db  11h
    db    0
    db 0E4h ; ‰
    db  1Bh
    db    0
    db    2
    db 0F8h ; ¯
    db  7Bh ; {
    db  0Eh
    db  7Ch ; |
    db    0
    db    0
    db    0
    db    0
    db  11h
    db    0
    db  20h
    db  1Bh
    db    0
    db    0
    db  24h ; $
    db  7Ch ; |
    db  3Ah ; :
    db  7Ch ; |
    db    0
    db    0
    db    0
    db    0
    db  14h
    db    0
    db  2Eh ; .
    db  1Bh
    db    0
    db    3
    db  24h ; $
    db  7Ch ; |
    db  3Ah ; :
    db  7Ch ; |
    db    0
    db    0
    db    0
    db    0
    db  14h
    db    0
    db  3Ch ; <
    db  1Bh
    db    0
    db    0
    db  50h ; P
    db  7Ch ; |
    db  66h ; f
    db  7Ch ; |
    db    5
    db    0
    db    0
    db    0
    db  16h
    db    0
    db  58h ; X
    db  1Bh
    db    0
    db    3
    db  50h ; P
    db  7Ch ; |
    db  66h ; f
    db  7Ch ; |
    db    4
    db    0
    db    0
    db    0
    db  16h
    db    0
    db  20h
    db  1Bh
    db    0
    db    0
    db  50h ; P
    db  7Ch ; |
    db  66h ; f
    db  7Ch ; |
    db    0
    db    0
    db    0
    db    0
    db  13h
    db    0
    db  2Eh ; .
    db  1Bh
    db    0
    db    3
    db  50h ; P
    db  7Ch ; |
    db  66h ; f
    db  7Ch ; |
    db    0
    db    0
    db    0
    db    0
    db  13h
    db    0
    db  74h ; t
    db  1Bh
    db    0
    db    3
    db 0D4h ; ‘
    db  7Ch ; |
    db 0EAh ; Í
    db  7Ch ; |
    db    0
    db    0
    db    0
    db    3
    db  15h
    db    0
    db  82h ; Ç
    db  1Bh
    db    0
    db    0
    db 0D4h ; ‘
    db  7Ch ; |
    db 0EAh ; Í
    db  7Ch ; |
    db    0
    db    0
    db    0
    db    3
    db  15h
    db    0
    db  90h ; ê
    db  1Bh
    db    0
    db    2
    db 0D4h ; ‘
    db  7Ch ; |
    db 0EAh ; Í
    db  7Ch ; |
    db    0
    db    0
    db    0
    db    3
    db  15h
    db    0
    db  9Eh ; û
    db  1Bh
    db    0
    db    1
    db 0D4h ; ‘
    db  7Ch ; |
    db 0EAh ; Í
    db  7Ch ; |
    db    0
    db    0
    db    0
    db    3
    db  15h
    db    0
    db 0B8h ; ∏
    db  1Fh
    db    0
    db    0
    db    0
    db  7Dh ; }
    db  16h
    db  7Dh ; }
    db    0
    db    0
    db    0
    db    0
    db  0Bh
    db    0
    db 0C6h ; ∆
    db  1Fh
    db    0
    db    3
    db    0
    db  7Dh ; }
    db  16h
    db  7Dh ; }
    db    0
    db    0
    db    0
    db    0
    db  0Bh
    db    0
    db 0E2h ; ‚
    db  1Fh
    db    0
    db    0
    db 0B0h ; ∞
    db  7Dh ; }
    db 0C6h ; ∆
    db  7Dh ; }
    db    0
    db 0FFh
    db    0
    db    0
    db  0Ah
    db    0
    db 0FEh ; ˛
    db  1Fh
    db    0
    db    1
    db 0B0h ; ∞
    db  7Dh ; }
    db 0C6h ; ∆
    db  7Dh ; }
    db    0
    db 0FFh
    db    0
    db    0
    db  0Ah
    db    0
    db 0D4h ; ‘
    db  1Fh
    db    0
    db    2
    db 0B0h ; ∞
    db  7Dh ; }
    db 0C6h ; ∆
    db  7Dh ; }
    db    0
    db 0FFh
    db    0
    db    0
    db  0Ah
    db    0
    db 0F0h ; 
    db  1Fh
    db    0
    db    3
    db 0B0h ; ∞
    db  7Dh ; }
    db 0C6h ; ∆
    db  7Dh ; }
    db    0
    db 0FFh
    db    0
    db    0
    db  0Ah
    db    0
    db  0Ch
    db  20h
    db    0
    db    0
    db  2Ch ; ,
    db  7Dh ; }
    db  42h ; B
    db  7Dh ; }
    db    4
    db    0
    db    0
    db    0
    db  22h ; "
    db    0
    db  1Ah
    db  20h
    db    0
    db    3
    db  2Ch ; ,
    db  7Dh ; }
    db  42h ; B
    db  7Dh ; }
    db    5
    db    0
    db    0
    db    0
    db  22h ; "
    db    0
    db  28h ; (
    db  20h
    db    0
    db    0
    db  58h ; X
    db  7Dh ; }
    db  6Eh ; n
    db  7Dh ; }
    db 0C6h ; ∆
    db    0
    db    0
    db    3
    db  20h
    db    0
    db  36h ; 6
    db  20h
    db    0
    db    3
    db  58h ; X
    db  7Dh ; }
    db  6Eh ; n
    db  7Dh ; }
    db 0C7h ; «
    db    0
    db    0
    db    3
    db  20h
    db    0
    db  44h ; D
    db  20h
    db    0
    db    2
    db  58h ; X
    db  7Dh ; }
    db  6Eh ; n
    db  7Dh ; }
    db 0C8h ; »
    db    0
    db    0
    db    3
    db  20h
    db    0
    db  52h ; R
    db  20h
    db    0
    db    1
    db  58h ; X
    db  7Dh ; }
    db  6Eh ; n
    db  7Dh ; }
    db 0C9h ; …
    db    0
    db    0
    db    3
    db  20h
    db    0
    db  60h ; `
    db  20h
    db    0
    db    0
    db  84h ; Ñ
    db  7Dh ; }
    db  9Ah ; ö
    db  7Dh ; }
    db 0CAh ;  
    db    0
    db    0
    db    3
    db  21h ; !
    db    0
    db  6Eh ; n
    db  20h
    db    0
    db    3
    db  84h ; Ñ
    db  7Dh ; }
    db  9Ah ; ö
    db  7Dh ; }
    db 0CBh ; À
    db    0
    db    0
    db    3
    db  21h ; !
    db    0
    db  7Ch ; |
    db  20h
    db    0
    db    2
    db  84h ; Ñ
    db  7Dh ; }
    db  9Ah ; ö
    db  7Dh ; }
    db 0CCh ; Ã
    db    0
    db    0
    db    3
    db  21h ; !
    db    0
    db  8Ah ; ä
    db  20h
    db    0
    db    1
    db  84h ; Ñ
    db  7Dh ; }
    db  9Ah ; ö
    db  7Dh ; }
    db 0CDh ; Õ
    db    0
    db    0
    db    3
    db  21h ; !
    db    0
    db  94h ; î
    db  1Ah
    db    0
    db    0
    db 0C4h ; ƒ
    db  7Ah ; z
    db 0DAh ; ⁄
    db  7Ah ; z
    db    0
    db    1
    db    1
    db    0
    db  0Ch
    db    0
    db 0F8h ; ¯
    db  1Dh
    db    0
    db    0
    db 0F0h ; 
    db  7Ah ; z
    db    6
    db  7Bh ; {
    db    0
    db    1
    db    1
    db    0
    db    6
    db    0
    db  14h
    db  1Eh
    db    0
    db    3
    db 0F0h ; 
    db  7Ah ; z
    db    6
    db  7Bh ; {
    db    0
    db    1
    db    1
    db    0
    db    6
    db    0
    db  30h ; 0
    db  1Eh
    db    0
    db    2
    db 0F0h ; 
    db  7Ah ; z
    db    6
    db  7Bh ; {
    db    0
    db    1
    db    1
    db    0
    db    6
    db    0
    db  4Ch ; L
    db  1Eh
    db    0
    db    1
    db 0F0h ; 
    db  7Ah ; z
    db    6
    db  7Bh ; {
    db    0
    db    1
    db    1
    db    0
    db    6
    db    0
    db  68h ; h
    db  1Eh
    db    0
    db    0
    db  1Ch
    db  7Bh ; {
    db  32h ; 2
    db  7Bh ; {
    db    0
    db    1
    db    1
    db    0
    db    7
    db    0
    db  84h ; Ñ
    db  1Eh
    db    0
    db    3
    db  1Ch
    db  7Bh ; {
    db  32h ; 2
    db  7Bh ; {
    db    0
    db    1
    db    1
    db    0
    db    7
    db    0
    db 0A0h ; †
    db  1Eh
    db    0
    db    2
    db  1Ch
    db  7Bh ; {
    db  32h ; 2
    db  7Bh ; {
    db    0
    db    1
    db    1
    db    0
    db    7
    db    0
    db 0BCh ; º
    db  1Eh
    db    0
    db    1
    db  1Ch
    db  7Bh ; {
    db  32h ; 2
    db  7Bh ; {
    db    0
    db    1
    db    1
    db    0
    db    7
    db    0
    db  24h ; $
    db  1Ah
    db    0
    db    0
    db  54h ; T
    db  77h ; w
    db  6Ah ; j
    db  77h ; w
    db  0Eh
    db    1
    db    0
    db    0
    db    0
    db    0
    db  40h ; @
    db  1Ah
    db    0
    db    2
    db  54h ; T
    db  77h ; w
    db  6Ah ; j
    db  77h ; w
    db  0Eh
    db    1
    db    0
    db    0
    db    0
    db    0
    db  32h ; 2
    db  1Ah
    db    0
    db    1
    db  54h ; T
    db  77h ; w
    db  6Ah ; j
    db  77h ; w
    db  0Fh
    db    1
    db    0
    db    0
    db    0
    db    0
    db  4Eh ; N
    db  1Ah
    db    0
    db    3
    db  54h ; T
    db  77h ; w
    db  6Ah ; j
    db  77h ; w
    db  0Fh
    db    1
    db    0
    db    0
    db    0
    db    0
    db  94h ; î
    db  1Ah
    db    0
    db    0
    db 0C4h ; ƒ
    db  7Ah ; z
    db 0DAh ; ⁄
    db  7Ah ; z
    db    0
    db    2
    db    1
    db    0
    db  0Ch
    db    0
    db 0F8h ; ¯
    db  1Dh
    db    0
    db    0
    db 0F0h ; 
    db  7Ah ; z
    db    6
    db  7Bh ; {
    db    0
    db    2
    db    1
    db    0
    db    6
    db    0
    db  14h
    db  1Eh
    db    0
    db    3
    db 0F0h ; 
    db  7Ah ; z
    db    6
    db  7Bh ; {
    db    0
    db    2
    db    1
    db    0
    db    6
    db    0
    db  30h ; 0
    db  1Eh
    db    0
    db    2
    db 0F0h ; 
    db  7Ah ; z
    db    6
    db  7Bh ; {
    db    0
    db    2
    db    1
    db    0
    db    6
    db    0
    db  4Ch ; L
    db  1Eh
    db    0
    db    1
    db 0F0h ; 
    db  7Ah ; z
    db    6
    db  7Bh ; {
    db    0
    db    2
    db    1
    db    0
    db    6
    db    0
    db  68h ; h
    db  1Eh
    db    0
    db    0
    db  1Ch
    db  7Bh ; {
    db  32h ; 2
    db  7Bh ; {
    db    0
    db    2
    db    1
    db    0
    db    7
    db    0
    db  84h ; Ñ
    db  1Eh
    db    0
    db    3
    db  1Ch
    db  7Bh ; {
    db  32h ; 2
    db  7Bh ; {
    db    0
    db    2
    db    1
    db    0
    db    7
    db    0
    db 0A0h ; †
    db  1Eh
    db    0
    db    2
    db  1Ch
    db  7Bh ; {
    db  32h ; 2
    db  7Bh ; {
    db    0
    db    2
    db    1
    db    0
    db    7
    db    0
    db 0BCh ; º
    db  1Eh
    db    0
    db    1
    db  1Ch
    db  7Bh ; {
    db  32h ; 2
    db  7Bh ; {
    db    0
    db    2
    db    1
    db    0
    db    7
    db    0
    db  24h ; $
    db  1Ah
    db    0
    db    0
    db  54h ; T
    db  77h ; w
    db  6Ah ; j
    db  77h ; w
    db  18h
    db    2
    db    0
    db    0
    db    0
    db    0
    db  40h ; @
    db  1Ah
    db    0
    db    2
    db  54h ; T
    db  77h ; w
    db  6Ah ; j
    db  77h ; w
    db  18h
    db    2
    db    0
    db    0
    db    0
    db    0
    db  32h ; 2
    db  1Ah
    db    0
    db    1
    db  54h ; T
    db  77h ; w
    db  6Ah ; j
    db  77h ; w
    db  19h
    db    2
    db    0
    db    0
    db    0
    db    0
    db  4Eh ; N
    db  1Ah
    db    0
    db    3
    db  54h ; T
    db  77h ; w
    db  6Ah ; j
    db  77h ; w
    db  19h
    db    2
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db  48h ; H
    db  7Bh ; {
    db  5Eh ; ^
    db  7Bh ; {
    db    0
    db    0
    db    0
    db    0
    db  4Ah ; J
    db    0
    db    0
    db    0
    db    0
    db    0
    db  7Ch ; |
    db  7Ch ; |
    db  92h ; í
    db  7Ch ; |
    db    0
    db    0
    db    0
    db    0
    db  48h ; H
    db    0
    db    0
    db    0
    db    0
    db    0
    db 0AEh ; Æ
    db  7Ah ; z
    db 0AEh ; Æ
    db  7Ah ; z
    db    0
    db    0
    db    0
    db    0
    db  47h ; G
    db    0
    db    0
    db    0
    db    0
    db    0
    db  38h ; 8
    db  79h ; y
    db  4Eh ; N
    db  79h ; y
    db    0
    db    0
    db    0
    db    0
    db  49h ; I
    db    0
    db    0
    db    0
    db    0
    db    0
    db  80h ; Ä
    db  77h ; w
    db  96h ; ñ
    db  77h ; w
    db    0
    db    0
    db    0
    db    0
    db  42h ; B
    db    0
    db    0
    db    0
    db    0
    db    2
    db  80h ; Ä
    db  77h ; w
    db  96h ; ñ
    db  77h ; w
    db    0
    db    0
    db    0
    db    0
    db  42h ; B
    db    0
    db    0
    db    0
    db    0
    db    3
    db  80h ; Ä
    db  77h ; w
    db  96h ; ñ
    db  77h ; w
    db    0
    db    0
    db    0
    db    0
    db  42h ; B
    db    0
    db    0
    db    0
    db    0
    db    1
    db  80h ; Ä
    db  77h ; w
    db  96h ; ñ
    db  77h ; w
    db    0
    db    0
    db    0
    db    0
    db  42h ; B
    db    0
    db    0
    db    0
    db    0
    db    0
    db  4Ch ; L
    db  76h ; v
    db  62h ; b
    db  76h ; v
    db    0
    db    0
    db    0
    db    0
    db  41h ; A
    db    0
    db    0
    db    0
    db    0
    db    2
    db  4Ch ; L
    db  76h ; v
    db  62h ; b
    db  76h ; v
    db    0
    db    0
    db    0
    db    0
    db  41h ; A
    db    0
    db    0
    db    0
    db    0
    db    3
    db  4Ch ; L
    db  76h ; v
    db  62h ; b
    db  76h ; v
    db    0
    db    0
    db    0
    db    0
    db  41h ; A
    db    0
    db    0
    db    0
    db    0
    db    1
    db  4Ch ; L
    db  76h ; v
    db  62h ; b
    db  76h ; v
    db    0
    db    0
    db    0
    db    0
    db  41h ; A
    db    0
    db    0
    db    0
    db    0
    db    0
    db    4
    db  78h ; x
    db  1Ah
    db  78h ; x
    db    0
    db    0
    db    0
    db    0
    db  44h ; D
    db    0
    db    0
    db    0
    db    0
    db    2
    db    4
    db  78h ; x
    db  1Ah
    db  78h ; x
    db    0
    db    0
    db    0
    db    0
    db  44h ; D
    db    0
    db    0
    db    0
    db    0
    db    3
    db    4
    db  78h ; x
    db  1Ah
    db  78h ; x
    db    0
    db    0
    db    0
    db    0
    db  44h ; D
    db    0
    db    0
    db    0
    db    0
    db    1
    db    4
    db  78h ; x
    db  1Ah
    db  78h ; x
    db    0
    db    0
    db    0
    db    0
    db  44h ; D
    db    0
    db    0
    db    0
    db    0
    db    0
    db  60h ; `
    db  7Eh ; ~
    db  76h ; v
    db  7Eh ; ~
    db    0
    db 0FFh
    db    0
    db    0
    db  45h ; E
    db    0
    db    0
    db    0
    db    0
    db    2
    db  60h ; `
    db  7Eh ; ~
    db  76h ; v
    db  7Eh ; ~
    db    0
    db 0FFh
    db    0
    db    0
    db  45h ; E
    db    0
    db    0
    db    0
    db    0
    db    3
    db  60h ; `
    db  7Eh ; ~
    db  76h ; v
    db  7Eh ; ~
    db    0
    db 0FFh
    db    0
    db    0
    db  45h ; E
    db    0
    db    0
    db    0
    db    0
    db    1
    db  60h ; `
    db  7Eh ; ~
    db  76h ; v
    db  7Eh ; ~
    db    0
    db 0FFh
    db    0
    db    0
    db  45h ; E
    db    0
    db    0
    db    0
    db    0
    db    0
    db  8Ch ; å
    db  7Eh ; ~
    db 0A2h ; ¢
    db  7Eh ; ~
    db    0
    db    0
    db    0
    db    0
    db  46h ; F
    db    0
    db    0
    db    0
    db    0
    db    2
    db  8Ch ; å
    db  7Eh ; ~
    db 0A2h ; ¢
    db  7Eh ; ~
    db    0
    db    0
    db    0
    db    0
    db  46h ; F
    db    0
    db    0
    db    0
    db    0
    db    3
    db  8Ch ; å
    db  7Eh ; ~
    db 0A2h ; ¢
    db  7Eh ; ~
    db    0
    db    0
    db    0
    db    0
    db  46h ; F
    db    0
    db    0
    db    0
    db    0
    db    1
    db  8Ch ; å
    db  7Eh ; ~
    db 0A2h ; ¢
    db  7Eh ; ~
    db    0
    db    0
    db    0
    db    0
    db  46h ; F
    db    0
    db    0
    db    0
    db    0
    db    0
    db 0B8h ; ∏
    db  7Eh ; ~
    db 0CEh ; Œ
    db  7Eh ; ~
    db    0
    db 0FFh
    db    0
    db    0
    db  43h ; C
    db    0
    db    0
    db    0
    db    0
    db    2
    db 0B8h ; ∏
    db  7Eh ; ~
    db 0CEh ; Œ
    db  7Eh ; ~
    db    0
    db 0FFh
    db    0
    db    0
    db  43h ; C
    db    0
    db    0
    db    0
    db    0
    db    3
    db 0B8h ; ∏
    db  7Eh ; ~
    db 0CEh ; Œ
    db  7Eh ; ~
    db    0
    db 0FFh
    db    0
    db    0
    db  43h ; C
    db    0
    db    0
    db    0
    db    0
    db    1
    db 0B8h ; ∏
    db  7Eh ; ~
    db 0CEh ; Œ
    db  7Eh ; ~
    db    0
    db 0FFh
    db    0
    db    0
    db  43h ; C
    db    0
    db  40h ; @
    db  1Ah
    db    0
    db    2
    db  54h ; T
    db  77h ; w
    db  6Ah ; j
    db  77h ; w
    db    4
    db    0
    db    0
    db    0
    db    0
    db    0
    db  32h ; 2
    db  1Ah
    db    0
    db    1
    db  54h ; T
    db  77h ; w
    db  6Ah ; j
    db  77h ; w
    db    5
    db    0
    db    0
    db    0
    db    0
    db    0
    db  4Eh ; N
    db  1Ah
    db    0
    db    3
    db  54h ; T
    db  77h ; w
    db  6Ah ; j
    db  77h ; w
    db    5
    db    0
    db    0
    db    0
    db    0
    db    0
    db 0C8h ; »
    db  1Bh
    db    0
    db    0
    db 0B4h ; ¥
    db  78h ; x
    db 0CAh ;  
    db  78h ; x
    db    0
    db    0
    db    1
    db    0
    db    1
    db    0
    db 0BAh ; ∫
    db  1Bh
    db    0
    db    3
    db 0B4h ; ¥
    db  78h ; x
    db 0CAh ;  
    db  78h ; x
    db    0
    db    0
    db    1
    db    0
    db    1
    db    0
    db 0ACh ; ¨
    db  1Bh
    db    0
    db    2
    db 0B4h ; ¥
    db  78h ; x
    db 0CAh ;  
    db  78h ; x
    db    0
    db    0
    db    1
    db    0
    db    1
    db    0
    db 0D6h ; ÷
    db  1Bh
    db    0
    db    1
    db 0B4h ; ¥
    db  78h ; x
    db 0CAh ;  
    db  78h ; x
    db    0
    db    0
    db    1
    db    0
    db    1
    db    0
    db 0C8h ; »
    db  1Bh
    db    0
    db    0
    db 0B4h ; ¥
    db  78h ; x
    db 0CAh ;  
    db  78h ; x
    db    0
    db    1
    db    1
    db    0
    db    1
    db    0
    db 0BAh ; ∫
    db  1Bh
    db    0
    db    3
    db 0B4h ; ¥
    db  78h ; x
    db 0CAh ;  
    db  78h ; x
    db    0
    db    1
    db    1
    db    0
    db    1
    db    0
    db 0ACh ; ¨
    db  1Bh
    db    0
    db    2
    db 0B4h ; ¥
    db  78h ; x
    db 0CAh ;  
    db  78h ; x
    db    0
    db    1
    db    1
    db    0
    db    1
    db    0
    db 0D6h ; ÷
    db  1Bh
    db    0
    db    1
    db 0B4h ; ¥
    db  78h ; x
    db 0CAh ;  
    db  78h ; x
    db    0
    db    1
    db    1
    db    0
    db    1
    db    0
    db 0C8h ; »
    db  1Bh
    db    0
    db    0
    db 0B4h ; ¥
    db  78h ; x
    db 0CAh ;  
    db  78h ; x
    db    0
    db    2
    db    1
    db    0
    db    1
    db    0
    db 0BAh ; ∫
    db  1Bh
    db    0
    db    3
    db 0B4h ; ¥
    db  78h ; x
    db 0CAh ;  
    db  78h ; x
    db    0
    db    2
    db    1
    db    0
    db    1
    db    0
    db 0ACh ; ¨
    db  1Bh
    db    0
    db    2
    db 0B4h ; ¥
    db  78h ; x
    db 0CAh ;  
    db  78h ; x
    db    0
    db    2
    db    1
    db    0
    db    1
    db    0
    db 0D6h ; ÷
    db  1Bh
    db    0
    db    1
    db 0B4h ; ¥
    db  78h ; x
    db 0CAh ;  
    db  78h ; x
    db    0
    db    2
    db    1
    db    0
    db    1
    db    0
    db  1Ch
    db  1Ch
    db    0
    db    0
    db  50h ; P
    db  7Ch ; |
    db  66h ; f
    db  7Ch ; |
    db 0DEh ; ﬁ
    db    0
    db    0
    db    0
    db  13h
    db    0
    db  46h ; F
    db  1Ch
    db    0
    db    3
    db  50h ; P
    db  7Ch ; |
    db  66h ; f
    db  7Ch ; |
    db 0DFh ; ﬂ
    db    0
    db    0
    db    0
    db  13h
    db    0
    db  2Ah ; *
    db  1Ch
    db    0
    db    2
    db  50h ; P
    db  7Ch ; |
    db  66h ; f
    db  7Ch ; |
    db 0E0h ; ‡
    db    0
    db    0
    db    0
    db  13h
    db    0
    db  38h ; 8
    db  1Ch
    db    0
    db    1
    db  50h ; P
    db  7Ch ; |
    db  66h ; f
    db  7Ch ; |
    db 0E1h ; ·
    db    0
    db    0
    db    0
    db  13h
    db    0
    db    0
    db    0
    db    0
    db    0
    db 0DCh ; ‹
    db  7Dh ; }
    db    0
    db    0
    db    0
    db    0
    db    1
    db    3
    db  20h
    db    0
    db    0
    db    0
    db    0
    db    3
    db 0DCh ; ‹
    db  7Dh ; }
    db    0
    db    0
    db    0
    db    0
    db    1
    db    3
    db  20h
    db    0
    db    0
    db    0
    db    0
    db    2
    db 0DCh ; ‹
    db  7Dh ; }
    db    0
    db    0
    db    0
    db    0
    db    1
    db    3
    db  20h
    db    0
    db    0
    db    0
    db    0
    db    1
    db 0DCh ; ‹
    db  7Dh ; }
    db    0
    db    0
    db    0
    db    0
    db    1
    db    3
    db  20h
    db    0
    db    0
    db    0
    db    0
    db    0
    db 0F2h ; Ú
    db  7Dh ; }
    db    0
    db    0
    db    0
    db    0
    db    1
    db    3
    db  21h ; !
    db    0
    db    0
    db    0
    db    0
    db    3
    db 0F2h ; Ú
    db  7Dh ; }
    db    0
    db    0
    db    0
    db    0
    db    1
    db    3
    db  21h ; !
    db    0
    db    0
    db    0
    db    0
    db    2
    db 0F2h ; Ú
    db  7Dh ; }
    db    0
    db    0
    db    0
    db    0
    db    1
    db    3
    db  21h ; !
    db    0
    db    0
    db    0
    db    0
    db    1
    db 0F2h ; Ú
    db  7Dh ; }
    db    0
    db    0
    db    0
    db    0
    db    1
    db    3
    db  21h ; !
    db    0
    db    0
    db    0
    db    0
    db    0
    db    8
    db  7Eh ; ~
    db    0
    db    0
    db    0
    db    0
    db    1
    db    1
    db  1Bh
    db    0
    db    0
    db    0
    db    0
    db    1
    db    8
    db  7Eh ; ~
    db    0
    db    0
    db    0
    db    0
    db    1
    db    2
    db  1Bh
    db    0
    db    0
    db    0
    db    0
    db    0
    db  3Ch ; <
    db  7Fh ; 
    db    0
    db    0
    db    0
    db    0
    db    1
    db    0
    db  1Ch
    db    0
    db    0
    db    0
    db    0
    db    1
    db  3Ch ; <
    db  7Fh ; 
    db    0
    db    0
    db    0
    db    0
    db    1
    db    0
    db  1Ch
    db    0
    db    0
    db    0
    db    0
    db    0
    db  52h ; R
    db  7Fh ; 
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db  1Eh
    db    0
    db    0
    db    0
    db    0
    db    1
    db  52h ; R
    db  7Fh ; 
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db  1Eh
    db    0
    db    0
    db    0
    db    0
    db    0
    db  82h ; Ç
    db  7Ah ; z
    db  82h ; Ç
    db  7Ah ; z
    db    0
    db    0
    db    0
    db    0
    db 0FFh
    db    0
    db    0
    db    0
    db    0
    db    0
    db  98h ; ò
    db  7Ah ; z
    db  98h ; ò
    db  7Ah ; z
    db    0
    db    0
    db    0
    db    0
    db 0FFh
    db    0
    db    0
    db    0
    db    0
    db    0
    db  68h ; h
    db  7Fh ; 
    db  7Eh ; ~
    db  7Fh ; 
    db    0
    db    0
    db    0
    db    0
    db 0FFh
    db    0
    db    0
    db    0
    db    0
    db    0
    db  94h ; î
    db  7Fh ; 
    db 0AAh ; ™
    db  7Fh ; 
    db    0
    db    0
    db    0
    db    0
    db 0FFh
    db    0
    db    0
    db    0
    db    0
    db    0
    db  2Ah ; *
    db  7Ah ; z
    db  2Ah ; *
    db  7Ah ; z
    db    0
    db    0
    db    1
    db    0
    db 0FFh
    db    0
    db    0
    db    0
    db    0
    db    0
    db  14h
    db  7Ah ; z
    db  14h
    db  7Ah ; z
    db    0
    db    0
    db    1
    db    0
    db 0FFh
    db    0
    db    0
    db    0
    db    0
    db    3
    db  14h
    db  7Ah ; z
    db  14h
    db  7Ah ; z
    db    0
    db    0
    db    1
    db    0
    db 0FFh
    db    0
    db    0
    db    0
    db    0
    db    2
    db  14h
    db  7Ah ; z
    db  14h
    db  7Ah ; z
    db    0
    db    0
    db    1
    db    0
    db 0FFh
    db    0
    db    0
    db    0
    db    0
    db    1
    db  14h
    db  7Ah ; z
    db  14h
    db  7Ah ; z
    db    0
    db    0
    db    1
    db    0
    db 0FFh
    db    0
    db    0
    db    0
    db    0
    db    0
    db 0FEh ; ˛
    db  79h ; y
    db 0FEh ; ˛
    db  79h ; y
    db    0
    db    0
    db    1
    db    0
    db 0FFh
    db    0
    db    0
    db    0
    db    0
    db    0
    db 0E8h ; Ë
    db  79h ; y
    db 0E8h ; Ë
    db  79h ; y
    db    0
    db    0
    db    1
    db    0
    db 0FFh
    db    0
    db    0
    db    0
    db    0
    db    3
    db 0E8h ; Ë
    db  79h ; y
    db 0E8h ; Ë
    db  79h ; y
    db    0
    db    0
    db    1
    db    0
    db 0FFh
    db    0
    db    0
    db    0
    db    0
    db    2
    db 0E8h ; Ë
    db  79h ; y
    db 0E8h ; Ë
    db  79h ; y
    db    0
    db    0
    db    1
    db    0
    db 0FFh
    db    0
    db    0
    db    0
    db    0
    db    1
    db 0E8h ; Ë
    db  79h ; y
    db 0E8h ; Ë
    db  79h ; y
    db    0
    db    0
    db    1
    db    0
    db 0FFh
    db    0
    db    0
    db    0
    db    0
    db    0
    db 0D2h ; “
    db  79h ; y
    db 0D2h ; “
    db  79h ; y
    db    0
    db    0
    db    1
    db    0
    db 0FFh
    db    0
    db    0
    db    0
    db    0
    db    3
    db 0D2h ; “
    db  79h ; y
    db 0D2h ; “
    db  79h ; y
    db    0
    db    0
    db    1
    db    0
    db 0FFh
    db    0
    db    0
    db    0
    db    0
    db    2
    db 0D2h ; “
    db  79h ; y
    db 0D2h ; “
    db  79h ; y
    db    0
    db    0
    db    1
    db    0
    db 0FFh
    db    0
    db    0
    db    0
    db    0
    db    1
    db 0D2h ; “
    db  79h ; y
    db 0D2h ; “
    db  79h ; y
    db    0
    db    0
    db    1
    db    0
    db 0FFh
    db    0
    db    0
    db    0
    db    0
    db    0
    db 0BCh ; º
    db  79h ; y
    db 0BCh ; º
    db  79h ; y
    db    0
    db    0
    db    1
    db    0
    db 0FFh
    db    0
    db    0
    db    0
    db    0
    db    3
    db 0BCh ; º
    db  79h ; y
    db 0BCh ; º
    db  79h ; y
    db    0
    db    0
    db    1
    db    0
    db 0FFh
    db    0
    db    0
    db    0
    db    0
    db    2
    db 0BCh ; º
    db  79h ; y
    db 0BCh ; º
    db  79h ; y
    db    0
    db    0
    db    1
    db    0
    db 0FFh
    db    0
    db    0
    db    0
    db    0
    db    1
    db 0BCh ; º
    db  79h ; y
    db 0BCh ; º
    db  79h ; y
    db    0
    db    0
    db    1
    db    0
    db 0FFh
    db    0
    db    0
    db    0
    db    0
    db    0
    db 0ECh ; Ï
    db  7Fh ; 
    db 0ECh ; Ï
    db  7Fh ; 
    db    0
    db    0
    db    1
    db    0
    db 0FFh
    db    0
    db    0
    db    0
    db    0
    db    0
    db    2
    db  80h ; Ä
    db    2
    db  80h ; Ä
    db    0
    db    0
    db    1
    db    0
    db 0FFh
    db    0
    db    0
    db    0
    db    0
    db    0
    db  18h
    db  80h ; Ä
    db  18h
    db  80h ; Ä
    db    0
    db    0
    db    1
    db    0
    db 0FFh
    db    0
    db    0
    db    0
    db    0
    db    0
    db  2Eh ; .
    db  80h ; Ä
    db  2Eh ; .
    db  80h ; Ä
    db    0
    db    0
    db    1
    db    0
    db 0FFh
    db    0
    db    0
    db    0
    db    0
    db    0
    db  44h ; D
    db  80h ; Ä
    db  44h ; D
    db  80h ; Ä
    db    0
    db    0
    db    1
    db    0
    db 0FFh
    db    0
    db    0
    db    0
    db    0
    db    0
    db  5Ah ; Z
    db  80h ; Ä
    db  5Ah ; Z
    db  80h ; Ä
    db    0
    db    0
    db    1
    db    0
    db 0FFh
    db    0
    db    0
    db    0
    db    0
    db    0
    db  70h ; p
    db  80h ; Ä
    db  70h ; p
    db  80h ; Ä
    db    0
    db    0
    db    1
    db    0
    db 0FFh
    db    0
    db    0
    db    0
    db    0
    db    0
    db  86h ; Ü
    db  80h ; Ä
    db  86h ; Ü
    db  80h ; Ä
    db    0
    db    0
    db    1
    db    0
    db 0FFh
    db    0
    db    0
    db    0
    db    0
    db    0
    db  9Ch ; ú
    db  80h ; Ä
    db  9Ch ; ú
    db  80h ; Ä
    db    0
    db    0
    db    1
    db    0
    db 0FFh
    db    0
    db    0
    db    0
    db    0
    db    0
    db 0B2h ; ≤
    db  80h ; Ä
    db 0B2h ; ≤
    db  80h ; Ä
    db    0
    db    0
    db    1
    db    0
    db 0FFh
    db    0
    db    0
    db    0
    db    0
    db    0
    db 0C8h ; »
    db  80h ; Ä
    db 0C8h ; »
    db  80h ; Ä
    db    0
    db    0
    db    1
    db    0
    db 0FFh
    db    0
    db    0
    db    0
    db    0
    db    0
    db 0DEh ; ﬁ
    db  80h ; Ä
    db 0DEh ; ﬁ
    db  80h ; Ä
    db    0
    db    0
    db    1
    db    0
    db 0FFh
    db    0
    db    0
    db    0
    db    0
    db    0
    db 0C0h ; ¿
    db  7Fh ; 
    db 0C0h ; ¿
    db  7Fh ; 
    db    0
    db    0
    db    1
    db    0
    db 0FFh
    db    0
    db    0
    db    0
    db 0E0h ; ‡
    db    0
    db  85h ; Ö
    db    1
word_3E590     dw 1C1h
    db  85h ; Ö
    db    1
    db 0E0h ; ‡
    db    0
    db    0
    db    0
    db  70h ; p
    db 0FEh ; ˛
    db  70h ; p
    db 0FEh ; ˛
    db 0A0h ; †
    db 0FEh ; ˛
    db 0D0h ; –
    db 0FEh ; ˛
    db 0F2h ; Ú
    db 0FEh ; ˛
    db  15h
    db 0FFh
    db  38h ; 8
    db 0FFh
    db    0
    db    0
    db  87h ; á
    db    0
    db  2Bh ; +
    db    1
    db  0Ch
    db    2
    db 0EDh ; Ì
    db    2
    db  92h ; í
    db    3
    db 0CFh ; œ
    db    3
    db    0
    db    0
    db 0B2h ; ≤
    db    0
    db  68h ; h
    db    1
    db  18h
    db    2
    db 0C0h ; ¿
    db    2
    db  64h ; d
    db    3
    db 0D0h ; –
    db    7
    db    0
    db    0
    db 0ECh ; Ï
    db 0FFh
    db 0D8h ; ÿ
    db 0FFh
    db 0C4h ; ƒ
    db 0FFh
    db 0B0h ; ∞
    db 0FFh
    db  9Ch ; ú
    db 0FFh
    db  88h ; à
    db 0FFh
    db  90h ; ê
    db    1
    db  69h ; i
    db    1
    db  40h ; @
    db    1
    db  14h
    db    1
    db 0E2h ; ‚
    db    0
    db 0AEh ; Æ
    db    0
    db  78h ; x
    db    0
    db    5
    db 0FFh
    db 0ACh ; ¨
    db 0FFh
    db  54h ; T
    db    0
    db 0FBh ; ˚
    db    0
    db    0
    db    0
    db 0A2h ; ¢
    db 0FFh
    db  45h ; E
    db 0FFh
    db 0E8h ; Ë
    db 0FEh ; ˛
    db  8Bh ; ã
    db 0FEh ; ˛
    db  2Eh ; .
    db 0FEh ; ˛
    db 0D1h ; —
    db 0FDh ; ˝
    db  74h ; t
    db 0FDh ; ˝
    db  17h
    db 0FDh ; ˝
    db 0BAh ; ∫
    db 0FCh ; ¸
    db  5Dh ; ]
    db 0FCh ; ¸
    db    0
    db 0FCh ; ¸
    db    0
    db    0
    db    0
    db    4
    db 0A3h ; £
    db    3
    db  46h ; F
    db    3
    db 0E9h ; È
    db    2
    db  8Ch ; å
    db    2
    db  2Fh ; /
    db    2
    db 0D2h ; “
    db    1
    db  75h ; u
    db    1
    db  18h
    db    1
    db 0BBh ; ª
    db    0
    db  5Eh ; ^
    db    0
    db    0
    db 0FEh ; ˛
    db 0B2h ; ≤
    db 0FEh ; ˛
    db  58h ; X
    db 0FFh
    db    0
    db    0
    db 0A8h ; ®
    db    0
    db  4Eh ; N
    db    1
    db 0E8h ; Ë
    db    3
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db  78h ; x
    db    0
    db  78h ; x
    db    0
    db  78h ; x
    db    0
    db 0A8h ; ®
    db    0
    db 0D8h ; ÿ
    db    0
    db    8
    db    1
    db  38h ; 8
    db    1
    db  68h ; h
    db    1
    db  68h ; h
    db    1
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db  88h ; à
    db 0FFh
    db    0
    db    0
    db 0E7h ; Á
    db 0FEh ; ˛
    db  88h ; à
    db 0FFh
    db    0
    db    0
    db  19h
    db 0FFh
    db  88h ; à
    db 0FFh
    db    0
    db    0
    db  19h
    db    1
    db  88h ; à
    db 0FFh
    db    0
    db    0
    db 0E7h ; Á
    db    0
    db  78h ; x
    db    0
    db    0
    db    0
    db 0E7h ; Á
    db 0FEh ; ˛
    db  78h ; x
    db    0
    db    0
    db    0
    db  19h
    db 0FFh
    db  78h ; x
    db    0
    db    0
    db    0
    db  19h
    db    1
    db  78h ; x
    db    0
    db    0
    db    0
    db 0E7h ; Á
    db    0
    db 0C4h ; ƒ
    db 0FFh
    db    0
    db    0
    db    0
    db 0FEh ; ˛
    db  3Ch ; <
    db    0
    db    0
    db    0
    db    0
    db    2
    db  78h ; x
    db 0FEh ; ˛
    db    0
    db    0
    db    0
    db    0
    db  88h ; à
    db 0FDh ; ˝
    db    0
    db    0
    db    0
    db    0
    db  88h ; à
    db    1
    db    0
    db    0
    db    0
    db    0
    db  78h ; x
    db    2
    db    0
    db    0
    db    0
    db    0
    db  17h
    db    0
    db    0
    db    0
    db    1
    db 0FFh
    db  61h ; a
    db    0
    db    0
    db    0
    db    1
    db 0FFh
    db  9Fh ; ü
    db 0FFh
    db    0
    db    0
    db 0FFh
    db    0
    db 0E9h ; È
    db 0FFh
    db    0
    db    0
    db 0FFh
    db    0
    db  67h ; g
    db  61h ; a
    db  6Dh ; m
    db  65h ; e
    db  31h ; 1
    db    0
    db  67h ; g
    db  61h ; a
    db  6Dh ; m
    db  65h ; e
    db  32h ; 2
    db    0
    db  63h ; c
    db  61h ; a
    db  72h ; r
    db  30h ; 0
    db    0
    db  63h ; c
    db  61h ; a
    db  72h ; r
    db  31h ; 1
    db    0
    db  63h ; c
    db  61h ; a
    db  72h ; r
    db  32h ; 2
    db    0
    db  65h ; e
    db  78h ; x
    db  70h ; p
    db  30h ; 0
    db    0
    db  65h ; e
    db  78h ; x
    db  70h ; p
    db  31h ; 1
    db    0
    db  65h ; e
    db  78h ; x
    db  70h ; p
    db  32h ; 2
    db    0
    db  65h ; e
    db  78h ; x
    db  70h ; p
    db  33h ; 3
    db    0
    db  63h ; c
    db  61h ; a
    db  72h ; r
    db  32h ; 2
    db    0
    db  63h ; c
    db  61h ; a
    db  72h ; r
    db  30h ; 0
    db    0
    db  63h ; c
    db  61h ; a
    db  72h ; r
    db  31h ; 1
    db    0
    db  63h ; c
    db  61h ; a
    db  72h ; r
    db  32h ; 2
    db    0
    db  65h ; e
    db  78h ; x
    db  70h ; p
    db  30h ; 0
    db    0
    db  65h ; e
    db  78h ; x
    db  70h ; p
    db  31h ; 1
    db    0
    db  65h ; e
    db  78h ; x
    db  70h ; p
    db  32h ; 2
    db    0
    db  65h ; e
    db  78h ; x
    db  70h ; p
    db  33h ; 3
    db    0
    db    0
    db  73h ; s
    db  74h ; t
byte_3E70C     db 78h
byte_3E70D     db 78h
byte_3E70E     db 78h
byte_3E70F     db 0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db  74h ; t
    db  63h ; c
    db  6Fh ; o
    db  6Dh ; m
    db  70h ; p
    db    0
    db    0
    db    0
    db    1
    db    0
    db    1
    db    0
    db    0
    db    1
    db    0
    db    0
    db    1
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    1
    db    2
    db    1
    db    3
    db    0
    db    2
    db    3
    db    0
    db    0
    db    1
    db    1
    db    3
    db    2
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    1
    db    2
    db    0
    db    3
    db    1
    db    0
    db    0
    db    3
    db    2
    db    2
    db    3
    db    1
    db    1
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    1
    db    1
    db    5
    db    0
    db    4
    db    5
    db    0
    db    0
    db    4
    db    1
    db    5
    db    4
    db    1
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    1
    db    0
    db    5
    db    1
    db    4
    db    0
    db    5
    db    4
    db    0
    db    5
    db    1
    db    1
    db    4
    db  6Eh ; n
    db  61h ; a
    db  6Dh ; m
    db    0
    db  70h ; p
    db  61h ; a
    db  74h ; t
    db  68h ; h
    db    0
    db  73h ; s
    db  70h ; p
    db  65h ; e
    db  64h ; d
    db    0
    db    0
aEngi     db 'ENGI',0
aEngi_0     db 'ENGI',0
aStar     db 'STAR',0
aStop     db 'STOP',0
aBlow     db 'BLOW',0
aCras     db 'CRAS',0
aSkid     db 'SKID',0
aSki2     db 'SKI2',0
aBump     db 'BUMP',0
aScra     db 'SCRA',0
aEngi_1     db 'ENGI',0
aEngi_2     db 'ENGI',0
aStar_0     db 'STAR',0
aStop_0     db 'STOP',0
aBlow_0     db 'BLOW',0
aCras_0     db 'CRAS',0
aSkid_0     db 'SKID',0
aSki2_0     db 'SKI2',0
aBump_0     db 'BUMP',0
aScra_0     db 'SCRA',0
aDefault     db 'default',0
aRbf     db 'rbf',0
aCop     db 'cop',0
    db 0F4h ; Ù
    db    1
    db  10h
    db  27h ; '
    db  28h ; (
    db  23h ; #
    db    0
    db    0
    dd aEngi                ; "ENGI"
    dd aEngi_0              ; "ENGI"
    dd aStar                ; "STAR"
    dd aStop                ; "STOP"
    dd aBlow                ; "BLOW"
    dd aCras                ; "CRAS"
    dd aSkid                ; "SKID"
    dd aSki2                ; "SKI2"
    dd aBump                ; "BUMP"
    dd aScra                ; "SCRA"
    db 0F4h ; Ù
    db    1
    db  10h
    db  27h ; '
    db  28h ; (
    db  23h ; #
    db    0
    db    0
    dd aEngi_1              ; "ENGI"
    dd aEngi_2              ; "ENGI"
    dd aStar_0              ; "STAR"
    dd aStop_0              ; "STOP"
    dd aBlow_0              ; "BLOW"
    dd aCras_0              ; "CRAS"
    dd aSkid_0              ; "SKID"
    dd aSki2_0              ; "SKI2"
    dd aBump_0              ; "BUMP"
    dd aScra_0              ; "SCRA"
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    4
    db    8
    db  0Ch
    db  10h
    db  14h
    db  18h
    db  1Ch
    db  20h
    db  24h ; $
    db  28h ; (
    db  2Ch ; ,
    db  30h ; 0
    db  34h ; 4
    db  38h ; 8
    db  3Ch ; <
    db  40h ; @
    db  44h ; D
    db  48h ; H
    db  4Ch ; L
    db  54h ; T
    db  5Ah ; Z
    db  62h ; b
    db  6Ah ; j
    db  72h ; r
    db  79h ; y
    db  7Fh ; 
    db  7Fh ; 
    db  7Fh ; 
    db  2Eh ; .
    db  72h ; r
    db  70h ; p
    db  6Ch ; l
    db    0
    db  77h ; w
    db  68h ; h
    db  6Ch ; l
    db  31h ; 1
    db  77h ; w
    db  68h ; h
    db  6Ch ; l
    db  32h ; 2
    db  77h ; w
    db  68h ; h
    db  6Ch ; l
    db  33h ; 3
    db  69h ; i
    db  6Eh ; n
    db  73h ; s
    db  32h ; 2
    db  67h ; g
    db  62h ; b
    db  6Fh ; o
    db  78h ; x
    db  69h ; i
    db  6Eh ; n
    db  73h ; s
    db  31h ; 1
    db  69h ; i
    db  6Eh ; n
    db  73h ; s
    db  33h ; 3
    db  69h ; i
    db  6Eh ; n
    db  6Dh ; m
    db  31h ; 1
    db  69h ; i
    db  6Eh ; n
    db  6Dh ; m
    db  33h ; 3
    db    0
    db  67h ; g
    db  6Eh ; n
    db  6Fh ; o
    db  62h ; b
    db  67h ; g
    db  6Eh ; n
    db  61h ; a
    db  62h ; b
    db  64h ; d
    db  6Fh ; o
    db  74h ; t
    db  20h
    db  64h ; d
    db  6Fh ; o
    db  74h ; t
    db  61h ; a
    db  64h ; d
    db  6Fh ; o
    db  74h ; t
    db  31h ; 1
    db  64h ; d
    db  6Fh ; o
    db  74h ; t
    db  32h ; 2
    db    0
    db  64h ; d
    db  69h ; i
    db  67h ; g
    db  30h ; 0
    db  64h ; d
    db  69h ; i
    db  67h ; g
    db  31h ; 1
    db  64h ; d
    db  69h ; i
    db  67h ; g
    db  32h ; 2
    db  64h ; d
    db  69h ; i
    db  67h ; g
    db  33h ; 3
    db  64h ; d
    db  69h ; i
    db  67h ; g
    db  34h ; 4
    db  64h ; d
    db  69h ; i
    db  67h ; g
    db  35h ; 5
    db  64h ; d
    db  69h ; i
    db  67h ; g
    db  36h ; 6
    db  64h ; d
    db  69h ; i
    db  67h ; g
    db  37h ; 7
    db  64h ; d
    db  69h ; i
    db  67h ; g
    db  38h ; 8
    db  64h ; d
    db  69h ; i
    db  67h ; g
    db  39h ; 9
    db    0
    db  64h ; d
    db  61h ; a
    db  73h ; s
    db  68h ; h
    db    0
    db  72h ; r
    db  6Fh ; o
    db  6Fh ; o
    db  66h ; f
    db    0
    db  72h ; r
    db  6Fh ; o
    db  6Fh ; o
    db  66h ; f
    db    0
    db  64h ; d
    db  61h ; a
    db  73h ; s
    db  74h ; t
    db    0
    db  64h ; d
    db  61h ; a
    db  73h ; s
    db  6Dh ; m
    db    0
    db  72h ; r
    db  6Fh ; o
    db  6Fh ; o
    db  66h ; f
    db    0
    db  72h ; r
    db  6Fh ; o
    db  6Fh ; o
    db  66h ; f
    db    0
    db  64h ; d
    db  61h ; a
    db  73h ; s
    db  68h ; h
    db    0
    db  73h ; s
    db  74h ; t
    db  64h ; d
    db  61h ; a
byte_3E916     db 78h
byte_3E917     db 78h
byte_3E918     db 78h
byte_3E919     db 78h
    db    0
    db    0
    db  73h ; s
    db  74h ; t
    db  64h ; d
    db  62h ; b
byte_3E920     db 78h
byte_3E921     db 78h
byte_3E922     db 78h
byte_3E923     db 78h
    db    0
    db  65h ; e
    db  6Eh ; n
    db  67h ; g
    db  31h ; 1
    db    0
    db  65h ; e
    db  6Eh ; n
    db  67h ; g
    db    0
    db  66h ; f
    db  6Fh ; o
    db  6Eh ; n
    db  74h ; t
    db  6Ch ; l
    db  65h ; e
    db  64h ; d
    db  2Eh ; .
    db  66h ; f
    db  6Eh ; n
    db  74h ; t
    db    0
    db  73h ; s
    db  64h ; d
    db  67h ; g
    db  61h ; a
    db  6Dh ; m
    db  65h ; e
    db    0
    db  67h ; g
    db  61h ; a
    db  6Dh ; m
    db  65h ; e
    db    0
    db  70h ; p
    db  6Ch ; l
    db  61h ; a
    db  6Eh ; n
    db    0
    db  77h ; w
    db  61h ; a
    db  6Ch ; l
    db  6Ch ; l
    db    0
    db  72h ; r
    db  70h ; p
    db  6Ch ; l
    db  79h ; y
    db  72h ; r
    db  70h ; p
    db  69h ; i
    db  63h ; c
    db  72h ; r
    db  70h ; p
    db  61h ; a
    db  63h ; c
    db  72h ; r
    db  70h ; p
    db  6Dh ; m
    db  63h ; c
    db  72h ; r
    db  70h ; p
    db  74h ; t
    db  63h ; c
    db  62h ; b
    db  6Fh ; o
    db  66h ; f
    db  36h ; 6
    db  62h ; b
    db  6Fh ; o
    db  66h ; f
    db  35h ; 5
    db  62h ; b
    db  6Fh ; o
    db  66h ; f
    db  34h ; 4
    db  62h ; b
    db  6Fh ; o
    db  66h ; f
    db  33h ; 3
    db  62h ; b
    db  6Fh ; o
    db  66h ; f
    db  32h ; 2
    db  62h ; b
    db  6Fh ; o
    db  66h ; f
    db  31h ; 1
    db  62h ; b
    db  6Fh ; o
    db  66h ; f
    db  30h ; 0
    db  7Ah ; z
    db  6Fh ; o
    db  6Fh ; o
    db  6Dh ; m
    db  70h ; p
    db  61h ; a
    db  6Eh ; n
    db  6Eh ; n
    db  62h ; b
    db  6Fh ; o
    db  6Eh ; n
    db  36h ; 6
    db  62h ; b
    db  6Fh ; o
    db  6Eh ; n
    db  35h ; 5
    db  62h ; b
    db  6Fh ; o
    db  6Eh ; n
    db  34h ; 4
    db  62h ; b
    db  6Fh ; o
    db  6Eh ; n
    db  33h ; 3
    db  62h ; b
    db  6Fh ; o
    db  6Eh ; n
    db  32h ; 2
    db  62h ; b
    db  6Fh ; o
    db  6Eh ; n
    db  31h ; 1
    db  62h ; b
    db  6Fh ; o
    db  66h ; f
    db  30h ; 0
    db  7Ah ; z
    db  6Fh ; o
    db  6Fh ; o
    db  6Dh ; m
    db  70h ; p
    db  61h ; a
    db  6Eh ; n
    db  6Eh ; n
    db    0
    db  6Dh ; m
    db  65h ; e
    db  6Eh ; n
    db    0
    db  63h ; c
    db  6Fh ; o
    db  6Eh ; n
    db    0
    db  72h ; r
    db  65h ; e
    db  70h ; p
    db    0
    db  2Eh ; .
    db  72h ; r
    db  70h ; p
    db  6Ch ; l
    db    0
    db  72h ; r
    db  65h ; e
    db  70h ; p
    db    0
    db  2Eh ; .
    db  72h ; r
    db  70h ; p
    db  6Ch ; l
    db    0
    db  66h ; f
    db  65h ; e
    db  78h ; x
    db    0
    db  73h ; s
    db  65h ; e
    db  72h ; r
    db    0
    db  6Dh ; m
    db  64h ; d
    db  6Fh ; o
    db    0
    db  77h ; w
    db  61h ; a
    db  69h ; i
    db    0
    db  77h ; w
    db  61h ; a
    db  69h ; i
    db    0
byte_3E9DB     db 6
    db    1
    db    7
    db    3
    db    4
    db    5
    db    6
    db    7
    db    8
    db    8
    db    0
    db    0
    db    0
    db    2
    db    2
    db    3
    db    4
    db    5
    db    1
    db    7
    db    0
    db    2
    db    6
    db    2
    db    3
    db    4
    db    5
    db    6
    db    7
    db    8
    db    0
    db    0
    db    1
    db    0
    db    0
    db    1
    db    1
    db    1
    db    7
    db    8
    db    0
    db    6
    db    6
    db    8
    db    7
    db  10h
    db    1
    db  6Dh ; m
    db    0
    db  12h
    db    1
    db 0E8h ; Ë
    db    0
    db 0BEh ; æ
    db    0
    db  97h ; ó
    db    0
    db  6Ch ; l
    db    0
    db  42h ; B
    db    0
word_3EA18     dw 0Ah
    db  3Ah ; :
    db    1
    db  97h ; ó
    db    0
    db  3Ah ; :
    db    1
    db  12h
    db    1
    db 0E8h ; Ë
    db    0
    db 0BEh ; æ
    db    0
    db  97h ; ó
    db    0
    db  5Bh ; [
    db    0
word_3EA2A     dw 2Fh
    db 0B0h ; ∞
    db    0
    db 0B0h ; ∞
    db    0
    db  9Ch ; ú
    db    0
    db  9Ch ; ú
    db    0
    db  9Ch ; ú
    db    0
    db  9Ch ; ú
    db    0
    db  9Ch ; ú
    db    0
word_3EA3A     dw 9Ch
word_3EA3C     dw 9Ch
    db 0C1h ; ¡
    db    0
    db 0C1h ; ¡
    db    0
    db 0ADh ; ≠
    db    0
    db 0ADh ; ≠
    db    0
    db 0ADh ; ≠
    db    0
    db 0ADh ; ≠
    db    0
    db 0ADh ; ≠
    db    0
word_3EA4C     dw 0C1h
word_3EA4E     dw 0C1h
    db    0
    db    0
    db  68h ; h
    db    0
    db  97h ; ó
    db    0
    db 0C8h ; »
    db    0
    db  70h ; p
    db  6Fh ; o
    db  6Ch ; l
    db  79h ; y
    db  69h ; i
    db  6Eh ; n
    db  66h ; f
    db  6Fh ; o
    db    0
    db    0
    db    0
    db    1
    db    2
    db    3
    db    4
    db    5
    db    6
    db    7
    db    8
    db    9
    db  0Ah
    db    2
    db    6
    db    3
    db    0
    db    0
    db    0
    db    5
    db    1
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    2
    db    3
    db    4
    db    0
    db    0
    db    0
    db    0
    db    0
    db  80h ; Ä
    db    0
    db    0
    db    0
    db  40h ; @
    db    0
    db    0
    db    0
    db  20h
    db    0
    db    0
    db    0
    db  10h
    db    0
    db    0
    db    0
    db    8
    db    0
    db    0
    db    0
    db    4
    db    0
    db    0
    db    0
    db    2
    db    0
    db    0
    db    0
    db    1
    db    0
    db    0
    db  80h ; Ä
    db    0
    db    0
    db    0
    db  40h ; @
    db    0
    db    0
    db    0
    db  20h
    db    0
    db    0
    db    0
    db  10h
    db    0
    db    0
    db    0
    db    8
    db    0
    db    0
    db    0
    db    4
    db    0
    db    0
    db    0
    db    2
    db    0
    db    0
    db    0
    db    1
    db    0
    db    0
    db  80h ; Ä
    db    0
    db    0
    db    0
    db  40h ; @
    db    0
    db    0
    db    0
    db  20h
    db    0
    db    0
    db    0
    db  10h
    db    0
    db    0
    db    0
    db    8
    db    0
    db    0
    db    0
    db    4
    db    0
    db    0
    db    0
    db    2
    db    0
    db    0
    db    0
    db    1
    db    0
    db    0
    db  80h ; Ä
    db    0
    db    0
    db    0
    db  40h ; @
    db    0
    db    0
    db    0
    db  20h
    db    0
    db    0
    db    0
    db  10h
    db    0
    db    0
    db    0
    db    8
    db    0
    db    0
    db    0
    db    4
    db    0
    db    0
    db    0
    db    2
    db    0
    db    0
    db    0
    db    1
    db    0
    db    0
    db    0
word_3EB02     dw 0FFFFh
    db  0Fh
    db  27h ; '
    db 0FFh
    db 0FFh
    db  0Fh
    db  27h ; '
    db 0FFh
    db 0FFh
    db  49h ; I
    db  6Eh ; n
    db  69h ; i
    db  74h ; t
    db  45h ; E
    db  6Eh ; n
    db  67h ; g
    db  69h ; i
    db  6Eh ; n
    db  65h ; e
    db  3Ah ; :
    db  20h
    db  41h ; A
    db  6Ch ; l
    db  6Ch ; l
    db  20h
    db  68h ; h
    db  61h ; a
    db  6Eh ; n
    db  64h ; d
    db  6Ch ; l
    db  65h ; e
    db  73h ; s
    db  20h
    db  75h ; u
    db  73h ; s
    db  65h ; e
    db  64h ; d
    db  2Eh ; .
    db    0
word_3EB2A     dw 0
aStartengineNew     db 'startengine() - new handle = %d',0Ah,0
    db    0
aId1     db 'id1',0
aId2     db 'id2',0
aId3     db 'id3',0
aId4     db 'id4',0
aSetup_exe     db 'setup.exe',0
aSdtitl_     db 'sdtitl.*',0
aTedit_     db 'tedit.*',0
aOpp1_     db 'opp1.*',0
    db 0DEh ; ﬁ
    db  33h ; 3
    db 0E2h ; ‚
    db  33h ; 3
    db 0E6h ; Ê
    db  33h ; 3
    db 0EAh ; Í
    db  33h ; 3
    db 0EEh ; Ó
    db  33h ; 3
    db 0F8h ; ¯
    db  33h ; 3
    db    1
    db  34h ; 4
    db    9
    db  34h ; 4
word_3EB90     dw 0
    db  6Ch ; l
    db  6Fh ; o
    db  61h ; a
    db    0
    db  2Ah ; *
    db    0
    db  6Ch ; l
    db  73h ; s
    db  75h ; u
    db    0
    db  6Ch ; l
    db  73h ; s
    db  64h ; d
    db    0
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db    0
    db  5Ch ; \
    db    0
    db  73h ; s
    db  61h ; a
    db  76h ; v
    db    0
    db    0
word_3EBB0     dw 0
word_3EBB2     dw 0
word_3EBB4     dw 0
word_3EBB6     dw 0
word_3EBB8     dw 0
word_3EBBA     dw 0
word_3EBBC     dw 0
word_3EBBE     dw 0
word_3EBC0     dw 0
word_3EBC2     dw 0
word_3EBC4     dw 0
a_res     db '.res',0
a_pre     db '.pre',0
aWai     db 'wai',0
    db ':',0
a_     db '.',0
byte_3EBD8     db 0
    db  6Ah ; j
    db  6Fh ; o
    db  79h ; y
    db    0
    db  6Ah ; j
    db  6Fh ; o
    db  78h ; x
    db    0
    db  6Bh ; k
    db  65h ; e
    db  79h ; y
    db    0
    db  6Dh ; m
    db  6Fh ; o
    db  75h ; u
    db    0
    db  70h ; p
    db  61h ; a
    db  75h ; u
    db    0
    db  6Dh ; m
    db  6Fh ; o
    db  6Eh ; n
    db    0
    db  6Dh ; m
    db  6Fh ; o
    db  66h ; f
    db    0
    db  73h ; s
    db  6Fh ; o
    db  6Eh ; n
    db    0
    db  73h ; s
    db  6Fh ; o
    db  66h ; f
    db    0
    db  64h ; d
    db  6Fh ; o
    db  73h ; s
    db    0
    db  6Dh ; m
    db  72h ; r
    db  6Ch ; l
    db    0
    db  6Dh ; m
    db  72h ; r
    db  73h ; s
    db    0
    db  64h ; d
    db  65h ; e
    db  61h ; a
    db    0
    db  64h ; d
    db  65h ; e
    db  72h ; r
    db    0
    db  6Dh ; m
    db  65h ; e
    db  72h ; r
    db    0
    db  2Eh ; .
    db  70h ; p
    db  33h ; 3
    db  73h ; s
    db    0
    db  2Eh ; .
    db  33h ; 3
    db  73h ; s
    db  68h ; h
    db    0
    db    0
    db  73h ; s
    db  64h ; d
    db  74h ; t
    db  65h ; e
    db  64h ; d
    db  69h ; i
    db  74h ; t
    db    0
    db  74h ; t
    db  65h ; e
    db  64h ; d
    db  69h ; i
    db  74h ; t
    db    0
    db  70h ; p
    db  62h ; b
    db  6Fh ; o
    db  78h ; x
    db    0
    db  73h ; s
    db  6Eh ; n
    db  61h ; a
    db  6Dh ; m
    db    0
    db  6Dh ; m
    db  6Eh ; n
    db  61h ; a
    db  6Dh ; m
    db    0
    db  74h ; t
    db  6Eh ; n
    db  61h ; a
    db  6Dh ; m
    db    0
    db  62h ; b
    db  74h ; t
    db  69h ; i
    db    0
    db  62h ; b
    db  73h ; s
    db  63h ; c
    db    0
    db  62h ; b
    db  6Ch ; l
    db  6Fh ; o
    db    0
    db  62h ; b
    db  73h ; s
    db  61h ; a
    db    0
    db  62h ; b
    db  63h ; c
    db  6Ch ; l
    db    0
    db  62h ; b
    db  65h ; e
    db  78h ; x
    db    0
    db  6Dh ; m
    db  73h ; s
    db  73h ; s
    db    0
    db  6Dh ; m
    db  65h ; e
    db  6Eh ; n
    db    0
    db  63h ; c
    db  68h ; h
    db  6Ch ; l
    db    0
    db  74h ; t
    db  72h ; r
    db  6Bh ; k
    db    0
    db  2Eh ; .
    db  74h ; t
    db  72h ; r
    db  6Bh ; k
    db    0
    db  2Eh ; .
    db  74h ; t
    db  72h ; r
    db  6Bh ; k
    db    0
    db  74h ; t
    db  72h ; r
    db  6Bh ; k
    db    0
    db  2Eh ; .
    db  74h ; t
    db  72h ; r
    db  6Bh ; k
    db    0
    db  66h ; f
    db  65h ; e
    db  78h ; x
    db    0
    db  73h ; s
    db  65h ; e
    db  72h ; r
    db    0
    db  63h ; c
    db  68h ; h
    db  78h ; x
    db    0
    db    0
    db  65h ; e
    db  6Fh ; o
    db  6Bh ; k
    db  65h ; e
    db  6Eh ; n
    db  73h ; s
    db  65h ; e
    db  69h ; i
    db  65h ; e
    db  65h ; e
    db  6Dh ; m
    db  73h ; s
    db  65h ; e
    db  65h ; e
    db  64h ; d
    db  65h ; e
    db  77h ; w
    db  77h ; w
    db  65h ; e
    db  66h ; f
    db  75h ; u
    db  65h ; e
    db  6Eh ; n
    db  70h ; p
    db  65h ; e
    db  73h ; s
    db  74h ; t
    db  65h ; e
    db  6Ah ; j
    db  73h ; s
    db  65h ; e
    db  6Ah ; j
    db  64h ; d
    db  65h ; e
    db  74h ; t
    db  65h ; e
    db  65h ; e
    db  77h ; w
    db  61h ; a
    db  65h ; e
    db  66h ; f
    db  74h ; t
    db  65h ; e
    db  61h ; a
    db  74h ; t
    db    0
    db  74h ; t
    db  65h ; e
    db  72h ; r
byte_3ECBB     db 30h
    db    0
    db    0
    db    0
    db  3Bh ; ;
    db    0
    db  3Ch ; <
    db    0
    db  3Dh ; =
    db    0
    db  3Eh ; >
    db    0
    db  3Fh ; ?
    db    0
    db  40h ; @
    db    0
    db  41h ; A
    db    0
    db  42h ; B
    db    0
    db  43h ; C
    db    0
    db  44h ; D
    db    0
    db    0
    db    0
    db    0
    db    9
    db    0
    db 0CAh ;  
    db    0
    db 0DCh ; ‹
    db    0
    db    8
    db    0
    db 0DCh ; ‹
    db    0
    db 0C7h ; «
    db    0
    db 0CEh ; Œ
    db    0
    db  3Bh ; ;
    db    1
    db 0C7h ; «
    db    0
    db  3Bh ; ;
    db    1
    db 0B5h ; µ
    db    0
    db    4
    db    0
    db  84h ; Ñ
    db    0
    db    4
    db    0
    db  24h ; $
    db    0
    db 0BBh ; ª
    db    0
    db 0B3h ; ≥
    db    0
    db  8Bh ; ã
    db    0
    db 0B3h ; ≥
    db    0
    db 0BBh ; ª
    db    0
    db  1Eh
    db    6
    db  1Dh
    db    9
    db  66h ; f
    db  6Ch ; l
    db  61h ; a
    db  74h ; t
    db  6Ch ; l
    db  61h ; a
    db  6Bh ; k
    db  65h ; e
    db  6Ch ; l
    db  61h ; a
    db  6Bh ; k
    db  31h ; 1
    db  6Ch ; l
    db  61h ; a
    db  6Bh ; k
    db  32h ; 2
    db  6Ch ; l
    db  61h ; a
    db  6Bh ; k
    db  33h ; 3
    db  6Ch ; l
    db  61h ; a
    db  6Bh ; k
    db  34h ; 4
    db  68h ; h
    db  69h ; i
    db  67h ; g
    db  68h ; h
    db  67h ; g
    db  6Fh ; o
    db  75h ; u
    db  6Eh ; n
    db  67h ; g
    db  6Fh ; o
    db  75h ; u
    db  77h ; w
    db  67h ; g
    db  6Fh ; o
    db  75h ; u
    db  73h ; s
    db  67h ; g
    db  6Fh ; o
    db  75h ; u
    db  65h ; e
    db  67h ; g
    db  6Fh ; o
    db  75h ; u
    db  31h ; 1
    db  67h ; g
    db  6Fh ; o
    db  75h ; u
    db  32h ; 2
    db  67h ; g
    db  6Fh ; o
    db  75h ; u
    db  33h ; 3
    db  67h ; g
    db  6Fh ; o
    db  75h ; u
    db  34h ; 4
    db  67h ; g
    db  6Fh ; o
    db  75h ; u
    db  35h ; 5
    db  67h ; g
    db  6Fh ; o
    db  75h ; u
    db  36h ; 6
    db  67h ; g
    db  6Fh ; o
    db  75h ; u
    db  37h ; 7
    db  67h ; g
    db  6Fh ; o
    db  75h ; u
    db  38h ; 8
    db    0
    db    0
    db  63h ; c
    db  72h ; r
    db  73h ; s
    db  30h ; 0
    db  63h ; c
    db  72h ; r
    db  73h ; s
    db  31h ; 1
    db  63h ; c
    db  72h ; r
    db  73h ; s
    db  32h ; 2
    db  63h ; c
    db  72h ; r
    db  73h ; s
    db  33h ; 3
    db    0
    db    0
    db  75h ; u
    db  63h ; c
    db  72h ; r
    db  30h ; 0
    db  75h ; u
    db  63h ; c
    db  72h ; r
    db  31h ; 1
    db  75h ; u
    db  63h ; c
    db  72h ; r
    db  32h ; 2
    db  75h ; u
    db  63h ; c
    db  72h ; r
    db  33h ; 3
    db    0
    db    0
word_3ED74     dw 0
word_3ED76     dw 0
off_3ED78     dw offset sub_2CE03
word_3ED7A     dw 0
    dw seg dseg
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
word_3EDCA     dw 360Ah
aC_file_info     db ';C_FILE_INFO',0
dword_3EDD9     dd 0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
word_3EDE5     dw 0
    db    0
    db    0
    db    0
    db    0
word_3EDEB     dw 0
byte_3EDED     db 0
    db    0
    db    0
byte_3EDF0     db 0
    db    0
word_3EDF2     dw 14h
    db  81h ; Å
    db  81h ; Å
    db  81h ; Å
    db    1
    db    1
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
argc     dw 0
argv     dw 0
word_3EE0C     dw 0
word_3EE0E     dw 36A2h
seg_3EE10     dw seg dseg
    db  43h ; C
    db    0
word_3EE14     dw 0
byte_3EE16     db 0
byte_3EE17     db 0
dword_3EE18     dd 0
unk_3EE1C     db    0
    db    0
word_3EE1E     dw 0
dword_3EE20     dd 0FFFFFFFFh
word_3EE24     dw 0AE20h
dword_3EE26     dd 0
    db    0
    db  16h
    db    2
    db    2
    db  18h
    db  0Dh
    db    9
    db  0Ch
    db  0Ch
    db  0Ch
    db    7
    db    8
    db  16h
    db  16h
    db 0FFh
    db  12h
    db  0Dh
    db  12h
    db    2
    db 0FFh
word_3EE3E     dw 0
    db  76h ; v
    db  93h ; ì
    db    0
    db    0
    db  76h ; v
    db  93h ; ì
    db    1
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    2
    db    1
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    2
    db    2
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db  84h ; Ñ
    db    3
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    2
    db    4
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    1
    db    0
    db    0
    db    2
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
word_3EF58     dw 3768h
aNull     db '(null)',0
aNull_0     db '(null)',0
    db '+- #',0
    db    0
word_3EF6E     dw 0
word_3EF70     dw 0
    db    0
    db    0
word_3EF74     dw 0
    db    0
    db    0
word_3EF78     dw 0
    db    0
    db    0
word_3EF7C     dw 0
word_3EF7E     dw 2000h
    db    0
    db    0
byte_3EF82     db 0
    db    0
off_3EF84     dd __fptrap
off_3EF88     dd __fptrap
    dd __fptrap
off_3EF90     dd __fptrap
off_3EF94     dd __fptrap
word_3EF98     dw 0
word_3EF9A     dw 0
    db    1
    db    0
    db    0
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  28h ; (
    db  28h ; (
    db  28h ; (
    db  28h ; (
    db  28h ; (
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  48h ; H
    db  10h
    db  10h
    db  10h
    db  10h
    db  10h
    db  10h
    db  10h
    db  10h
    db  10h
    db  10h
    db  10h
    db  10h
    db  10h
    db  10h
    db  10h
    db  84h ; Ñ
    db  84h ; Ñ
    db  84h ; Ñ
    db  84h ; Ñ
    db  84h ; Ñ
    db  84h ; Ñ
    db  84h ; Ñ
    db  84h ; Ñ
    db  84h ; Ñ
    db  84h ; Ñ
    db  10h
    db  10h
    db  10h
    db  10h
    db  10h
    db  10h
    db  10h
    db  81h ; Å
    db  81h ; Å
    db  81h ; Å
    db  81h ; Å
    db  81h ; Å
    db  81h ; Å
    db    1
    db    1
    db    1
    db    1
    db    1
    db    1
    db    1
    db    1
    db    1
    db    1
    db    1
    db    1
    db    1
    db    1
    db    1
    db    1
    db    1
    db    1
    db    1
    db    1
    db  10h
    db  10h
    db  10h
    db  10h
    db  10h
    db  10h
    db  82h ; Ç
    db  82h ; Ç
    db  82h ; Ç
    db  82h ; Ç
    db  82h ; Ç
    db  82h ; Ç
    db    2
    db    2
    db    2
    db    2
    db    2
    db    2
    db    2
    db    2
    db    2
    db    2
    db    2
    db    2
    db    2
    db    2
    db    2
    db    2
    db    2
    db    2
    db    2
    db    2
    db  10h
    db  10h
    db  10h
    db  10h
    db  20h
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
word_3F0A0     dw 1
word_3F0A2     dw 0
word_3F0A4     dw 0
word_3F0A6     dw 0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
dword_3F0BC     dd 0
    db    0
    db    1
    db    1
    db    2
    db    3
    db    3
    db    4
    db    4
    db    5
    db    6
    db    6
    db    7
    db    8
    db    8
    db    9
    db  0Ah
    db  0Ah
    db  0Bh
    db  0Bh
    db  0Ch
    db  0Dh
    db  0Dh
    db  0Eh
    db  0Fh
    db  0Fh
    db  10h
    db  10h
    db  11h
    db  12h
    db  12h
    db  13h
    db  14h
    db  14h
    db  15h
    db  16h
    db  16h
    db  17h
    db  17h
    db  18h
    db  19h
    db  19h
    db  1Ah
    db  1Bh
    db  1Bh
    db  1Ch
    db  1Ch
    db  1Dh
    db  1Eh
    db  1Eh
    db  1Fh
    db  1Fh
    db  20h
    db  21h ; !
    db  21h ; !
    db  22h ; "
    db  22h ; "
    db  23h ; #
    db  24h ; $
    db  24h ; $
    db  25h ; %
    db  26h ; &
    db  26h ; &
    db  27h ; '
    db  27h ; '
    db  28h ; (
    db  29h ; )
    db  29h ; )
    db  2Ah ; *
    db  2Ah ; *
    db  2Bh ; +
    db  2Ch ; ,
    db  2Ch ; ,
    db  2Dh ; -
    db  2Dh ; -
    db  2Eh ; .
    db  2Eh ; .
    db  2Fh ; /
    db  30h ; 0
    db  30h ; 0
    db  31h ; 1
    db  31h ; 1
    db  32h ; 2
    db  33h ; 3
    db  33h ; 3
    db  34h ; 4
    db  34h ; 4
    db  35h ; 5
    db  35h ; 5
    db  36h ; 6
    db  37h ; 7
    db  37h ; 7
    db  38h ; 8
    db  38h ; 8
    db  39h ; 9
    db  39h ; 9
    db  3Ah ; :
    db  3Ah ; :
    db  3Bh ; ;
    db  3Ch ; <
    db  3Ch ; <
    db  3Dh ; =
    db  3Dh ; =
    db  3Eh ; >
    db  3Eh ; >
    db  3Fh ; ?
    db  3Fh ; ?
    db  40h ; @
    db  41h ; A
    db  41h ; A
    db  42h ; B
    db  42h ; B
    db  43h ; C
    db  43h ; C
    db  44h ; D
    db  44h ; D
    db  45h ; E
    db  45h ; E
    db  46h ; F
    db  46h ; F
    db  47h ; G
    db  47h ; G
    db  48h ; H
    db  48h ; H
    db  49h ; I
    db  4Ah ; J
    db  4Ah ; J
    db  4Bh ; K
    db  4Bh ; K
    db  4Ch ; L
    db  4Ch ; L
    db  4Dh ; M
    db  4Dh ; M
    db  4Eh ; N
    db  4Eh ; N
    db  4Fh ; O
    db  4Fh ; O
    db  50h ; P
    db  50h ; P
    db  51h ; Q
    db  51h ; Q
    db  52h ; R
    db  52h ; R
    db  53h ; S
    db  53h ; S
    db  54h ; T
    db  54h ; T
    db  54h ; T
    db  55h ; U
    db  55h ; U
    db  56h ; V
    db  56h ; V
    db  57h ; W
    db  57h ; W
    db  58h ; X
    db  58h ; X
    db  59h ; Y
    db  59h ; Y
    db  5Ah ; Z
    db  5Ah ; Z
    db  5Bh ; [
    db  5Bh ; [
    db  5Bh ; [
    db  5Ch ; \
    db  5Ch ; \
    db  5Dh ; ]
    db  5Dh ; ]
    db  5Eh ; ^
    db  5Eh ; ^
    db  5Fh ; _
    db  5Fh ; _
    db  60h ; `
    db  60h ; `
    db  60h ; `
    db  61h ; a
    db  61h ; a
    db  62h ; b
    db  62h ; b
    db  63h ; c
    db  63h ; c
    db  63h ; c
    db  64h ; d
    db  64h ; d
    db  65h ; e
    db  65h ; e
    db  66h ; f
    db  66h ; f
    db  66h ; f
    db  67h ; g
    db  67h ; g
    db  68h ; h
    db  68h ; h
    db  68h ; h
    db  69h ; i
    db  69h ; i
    db  6Ah ; j
    db  6Ah ; j
    db  6Ah ; j
    db  6Bh ; k
    db  6Bh ; k
    db  6Ch ; l
    db  6Ch ; l
    db  6Ch ; l
    db  6Dh ; m
    db  6Dh ; m
    db  6Eh ; n
    db  6Eh ; n
    db  6Eh ; n
    db  6Fh ; o
    db  6Fh ; o
    db  70h ; p
    db  70h ; p
    db  70h ; p
    db  71h ; q
    db  71h ; q
    db  71h ; q
    db  72h ; r
    db  72h ; r
    db  73h ; s
    db  73h ; s
    db  73h ; s
    db  74h ; t
    db  74h ; t
    db  74h ; t
    db  75h ; u
    db  75h ; u
    db  76h ; v
    db  76h ; v
    db  76h ; v
    db  77h ; w
    db  77h ; w
    db  77h ; w
    db  78h ; x
    db  78h ; x
    db  78h ; x
    db  79h ; y
    db  79h ; y
    db  79h ; y
    db  7Ah ; z
    db  7Ah ; z
    db  7Ah ; z
    db  7Bh ; {
    db  7Bh ; {
    db  7Bh ; {
    db  7Ch ; |
    db  7Ch ; |
    db  7Ch ; |
    db  7Dh ; }
    db  7Dh ; }
    db  7Dh ; }
    db  7Eh ; ~
    db  7Eh ; ~
    db  7Eh ; ~
    db  7Fh ; 
    db  7Fh ; 
    db  7Fh ; 
    db  80h ; Ä
    db  80h ; Ä
    db    0
word_3F1C2     dw 0
word_3F1C4     dw 0
    db    0
    db    0
    db    3
    db    0
    db  0Ch
    db    0
    db  0Fh
    db    0
    db  30h ; 0
    db    0
    db  33h ; 3
    db    0
    db  3Ch ; <
    db    0
    db  3Fh ; ?
    db    0
    db 0C0h ; ¿
    db    0
    db 0C3h ; √
    db    0
    db 0CCh ; Ã
    db    0
    db 0CFh ; œ
    db    0
    db 0F0h ; 
    db    0
    db 0F3h ; Û
    db    0
    db 0FCh ; ¸
    db    0
    db 0FFh
    db    0
    db    0
    db    3
    db    3
    db    3
    db  0Ch
    db    3
    db  0Fh
    db    3
    db  30h ; 0
    db    3
    db  33h ; 3
    db    3
    db  3Ch ; <
    db    3
    db  3Fh ; ?
    db    3
    db 0C0h ; ¿
    db    3
    db 0C3h ; √
    db    3
    db 0CCh ; Ã
    db    3
    db 0CFh ; œ
    db    3
    db 0F0h ; 
    db    3
    db 0F3h ; Û
    db    3
    db 0FCh ; ¸
    db    3
    db 0FFh
    db    3
    db    0
    db  0Ch
    db    3
    db  0Ch
    db  0Ch
    db  0Ch
    db  0Fh
    db  0Ch
    db  30h ; 0
    db  0Ch
    db  33h ; 3
    db  0Ch
    db  3Ch ; <
    db  0Ch
    db  3Fh ; ?
    db  0Ch
    db 0C0h ; ¿
    db  0Ch
    db 0C3h ; √
    db  0Ch
    db 0CCh ; Ã
    db  0Ch
    db 0CFh ; œ
    db  0Ch
    db 0F0h ; 
    db  0Ch
    db 0F3h ; Û
    db  0Ch
    db 0FCh ; ¸
    db  0Ch
    db 0FFh
    db  0Ch
    db    0
    db  0Fh
    db    3
    db  0Fh
    db  0Ch
    db  0Fh
    db  0Fh
    db  0Fh
    db  30h ; 0
    db  0Fh
    db  33h ; 3
    db  0Fh
    db  3Ch ; <
    db  0Fh
    db  3Fh ; ?
    db  0Fh
    db 0C0h ; ¿
    db  0Fh
    db 0C3h ; √
    db  0Fh
    db 0CCh ; Ã
    db  0Fh
    db 0CFh ; œ
    db  0Fh
    db 0F0h ; 
    db  0Fh
    db 0F3h ; Û
    db  0Fh
    db 0FCh ; ¸
    db  0Fh
    db 0FFh
    db  0Fh
    db    0
    db  30h ; 0
    db    3
    db  30h ; 0
    db  0Ch
    db  30h ; 0
    db  0Fh
    db  30h ; 0
    db  30h ; 0
    db  30h ; 0
    db  33h ; 3
    db  30h ; 0
    db  3Ch ; <
    db  30h ; 0
    db  3Fh ; ?
    db  30h ; 0
    db 0C0h ; ¿
    db  30h ; 0
    db 0C3h ; √
    db  30h ; 0
    db 0CCh ; Ã
    db  30h ; 0
    db 0CFh ; œ
    db  30h ; 0
    db 0F0h ; 
    db  30h ; 0
    db 0F3h ; Û
    db  30h ; 0
    db 0FCh ; ¸
    db  30h ; 0
    db 0FFh
    db  30h ; 0
    db    0
    db  33h ; 3
    db    3
    db  33h ; 3
    db  0Ch
    db  33h ; 3
    db  0Fh
    db  33h ; 3
    db  30h ; 0
    db  33h ; 3
    db  33h ; 3
    db  33h ; 3
    db  3Ch ; <
    db  33h ; 3
    db  3Fh ; ?
    db  33h ; 3
    db 0C0h ; ¿
    db  33h ; 3
    db 0C3h ; √
    db  33h ; 3
    db 0CCh ; Ã
    db  33h ; 3
    db 0CFh ; œ
    db  33h ; 3
    db 0F0h ; 
    db  33h ; 3
    db 0F3h ; Û
    db  33h ; 3
    db 0FCh ; ¸
    db  33h ; 3
    db 0FFh
    db  33h ; 3
    db    0
    db  3Ch ; <
    db    3
    db  3Ch ; <
    db  0Ch
    db  3Ch ; <
    db  0Fh
    db  3Ch ; <
    db  30h ; 0
    db  3Ch ; <
    db  33h ; 3
    db  3Ch ; <
    db  3Ch ; <
    db  3Ch ; <
    db  3Fh ; ?
    db  3Ch ; <
    db 0C0h ; ¿
    db  3Ch ; <
    db 0C3h ; √
    db  3Ch ; <
    db 0CCh ; Ã
    db  3Ch ; <
    db 0CFh ; œ
    db  3Ch ; <
    db 0F0h ; 
    db  3Ch ; <
    db 0F3h ; Û
    db  3Ch ; <
    db 0FCh ; ¸
    db  3Ch ; <
    db 0FFh
    db  3Ch ; <
    db    0
    db  3Fh ; ?
    db    3
    db  3Fh ; ?
    db  0Ch
    db  3Fh ; ?
    db  0Fh
    db  3Fh ; ?
    db  30h ; 0
    db  3Fh ; ?
    db  33h ; 3
    db  3Fh ; ?
    db  3Ch ; <
    db  3Fh ; ?
    db  3Fh ; ?
    db  3Fh ; ?
    db 0C0h ; ¿
    db  3Fh ; ?
    db 0C3h ; √
    db  3Fh ; ?
    db 0CCh ; Ã
    db  3Fh ; ?
    db 0CFh ; œ
    db  3Fh ; ?
    db 0F0h ; 
    db  3Fh ; ?
    db 0F3h ; Û
    db  3Fh ; ?
    db 0FCh ; ¸
    db  3Fh ; ?
    db 0FFh
    db  3Fh ; ?
    db    0
    db 0C0h ; ¿
    db    3
    db 0C0h ; ¿
    db  0Ch
    db 0C0h ; ¿
    db  0Fh
    db 0C0h ; ¿
    db  30h ; 0
    db 0C0h ; ¿
    db  33h ; 3
    db 0C0h ; ¿
    db  3Ch ; <
    db 0C0h ; ¿
    db  3Fh ; ?
    db 0C0h ; ¿
    db 0C0h ; ¿
    db 0C0h ; ¿
    db 0C3h ; √
    db 0C0h ; ¿
    db 0CCh ; Ã
    db 0C0h ; ¿
    db 0CFh ; œ
    db 0C0h ; ¿
    db 0F0h ; 
    db 0C0h ; ¿
    db 0F3h ; Û
    db 0C0h ; ¿
    db 0FCh ; ¸
    db 0C0h ; ¿
    db 0FFh
    db 0C0h ; ¿
    db    0
    db 0C3h ; √
    db    3
    db 0C3h ; √
    db  0Ch
    db 0C3h ; √
    db  0Fh
    db 0C3h ; √
    db  30h ; 0
    db 0C3h ; √
    db  33h ; 3
    db 0C3h ; √
    db  3Ch ; <
    db 0C3h ; √
    db  3Fh ; ?
    db 0C3h ; √
    db 0C0h ; ¿
    db 0C3h ; √
    db 0C3h ; √
    db 0C3h ; √
    db 0CCh ; Ã
    db 0C3h ; √
    db 0CFh ; œ
    db 0C3h ; √
    db 0F0h ; 
    db 0C3h ; √
    db 0F3h ; Û
    db 0C3h ; √
    db 0FCh ; ¸
    db 0C3h ; √
    db 0FFh
    db 0C3h ; √
    db    0
    db 0CCh ; Ã
    db    3
    db 0CCh ; Ã
    db  0Ch
    db 0CCh ; Ã
    db  0Fh
    db 0CCh ; Ã
    db  30h ; 0
    db 0CCh ; Ã
    db  33h ; 3
    db 0CCh ; Ã
    db  3Ch ; <
    db 0CCh ; Ã
    db  3Fh ; ?
    db 0CCh ; Ã
    db 0C0h ; ¿
    db 0CCh ; Ã
    db 0C3h ; √
    db 0CCh ; Ã
    db 0CCh ; Ã
    db 0CCh ; Ã
    db 0CFh ; œ
    db 0CCh ; Ã
    db 0F0h ; 
    db 0CCh ; Ã
    db 0F3h ; Û
    db 0CCh ; Ã
    db 0FCh ; ¸
    db 0CCh ; Ã
    db 0FFh
    db 0CCh ; Ã
    db    0
    db 0CFh ; œ
    db    3
    db 0CFh ; œ
    db  0Ch
    db 0CFh ; œ
    db  0Fh
    db 0CFh ; œ
    db  30h ; 0
    db 0CFh ; œ
    db  33h ; 3
    db 0CFh ; œ
    db  3Ch ; <
    db 0CFh ; œ
    db  3Fh ; ?
    db 0CFh ; œ
    db 0C0h ; ¿
    db 0CFh ; œ
    db 0C3h ; √
    db 0CFh ; œ
    db 0CCh ; Ã
    db 0CFh ; œ
    db 0CFh ; œ
    db 0CFh ; œ
    db 0F0h ; 
    db 0CFh ; œ
    db 0F3h ; Û
    db 0CFh ; œ
    db 0FCh ; ¸
    db 0CFh ; œ
    db 0FFh
    db 0CFh ; œ
    db    0
    db 0F0h ; 
    db    3
    db 0F0h ; 
    db  0Ch
    db 0F0h ; 
    db  0Fh
    db 0F0h ; 
    db  30h ; 0
    db 0F0h ; 
    db  33h ; 3
    db 0F0h ; 
    db  3Ch ; <
    db 0F0h ; 
    db  3Fh ; ?
    db 0F0h ; 
    db 0C0h ; ¿
    db 0F0h ; 
    db 0C3h ; √
    db 0F0h ; 
    db 0CCh ; Ã
    db 0F0h ; 
    db 0CFh ; œ
    db 0F0h ; 
    db 0F0h ; 
    db 0F0h ; 
    db 0F3h ; Û
    db 0F0h ; 
    db 0FCh ; ¸
    db 0F0h ; 
    db 0FFh
    db 0F0h ; 
    db    0
    db 0F3h ; Û
    db    3
    db 0F3h ; Û
    db  0Ch
    db 0F3h ; Û
    db  0Fh
    db 0F3h ; Û
    db  30h ; 0
    db 0F3h ; Û
    db  33h ; 3
    db 0F3h ; Û
    db  3Ch ; <
    db 0F3h ; Û
    db  3Fh ; ?
    db 0F3h ; Û
    db 0C0h ; ¿
    db 0F3h ; Û
    db 0C3h ; √
    db 0F3h ; Û
    db 0CCh ; Ã
    db 0F3h ; Û
    db 0CFh ; œ
    db 0F3h ; Û
    db 0F0h ; 
    db 0F3h ; Û
    db 0F3h ; Û
    db 0F3h ; Û
    db 0FCh ; ¸
    db 0F3h ; Û
    db 0FFh
    db 0F3h ; Û
    db    0
    db 0FCh ; ¸
    db    3
    db 0FCh ; ¸
    db  0Ch
    db 0FCh ; ¸
    db  0Fh
    db 0FCh ; ¸
    db  30h ; 0
    db 0FCh ; ¸
    db  33h ; 3
    db 0FCh ; ¸
    db  3Ch ; <
    db 0FCh ; ¸
    db  3Fh ; ?
    db 0FCh ; ¸
    db 0C0h ; ¿
    db 0FCh ; ¸
    db 0C3h ; √
    db 0FCh ; ¸
    db 0CCh ; Ã
    db 0FCh ; ¸
    db 0CFh ; œ
    db 0FCh ; ¸
    db 0F0h ; 
    db 0FCh ; ¸
    db 0F3h ; Û
    db 0FCh ; ¸
    db 0FCh ; ¸
    db 0FCh ; ¸
    db 0FFh
    db 0FCh ; ¸
    db    0
    db 0FFh
    db    3
    db 0FFh
    db  0Ch
    db 0FFh
    db  0Fh
    db 0FFh
    db  30h ; 0
    db 0FFh
    db  33h ; 3
    db 0FFh
    db  3Ch ; <
    db 0FFh
    db  3Fh ; ?
    db 0FFh
    db 0C0h ; ¿
    db 0FFh
    db 0C3h ; √
    db 0FFh
    db 0CCh ; Ã
    db 0FFh
    db 0CFh ; œ
    db 0FFh
    db 0F0h ; 
    db 0FFh
    db 0F3h ; Û
    db 0FFh
    db 0FCh ; ¸
    db 0FFh
    db 0FFh
    db 0FFh
    db  28h ; (
    db    0
    db 0A8h ; ®
    db  3Ch ; <
    db 0A8h ; ®
    db  3Ch ; <
    db 0A9h ; ©
    db  3Ch ; <
    db 0ABh ; ´
    db  3Ch ; <
    db 0AEh ; Æ
    db  3Ch ; <
    db 0B2h ; ≤
    db  3Ch ; <
    db 0B7h ; ∑
    db  3Ch ; <
    db 0BDh ; Ω
    db  3Ch ; <
    db 0C4h ; ƒ
    db  3Ch ; <
    db 0CCh ; Ã
    db  3Ch ; <
    db 0D5h ; ’
    db  3Ch ; <
    db 0DFh ; ﬂ
    db  3Ch ; <
    db 0EAh ; Í
    db  3Ch ; <
    db 0F6h ; ˆ
    db  3Ch ; <
    db    3
    db  3Dh ; =
    db  11h
    db  3Dh ; =
    db  20h
    db  3Dh ; =
    db  30h ; 0
    db  3Dh ; =
    db  41h ; A
    db  3Dh ; =
    db  53h ; S
    db  3Dh ; =
    db  66h ; f
    db  3Dh ; =
    db  7Ah ; z
    db  3Dh ; =
    db  8Fh ; è
    db  3Dh ; =
    db 0A5h ; •
    db  3Dh ; =
    db 0BCh ; º
    db  3Dh ; =
    db 0D4h ; ‘
    db  3Dh ; =
    db 0EDh ; Ì
    db  3Dh ; =
    db    7
    db  3Eh ; >
    db  22h ; "
    db  3Eh ; >
    db  3Eh ; >
    db  3Eh ; >
    db  5Bh ; [
    db  3Eh ; >
    db  79h ; y
    db  3Eh ; >
    db  98h ; ò
    db  3Eh ; >
    db 0B8h ; ∏
    db  3Eh ; >
    db 0D9h ; Ÿ
    db  3Eh ; >
    db 0FBh ; ˚
    db  3Eh ; >
    db  1Eh
    db  3Fh ; ?
    db  42h ; B
    db  3Fh ; ?
    db  67h ; g
    db  3Fh ; ?
    db  8Dh ; ç
    db  3Fh ; ?
    db    1
    db    2
    db    3
    db    3
    db    4
    db    4
    db    3
    db    4
    db    5
    db    5
    db    4
    db    5
    db    6
    db    6
    db    6
    db    4
    db    6
    db    6
    db    7
    db    7
    db    8
    db    5
    db    6
    db    7
    db    8
    db    8
    db    9
    db    9
    db    5
    db    7
    db    8
    db    9
    db    9
    db  0Ah
    db  0Ah
    db  0Ah
    db    5
    db    7
    db    8
    db    9
    db  0Ah
    db  0Bh
    db  0Bh
    db  0Bh
    db  0Bh
    db    5
    db    8
    db    9
    db  0Ah
    db  0Bh
    db  0Bh
    db  0Ch
    db  0Ch
    db  0Ch
    db  0Dh
    db    6
    db    8
    db    9
    db  0Bh
    db  0Ch
    db  0Ch
    db  0Dh
    db  0Dh
    db  0Eh
    db  0Eh
    db  0Eh
    db    6
    db    8
    db  0Ah
    db  0Bh
    db  0Ch
    db  0Dh
    db  0Eh
    db  0Eh
    db  0Fh
    db  0Fh
    db  0Fh
    db  0Fh
    db    6
    db    9
    db  0Ah
    db  0Ch
    db  0Dh
    db  0Eh
    db  0Eh
    db  0Fh
    db  0Fh
    db  10h
    db  10h
    db  10h
    db  10h
    db    6
    db    9
    db  0Bh
    db  0Ch
    db  0Dh
    db  0Eh
    db  0Fh
    db  10h
    db  10h
    db  11h
    db  11h
    db  11h
    db  11h
    db  12h
    db    7
    db    9
    db  0Bh
    db  0Dh
    db  0Eh
    db  0Fh
    db  10h
    db  11h
    db  11h
    db  12h
    db  12h
    db  12h
    db  13h
    db  13h
    db  13h
    db    7
    db  0Ah
    db  0Ch
    db  0Dh
    db  0Fh
    db  10h
    db  11h
    db  11h
    db  12h
    db  13h
    db  13h
    db  13h
    db  14h
    db  14h
    db  14h
    db  14h
    db    7
    db  0Ah
    db  0Ch
    db  0Eh
    db  0Fh
    db  10h
    db  11h
    db  12h
    db  13h
    db  13h
    db  14h
    db  14h
    db  15h
    db  15h
    db  15h
    db  15h
    db  15h
    db    7
    db  0Ah
    db  0Ch
    db  0Eh
    db  10h
    db  11h
    db  12h
    db  13h
    db  13h
    db  14h
    db  15h
    db  15h
    db  16h
    db  16h
    db  16h
    db  16h
    db  16h
    db  17h
    db    8
    db  0Bh
    db  0Dh
    db  0Fh
    db  10h
    db  11h
    db  12h
    db  13h
    db  14h
    db  15h
    db  16h
    db  16h
    db  17h
    db  17h
    db  17h
    db  17h
    db  18h
    db  18h
    db  18h
    db    8
    db  0Bh
    db  0Dh
    db  0Fh
    db  11h
    db  12h
    db  13h
    db  14h
    db  15h
    db  16h
    db  16h
    db  17h
    db  17h
    db  18h
    db  18h
    db  18h
    db  19h
    db  19h
    db  19h
    db  19h
    db    8
    db  0Bh
    db  0Eh
    db  0Fh
    db  11h
    db  12h
    db  14h
    db  15h
    db  16h
    db  16h
    db  17h
    db  18h
    db  18h
    db  19h
    db  19h
    db  19h
    db  1Ah
    db  1Ah
    db  1Ah
    db  1Ah
    db  1Ah
    db    8
    db  0Bh
    db  0Eh
    db  10h
    db  11h
    db  13h
    db  14h
    db  15h
    db  16h
    db  17h
    db  18h
    db  18h
    db  19h
    db  1Ah
    db  1Ah
    db  1Ah
    db  1Bh
    db  1Bh
    db  1Bh
    db  1Bh
    db  1Bh
    db  1Ch
    db    8
    db  0Ch
    db  0Eh
    db  10h
    db  12h
    db  13h
    db  15h
    db  16h
    db  17h
    db  18h
    db  19h
    db  19h
    db  1Ah
    db  1Ah
    db  1Bh
    db  1Bh
    db  1Ch
    db  1Ch
    db  1Ch
    db  1Dh
    db  1Dh
    db  1Dh
    db  1Dh
    db    9
    db  0Ch
    db  0Fh
    db  11h
    db  12h
    db  14h
    db  15h
    db  16h
    db  17h
    db  18h
    db  19h
    db  1Ah
    db  1Bh
    db  1Bh
    db  1Ch
    db  1Ch
    db  1Dh
    db  1Dh
    db  1Dh
    db  1Eh
    db  1Eh
    db  1Eh
    db  1Eh
    db  1Eh
    db    9
    db  0Ch
    db  0Fh
    db  11h
    db  13h
    db  14h
    db  16h
    db  17h
    db  18h
    db  19h
    db  1Ah
    db  1Bh
    db  1Bh
    db  1Ch
    db  1Dh
    db  1Dh
    db  1Eh
    db  1Eh
    db  1Eh
    db  1Fh
    db  1Fh
    db  1Fh
    db  1Fh
    db  1Fh
    db  1Fh
    db    9
    db  0Dh
    db  0Fh
    db  11h
    db  13h
    db  15h
    db  16h
    db  17h
    db  19h
    db  1Ah
    db  1Bh
    db  1Bh
    db  1Ch
    db  1Dh
    db  1Dh
    db  1Eh
    db  1Eh
    db  1Fh
    db  1Fh
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  21h ; !
    db    9
    db  0Dh
    db  0Fh
    db  12h
    db  14h
    db  15h
    db  17h
    db  18h
    db  19h
    db  1Ah
    db  1Bh
    db  1Ch
    db  1Dh
    db  1Eh
    db  1Eh
    db  1Fh
    db  1Fh
    db  20h
    db  20h
    db  21h ; !
    db  21h ; !
    db  21h ; !
    db  21h ; !
    db  22h ; "
    db  22h ; "
    db  22h ; "
    db  22h ; "
    db    9
    db  0Dh
    db  10h
    db  12h
    db  14h
    db  16h
    db  17h
    db  18h
    db  1Ah
    db  1Bh
    db  1Ch
    db  1Dh
    db  1Eh
    db  1Eh
    db  1Fh
    db  20h
    db  20h
    db  21h ; !
    db  21h ; !
    db  22h ; "
    db  22h ; "
    db  22h ; "
    db  22h ; "
    db  23h ; #
    db  23h ; #
    db  23h ; #
    db  23h ; #
    db  23h ; #
    db    9
    db  0Dh
    db  10h
    db  12h
    db  14h
    db  16h
    db  18h
    db  19h
    db  1Ah
    db  1Bh
    db  1Ch
    db  1Dh
    db  1Eh
    db  1Fh
    db  20h
    db  20h
    db  21h ; !
    db  22h ; "
    db  22h ; "
    db  22h ; "
    db  23h ; #
    db  23h ; #
    db  23h ; #
    db  24h ; $
    db  24h ; $
    db  24h ; $
    db  24h ; $
    db  24h ; $
    db  24h ; $
    db  0Ah
    db  0Dh
    db  10h
    db  13h
    db  15h
    db  17h
    db  18h
    db  19h
    db  1Bh
    db  1Ch
    db  1Dh
    db  1Eh
    db  1Fh
    db  20h
    db  20h
    db  21h ; !
    db  22h ; "
    db  22h ; "
    db  23h ; #
    db  23h ; #
    db  24h ; $
    db  24h ; $
    db  24h ; $
    db  25h ; %
    db  25h ; %
    db  25h ; %
    db  25h ; %
    db  25h ; %
    db  25h ; %
    db  26h ; &
    db  0Ah
    db  0Eh
    db  11h
    db  13h
    db  15h
    db  17h
    db  19h
    db  1Ah
    db  1Bh
    db  1Dh
    db  1Eh
    db  1Fh
    db  20h
    db  20h
    db  21h ; !
    db  22h ; "
    db  23h ; #
    db  23h ; #
    db  24h ; $
    db  24h ; $
    db  25h ; %
    db  25h ; %
    db  25h ; %
    db  26h ; &
    db  26h ; &
    db  26h ; &
    db  26h ; &
    db  27h ; '
    db  27h ; '
    db  27h ; '
    db  27h ; '
    db  0Ah
    db  0Eh
    db  11h
    db  13h
    db  15h
    db  17h
    db  19h
    db  1Ah
    db  1Ch
    db  1Dh
    db  1Eh
    db  1Fh
    db  20h
    db  21h ; !
    db  22h ; "
    db  23h ; #
    db  23h ; #
    db  24h ; $
    db  25h ; %
    db  25h ; %
    db  26h ; &
    db  26h ; &
    db  26h ; &
    db  27h ; '
    db  27h ; '
    db  27h ; '
    db  28h ; (
    db  28h ; (
    db  28h ; (
    db  28h ; (
    db  28h ; (
    db  28h ; (
    db  0Ah
    db  0Eh
    db  11h
    db  14h
    db  16h
    db  18h
    db  19h
    db  1Bh
    db  1Ch
    db  1Eh
    db  1Fh
    db  20h
    db  21h ; !
    db  22h ; "
    db  23h ; #
    db  23h ; #
    db  24h ; $
    db  25h ; %
    db  25h ; %
    db  26h ; &
    db  26h ; &
    db  27h ; '
    db  27h ; '
    db  28h ; (
    db  28h ; (
    db  28h ; (
    db  29h ; )
    db  29h ; )
    db  29h ; )
    db  29h ; )
    db  29h ; )
    db  29h ; )
    db  29h ; )
    db  0Ah
    db  0Eh
    db  11h
    db  14h
    db  16h
    db  18h
    db  1Ah
    db  1Bh
    db  1Dh
    db  1Eh
    db  1Fh
    db  20h
    db  21h ; !
    db  22h ; "
    db  23h ; #
    db  24h ; $
    db  25h ; %
    db  26h ; &
    db  26h ; &
    db  27h ; '
    db  27h ; '
    db  28h ; (
    db  28h ; (
    db  29h ; )
    db  29h ; )
    db  29h ; )
    db  2Ah ; *
    db  2Ah ; *
    db  2Ah ; *
    db  2Ah ; *
    db  2Ah ; *
    db  2Ah ; *
    db  2Ah ; *
    db  2Bh ; +
    db  0Ah
    db  0Fh
    db  12h
    db  14h
    db  17h
    db  18h
    db  1Ah
    db  1Ch
    db  1Dh
    db  1Fh
    db  20h
    db  21h ; !
    db  22h ; "
    db  23h ; #
    db  24h ; $
    db  25h ; %
    db  26h ; &
    db  26h ; &
    db  27h ; '
    db  28h ; (
    db  28h ; (
    db  29h ; )
    db  29h ; )
    db  2Ah ; *
    db  2Ah ; *
    db  2Ah ; *
    db  2Bh ; +
    db  2Bh ; +
    db  2Bh ; +
    db  2Bh ; +
    db  2Bh ; +
    db  2Ch ; ,
    db  2Ch ; ,
    db  2Ch ; ,
    db  2Ch ; ,
    db  0Bh
    db  0Fh
    db  12h
    db  15h
    db  17h
    db  19h
    db  1Bh
    db  1Ch
    db  1Eh
    db  1Fh
    db  20h
    db  22h ; "
    db  23h ; #
    db  24h ; $
    db  25h ; %
    db  25h ; %
    db  26h ; &
    db  27h ; '
    db  28h ; (
    db  28h ; (
    db  29h ; )
    db  29h ; )
    db  2Ah ; *
    db  2Ah ; *
    db  2Bh ; +
    db  2Bh ; +
    db  2Ch ; ,
    db  2Ch ; ,
    db  2Ch ; ,
    db  2Ch ; ,
    db  2Dh ; -
    db  2Dh ; -
    db  2Dh ; -
    db  2Dh ; -
    db  2Dh ; -
    db  2Dh ; -
    db  0Bh
    db  0Fh
    db  12h
    db  15h
    db  17h
    db  19h
    db  1Bh
    db  1Dh
    db  1Eh
    db  20h
    db  21h ; !
    db  22h ; "
    db  23h ; #
    db  24h ; $
    db  25h ; %
    db  26h ; &
    db  27h ; '
    db  28h ; (
    db  28h ; (
    db  29h ; )
    db  2Ah ; *
    db  2Ah ; *
    db  2Bh ; +
    db  2Bh ; +
    db  2Ch ; ,
    db  2Ch ; ,
    db  2Dh ; -
    db  2Dh ; -
    db  2Dh ; -
    db  2Dh ; -
    db  2Eh ; .
    db  2Eh ; .
    db  2Eh ; .
    db  2Eh ; .
    db  2Eh ; .
    db  2Eh ; .
    db  2Eh ; .
    db  0Bh
    db  0Fh
    db  12h
    db  15h
    db  18h
    db  1Ah
    db  1Bh
    db  1Dh
    db  1Fh
    db  20h
    db  21h ; !
    db  23h ; #
    db  24h ; $
    db  25h ; %
    db  26h ; &
    db  27h ; '
    db  28h ; (
    db  28h ; (
    db  29h ; )
    db  2Ah ; *
    db  2Ah ; *
    db  2Bh ; +
    db  2Ch ; ,
    db  2Ch ; ,
    db  2Dh ; -
    db  2Dh ; -
    db  2Dh ; -
    db  2Eh ; .
    db  2Eh ; .
    db  2Eh ; .
    db  2Fh ; /
    db  2Fh ; /
    db  2Fh ; /
    db  2Fh ; /
    db  2Fh ; /
    db  2Fh ; /
    db  2Fh ; /
    db  30h ; 0
    db  0Bh
    db  0Fh
    db  13h
    db  16h
    db  18h
    db  1Ah
    db  1Ch
    db  1Eh
    db  1Fh
    db  21h ; !
    db  22h ; "
    db  23h ; #
    db  24h ; $
    db  25h ; %
    db  26h ; &
    db  27h ; '
    db  28h ; (
    db  29h ; )
    db  2Ah ; *
    db  2Bh ; +
    db  2Bh ; +
    db  2Ch ; ,
    db  2Ch ; ,
    db  2Dh ; -
    db  2Eh ; .
    db  2Eh ; .
    db  2Eh ; .
    db  2Fh ; /
    db  2Fh ; /
    db  2Fh ; /
    db  30h ; 0
    db  30h ; 0
    db  30h ; 0
    db  30h ; 0
    db  30h ; 0
    db  31h ; 1
    db  31h ; 1
    db  31h ; 1
    db  31h ; 1
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db  45h ; E
    db  58h ; X
    db  49h ; I
    db  54h ; T
    db  20h
    db  4Ch ; L
    db  49h ; I
    db  53h ; S
    db  54h ; T
    db  20h
    db  4Fh ; O
    db  56h ; V
    db  45h ; E
    db  52h ; R
    db  46h ; F
    db  4Ch ; L
    db  4Fh ; O
    db  57h ; W
    db  0Dh
    db  0Ah
    db    0
    db    0
    db  25h ; %
    db  73h ; s
    db  20h
    db  46h ; F
    db  49h ; I
    db  4Ch ; L
    db  45h ; E
    db  20h
    db  45h ; E
    db  52h ; R
    db  52h ; R
    db  4Fh ; O
    db  52h ; R
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
word_3F7D8     dw 0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
byte_3F85A     db 0
    db    0
    db  35h ; 5
    db  28h ; (
    db  2Ch ; ,
    db    7
    db  79h ; y
    db    2
    db  64h ; d
    db  6Eh ; n
    db    2
    db    2
    db    0
    db    0
    db  61h ; a
    db  50h ; P
    db  52h ; R
    db  0Fh
    db  19h
    db    6
    db  19h
    db  19h
    db    2
    db  0Dh
    db  0Bh
    db  0Ch
dword_3F874     dd 0
word_3F878     dw 0
word_3F87A     dw 0
word_3F87C     dw 0
word_3F87E     dw 0
byte_3F880     db 0
byte_3F881     db 0
word_3F882     dw 0
word_3F884     dw 0
word_3F886     dw 0
word_3F888     dw 0
word_3F88A     dw 0
byte_3F88C     db 0
    db    3
word_3F88E     dw 0
word_3F890     dw 0
word_3F892     dw 0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db  4Eh ; N
    db  4Fh ; O
    db  20h
    db  52h ; R
    db  4Fh ; O
    db  4Fh ; O
    db  4Dh ; M
    db  20h
    db  4Ch ; L
    db  45h ; E
    db  46h ; F
    db  54h ; T
    db  20h
    db  4Fh ; O
    db  4Eh ; N
    db  20h
    db  54h ; T
    db  49h ; I
    db  4Dh ; M
    db  45h ; E
    db  52h ; R
    db  20h
    db  49h ; I
    db  4Eh ; N
    db  54h ; T
    db  45h ; E
    db  52h ; R
    db  52h ; R
    db  55h ; U
    db  50h ; P
    db  54h ; T
    db  20h
    db  52h ; R
    db  4Fh ; O
    db  55h ; U
    db  54h ; T
    db  49h ; I
    db  4Eh ; N
    db  45h ; E
    db  20h
    db  4Ch ; L
    db  49h ; I
    db  53h ; S
    db  54h ; T
    db  0Dh
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
unk_3F9CC     db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
byte_3F9E0     db 0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db  48h ; H
    db    0
    db  50h ; P
    db    0
    db    0
    db    0
    db  4Dh ; M
    db    0
    db  49h ; I
    db    0
    db  51h ; Q
    db    0
    db    0
    db    0
    db  4Bh ; K
    db    0
    db  47h ; G
    db    0
    db  4Fh ; O
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
word_3FB02     dw 0
word_3FB04     dw 0
byte_3FB06     db 0
    db    0
byte_3FB08     db 39h
byte_3FB09     db 1Ch
byte_3FB0A     db 47h
byte_3FB0B     db 48h
byte_3FB0C     db 49h
byte_3FB0D     db 4Dh
byte_3FB0E     db 51h
byte_3FB0F     db 50h
byte_3FB10     db 4Fh
byte_3FB11     db 4Bh
word_3FB12     dw 0
word_3FB14     dw 0
byte_3FB16     db 0
byte_3FB17     db 0
word_3FB18     dw 50h
word_3FB1A     dw 0
word_3FB1C     dw 0
word_3FB1E     dw 0
word_3FB20     dw 14h
word_3FB22     dw 0
word_3FB24     dw 0
word_3FB26     dw 50h
word_3FB28     dw 0
word_3FB2A     dw 0
word_3FB2C     dw 0
word_3FB2E     dw 14h
word_3FB30     dw 0
word_3FB32     dw 0
word_3FB34     dw 0
word_3FB36     dw 0
    db    0
    db    1
    db    5
    db    0
    db    3
    db    2
    db    4
    db    3
    db    7
    db    8
    db    6
    db    7
    db    0
    db    1
    db    5
    db    0
word_3FB48     dw 0
word_3FB4A     dw 0
word_3FB4C     dw 0
word_3FB4E     dw 0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
word_3FBD0     dw 0
word_3FBD2     dw 0
word_3FBD4     dw 2
word_3FBD6     dw 0
word_3FBD8     dw 0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
byte_3FBF7     db 0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
byte_3FC04     db 0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
byte_3FC10     db 0
    db    0
byte_3FC12     db 0
    db    0
byte_3FC14     db 0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db  1Bh
    db  31h ; 1
    db  32h ; 2
    db  33h ; 3
    db  34h ; 4
    db  35h ; 5
    db  36h ; 6
    db  37h ; 7
    db  38h ; 8
    db  39h ; 9
    db  30h ; 0
    db  2Dh ; -
    db  3Dh ; =
    db    8
    db    9
    db  71h ; q
    db  77h ; w
    db  65h ; e
    db  72h ; r
    db  74h ; t
    db  79h ; y
    db  75h ; u
    db  69h ; i
    db  6Fh ; o
    db  70h ; p
    db  5Bh ; [
    db  5Dh ; ]
    db  0Dh
    db    0
    db  61h ; a
    db  73h ; s
    db  64h ; d
    db  66h ; f
    db  67h ; g
    db  68h ; h
    db  6Ah ; j
    db  6Bh ; k
    db  6Ch ; l
    db  3Bh ; ;
    db  27h ; '
    db  60h ; `
    db    0
    db  5Ch ; \
    db  7Ah ; z
    db  78h ; x
    db  63h ; c
    db  76h ; v
    db  62h ; b
    db  6Eh ; n
    db  6Dh ; m
    db  2Ch ; ,
    db  2Eh ; .
    db  2Fh ; /
    db    0
    db  2Ah ; *
    db    0
    db  20h
    db    0
    db 0BBh ; ª
    db 0BCh ; º
    db 0BDh ; Ω
    db 0BEh ; æ
    db 0BFh ; ø
    db 0C0h ; ¿
    db 0C1h ; ¡
    db 0C2h ; ¬
    db 0C3h ; √
    db 0C4h ; ƒ
    db    0
    db    0
    db 0C7h ; «
    db 0C8h ; »
    db 0C9h ; …
    db  2Dh ; -
    db 0CBh ; À
    db 0CCh ; Ã
    db 0CDh ; Õ
    db  2Bh ; +
    db 0CFh ; œ
    db 0D0h ; –
    db 0D1h ; —
    db 0D2h ; “
    db 0D3h ; ”
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db  1Bh
    db  21h ; !
    db  40h ; @
    db  23h ; #
    db  24h ; $
    db  25h ; %
    db  5Eh ; ^
    db  26h ; &
    db  2Ah ; *
    db  28h ; (
    db  29h ; )
    db  5Fh ; _
    db  2Bh ; +
    db    8
    db  8Fh ; è
    db  51h ; Q
    db  57h ; W
    db  45h ; E
    db  52h ; R
    db  54h ; T
    db  59h ; Y
    db  55h ; U
    db  49h ; I
    db  4Fh ; O
    db  50h ; P
    db  7Bh ; {
    db  7Dh ; }
    db  0Dh
    db    0
    db  41h ; A
    db  53h ; S
    db  44h ; D
    db  46h ; F
    db  47h ; G
    db  48h ; H
    db  4Ah ; J
    db  4Bh ; K
    db  4Ch ; L
    db  3Ah ; :
    db  22h ; "
    db  7Eh ; ~
    db    0
    db  7Ch ; |
    db  5Ah ; Z
    db  58h ; X
    db  43h ; C
    db  56h ; V
    db  42h ; B
    db  4Eh ; N
    db  4Dh ; M
    db  3Ch ; <
    db  3Eh ; >
    db  3Fh ; ?
    db    0
    db    0
    db    0
    db  20h
    db    0
    db 0D4h ; ‘
    db 0D5h ; ’
    db 0D6h ; ÷
    db 0D7h ; ◊
    db 0D8h ; ÿ
    db 0D9h ; Ÿ
    db 0DAh ; ⁄
    db 0DBh ; €
    db 0DCh ; ‹
    db 0DDh ; ›
    db    0
    db    0
    db 0C7h ; «
    db 0C8h ; »
    db 0C9h ; …
    db  2Dh ; -
    db 0CBh ; À
    db 0CCh ; Ã
    db 0CDh ; Õ
    db  2Bh ; +
    db 0CFh ; œ
    db 0D0h ; –
    db 0D1h ; —
    db 0D2h ; “
    db 0D3h ; ”
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db  1Bh
    db  31h ; 1
    db  32h ; 2
    db  33h ; 3
    db  34h ; 4
    db  35h ; 5
    db  36h ; 6
    db  37h ; 7
    db  38h ; 8
    db  39h ; 9
    db  30h ; 0
    db  2Dh ; -
    db  3Dh ; =
    db    8
    db  8Fh ; è
    db  51h ; Q
    db  57h ; W
    db  45h ; E
    db  52h ; R
    db  54h ; T
    db  59h ; Y
    db  55h ; U
    db  49h ; I
    db  4Fh ; O
    db  50h ; P
    db  5Bh ; [
    db  5Dh ; ]
    db  0Dh
    db    0
    db  41h ; A
    db  53h ; S
    db  44h ; D
    db  46h ; F
    db  47h ; G
    db  48h ; H
    db  4Ah ; J
    db  4Bh ; K
    db  4Ch ; L
    db  3Bh ; ;
    db  27h ; '
    db  60h ; `
    db    0
    db  5Ch ; \
    db  5Ah ; Z
    db  58h ; X
    db  43h ; C
    db  56h ; V
    db  42h ; B
    db  4Eh ; N
    db  4Dh ; M
    db  2Ch ; ,
    db  2Eh ; .
    db  2Fh ; /
    db    0
    db    0
    db    0
    db  20h
    db    0
    db 0D4h ; ‘
    db 0D5h ; ’
    db 0D6h ; ÷
    db 0D7h ; ◊
    db 0D8h ; ÿ
    db 0D9h ; Ÿ
    db 0DAh ; ⁄
    db 0DBh ; €
    db 0DCh ; ‹
    db 0DDh ; ›
    db    0
    db    0
    db 0C7h ; «
    db 0C8h ; »
    db 0C9h ; …
    db  2Dh ; -
    db 0CBh ; À
    db 0CCh ; Ã
    db 0CDh ; Õ
    db  2Bh ; +
    db 0CFh ; œ
    db 0D0h ; –
    db 0D1h ; —
    db 0D2h ; “
    db 0D3h ; ”
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db  1Bh
    db  21h ; !
    db    0
    db  23h ; #
    db  24h ; $
    db  25h ; %
    db  1Eh
    db  26h ; &
    db  2Ah ; *
    db  28h ; (
    db  29h ; )
    db  1Fh
    db  2Bh ; +
    db  7Fh ; 
    db    9
    db  11h
    db  17h
    db    5
    db  12h
    db  14h
    db  19h
    db  15h
    db    9
    db  0Fh
    db  10h
    db  1Bh
    db  1Dh
    db  0Dh
    db    0
    db    1
    db  13h
    db    4
    db    6
    db    7
    db    8
    db  0Ah
    db  0Bh
    db  0Ch
    db  3Bh ; ;
    db  2Ch ; ,
    db  60h ; `
    db    0
    db  1Ch
    db  1Ah
    db  18h
    db    3
    db  16h
    db    2
    db  0Eh
    db 0B2h ; ≤
    db  3Ch ; <
    db  3Eh ; >
    db  3Fh ; ?
    db    0
    db    0
    db    0
    db  20h
    db    0
    db 0DEh ; ﬁ
    db 0DFh ; ﬂ
    db 0E0h ; ‡
    db 0E1h ; ·
    db 0E2h ; ‚
    db 0E3h ; „
    db 0E4h ; ‰
    db 0E5h ; Â
    db 0E6h ; Ê
    db 0E7h ; Á
    db    0
    db    0
    db 0C7h ; «
    db 0C8h ; »
    db 0C9h ; …
    db  2Dh ; -
    db 0CBh ; À
    db 0CCh ; Ã
    db 0CDh ; Õ
    db  2Bh ; +
    db 0CFh ; œ
    db 0D0h ; –
    db 0D1h ; —
    db 0D2h ; “
    db 0D3h ; ”
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db  1Bh
    db  21h ; !
    db  40h ; @
    db  23h ; #
    db  24h ; $
    db  25h ; %
    db  5Eh ; ^
    db  26h ; &
    db  2Ah ; *
    db  28h ; (
    db  29h ; )
    db  5Fh ; _
    db  2Bh ; +
    db    8
    db  8Fh ; è
    db  90h ; ê
    db  91h ; ë
    db  92h ; í
    db  93h ; ì
    db  94h ; î
    db  95h ; ï
    db  96h ; ñ
    db  97h ; ó
    db  98h ; ò
    db  99h ; ô
    db  7Bh ; {
    db  7Dh ; }
    db  0Dh
    db    0
    db  9Eh ; û
    db  9Fh ; ü
    db 0A0h ; †
    db 0A1h ; °
    db 0A2h ; ¢
    db 0A3h ; £
    db 0A4h ; §
    db 0A5h ; •
    db 0A6h ; ¶
    db  3Ah ; :
    db  22h ; "
    db  7Eh ; ~
    db    0
    db  7Ch ; |
    db 0ACh ; ¨
    db 0ADh ; ≠
    db 0AEh ; Æ
    db 0AFh ; Ø
    db 0B0h ; ∞
    db 0B1h ; ±
    db 0B2h ; ≤
    db  3Ch ; <
    db  3Eh ; >
    db  3Fh ; ?
    db    0
    db    0
    db    0
    db  20h
    db    0
    db 0F8h ; ¯
    db 0F9h ; ˘
    db 0FAh ; ˙
    db 0FBh ; ˚
    db 0FCh ; ¸
    db 0FDh ; ˝
    db 0FEh ; ˛
    db 0FFh
    db  80h ; Ä
    db  81h ; Å
    db    0
    db    0
    db 0C7h ; «
    db 0C8h ; »
    db 0C9h ; …
    db  2Dh ; -
    db 0CBh ; À
    db 0CCh ; Ã
    db 0CDh ; Õ
    db  2Bh ; +
    db 0CFh ; œ
    db 0D0h ; –
    db 0D1h ; —
    db 0D2h ; “
    db 0D3h ; ”
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
word_3FDFC     dw 2001h
seg_3FDFE     dw seg seg012
byte_3FE00     db 0
    db    0
aSFileError     db '%s FILE ERROR',0Dh,0
aSNotPackedFile     db '%s NOT PACKED FILE',0Dh,0
aSInvalidPackTy     db '%s INVALID PACK TYPE',0Dh,0
    db    0
aLocateshape4_4     db 'locateshape - %-4.4s SHAPE NOT FOUND',0Dh,0Ah
aReservememoryO     db 'reservememory - OUT OF MEMORY SLOTS RESERVING %s',0Dh,0Ah,0
aMemoryManagerB     db 'memory manager - BLOCK NOT FOUND at SEG= %x',0Dh,0Ah,0
aResizememoryCa     db 'resizememory - CANNOT EXPAND BLOCK NOT AT TOP',0Dh,0Ah,0
aResizememoryNo     db 'resizememory - NO MEMORY LEFT TO EXPAND HW=%x',0Dh,0Ah,0
    db    0
word_3FF82     dw 0
word_3FF84     dw 0
word_3FF86     dw 0
word_3FF88     dw 0
word_3FF8A     dw 0
    db '            ',0
    db    0
    db    0
    db    0
    db    2
    db    0
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    dw 2020h
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db '            ',0
    db    0
    db    0
    db    0
    db    0
    db    0
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db  20h
    db    0
    db    0
    db    0
    db    0
    db    1
    db    0
word_40310     dw 481Ch
word_40312     dw 481Ch
word_40314     dw 4B8Eh
word_40316     dw 4B8Eh
word_40318     dw 0
word_4031A     dw 0
word_4031C     dw 0
word_4031E     dw 0
word_40320     dw 0
off_40322     dd 2EA20000h
off_40326     dd 2EA20000h
byte_4032A     db 0
byte_4032B     db 0
byte_4032C     db 7
    db    0
word_4032E     dw 0F00h
word_40330     dw 0
word_40332     dw 0
word_40334     dw 0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
word_403AE     dw 0A0h
word_403B0     dw 64h
word_403B2     dw 0
word_403B4     dw 0
word_403B6     dw 0A0h
word_403B8     dw 64h
word_403BA     dw 0
word_403BC     dw 0
word_403BE     dw 0
word_403C0     dw 0
aWindowReleased     db 'Window Released Out of Order',0Dh,0Ah,0
    db    0
aSFileError_0     db '%s FILE ERROR',0Dh,0
    db    0
byte_403F2     db 0
byte_403F3     db 0
    db    0
    db    0
    db  65h ; e
    db    0
    db 0C9h ; …
    db    0
    db  2Eh ; .
    db    1
    db  92h ; í
    db    1
    db 0F7h ; ˜
    db    1
    db  5Bh ; [
    db    2
    db 0C0h ; ¿
    db    2
    db  24h ; $
    db    3
    db  88h ; à
    db    3
    db 0EDh ; Ì
    db    3
    db  51h ; Q
    db    4
    db 0B5h ; µ
    db    4
    db  1Ah
    db    5
    db  7Eh ; ~
    db    5
    db 0E2h ; ‚
    db    5
    db  46h ; F
    db    6
    db 0AAh ; ™
    db    6
    db  0Eh
    db    7
    db  72h ; r
    db    7
    db 0D6h ; ÷
    db    7
    db  39h ; 9
    db    8
    db  9Dh ; ù
    db    8
    db    1
    db    9
    db  64h ; d
    db    9
    db 0C7h ; «
    db    9
    db  2Bh ; +
    db  0Ah
    db  8Eh ; é
    db  0Ah
    db 0F1h ; Ò
    db  0Ah
    db  54h ; T
    db  0Bh
    db 0B7h ; ∑
    db  0Bh
    db  1Ah
    db  0Ch
    db  7Ch ; |
    db  0Ch
    db 0DFh ; ﬂ
    db  0Ch
    db  41h ; A
    db  0Dh
    db 0A4h ; §
    db  0Dh
    db    6
    db  0Eh
    db  68h ; h
    db  0Eh
    db 0CAh ;  
    db  0Eh
    db  2Bh ; +
    db  0Fh
    db  8Dh ; ç
    db  0Fh
    db 0EEh ; Ó
    db  0Fh
    db  50h ; P
    db  10h
    db 0B1h ; ±
    db  10h
    db  12h
    db  11h
    db  73h ; s
    db  11h
    db 0D3h ; ”
    db  11h
    db  34h ; 4
    db  12h
    db  94h ; î
    db  12h
    db 0F4h ; Ù
    db  12h
    db  54h ; T
    db  13h
    db 0B4h ; ¥
    db  13h
    db  13h
    db  14h
    db  73h ; s
    db  14h
    db 0D2h ; “
    db  14h
    db  31h ; 1
    db  15h
    db  90h ; ê
    db  15h
    db 0EEh ; Ó
    db  15h
    db  4Ch ; L
    db  16h
    db 0ABh ; ´
    db  16h
    db    9
    db  17h
    db  66h ; f
    db  17h
    db 0C4h ; ƒ
    db  17h
    db  21h ; !
    db  18h
    db  7Eh ; ~
    db  18h
    db 0DBh ; €
    db  18h
    db  37h ; 7
    db  19h
    db  93h ; ì
    db  19h
    db 0EFh ; Ô
    db  19h
    db  4Bh ; K
    db  1Ah
    db 0A7h ; ß
    db  1Ah
    db    2
    db  1Bh
    db  5Dh ; ]
    db  1Bh
    db 0B8h ; ∏
    db  1Bh
    db  12h
    db  1Ch
    db  6Ch ; l
    db  1Ch
    db 0C6h ; ∆
    db  1Ch
    db  20h
    db  1Dh
    db  79h ; y
    db  1Dh
    db 0D3h ; ”
    db  1Dh
    db  2Bh ; +
    db  1Eh
    db  84h ; Ñ
    db  1Eh
    db 0DCh ; ‹
    db  1Eh
    db  34h ; 4
    db  1Fh
    db  8Ch ; å
    db  1Fh
    db 0E3h ; „
    db  1Fh
    db  3Ah ; :
    db  20h
    db  91h ; ë
    db  20h
    db 0E7h ; Á
    db  20h
    db  3Dh ; =
    db  21h ; !
    db  93h ; ì
    db  21h ; !
    db 0E8h ; Ë
    db  21h ; !
    db  3Dh ; =
    db  22h ; "
    db  92h ; í
    db  22h ; "
    db 0E7h ; Á
    db  22h ; "
    db  3Bh ; ;
    db  23h ; #
    db  8Eh ; é
    db  23h ; #
    db 0E2h ; ‚
    db  23h ; #
    db  35h ; 5
    db  24h ; $
    db  88h ; à
    db  24h ; $
    db 0DAh ; ⁄
    db  24h ; $
    db  2Ch ; ,
    db  25h ; %
    db  7Eh ; ~
    db  25h ; %
    db 0CFh ; œ
    db  25h ; %
    db  20h
    db  26h ; &
    db  71h ; q
    db  26h ; &
    db 0C1h ; ¡
    db  26h ; &
    db  11h
    db  27h ; '
    db  60h ; `
    db  27h ; '
    db 0AFh ; Ø
    db  27h ; '
    db 0FEh ; ˛
    db  27h ; '
    db  4Ch ; L
    db  28h ; (
    db  9Ah ; ö
    db  28h ; (
    db 0E7h ; Á
    db  28h ; (
    db  35h ; 5
    db  29h ; )
    db  81h ; Å
    db  29h ; )
    db 0CEh ; Œ
    db  29h ; )
    db  1Ah
    db  2Ah ; *
    db  65h ; e
    db  2Ah ; *
    db 0B0h ; ∞
    db  2Ah ; *
    db 0FBh ; ˚
    db  2Ah ; *
    db  45h ; E
    db  2Bh ; +
    db  8Fh ; è
    db  2Bh ; +
    db 0D8h ; ÿ
    db  2Bh ; +
    db  21h ; !
    db  2Ch ; ,
    db  6Ah ; j
    db  2Ch ; ,
    db 0B2h ; ≤
    db  2Ch ; ,
    db 0FAh ; ˙
    db  2Ch ; ,
    db  41h ; A
    db  2Dh ; -
    db  88h ; à
    db  2Dh ; -
    db 0CFh ; œ
    db  2Dh ; -
    db  15h
    db  2Eh ; .
    db  5Ah ; Z
    db  2Eh ; .
    db  9Fh ; ü
    db  2Eh ; .
    db 0E4h ; ‰
    db  2Eh ; .
    db  28h ; (
    db  2Fh ; /
    db  6Ch ; l
    db  2Fh ; /
    db 0AFh ; Ø
    db  2Fh ; /
    db 0F2h ; Ú
    db  2Fh ; /
    db  34h ; 4
    db  30h ; 0
    db  76h ; v
    db  30h ; 0
    db 0B8h ; ∏
    db  30h ; 0
    db 0F9h ; ˘
    db  30h ; 0
    db  39h ; 9
    db  31h ; 1
    db  79h ; y
    db  31h ; 1
    db 0B9h ; π
    db  31h ; 1
    db 0F8h ; ¯
    db  31h ; 1
    db  36h ; 6
    db  32h ; 2
    db  74h ; t
    db  32h ; 2
    db 0B2h ; ≤
    db  32h ; 2
    db 0EFh ; Ô
    db  32h ; 2
    db  2Ch ; ,
    db  33h ; 3
    db  68h ; h
    db  33h ; 3
    db 0A3h ; £
    db  33h ; 3
    db 0DFh ; ﬂ
    db  33h ; 3
    db  19h
    db  34h ; 4
    db  53h ; S
    db  34h ; 4
    db  8Dh ; ç
    db  34h ; 4
    db 0C6h ; ∆
    db  34h ; 4
    db 0FFh
    db  34h ; 4
    db  37h ; 7
    db  35h ; 5
    db  6Eh ; n
    db  35h ; 5
    db 0A5h ; •
    db  35h ; 5
    db 0DCh ; ‹
    db  35h ; 5
    db  12h
    db  36h ; 6
    db  48h ; H
    db  36h ; 6
    db  7Dh ; }
    db  36h ; 6
    db 0B1h ; ±
    db  36h ; 6
    db 0E5h ; Â
    db  36h ; 6
    db  18h
    db  37h ; 7
    db  4Bh ; K
    db  37h ; 7
    db  7Eh ; ~
    db  37h ; 7
    db 0B0h ; ∞
    db  37h ; 7
    db 0E1h ; ·
    db  37h ; 7
    db  12h
    db  38h ; 8
    db  42h ; B
    db  38h ; 8
    db  71h ; q
    db  38h ; 8
    db 0A1h ; °
    db  38h ; 8
    db 0CFh ; œ
    db  38h ; 8
    db 0FDh ; ˝
    db  38h ; 8
    db  2Bh ; +
    db  39h ; 9
    db  58h ; X
    db  39h ; 9
    db  84h ; Ñ
    db  39h ; 9
    db 0B0h ; ∞
    db  39h ; 9
    db 0DBh ; €
    db  39h ; 9
    db    6
    db  3Ah ; :
    db  30h ; 0
    db  3Ah ; :
    db  59h ; Y
    db  3Ah ; :
    db  82h ; Ç
    db  3Ah ; :
    db 0ABh ; ´
    db  3Ah ; :
    db 0D3h ; ”
    db  3Ah ; :
    db 0FAh ; ˙
    db  3Ah ; :
    db  21h ; !
    db  3Bh ; ;
    db  47h ; G
    db  3Bh ; ;
    db  6Dh ; m
    db  3Bh ; ;
    db  92h ; í
    db  3Bh ; ;
    db 0B6h ; ∂
    db  3Bh ; ;
    db 0DAh ; ⁄
    db  3Bh ; ;
    db 0FDh ; ˝
    db  3Bh ; ;
    db  20h
    db  3Ch ; <
    db  42h ; B
    db  3Ch ; <
    db  64h ; d
    db  3Ch ; <
    db  85h ; Ö
    db  3Ch ; <
    db 0A5h ; •
    db  3Ch ; <
    db 0C5h ; ≈
    db  3Ch ; <
    db 0E4h ; ‰
    db  3Ch ; <
    db    3
    db  3Dh ; =
    db  21h ; !
    db  3Dh ; =
    db  3Fh ; ?
    db  3Dh ; =
    db  5Bh ; [
    db  3Dh ; =
    db  78h ; x
    db  3Dh ; =
    db  93h ; ì
    db  3Dh ; =
    db 0AFh ; Ø
    db  3Dh ; =
    db 0C9h ; …
    db  3Dh ; =
    db 0E3h ; „
    db  3Dh ; =
    db 0FCh ; ¸
    db  3Dh ; =
    db  15h
    db  3Eh ; >
    db  2Dh ; -
    db  3Eh ; >
    db  45h ; E
    db  3Eh ; >
    db  5Ch ; \
    db  3Eh ; >
    db  72h ; r
    db  3Eh ; >
    db  88h ; à
    db  3Eh ; >
    db  9Dh ; ù
    db  3Eh ; >
    db 0B1h ; ±
    db  3Eh ; >
    db 0C5h ; ≈
    db  3Eh ; >
    db 0D8h ; ÿ
    db  3Eh ; >
    db 0EBh ; Î
    db  3Eh ; >
    db 0FDh ; ˝
    db  3Eh ; >
    db  0Fh
    db  3Fh ; ?
    db  20h
    db  3Fh ; ?
    db  30h ; 0
    db  3Fh ; ?
    db  40h ; @
    db  3Fh ; ?
    db  4Fh ; O
    db  3Fh ; ?
    db  5Dh ; ]
    db  3Fh ; ?
    db  6Bh ; k
    db  3Fh ; ?
    db  78h ; x
    db  3Fh ; ?
    db  85h ; Ö
    db  3Fh ; ?
    db  91h ; ë
    db  3Fh ; ?
    db  9Ch ; ú
    db  3Fh ; ?
    db 0A7h ; ß
    db  3Fh ; ?
    db 0B1h ; ±
    db  3Fh ; ?
    db 0BBh ; ª
    db  3Fh ; ?
    db 0C4h ; ƒ
    db  3Fh ; ?
    db 0CCh ; Ã
    db  3Fh ; ?
    db 0D4h ; ‘
    db  3Fh ; ?
    db 0DBh ; €
    db  3Fh ; ?
    db 0E1h ; ·
    db  3Fh ; ?
    db 0E7h ; Á
    db  3Fh ; ?
    db 0ECh ; Ï
    db  3Fh ; ?
    db 0F1h ; Ò
    db  3Fh ; ?
    db 0F5h ; ı
    db  3Fh ; ?
    db 0F8h ; ¯
    db  3Fh ; ?
    db 0FBh ; ˚
    db  3Fh ; ?
    db 0FDh ; ˝
    db  3Fh ; ?
    db 0FFh
    db  3Fh ; ?
    db    0
    db  40h ; @
    db    0
    db  40h ; @
word_405F6     dw 0
word_405F8     dw 0
word_405FA     dw 0
word_405FC     dw 0
off_405FE     dd word_3B1F0
    db  0Ah
    db    0
    db  68h ; h
    db  64h ; d
    db  72h ; r
    db  31h ; 1
    db    0
    db    0
dword_4060A     dd 0
    db    1
    db    0
    db    2
    db    0
    db    4
    db    0
    db    8
    db    0
    db  10h
    db    0
    db  20h
    db    0
    db  40h ; @
    db    0
    db  80h ; Ä
    db    0
    db    0
    db    1
    db    0
    db    2
    db    0
    db    4
    db    0
    db    8
    db    0
    db  10h
    db    0
    db  20h
    db    0
    db  40h ; @
    db    0
    db  80h ; Ä
    db    0
    db    0
byte_40630     db 0
byte_40631     db 1
byte_40632     db 0
byte_40633     db 1
byte_40634     db 0
byte_40635     db 0
    db  10h
    db    0
    db  16h
byte_40639     db 0
word_4063A     dw 1
word_4063C     dw 0
    db    0
    db  64h ; d
    db  72h ; r
    db  76h ; v
    db    0
    db  6Dh ; m
    db  74h ; t
    db  33h ; 3
    db  32h ; 2
    db  2Eh ; .
    db  70h ; p
    db  6Ch ; l
    db  62h ; b
    db    0
    db  43h ; C
    db  61h ; a
    db  6Eh ; n
    db  27h ; '
    db  74h ; t
    db  20h
    db  66h ; f
    db  69h ; i
    db  6Eh ; n
    db  64h ; d
    db  20h
    db  64h ; d
    db  72h ; r
    db  69h ; i
    db  76h ; v
    db  65h ; e
    db  72h ; r
    db  21h ; !
    db  0Ah
    db    0
    db  67h ; g
    db  65h ; e
    db    0
    db  67h ; g
    db  65h ; e
    db    0
    db    0
    db    0
    db  64h ; d
    db  73h ; s
    db  66h ; f
    db    0
    db  73h ; s
    db  66h ; f
    db  78h ; x
    db    0
    db  63h ; c
    db  61h ; a
    db  6Eh ; n
    db  6Eh ; n
    db  6Fh ; o
    db  74h ; t
    db  20h
    db  6Ch ; l
    db  6Fh ; o
    db  61h ; a
    db  64h ; d
    db  20h
    db  73h ; s
    db  66h ; f
    db  78h ; x
    db  20h
    db  66h ; f
    db  69h ; i
    db  6Ch ; l
    db  65h ; e
    db  20h
    db  25h ; %
    db  73h ; s
    db    0
    db  6Bh ; k
    db  6Dh ; m
    db  73h ; s
    db    0
    db  63h ; c
    db  61h ; a
    db  6Eh ; n
    db  6Eh ; n
    db  6Fh ; o
    db  74h ; t
    db  20h
    db  6Ch ; l
    db  6Fh ; o
    db  61h ; a
    db  64h ; d
    db  20h
    db  73h ; s
    db  6Fh ; o
    db  6Eh ; n
    db  67h ; g
    db  20h
    db  66h ; f
    db  69h ; i
    db  6Ch ; l
    db  65h ; e
    db  20h
    db  25h ; %
    db  73h ; s
    db    0
    db  64h ; d
    db  76h ; v
    db  63h ; c
    db    0
    db  76h ; v
    db  63h ; c
    db  65h ; e
    db    0
    db  63h ; c
    db  61h ; a
    db  6Eh ; n
    db  6Eh ; n
    db  6Fh ; o
    db  74h ; t
    db  20h
    db  6Ch ; l
    db  6Fh ; o
    db  61h ; a
    db  64h ; d
    db  20h
    db  76h ; v
    db  6Fh ; o
    db  69h ; i
    db  63h ; c
    db  65h ; e
    db  20h
    db  66h ; f
    db  69h ; i
    db  6Ch ; l
    db  65h ; e
    db  20h
    db  25h ; %
    db  73h ; s
    db    0
    db  73h ; s
    db  6Ch ; l
    db  62h ; b
    db    0
    db  63h ; c
    db  61h ; a
    db  6Eh ; n
    db  6Eh ; n
    db  6Fh ; o
    db  74h ; t
    db  20h
    db  6Ch ; l
    db  6Fh ; o
    db  61h ; a
    db  64h ; d
    db  20h
    db  73h ; s
    db  61h ; a
    db  6Dh ; m
    db  70h ; p
    db  6Ch ; l
    db  65h ; e
    db  20h
    db  66h ; f
    db  69h ; i
    db  6Ch ; l
    db  65h ; e
    db  20h
    db  25h ; %
    db  73h ; s
    db    0
    db  68h ; h
    db  64h ; d
    db  72h ; r
    db  31h ; 1
    db    0
    db  42h ; B
    db  41h ; A
    db  53h ; S
    db  44h ; D
    db    0
    db  53h ; S
    db  4Eh ; N
    db  41h ; A
    db  52h ; R
    db    0
    db  54h ; T
    db  4Fh ; O
    db  4Dh ; M
    db  4Dh ; M
    db    0
    db  52h ; R
    db  49h ; I
    db  44h ; D
    db  45h ; E
    db    0
    db  43h ; C
    db  52h ; R
    db  53h ; S
    db  48h ; H
    db    0
    db  43h ; C
    db  48h ; H
    db  48h ; H
    db  54h ; T
    db    0
    db  4Fh ; O
    db  48h ; H
    db  48h ; H
    db  54h ; T
    db    0
    db  68h ; h
    db  64h ; d
    db  72h ; r
    db  31h ; 1
    db    0
    db  73h ; s
    db  77h ; w
    db  50h ; P
    db  61h ; a
    db  75h ; u
    db  73h ; s
    db  65h ; e
    db  20h
    db  3Dh ; =
    db  20h
    db  25h ; %
    db  64h ; d
    db  2Ch ; ,
    db  20h
    db  73h ; s
    db  77h ; w
    db  53h ; S
    db  6Fh ; o
    db  6Eh ; n
    db  67h ; g
    db  20h
    db  3Dh ; =
    db  20h
    db  25h ; %
    db  64h ; d
    db  2Ch ; ,
    db  20h
    db  62h ; b
    db  53h ; S
    db  6Fh ; o
    db  6Eh ; n
    db  67h ; g
    db  20h
    db  3Dh ; =
    db  20h
    db  25h ; %
    db  64h ; d
    db  2Ch ; ,
    db  73h ; s
    db  77h ; w
    db  53h ; S
    db  46h ; F
    db  58h ; X
    db  20h
    db  3Dh ; =
    db  20h
    db  25h ; %
    db  64h ; d
    db  0Ah
    db    0
    db  75h ; u
    db  62h ; b
    db  4Dh ; M
    db  75h ; u
    db  73h ; s
    db  69h ; i
    db  63h ; c
    db  56h ; V
    db  6Fh ; o
    db  6Ch ; l
    db  75h ; u
    db  6Dh ; m
    db  65h ; e
    db  20h
    db  3Dh ; =
    db  20h
    db  25h ; %
    db  64h ; d
    db  2Ch ; ,
    db  20h
    db  75h ; u
    db  62h ; b
    db  53h ; S
    db  66h ; f
    db  78h ; x
    db  56h ; V
    db  6Fh ; o
    db  6Ch ; l
    db  75h ; u
    db  6Dh ; m
    db  65h ; e
    db  20h
    db  3Dh ; =
    db  20h
    db  25h ; %
    db  64h ; d
    db  0Ah
    db    0
    db  54h ; T
    db  25h ; %
    db  30h ; 0
    db  32h ; 2
    db  78h ; x
    db  2Dh ; -
    db  4Eh ; N
    db  44h ; D
    db  3Dh ; =
    db  25h ; %
    db  6Ch ; l
    db  78h ; x
    db  2Ch ; ,
    db  44h ; D
    db  4Ch ; L
    db  3Dh ; =
    db  25h ; %
    db  6Ch ; l
    db  64h ; d
    db  0Ah
    db    0
    db  50h ; P
    db  72h ; r
    db  65h ; e
    db  73h ; s
    db  73h ; s
    db  20h
    db  61h ; a
    db  20h
    db  4Bh ; K
    db  65h ; e
    db  79h ; y
    db  0Ah
    db    0
    db  48h ; H
    db  25h ; %
    db  30h ; 0
    db  32h ; 2
    db  78h ; x
    db  20h
    db  2Dh ; -
    db  20h
    db  53h ; S
    db  54h ; T
    db  3Dh ; =
    db  25h ; %
    db  64h ; d
    db  2Ch ; ,
    db  54h ; T
    db  50h ; P
    db  3Dh ; =
    db  25h ; %
    db  6Ch ; l
    db  78h ; x
    db  2Ch ; ,
    db  54h ; T
    db  4Ch ; L
    db  3Dh ; =
    db  25h ; %
    db  6Ch ; l
    db  78h ; x
    db  0Ah
    db    0
word_407AA     dw 0
    db  2Eh ; .
    db    0
    db    1
    db    0
    db    2
    db    0
    db    3
    db    0
    db    4
    db    0
    db    5
    db    0
    db    6
    db    0
    db    7
    db    0
    db    8
    db    0
    db    9
    db    0
    db  0Ah
    db    0
    db  0Bh
    db    0
    db  0Ch
    db    0
    db  0Dh
    db    0
    db  0Eh
    db    0
word_407CA     dw 0Fh
word_407CC     dw 10h
word_407CE     dw 5
word_407D0     dw 0Eh
word_407D2     dw 8
word_407D4     dw 0Fh
word_407D6     dw 8
word_407D8     dw 0Bh
word_407DA     dw 3
word_407DC     dw 0Ch
word_407DE     dw 4
word_407E0     dw 9
word_407E2     dw 1
word_407E4     dw 0Ah
word_407E6     dw 2
word_407E8     dw 0Dh
word_407EA     dw 5
word_407EC     dw 0Bh
word_407EE     dw 9
word_407F0     dw 1
word_407F2     dw 0Ch
word_407F4     dw 0Fh
word_407F6     dw 8
word_407F8     dw 7
word_407FA     dw 9
word_407FC     dw 1
word_407FE     dw 4
word_40800     dw 1
word_40802     dw 4
    db    0
    db    0
    db    1
    db    0
    db    2
    db    0
    db    3
    db    0
    db    4
    db    0
    db    5
    db    0
    db    6
    db    0
    db    7
    db    0
    db    8
    db    0
    db    9
    db    0
    db  0Ah
    db    0
    db  0Bh
    db    0
    db  0Ch
    db    0
    db  0Dh
    db    0
    db  0Eh
    db    0
    db  0Fh
    db    0
    db  6Ch ; l
    db    0
    db  74h ; t
    db    0
    db  0Fh
    db    0
    db  1Ch
    db    0
    db  1Dh
    db    0
    db  0Eh
    db    0
    db  1Ch
    db    0
    db  1Fh
    db    0
    db  0Eh
    db    0
    db 0C8h ; »
    db    0
    db 0C6h ; ∆
    db    0
    db 0C4h ; ƒ
    db    0
    db  70h ; p
    db    0
    db  72h ; r
    db    0
    db  74h ; t
    db    0
    db 0C2h ; ¬
    db    0
    db 0C5h ; ≈
    db    0
    db 0C8h ; »
    db    0
    db  92h ; í
    db    0
    db  25h ; %
    db    0
    db  23h ; #
    db    0
    db 0B5h ; µ
    db    0
    db  1Dh
    db    0
    db  1Fh
    db    0
    db  13h
    db    0
    db    3
    db    0
    db  0Bh
    db    0
    db  1Bh
    db    0
    db    0
    db    0
    db    4
    db    0
    db    4
    db    0
    db  0Ch
    db    0
    db  9Ch ; ú
    db    0
    db  9Ah ; ö
    db    0
    db  98h ; ò
    db    0
    db  96h ; ñ
    db    0
    db  2Ah ; *
    db    0
    db  28h ; (
    db    0
    db  26h ; &
    db    0
    db  25h ; %
    db    0
    db  1Bh
    db    0
    db  1Ah
    db    0
    db  19h
    db    0
    db  18h
    db    0
    db  48h ; H
    db    0
    db  46h ; F
    db    0
    db  44h ; D
    db    0
    db  42h ; B
    db    0
    db  7Bh ; {
    db    0
    db  79h ; y
    db    0
    db  78h ; x
    db    0
    db  75h ; u
    db    0
    db  5Ch ; \
    db    0
    db  5Ah ; Z
    db    0
    db  58h ; X
    db    0
    db  57h ; W
    db    0
    db 0ADh ; ≠
    db    0
    db 0ABh ; ´
    db    0
    db 0A9h ; ©
    db    0
    db 0A7h ; ß
    db    0
    db  14h
    db    0
    db  13h
    db    0
    db  12h
    db    0
    db  11h
    db    0
    db  4Dh ; M
    db    0
    db  4Ch ; L
    db    0
    db  4Ah ; J
    db    0
    db  49h ; I
    db    0
    db  2Dh ; -
    db    0
    db  2Ch ; ,
    db    0
    db  2Ah ; *
    db    0
    db  29h ; )
    db    0
    db  9Fh ; ü
    db    0
    db 0AFh ; Ø
    db    0
    db 0AEh ; Æ
    db    0
    db 0ACh ; ¨
    db    0
    db  1Dh
    db    0
    db  1Ch
    db    0
    db  12h
    db    0
    db  5Ah ; Z
    db    0
    db  0Fh
    db    0
    db    7
    db    0
    db 0C8h ; »
    db    0
    db 0DBh ; €
    db    0
    db  88h ; à
    db    0
    db  63h ; c
    db    0
    db  65h ; e
    db    0
    db  67h ; g
    db    0
    db  68h ; h
    db    0
    db  6Ah ; j
    db    0
    db  11h
    db    0
    db  14h
    db    0
    db  3Ch ; <
    db    0
    db  4Dh ; M
    db    0
    db  2Eh ; .
    db    0
    db  3Dh ; =
    db    0
    db  2Dh ; -
    db    0
    db 0CAh ;  
    db    0
    db 0BEh ; æ
    db    0
    db 0BAh ; ∫
    db    0
    db 0B7h ; ∑
    db    0
    db 0B4h ; ¥
    db    0
    db    0
    db    0
    db  1Ch
    db    0
    db  1Eh
    db    0
    db  10h
    db    0
    db  14h
    db    0
    db  44h ; D
    db    0
    db  36h ; 6
    db    0
    db  27h ; '
    db    0
    db  2Bh ; +
    db    0
    db  0Ch
    db    0
    db  11h
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    1
    db    0
    db    1
    db    0
    db    1
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    1
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    1
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    1
    db    0
    db    1
    db    0
    db    1
    db    0
    db    1
    db    0
    db    1
    db    0
    db    1
    db    0
    db    1
    db    0
    db    1
    db    0
    db    1
    db    0
    db    0
    db    0
    db    0
    db    0
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db    0
    db    0
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0DDh ; ›
    db  77h ; w
    db  77h ; w
    db 0DDh ; ›
    db 0DDh ; ›
    db  77h ; w
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0DDh ; ›
    db  77h ; w
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db 0DDh ; ›
    db  77h ; w
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db 0FFh
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db  33h ; 3
    db 0CCh ; Ã
    db 0CCh ; Ã
    db  33h ; 3
    db  6Dh ; m
    db 0B6h ; ∂
    db  92h ; í
    db  49h ; I
    db  6Dh ; m
    db 0B6h ; ∂
    db  92h ; í
    db  49h ; I
    db  6Dh ; m
    db 0B6h ; ∂
    db  92h ; í
    db  49h ; I
    db    0
    db    0
    db    0
    db    0
word_40B0A     dw 5094h
word_40B0C     dw 5094h
word_40B0E     dw 5196h
word_40B10     dw 5298h
byte_40B12     db 70h
byte_40B13     db 63h
    db  31h ; 1
    db  35h ; 5
    db    0
    db  73h ; s
    db  64h ; d
    db  6Dh ; m
    db  61h ; a
    db  69h ; i
    db  6Eh ; n
    db    0
    db  21h ; !
    db  70h ; p
    db  61h ; a
    db  6Ch ; l
    db    0
    db  73h ; s
    db  6Dh ; m
    db  6Fh ; o
    db  75h ; u
    db    0
    db  73h ; s
    db  64h ; d
    db  6Dh ; m
    db  61h ; a
    db  69h ; i
    db  6Eh ; n
    db    0
    db  73h ; s
    db  6Dh ; m
    db  6Fh ; o
    db  75h ; u
    db    0
    db  6Dh ; m
    db  6Dh ; m
    db  6Fh ; o
    db  75h ; u
    db    0
    db    0
    db  20h
    db    0
    db    3
    db    0
    db    0
    db    0
    db  43h ; C
    db  6Fh ; o
    db  70h ; p
    db  79h ; y
    db  72h ; r
    db  69h ; i
    db  67h ; g
    db  68h ; h
    db  74h ; t
    db  20h
    db  28h ; (
    db  43h ; C
    db  29h ; )
    db  20h
    db  55h ; U
    db  6Eh ; n
    db  6Ch ; l
    db  69h ; i
    db  6Dh ; m
    db  69h ; i
    db  74h ; t
    db  65h ; e
    db  64h ; d
    db  20h
    db  53h ; S
    db  6Fh ; o
    db  66h ; f
    db  74h ; t
    db  77h ; w
    db  61h ; a
    db  72h ; r
    db  65h ; e
    db  20h
    db  49h ; I
    db  6Eh ; n
    db  63h ; c
    db  2Eh ; .
    db  20h
    db  31h ; 1
    db  39h ; 9
    db  38h ; 8
    db  39h ; 9
    db  2Ch ; ,
    db  31h ; 1
    db  39h ; 9
    db  39h ; 9
    db  30h ; 0
    db  2Eh ; .
    db  20h
    db  20h
    db  41h ; A
    db  6Ch ; l
    db  6Ch ; l
    db  20h
    db  72h ; r
    db  69h ; i
    db  67h ; g
    db  68h ; h
    db  74h ; t
    db  73h ; s
    db  20h
    db  72h ; r
    db  65h ; e
    db  73h ; s
    db  65h ; e
    db  72h ; r
    db  76h ; v
    db  65h ; e
    db  64h ; d
    db  2Eh ; .
byte_40B86     db 0
    db    0
    db    0
    db    0
    db    0
    db    0
    db  4Dh ; M
    db  43h ; C
    db  47h ; G
    db  41h ; A
    db  20h
    db  57h ; W
    db  49h ; I
    db  4Eh ; N
    db  44h ; D
    db  4Fh ; O
    db  57h ; W
    db    0
    db  77h ; w
    db  69h ; i
    db  6Eh ; n
    db  64h ; d
    db  6Fh ; o
    db  77h ; w
    db  64h ; d
    db  65h ; e
    db  66h ; f
    db  20h
    db  2Dh ; -
    db  20h
    db  4Fh ; O
    db  55h ; U
    db  54h ; T
    db  20h
    db  4Fh ; O
    db  46h ; F
    db  20h
    db  52h ; R
    db  4Fh ; O
    db  57h ; W
    db  20h
    db  54h ; T
    db  41h ; A
    db  42h ; B
    db  4Ch ; L
    db  45h ; E
    db  20h
    db  53h ; S
    db  50h ; P
    db  41h ; A
    db  43h ; C
    db  45h ; E
    db  0Dh
    db    0
    db  2Eh ; .
    db  50h ; P
    db  56h ; V
    db  53h ; S
    db    0
    db  2Eh ; .
    db  58h ; X
    db  56h ; V
    db  53h ; S
    db    0
    db  2Eh ; .
    db  56h ; V
    db  53h ; S
    db  48h ; H
    db    0
    db  2Eh ; .
    db  50h ; P
    db  45h ; E
    db  53h ; S
    db    0
    db  2Eh ; .
    db  45h ; E
    db  53h ; S
    db  48h ; H
    db    0
    db    0
    db  4Ch ; L
    db  54h ; T
    db  51h ; Q
    db  54h ; T
    db  56h ; V
    db  54h ; T
    db  5Bh ; [
    db  54h ; T
    db  60h ; `
    db  54h ; T
    db  65h ; e
    db  54h ; T
    db    0
    db    1
    db    2
    db    3
    db    4
    db    5
    db    6
    db    7
    db    8
    db    9
    db  0Ah
    db  0Bh
    db  0Ch
    db  0Dh
    db  0Eh
    db  0Fh
    db  2Eh ; .
    db  50h ; P
    db  56h ; V
    db  53h ; S
    db    0
    db  55h ; U
    db  4Eh ; N
    db  46h ; F
    db  4Ch ; L
    db  49h ; I
    db  50h ; P
    db    0
    db  2Eh ; .
    db  58h ; X
    db  56h ; V
    db  53h ; S
    db    0
    db  2Eh ; .
    db  50h ; P
    db  45h ; E
    db  53h ; S
    db    0
    db  55h ; U
    db  4Eh ; N
    db  46h ; F
    db  4Ch ; L
    db  49h ; I
    db  50h ; P
    db    0
    db  2Eh ; .
    db  45h ; E
    db  53h ; S
    db  48h ; H
    db    0
    db  21h ; !
    db  4Dh ; M
    db  47h ; G
    db  41h ; A
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
dword_40C1E     dd 0
dword_40C22     dd 0
dword_40C26     dd 0
    dd _flushall
aNmsg     db '<<NMSG>>',0
    db    0
aR6000StackOver     db 'R6000',0Dh,0Ah
    db    3
    db    0
    db  52h ; R
    db  36h ; 6
    db  30h ; 0
    db  30h ; 0
    db  33h ; 3
    db  0Dh
    db  0Ah
    db  2Dh ; -
    db  20h
    db  69h ; i
    db  6Eh ; n
    db  74h ; t
    db  65h ; e
    db  67h ; g
    db  65h ; e
    db  72h ; r
    db  20h
    db  64h ; d
    db  69h ; i
    db  76h ; v
    db  69h ; i
    db  64h ; d
    db  65h ; e
    db  20h
    db  62h ; b
    db  79h ; y
    db  20h
    db  30h ; 0
    db  0Dh
    db  0Ah
    db    0
    db    9
    db    0
    db  52h ; R
    db  36h ; 6
    db  30h ; 0
    db  30h ; 0
    db  39h ; 9
    db  0Dh
    db  0Ah
    db  2Dh ; -
    db  20h
    db  6Eh ; n
    db  6Fh ; o
    db  74h ; t
    db  20h
    db  65h ; e
    db  6Eh ; n
    db  6Fh ; o
    db  75h ; u
    db  67h ; g
    db  68h ; h
    db  20h
    db  73h ; s
    db  70h ; p
    db  61h ; a
    db  63h ; c
    db  65h ; e
    db  20h
    db  66h ; f
    db  6Fh ; o
    db  72h ; r
    db  20h
    db  65h ; e
    db  6Eh ; n
    db  76h ; v
    db  69h ; i
    db  72h ; r
    db  6Fh ; o
    db  6Eh ; n
    db  6Dh ; m
    db  65h ; e
    db  6Eh ; n
    db  74h ; t
    db  0Dh
    db  0Ah
    db    0
    db 0FCh ; ¸
    db    0
    db  0Dh
    db  0Ah
    db    0
    db 0FFh
    db    0
    db  72h ; r
    db  75h ; u
    db  6Eh ; n
    db  2Dh ; -
    db  74h ; t
    db  69h ; i
    db  6Dh ; m
    db  65h ; e
    db  20h
    db  65h ; e
    db  72h ; r
    db  72h ; r
    db  6Fh ; o
    db  72h ; r
    db  20h
    db    0
    db    2
    db    0
    db  52h ; R
    db  36h ; 6
    db  30h ; 0
    db  30h ; 0
    db  32h ; 2
    db  0Dh
    db  0Ah
    db  2Dh ; -
    db  20h
    db  66h ; f
    db  6Ch ; l
    db  6Fh ; o
    db  61h ; a
    db  74h ; t
    db  69h ; i
    db  6Eh ; n
    db  67h ; g
    db  20h
    db  70h ; p
    db  6Fh ; o
    db  69h ; i
    db  6Eh ; n
    db  74h ; t
    db  20h
    db  6Eh ; n
    db  6Fh ; o
    db  74h ; t
    db  20h
    db  6Ch ; l
    db  6Fh ; o
    db  61h ; a
    db  64h ; d
    db  65h ; e
    db  64h ; d
    db  0Dh
    db  0Ah
    db    0
    db    1
    db    0
    db  52h ; R
    db  36h ; 6
    db  30h ; 0
    db  30h ; 0
    db  31h ; 1
    db  0Dh
    db  0Ah
    db  2Dh ; -
    db  20h
    db  6Eh ; n
    db  75h ; u
    db  6Ch ; l
    db  6Ch ; l
    db  20h
    db  70h ; p
    db  6Fh ; o
    db  69h ; i
    db  6Eh ; n
    db  74h ; t
    db  65h ; e
    db  72h ; r
    db  20h
    db  61h ; a
    db  73h ; s
    db  73h ; s
    db  69h ; i
    db  67h ; g
    db  6Eh ; n
    db  6Dh ; m
    db  65h ; e
    db  6Eh ; n
    db  74h ; t
    db  0Dh
    db  0Ah
    db    0
    db  0Ah
    db    0
    db  0Ah
    db  41h ; A
    db  62h ; b
    db  6Eh ; n
    db  6Fh ; o
    db  72h ; r
    db  6Dh ; m
    db  61h ; a
    db  6Ch ; l
    db  20h
    db  70h ; p
    db  72h ; r
    db  6Fh ; o
    db  67h ; g
    db  72h ; r
    db  61h ; a
    db  6Dh ; m
    db  20h
    db  74h ; t
    db  65h ; e
    db  72h ; r
    db  6Dh ; m
    db  69h ; i
    db  6Eh ; n
    db  61h ; a
    db  74h ; t
    db  69h ; i
    db  6Fh ; o
    db  6Eh ; n
    db  0Ah
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db    0
    db 0FFh
    db    0
word_40D3A     dw 0
word_40D3C     dw 0
word_40D3E     dw 0
dseg ends
end
