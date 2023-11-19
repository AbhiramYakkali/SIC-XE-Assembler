0000    SUM      START    0                        
0000    FIRST    LDX     #0                        050000
0003             LDA     #0                        010000
0006            +LDB     #TABLE2                   691017A0
000A             BASE     TABLE2                   
000A    LOOP     ADD      TABLE,X                  1BA026
000D             ADD      TABLE2,X                 1BC000
0010             NOBASE                            
0010            +SUB      TABLE2                   1F1017A0
0014             SUBR     A,X                      9401
0016             TIX      COUNT                    2F2017
0019             JLT      LOOP                     3B2FF1
001C            +STA      TOTAL                    0F102F10
0020             RSUB                              4F0000
0023    MYLIT    LDA     =C'EOF'                   032003
0026             LTORG                             
0026    *       =C'EOF'                            454F46
0029            +LDX     =X'F17'                   07102F14
002D    COUNT    RESW     1                        
0030    TABLE    RESW     2000                     
17A0    TABLE2   RESW     2000                     
2F10    TOTAL    RESW     1                        
2F13    TEST     EQU      COUNT-15                 
2F14    *       =X'F17'                            F17
                 END      FIRST                    
