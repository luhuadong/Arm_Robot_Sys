MODULE Module1
	CONST robtarget p60:=[[921.57,-665.43,1400.10],[0.420819,-1.96661E-07,0.907145,-3.80985E-09],[-1,0,-1,0],[0,0,0,0,0,0]];
	CONST robtarget p10:=[[940.00,0.00,1455.00],[0.707107,0,0.707107,0],[0,0,0,0],[0,0,0,0,0,0]];
    CONST robtarget p20:=[[921.57,-665.43,1400.10],[0.420819,-1.96661E-07,0.907145,-3.80985E-09],[-1,0,-1,0],[0,0,0,0,0,0]];
    CONST robtarget p30:=[[403.34,218.34,1400.10],[0.420818,-5.72188E-08,0.907145,1.96676E-08],[0,-1,0,0],[0,0,0,0,0,0]];
    CONST robtarget p40:=[[921.57,218.34,1400.10],[0.420819,-9.99145E-08,0.907145,1.88407E-08],[0,-1,0,0],[0,0,0,0,0,0]];
    CONST robtarget p50:=[[403.34,218.34,1400.10],[0.420818,-5.72188E-08,0.907145,1.96676E-08],[0,-1,0,0],[0,0,0,0,0,0]];
	PERS tooldata MyNewTool2:=[TRUE,[[0,0,0],[1,0,0,0]],[1,[0,0,1],[1,0,0,0],0,0,0]];
    PROC main()
		MoveJ p10, v1000, z0, tool0\WObj:=wobj0;
		MoveJ p20, v1000, z0, tool0\WObj:=wobj0;
		MoveJ p30, v1000, z0, tool0\WObj:=wobj0;
		MoveJ p40, v1000, z0, tool0\WObj:=wobj0;
		MoveJ p50, v1000, z0, tool0\WObj:=wobj0;
		MoveJ p60, v1000, z0, tool0\WObj:=wobj0;
		MoveJ p10, v1000, z0, tool0\WObj:=wobj0;
		MoveJ p20, v1000, z0, tool0\WObj:=wobj0;
		MoveJ p30, v1000, z0, tool0\WObj:=wobj0;
		MoveJ p40, v1000, z0, tool0\WObj:=wobj0;
		MoveJ p50, v1000, z0, tool0\WObj:=wobj0;
		MoveJ p60, v1000, z0, tool0\WObj:=wobj0;
		MoveJ p10, v1000, z0, tool0\WObj:=wobj0;
		MoveJ p20, v1000, z0, tool0\WObj:=wobj0;
		MoveJ p30, v1000, z0, tool0\WObj:=wobj0;
		MoveJ p40, v1000, z0, tool0\WObj:=wobj0;
		MoveJ p50, v1000, z0, tool0\WObj:=wobj0;
		MoveJ p60, v1000, z0, tool0\WObj:=wobj0;
		MoveJ p10, v1000, z0, tool0\WObj:=wobj0;
		MoveJ p20, v1000, z0, tool0\WObj:=wobj0;
		MoveJ p30, v1000, z0, tool0\WObj:=wobj0;
		MoveJ p40, v1000, z0, tool0\WObj:=wobj0;
		MoveJ p50, v1000, z0, tool0\WObj:=wobj0;
		MoveJ p60, v1000, z0, tool0\WObj:=wobj0;
		MoveJ p10, v1000, z0, tool0\WObj:=wobj0;
		MoveJ p20, v1000, z0, tool0\WObj:=wobj0;
		MoveJ p30, v1000, z0, tool0\WObj:=wobj0;
		MoveJ p40, v1000, z0, tool0\WObj:=wobj0;
		MoveJ p50, v1000, z0, tool0\WObj:=wobj0;
		MoveJ p60, v1000, z0, tool0\WObj:=wobj0;
	ENDPROC
ENDMODULE