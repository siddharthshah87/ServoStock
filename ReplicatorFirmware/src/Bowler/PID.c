#include "main.h"


static AbsPID 			pidGroups[numPidTotal];
static PD_VEL 			vel[numPidTotal];
static PidLimitEvent            limits[numPidTotal];


float getPositionMine(int group);
void setOutputMine(int group, float val);
int resetPositionMine(int group, int target);
BOOL asyncCallback(BowlerPacket *Packet);
void onPidConfigureMine(int);
PidLimitEvent * checkPIDLimitEventsMine(BYTE group);

void initPIDLocal(){
   	BYTE i;
	//WORD loop;
	for (i=0;i<numPidTotal;i++){
		pidGroups[i].Enabled=FALSE;
		pidGroups[i].channel = i;
                pidGroups[i].K.P=.08;
                pidGroups[i].K.I=.06;
                pidGroups[i].K.D=.15;
                pidGroups[i].Polarity=1;
		vel[i].enabled=FALSE;
		limits[i].type=NO_LIMIT;
                if(i==LINK0_INDEX || i== LINK1_INDEX || i== LINK2_INDEX){
                  pidGroups[i].Polarity=0;
                }
                if(i==EXTRUDER0_INDEX){
                    pidGroups[i].K.P=.1;
                    pidGroups[i].K.I=0;
                    pidGroups[i].K.D=0;
                    pidGroups[i].Polarity=1;
                }
                if(i>numPidMotors){
                    //These are the PID gains for the tempreture system
                    pidGroups[i].K.P=.1;
                    pidGroups[i].K.I=0;
                    pidGroups[i].K.D=0;
                    pidGroups[i].Polarity=1;
                }
	}

	InitilizePidController( pidGroups,
                                vel,
                                numPidTotal,
                                &getPositionMine,
                                &setOutputMine,
                                &resetPositionMine,
                                &asyncCallback,
                                &onPidConfigureMine,
                                &checkPIDLimitEventsMine); 
       setPidIsr(FALSE);
}

BOOL runPidIsr = FALSE;

BOOL getRunPidIsr(){
    return runPidIsr;
}

void setPidIsr(BOOL v){
    runPidIsr=v;
//    if(runPidIsr){
//        OpenTimer3(T3_ON | T3_SOURCE_INT | T3_PS_1_256, 300*5);
//        ConfigIntTimer3(T3_INT_ON | T3_INT_PRIOR_4);
//        Print_Level l = getPrintLevel();
//        setPrintLevelInfoPrint();
//        println_I("Starting PID ISR");
//        setPrintLevel(l);
//    }else{
//        CloseTimer3();
//        Print_Level l = getPrintLevel();
//        setPrintLevelInfoPrint();
//        println_I("Closing PID ISR");
//        setPrintLevel(l);
//    }
}

void __ISR(_TIMER_3_VECTOR, ipl4) Timer3Handler(void){
//    if(getRunPidIsr()){
//        Print_Level l = getPrintLevel();
//        setPrintLevelNoPrint();
//        RunPIDControl();
//        setPrintLevel(l);
//    }
    mT3ClearIntFlag();
}


BOOL asyncCallback(BowlerPacket *Packet){
    PutBowlerPacket(Packet);// This only works with USB and UART
    return isUSBActave();
}

void onPidConfigureMine(int group){
    if(group==LINK0_INDEX || group== LINK1_INDEX || group== LINK2_INDEX){
        //Synchronized gains for all 3 links, needed for stability
        float p = pidGroups[group].K.P;
        float i = pidGroups[group].K.I;
        float d = pidGroups[group].K.D;
        setPIDConstants(LINK0_INDEX,p,i,d);
        setPIDConstants(LINK1_INDEX,p,i,d);
        setPIDConstants(LINK2_INDEX,p,i,d);
    }
}

void trigerPIDLimit(BYTE chan,PidLimitType type,INT32  tick){
	limits[chan].group=chan;
	limits[chan].type=type;
	limits[chan].value=tick;
	limits[chan].time=getMs();
}

PidLimitEvent * checkPIDLimitEventsMine(BYTE group){
	return & limits[group];
}


int resetPositionMine(int group, int current){
    println_I("Resetting PID Local ");p_ul_I(group);print_I(" to ");p_ul_I(current);print_I(" from ");p_fl_I(getPositionMine(group));
    if(i<numPidMotors){
        setCurrentValue(group, current);
    }else{
        resetHeater(group, current);
    }
    return getPositionMine(group);
}

float getPositionMine(int group){
    float val=0;
    if(i<numPidMotors){
        val = readEncoder(group);
    }else{
        val = getHeaterTempreture(group);
    }

    return val;
}

void setOutputMine(int group, float v){
    if(i<numPidMotors){
        int val = (int)(v);
        val += 128;
        if (val>255)
                val=255;
        if(val<0)
                val=0;

	int set = (int)val;

        setServo(group,set,0);
    }else{
       setHeater( group,  v);
    }
}