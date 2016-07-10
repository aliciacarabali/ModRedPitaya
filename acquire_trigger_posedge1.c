/* Red Pitaya C API example Acquiring a signal from a buffer  
 * This application acquires a signal on a specific channel */
 
 /* Modificado por Alicia Carabali para guardar los datos en un archico de texto llamado text.1 con hora
 de inicio y finalización de la adquicisión, el nombre es el del momento de adq*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "redpitaya/rp.h"

int main(int argc, char **argv){

        /* Print error, if rp_Init() function failed */
        if(rp_Init() != RP_OK){
                fprintf(stderr, "Rp api init failed!\n");
        }

        uint32_t buff_size = 16384;
        float *buff = (float *)malloc(buff_size * sizeof(float));

        rp_AcqReset();
        rp_AcqSetDecimation(1);
        rp_AcqSetTriggerLevel(-0.1); //Trig level is set in Volts while in SCPI 
        rp_AcqSetTriggerDelay(0);

        rp_AcqStart();

        /* After acquisition is started some time delay is needed in order to acquire fresh samples in to buffer*/
        /* Here we have used time delay of one second but you can calculate exact value taking in to account buffer*/
        /*length and smaling rate*/

        sleep(1);
        rp_AcqSetTriggerSrc(RP_TRIG_SRC_CHA_PE);
        rp_acq_trig_state_t state = RP_TRIG_STATE_TRIGGERED;

        while(1){
                rp_AcqGetTriggerState(&state);
                if(state == RP_TRIG_STATE_TRIGGERED){
                sleep(1);
                break;
                }
                
        rp_AcqGetOldestDataV(RP_CH_1, &buff_size, buff);
        int i;

	time_t t;
	struct tm * timeinfo;
	//char buffy1 [20];

	time(&t);
        timeinfo=localtime(&t);
	char s [30];
	strftime(s,30,"%c",timeinfo);
	FILE *fp;
        fp=fopen(s,"w");
	

	fprintf(fp,"INICIO: %s", asctime(timeinfo)); 

        for(i = 0; i < buff_size; i++){
                 printf("%f\n", buff[i]);
		fprintf(fp, "%f\n",buff[i]);
        }
	time(&t);
        timeinfo=localtime(&t);
	fprintf(fp, "FIN: %s", asctime(timeinfo)); 
	
	fclose(fp);
        /* Releasing resolsurces */
        free(buff);
        rp_Release();
        return 0;

}

        }
