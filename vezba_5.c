#include "remote_controller.h"
#include "stream_controller.h"
#include <signal.h>

static inline void textColor(int32_t attr, int32_t fg, int32_t bg)
{
   char command[13];

   /* command is the control command to the terminal */
   sprintf(command, "%c[%d;%d;%dm", 0x1B, attr, fg + 30, bg + 40);
   printf("%s", command);
}

/* macro function for error checking */
#define ERRORCHECK(x)                                                       \
{                                                                           \
if (x != 0)                                                                 \
 {                                                                          \
    textColor(1,1,0);                                                       \
    printf(" Error!\n File: %s \t Line: <%d>\n", __FILE__, __LINE__);       \
    textColor(0,7,0);                                                       \
    return -1;                                                              \
 }                                                                          \
}

static void remoteControllerCallback(uint16_t code, uint16_t type, uint32_t value);
static pthread_cond_t deinitCond = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t deinitMutex = PTHREAD_MUTEX_INITIALIZER;
static ChannelInfo channelInfo;
static ConfigFileInfo configFileInfo;
void inputNumber(uint16_t button);
void changeChannel();
char varName[10];
static int32_t key1=0;
static int32_t key2=0;
static int32_t key3=0;
static int32_t pressedKeys=0;

int parseConfigFile(char filename[]){
    FILE *f=NULL;
    printf("Try to open file %s\n",filename);
    f=fopen(filename,"r");
    if(f==NULL)
      return -1;
      fscanf(f,"%s %d %s %d %s %d %s %d", varName, &configFileInfo.Frequency,varName,&configFileInfo.Bandwidth,
                                          varName, &configFileInfo.programNumber,varName,&configFileInfo.Modul);
      return 0;    
}


int main(int argc, char* argv[])
{
	int check;
  if(argc==2){
    check=parseConfigFile(argv[1]);
        if(check<0){
            printf("Can't find config file");
            return -1;
        }
  }else if(argc>2){
        printf("To mani arguments");
        return -1;
  }else{
        printf("Input argumets");
  }
  printf("Frequency: %d\nBandwidth: %d\nProgram_number: %d\nmodule: %d\n", configFileInfo.Frequency,configFileInfo.Bandwidth, configFileInfo.programNumber, init_config.Modul);

    /* initialize remote controller module */
    ERRORCHECK(remoteControllerInit());

    /* register remote controller callback */
    ERRORCHECK(registerRemoteControllerCallback(remoteControllerCallback));

    /* initialize stream controller module */
    ERRORCHECK(streamControllerInit());

    /* wait for a EXIT remote controller key press event */
    pthread_mutex_lock(&deinitMutex);
	if (ETIMEDOUT == pthread_cond_wait(&deinitCond, &deinitMutex))
	{
		printf("\n%s : ERROR Lock timeout exceeded!\n", __FUNCTION__);
	}
	pthread_mutex_unlock(&deinitMutex);

    /* unregister remote controller callback */
    ERRORCHECK(unregisterRemoteControllerCallback(remoteControllerCallback));

    /* deinitialize remote controller module */
    ERRORCHECK(remoteControllerDeinit());

    /* deinitialize stream controller module */
    ERRORCHECK(streamControllerDeinit());

    return 0;
}

void remoteControllerCallback(uint16_t code, uint16_t type, uint32_t value)
{
    switch(code)
	{
		case KEYCODE_INFO:
            printf("\nInfo pressed\n");
            if (getChannelInfo(&channelInfo) == SC_NO_ERROR)
            {
                printf("\n********************* Channel info *********************\n");
                printf("Program number: %d\n", channelInfo.programNumber);
                printf("Audio pid: %d\n", channelInfo.audioPid);
                printf("Video pid: %d\n", channelInfo.videoPid);
                printf("**********************************************************\n");
            }
			break;
		case KEYCODE_P_PLUS:
			printf("\nCH+ pressed\n");
            channelUp();
			break;
		case KEYCODE_P_MINUS:
		    printf("\nCH- pressed\n");
            channelDown();
			break;
		case KEYCODE_EXIT:
			printf("\nExit pressed\n");
            pthread_mutex_lock(&deinitMutex);
		    pthread_cond_signal(&deinitCond);
		    pthread_mutex_unlock(&deinitMutex);
			break;
      case KEYCODE_1:
          printf("\n CH 1 pressed\n");
          inputNumber(1);
          break;
      case KEYCODE_2:
          printf("\n CH 2 pressed\n");
          inputNumber(2);
          break;
      case KEYCODE_3:
          printf("\n CH 3 pressed\n");
          inputNumber(3);
          break;
      case KEYCODE_4:
          printf("\n CH 4 pressed\n");
          inputNumber(4);
          break;
      case KEYCODE_5:
          printf("\n CH 5 pressed\n");
          inputNumber(5);
          break;
      case KEYCODE_6:
          printf("\n CH 6 pressed\n");
          inputNumber(1);
          break;
      case KEYCODE_7:
          printf("\n CH 7 pressed\n");
          inputNumber(7);
          break;
      case KEYCODE_8:
          printf("\n CH 8 pressed\n");
          inputNumber(8);
          break;
      case KEYCODE_9:
          printf("\n CH 9 pressed\n");
          inputNumber(9);
          break;
      case KEYCODE_0:
          printf("\n CH 0 pressed\n");
          inputNumber(0);
          break;
      default:
          printf("\nPress P+, P-,info or exit!\n\n");
	}
}

void inputNumber(uint16_t button){

      if(pressedKeys==0){
          key1=button;
          pressedKeys++;

      }else if(pressedKeys==1){
          key2=button;
          pressedKeys++;
      }else if(pressedKeys==2){
          key3=button;
          pressedKeys++;
      }

      printf("\nKey1:%d\n",key1);
      printf("\nKey2:%d\n",key2);
      printf("\nKey3:%d\n",key3);
      printf("\nKeys pressed: %d\n",pressedKeys);

      if(pressedKeys==3){
          int changedChanel=key1*100+key2*10+key3;
          changeChannel(modifyChannel(changeChannel));
          pressedKeys=0;
      }
}

void changeChannel(uint16_t channel){
  if(channel>=0 && channel<=3){
        changeChannelByNumber(channel);
  }
}

int_32 modifyChannel(uint16_t channelNum){
    int32_t returnValue;

    if(channelNum==0){
        returnValue=0;
    }
return returnValue;
}
