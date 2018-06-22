MODULE Module1
	CONST robtarget Target_10:=[[992.02143972009003,105.58588353520541,688.62696116427981],[0.037518392060359500,-0.88221442709096565,-0.034540933500233607,-0.46807798367654346],[0,0,0,0],[0,0,0,0,0,0]];
	CONST robtarget Target_20:=[[974.69312433600101,-102.44437545204826,717.19053833095438],[0.025372975071369635,0.87342107122782642,-0.045774058722066899,0.48414520551115792],[-1,0,0,0],[0,0,0,0,0,0]];
	CONST robtarget Target_40:=[[831.70675512279195,-336.03134127386653,954.43770175741713],[0.11747374759893031,0.77353277658943742,-0.15035954024161249,0.60435003998115933],[-1,0,0,0],[0,0,0,0,0,0]];
	CONST zonedata z0:=[FALSE,0.3,0.3,0.3,0.03,0.3,0.03];
	CONST speeddata v1000:=[1000,500,5000,1000];
	CONST num time:=0.5;
	PROC main()
	    MoveJ Target_10,v1000\T:=time,z0,MyNewTool\WObj:=wobj0;
	    MoveJ Target_20,v1000\T:=time,z0,MyNewTool\WObj:=wobj0;
	    MoveJ Target_40,v1000\T:=time,z0,MyNewTool\WObj:=wobj0;
	ENDPROC
	PROC Path_20()
	    MoveL Target_230,v1000,z0,MyNewTool;
	    MoveL Target_240,v1000,z0,MyNewTool;
	    MoveL Target_250,v1000,z0,MyNewTool;
	    MoveL Target_260,v1000,z0,MyNewTool;
	    MoveC Target_270,Target_280,v1000,z0,MyNewTool;
	    MoveC Target_290,Target_300,v1000,z0,MyNewTool;
	    MoveC Target_310,Target_320,v1000,z0,MyNewTool;
	    MoveL Target_330,v1000,z0,MyNewTool;
        Path_30;
	ENDPROC
	PROC Path_30()
	    MoveL Target_340,v1000,z0,MyNewTool;
	    MoveL Target_350,v1000,z0,MyNewTool;
	    MoveL Target_360,v1000,z0,MyNewTool;
        Path_40;
	ENDPROC
	PROC Path_40()
	    MoveL Target_370,v1000,z0,MyNewTool;
	    MoveC Target_380,Target_390,v1000,z0,MyNewTool;
	ENDPROC
ENDMODULE